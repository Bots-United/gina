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

#include "../main.h"

bool CBotCS::AINode_Buy()
{
   // if we haven't reached the time to buy, don't proceed
   if (m_flBuyTime > g_pServer->GetTime())
      AINODE_RETURN; // run again at next think cycle

   // don't buy if we're the VIP
   if (IsVIP()) {
      AINODE_SWITCH(CAINode_Normal);
   }
 
   // do all the jobs of buying weapons
   if (BuyStuff()) {
      AINODE_SWITCH(CAINode_Normal); // we've finished buying
   } else {
      AINODE_RETURN; // we've NOT finished buying
   }
}

void CBotCS::AIEnter_PlantBomb()
{
   // send a radio message randomly to inform the teammates
   if (RandomLong(0, 100) < 66) {
      if (RandomLong(0, 100) < 50)
         SendRadio(RADIO_COVERME);
      else
         SendRadio(RADIO_NEEDBACKUP);
   }
}

bool CBotCS::AINode_PlantBomb()
{
   if (!HasWeapon(CS_WEAPON_C4) || !m_fInBombZone) {
      AINODE_SWITCH(CAINode_Normal); // bot is not carrying the bomb
   }

   // else duck under, press fire and don't move...
   bi.weapon = CS_WEAPON_C4; // switch to C4 if we haven't already done so
   bi.actionflags = (ACTION_ATTACK | ACTION_CROUCH);

   AINODE_RETURN;
}

void CBotCS::AIEnter_DefuseBomb()
{
   // send a radio message randomly to inform the teammates
   if (RandomLong(0, 100) < 66) {
      if (RandomLong(0, 100) < 50)
         SendRadio(RADIO_COVERME);
      else
         SendRadio(RADIO_NEEDBACKUP);
   }
}

bool CBotCS::AINode_DefuseBomb()
{
   CEntity *pBomb = NULL;

   // find the bomb entity...
   while (pBomb = UTIL_FindEntityByClassname(pBomb, "grenade")) {
      // see if it's the C4 bomb...
      if (strcmp(pBomb->GetModel(), "models/w_c4.mdl") == 0) {
         // this is the C4, check if it's near enough
         if ((pBomb->GetOrigin() - GetGunPosition()).Length() >= 72) {
            AIEnter(new CAINode_Normal(this));
            return true; // bomb is too far; we can't defuse it now
         }

         break;
      }
   }

   if (!pBomb) {
      AINODE_SWITCH(CAINode_Normal); // bomb is not found
   }

   FacePosition(pBomb->GetOrigin()); // face the bomb
   bi.actionflags = (ACTION_CROUCH | ACTION_USE); // don't move, crouch and 'use' the bomb

   AINODE_RETURN;
}

bool CBotCS::AINode_Normal()
{
   Nav()->Run();

   // fight the enemy if we found one
   if (FindEnemy())
      AINODE_SWITCH(CAINode_CS_CombatFight);

   if (m_fInBombZone && HasWeapon(CS_WEAPON_C4))
      AINODE_SWITCH(CAINode_CS_PlantBomb);

   if (GetTeam() == CS_TEAM_CT) {
      CEntity *pBomb = NULL;

      // find the bomb entity...
      while (pBomb = UTIL_FindEntityByClassname(pBomb, "grenade")) {
         // see if it's the C4 bomb...
         if (strcmp(pBomb->GetModel(), "models/w_c4.mdl") == 0) {
            break;
         }
      }

      if (pBomb != NULL) {
         // this is the C4, check if it's near enough
         if ((pBomb->GetOrigin() - GetGunPosition()).Length() < 72)
            AINODE_SWITCH(CAINode_CS_DefuseBomb);
      }
   }

   return false;
}

void CBotCS::AIEnter_CombatFight()
{
}

bool CBotCS::AINode_CombatFight()
{
   FindEnemy();

   if (!m_pEnemy) {
      bi.actionflags &= ~ACTION_ATTACK;
      AIEnter(new CAINode_Normal(this));
      return true;
   }

   FacePosition(m_pEnemy->GetOrigin());
   if (RandomLong(0, 1))
      bi.actionflags |= ACTION_ATTACK;
   else
      bi.actionflags &= ~ACTION_ATTACK;

   return false;
}

