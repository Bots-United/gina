/*
 * The GINA Bot - a computer opponent for Valve's FPS game Half-Life
 * Copyright (c) 2011, Wei Mingzhi <whistler_wmz@users.sf.net>
 *
 * This file is part of The GINA Bot.
 *
 * The GINA Bot is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * The GINA Bot is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The GINA Bot; if not, visit <http://www.gnu.org/licenses>.
 *
 * In addition, as a special exception, the author gives permission to
 * link the code of this program with the Half-Life Game Engine ("HL
 * Engine") and Modified Game Libraries ("MODs") developed by Valve,
 * L.L.C ("Valve").  You must obey the GNU General Public License in all
 * respects for all of the code used other than the HL Engine and MODs
 * from Valve.  If you modify this file, you may extend this exception
 * to your version of the file, but you are not obligated to do so.  If
 * you do not wish to do so, delete this exception statement from your
 * version.
 */

//
// bot_combat.cpp
//

#include "main.h"

bool CBaseBot::FindEnemy()
{
   // check if the health is decreased
   bool fHealthDecreased = m_iPrevHealth > GetHealth();
   m_iPrevHealth = GetHealth(); // store away the current health value

   float cur_dist;

   if (m_pEnemy && (!m_pEnemy->IsValid() || !m_pEnemy->IsAlive()))
      m_pEnemy = NULL; // null out the enemy pointer as it's no longer valid

   Vector                vecHisPos;
   unsigned char         cHit;

   // see if we can still see the current enemy...
   if (m_pEnemy) {
      if (FBoxVisible(m_pEnemy, &vecHisPos, &cHit)) {
         m_vecEnemy = vecHisPos;
         m_ucVisibility = cHit;
      } else {
         m_pEnemy = NULL; // we can no longer see this enemy
      }
   }

   // if we already have an enemy...
   if (m_pEnemy) {
      // don't discard important enemies (bomb/flag/hostage carrier, VIP, etc)
      if (g_pServer->ClientIsImportant(EnemyClient()))
         return false;
      // calculate the distance to the enemy
      cur_dist = (m_pEnemy->GetOrigin() - GetOrigin()).Length();
   } else {
      cur_dist = FLT_MAX; // just some crazy value
   }

   // loop through all the clients...
   for (int i = 0; i < g_pServer->GetMaxClients(); i++) {
      if (i == entindex() - 1 || (m_pEnemy && i == m_pEnemy->entindex() - 1))
         continue; // skip myself and the current enemy

      CClient *pClient = g_pServer->m_rgpClients[i];
      if (!pClient || !pClient->IsValid() || !pClient->IsAlive())
         continue;

      float dist = (pClient->GetOrigin() - GetOrigin()).Length();

      // if this enemy is further away than the current one...
      if (dist > cur_dist && !g_pServer->ClientIsImportant(pClient))
         continue; // skip it

      if (dist > 900 + 4000 * ((GetDifficulty() - 1) / 4.0))
         continue; // enemy is too far

      if (g_pServer->IsTeamplay() && GetTeam() == g_pServer->GetTeam(pClient))
         continue; // skip our teammates

      float fov;

      // if the bot's health decreased or the enemy is shooting
      if (!m_pEnemy && (fHealthDecreased || pClient->IsShooting()))
         fov = 360;
      else
         fov = GetFov() * 2 - (GetFov() - (dist > GetFov() * 9 ? GetFov() * 9 : dist) / 9);

      // check if enemy is in the view cone
      if (!FInViewCone(pClient, fov))
         continue; // enemy isn't in bot's view cone

      // check if enemy is visible
      if (!FBoxVisible(pClient, &vecHisPos, &cHit)) {
         continue; // skip this enemy
      }

      // if the enemy is quite far away, not shooting and the bot is not damaged
      if (!m_pEnemy && dist > 200 && !fHealthDecreased && !pClient->IsShooting()) {
         // if the bot isn't in the fov of the enemy and the bot doesn't really want to fight
         if (!pClient->FInViewCone(this, 120) /*&& BotWantsToRetreat()*/)
            continue; // skip this enemy
      }

      m_pEnemy = pClient; // found a new enemy
      m_vecEnemy = vecHisPos;
      m_ucVisibility = cHit;

      DebugMsg(DEBUG_BOTCOMBAT, "Found new enemy: %s", m_pEnemy->GetNetName());

      return true;
   }

   return false; // no new enemy is found
}

