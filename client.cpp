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
// client.cpp
//

#include "main.h"

CClient::CClient():
m_iCurrentWeaponId(-1),
m_flLastShootTime(0)
{
   int i;
   for (i = 0; i < MAX_AMMO_SLOTS; i++)
      m_rgAmmo[i] = 0;
   for (i = 0; i < MAX_WEAPONS; i++)
      m_rgAmmoInClip[i] = -1;
}

CClient::~CClient()
{
}

int CClient::GetTeam()
{
   return g_pServer->GetTeam(this);
}

bool CClient::MessageWrite(void *p)
{
   // return true when the message is handled, false if not.
   static int iState, iId;

   int iMsgType = g_pServer->GetCurMsgType();
   int iMsgState = g_pServer->GetCurMsgState();

   CBaseBot *pBot = NULL;
   if (IsBot()) {
      pBot = dynamic_cast<CBaseBot *>(this);
   }

   // we don't like hundreds of strcmp()'s when one message gets sent
   // so cache them here...
   static int message_CurWeapon = -1;
   static int message_AmmoX = -1;
   static int message_Damage = -1;

   if (message_CurWeapon == -1) {
      message_CurWeapon = g_General.GetUserMsgId("CurWeapon");
      message_AmmoX = g_General.GetUserMsgId("AmmoX");
      message_Damage = g_General.GetUserMsgId("Damage");
   }

   if (iMsgType == message_CurWeapon) {
      if (iMsgState == 0)
         iState = *(int *)p; // get the state of the current weapon
      else if (iMsgState == 1)
         iId = *(int *)p; // get the weapon ID of current weapon
      else if (iMsgState == 2) {
         int iClip = *(int *)p; // get the ammo currently in the clip for this weapon
         if (iId >= 0 && iId < MAX_WEAPONS) {
            // Ammo amount decreased? Must have fired a bullet...
            if (iId == m_iCurrentWeaponId && iClip < m_rgAmmoInClip[iId])
               m_flLastShootTime = gpGlobals->time; // remember the last bullet time
            m_rgAmmoInClip[iId] = iClip;
            if (iState != 0)
               m_iCurrentWeaponId = iId;
         }
      }
   } else if (iMsgType == message_AmmoX) {
      if (iMsgState == 0)
         iId = *(int *)p; // get the ammo index (for knowing what type of ammo it is)
      else if (iMsgState == 1)
         if (iId >= 0 && iId < MAX_AMMO_SLOTS)
            m_rgAmmo[iId] = *(int *)p; // amount of ammo currently available
   } else if (iMsgType == message_Damage) {
      // This message gets sent when the client is getting damaged
      // only proceed if this is a bot
      if (IsBot()) {
         assert(pBot != NULL);

         static int damage_armor;
         static int damage_taken;
         static int damage_bits;
         static Vector damage_origin;

         if (iMsgState == 0) {
            damage_armor = *(int *)p; // get the amount of damage inflicted to armor
         } else if (iMsgState == 1) {
            damage_taken = *(int *)p; // get the amount of damage inflicted to health
         } else if (iMsgState == 2) {
            damage_bits = *(int *)p; // get the type of damage it is (bullet, fall, heat, etc...)
         } else if (iMsgState == 3) {
            damage_origin.x = *(float *)p; // get the x coordinate of the damage origin vector
         } else if (iMsgState == 4) {
            damage_origin.y = *(float *)p; // get the y coordinate of the damage origin vector
         } else if (iMsgState == 5) {
            damage_origin.z = *(float *)p; // get the z coordinate of the damage origin vector

            // If bot is damaged...
            if (damage_armor > 0 || damage_taken > 0) {
               pBot->TakeDamage(GetDamageInflictor(), damage_origin,
                  damage_taken, damage_armor, damage_bits);
            }
         }
      }
   } else {
      return false;
   }

   return true;
}

void CClient::Kick()
{
   // kick this client away from the server
   SERVER_COMMAND(va("kick \"%s\"\n", GetNetName()));
}

void CClient::Kill()
{
   // kill this client
   MDLL_ClientKill(edict());
}

CClient *CClient::Instance(edict_t *pent)
{
   if (FNullEnt(pent) || !g_pServer)
      return NULL; // reliability check

   int index = ENTINDEX(pent) - 1;
   if (index < 0 || index >= 32)
      return NULL;

   return g_pServer->m_rgpClients[index];
}

// print a text message to this client
void CClient::TextMessage(msg_dest iMsgDest, const char *fmt, ...)
{
   assert(IsValid());
   if (!IsValid() || IsBot() || (GetFlags() & FL_FAKECLIENT))
      return; // don't try to print text messages for bots

   va_list argptr;
   static char string[256];

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   _vsnprintf(string, sizeof(string), fmt, argptr);
   va_end(argptr);

   PRINT_TYPE ptype = print_console;

   switch (iMsgDest) {
   case TEXT_CENTER:
      ptype = print_center;
      break;
   case TEXT_CONSOLE:
      ptype = print_console;
      break;
   case TEXT_SAY:
      ptype = print_chat;
      break;
   }

   g_engfuncs.pfnClientPrintf(edict(), ptype, string);
}

