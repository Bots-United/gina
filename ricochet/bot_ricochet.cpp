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
// bot_ricochet.cpp
//

#include "../main.h"

CBotRC::CBotRC()
{
   m_uiPowerups = 0;
   m_iDiscNum = 0;

   m_vecDest = NULLVEC;
}

CBotRC::~CBotRC()
{
}

bool CBotRC::MessageWrite(void *p)
{
   // return true when the message is handled, false if not.

   // handles net messages sent to this bot...
   static int message_AmmoX = -1;
   static int message_Powerup = -1;

   if (message_AmmoX == -1) {
      // cache the message ID in local static variables for speedup
      message_AmmoX = g_General.GetUserMsgId("AmmoX");
      message_Powerup = g_General.GetUserMsgId("Powerup");
   }

   if (g_pServer->GetCurMsgType() == message_AmmoX) {
      // This message is sent whenever ammo amounts are adjusted (up or down).
      if (g_pServer->GetCurMsgState() == 1) {
         m_iDiscNum = *reinterpret_cast<int *>(p); // store it away
      }
   } else if (g_pServer->GetCurMsgType() == message_Powerup) {
      // This message is sent whenever a player gets or loses powerup(s).
      m_uiPowerups = *reinterpret_cast<int *>(p);
   } else {
      return CClient::MessageWrite(p); // message not handled
   }

   return true; // message handled
}

CBotGoal CBotRC::FindGoal()
{
   // not used for Ricochet, just use some dummy value
   CBotGoal goal;

   goal.flags = 0;
   goal.maxs = goal.mins = goal.origin = NULLVEC;
   goal.pEntity = NULL;

   return goal;
}

bool CBotRC::AINode_Respawn()
{
   if (IsAlive()) {
      AINODE_SWITCH(CAINode_Normal);
   }

   if (RandomLong(0, 1)) {
      bi.actionflags |= ACTION_ATTACK; // press attack randomly to respawn
   }

   return false;
}

bool CBotRC::FindEnemy()
{
   // does the bot already have an enemy?
   if (m_pEnemy != NULL && m_pEnemy->IsValid() && m_pEnemy->IsAlive()) {
      if (FVisible(m_pEnemy->GetOrigin()) && fabs(GetOrigin().z - m_pEnemy->GetOrigin().z) < 72)
         return false; // if enemy is still visible and in field of view, keep it
   }

   m_pEnemy = NULL;

   float nearestdistance = 2500;

   // loop through all the clients...
   for (int i = 0; i < g_pServer->GetMaxClients(); i++) {
      if (i == entindex() - 1)
         continue; // skip myself

      CClient *pClient = g_pServer->m_rgpClients[i];
      if (!pClient || !pClient->IsValid() || !pClient->IsAlive())
         continue;

      // skip this player is he is in a different arena
      if (GetGroupInfo() != pClient->GetGroupInfo())
         continue;

      // see if the enemy is at the same height as the bot
      if (fabs(GetOrigin().z - pClient->GetOrigin().z) >= 72)
         continue;

      // see if bot can see the enemy...
      if (FVisible(pClient)) {
         float distance = (GetOrigin() - pClient->GetOrigin()).Length();
         if (distance < nearestdistance) {
            nearestdistance = distance;
            m_pEnemy = pClient;
         }
      }
   }

   if (m_pEnemy != NULL) {
      DebugMsg(DEBUG_BOTCOMBAT, "Found new enemy: %s", m_pEnemy->GetNetName());
      return true;
   }

   return false; // no enemy found
}

void CBotRC::ShootAtEnemy()
{
   FindEnemy();

   if (m_pEnemy == NULL)
      return; // no enemy, no need to shoot

#ifdef _DEBUG
   g_General.DrawBeam(NULL, GetOrigin(), m_pEnemy->GetOrigin(), 10, 0, 255, 255, 255, 255, 10, 1);
#endif

   FacePosition(m_pEnemy->GetOrigin());

   // is the bot's crosshair on the enemy yet?
   Vector vecDir = (m_pEnemy->GetOrigin() - GetGunPosition()).Normalize(), forward;
   GetViewAngles().AngleVectors(&forward);

   if (DotProduct(forward, vecDir) > 0.9) {
      // we will likely hit the enemy, FIRE!!
      float distance = (GetOrigin() - m_pEnemy->GetOrigin()).Length();
      // if enemy is near enough, we have enough discs,
      // and we haven't "Power Shot" powerup...
      if (distance < 128 && m_iDiscNum >= 3 && !(m_uiPowerups & POW_HARD)
         && RandomLong(1, 100) < GetSkill())
         bi.actionflags |= ACTION_ATTACK2; // Decapitate him!!!!
      else
         bi.actionflags |= ACTION_ATTACK;
   }
}

void CBotRC::HeadToJumpPad()
{
   if (IsOnFloor()) {
      if (m_vecDest == NULLVEC) {
         // try to find a jump arrow
         int iCount = 0;
         CEntity *p = NULL;
         Vector vecDest[8];

         // find a jumping pad
         while (p = UTIL_FindEntityInSphere(p, GetOrigin(), 256.0)) {
            if (strcmp(p->GetClassname(), "trigger_jump") != 0)
               continue; // not a jumping pad, skip

            // find one at the same height of the bot & bot can reach it without falling
            if (fabs(GetAbsMin().z - p->GetAbsMin().z) < 32 && !Nav()->IsDeadlyDrop(p->Center())) {
               vecDest[iCount++] = p->Center();
               if (iCount >= 8)
                  break;
            }
         }
         if (iCount > 0) {
            m_vecDest = vecDest[RandomLong(0, iCount - 1)];
         }
      } else {
         // just move towards the destination...
         if (m_pEnemy != NULL)
            Nav()->MoveToward(m_vecDest);
         else
            Nav()->HeadToward(m_vecDest);
      }
#ifdef _DEBUG
      g_General.DrawBeam(NULL, GetOrigin(), m_vecDest, 10, 0, 255, 255, 255, 255, 10, 1);
#endif
   } else {
      m_vecDest = NULLVEC;
      bi.dir = NULLVEC;
   }
}

bool CBotRC::AINode_Normal()
{
   if (!IsAlive()) {
      AINODE_SWITCH(CAINode_RC_Respawn);
   }

   ShootAtEnemy();
   HeadToJumpPad();

   return false;
}
