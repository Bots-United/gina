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
// server.cpp
//

#include "main.h"

CServer *g_pServer = NULL;

extern "C" void player(entvars_t *pev);

// This function gets called at the map start
CServer::CServer():
m_flMsecvalRest(0),
m_flPrevTime(0),
m_iMsecval(0),
m_fTeamChecked(false)
{
   // initialize the clients array...
   memset(m_rgpClients, 0, sizeof(CClient *) * 32);
   m_pBotManager = new CBotManager;
   m_pWorld = new CWorld;
   if (!m_pBotManager || !m_pWorld)
      g_General.TerminateOnError("Memory allocation error !");
}

// This function gets called when the server is shut down
CServer::~CServer()
{
   // free all the memory allocated for all clients (including bots)...
   for (int i = 0; i < 32; i++) {
      if (m_rgpClients[i]) {
         delete m_rgpClients[i];
         m_rgpClients[i] = NULL;
      }
   }

   delete m_pBotManager;
   delete m_pWorld;
}

void CServer::UpdateMsec()
{
   // Adapted from POD-Bot mm by KWo
   float flMsecval = (GetTime() - m_flPrevTime) * 1000.0;
   m_flPrevTime = GetTime();

   int iMsecvalRest = 0;
   m_iMsecval = (int)flMsecval;

   if (m_iMsecval < 10) {
      flMsecval -= m_iMsecval + m_flMsecvalRest;
      iMsecvalRest = flMsecval;
      m_flMsecvalRest = flMsecval - iMsecvalRest;
   }
   m_iMsecval += iMsecvalRest;

   if (m_iMsecval < 1)
      m_iMsecval = 1; // don't allow the msec delay to be too low
   else if (m_iMsecval > 100)
      m_iMsecval = 100; // don't allow it to last longer than 100 milliseconds either
}

void CServer::Think()
{
   int i;

   UpdateMsec(); // calculate the msec value
   CheckClients(); // check and update our client list

   BotManager()->Think();
   World()->Think();

   for (i = 0; i < GetMaxClients(); i++) {
      if (m_rgpClients[i] != NULL) {
         // check if this client is a bot
         CBaseBot *pBot = m_rgpClients[i]->GetBotPointer();
         if (pBot) {
            // this is a bot...
            pBot->BotThink(); // call its think function to make it run
         }
      }
   }
}

void CServer::CheckClients()
{
   // check if any clients (or bots) have joined the game or
   // disconnected, so that we can update the list we have...
   for (int i = 0; i < GetMaxClients(); i++) {
      edict_t *pPlayer = INDEXENT(i + 1);
      if (!FNullEnt(pPlayer) && !pPlayer->free && STRING(pPlayer->v.netname)[0]) {
         if (m_rgpClients[i] == NULL) {
            // a client has joined the game
            g_Debug.DebugLog(DEBUG_GENERAL, "client index #%d joined the game", i + 1);
            m_rgpClients[i] = new CClient; // add him/her to the list
            m_rgpClients[i]->pev = VARS(pPlayer);
         }
      } else {
         if (m_rgpClients[i] != NULL) {
            // a client (or bot) has disconnected from the game
            g_Debug.DebugLog(DEBUG_GENERAL, "client index #%d left the game", i + 1);
            delete m_rgpClients[i]; // delete him/her/it from the list
            m_rgpClients[i] = NULL;
         }
      }
   }
}

bool CServer::IsTeamplay(void)
{
   // This function returns if the game is Teamplay or not
   static bool bChecked = false;
   static bool bIsTeamplay;

   if (!bChecked) {
      // use server CVAR to check if it's teamplay or not
      bIsTeamplay = (CVAR_GET_FLOAT("mp_teamplay") != 0);
      bChecked = true; // set the checked flag
   }

   return bIsTeamplay;
}

void CServer::MessageBegin(int iType, edict_t *ed)
{
   m_iCurrMsgType = iType;
   m_pCurrMsgClient = CClient::Instance(ed);
   m_iCurrMsgState = 0;
}

bool CServer::MessageWrite(void *p)
{
   // return true if message is handled, false if not
   if (m_pCurrMsgClient) {
      if (m_pCurrMsgClient->MessageWrite(p)) {
         m_iCurrMsgState++;
         return true;
      }
   }

   static int message_WeaponList = -1;
   static int message_DeathMsg = -1;

   if (message_WeaponList == -1) {
      message_WeaponList = g_General.GetUserMsgId("WeaponList");
      message_DeathMsg = g_General.GetUserMsgId("DeathMsg");
   }

   if (m_iCurrMsgType == message_WeaponList) {
      static weaponinfo_t bot_weapon;
      if (m_iCurrMsgState == 0)
         strncpy(bot_weapon.szClassname, (char *)p, 64);
      else if (m_iCurrMsgState == 1)
         bot_weapon.iAmmo1 = *(int *)p;  // ammo index 1
      else if (m_iCurrMsgState == 2)
         bot_weapon.iMaxAmmo1 = *(int *)p;  // max ammo1
      else if (m_iCurrMsgState == 3)
         bot_weapon.iAmmo2 = *(int *)p;  // ammo index 2
      else if (m_iCurrMsgState == 4)
         bot_weapon.iMaxAmmo2 = *(int *)p;  // max ammo2
      else if (m_iCurrMsgState == 5)
         bot_weapon.iSlot = *(int *)p;  // slot for this weapon
      else if (m_iCurrMsgState == 6)
         bot_weapon.iPosition = *(int *)p;  // position in slot
      else if (m_iCurrMsgState == 7)
         bot_weapon.iId = *(int *)p;  // weapon ID
      else if (m_iCurrMsgState == 8) {
         bot_weapon.iFlags = *(int *)p;  // flags for weapon
         // store away this weapon with it's ammo information...
         g_General.m_rgWeaponDefs[bot_weapon.iId] = bot_weapon;
      }
   } else if (m_iCurrMsgType == message_DeathMsg) {
      static int killer_index;
      static int victim_index;

      if (m_iCurrMsgState == 0) {
         killer_index = *(int *)p - 1;  // index of killer
      } else if (m_iCurrMsgState == 1) {
         victim_index = *(int *)p - 1;  // index of victim

         if (killer_index >= 0 && victim_index >= 0 &&
			killer_index < MAX_PLAYERS && victim_index < MAX_PLAYERS) {
            CClient *pKiller = m_rgpClients[killer_index];
            CClient *pVictim = m_rgpClients[victim_index];

            if (pKiller->IsBot()) {
               (dynamic_cast<CBaseBot *>(pKiller))->Chat()->m_strLastVictim = pVictim->GetNetName();
            }

            if (pVictim->IsBot()) {
               (dynamic_cast<CBaseBot *>(pVictim))->Chat()->m_strLastVictim = pKiller->GetNetName();
            }
         }
      }
   } else {
      return false;
   }

   m_iCurrMsgState++;
   return true;
}

void CServer::AddBot(CBaseBot *pBot)
{
   m_rgpClients[pBot->entindex() - 1] = (CClient *)pBot;
}

void CServer::BotCreate(struct bot_profile_s *profile)
{
   edict_t *BotEnt = CreateFakeClient(profile->name);

   if (!FNullEnt(BotEnt)) {
      char *infobuffer = GET_INFOBUFFER(BotEnt);
      int clientIndex = ENTINDEX(BotEnt);

      CBaseBot *pBot = NewBotInstance();
      pBot->pev = &BotEnt->v;
      pBot->m_pProfile = profile;

      if (!IsTeamplay()) {
         SET_CLIENT_KEY_VALUE(clientIndex, infobuffer, "model", profile->skin);

         // use random colors
         SET_CLIENT_KEY_VALUE(clientIndex, infobuffer, "topcolor", va("%d", RandomLong(0, 255)));
         SET_CLIENT_KEY_VALUE(clientIndex, infobuffer, "bottomcolor", va("%d", RandomLong(0, 255)));
      }

      AddBot(pBot);

      pBot->m_pProfile->is_used = true;
   }
}

edict_t *CServer::CreateFakeClient(const char *name)
{
   edict_t *pEdict;
   pEdict = CREATE_FAKE_CLIENT(name);

   if (!FNullEnt(pEdict)) {
      char ptr[128]; // allocate space for message from ClientConnect

      if (pEdict->pvPrivateData != NULL)
         FREE_PRIVATE(pEdict); // free our predecessor's private data
      pEdict->pvPrivateData = NULL; // null out the private data pointer

      pEdict->v.frags = 0; // reset his frag count

      // set the max speed for this player
      pEdict->v.maxspeed = CVAR_GET_FLOAT("sv_maxspeed");

      // create the player entity by calling MOD's player function
      // (from LINK_ENTITY_TO_CLASS for player object)
      if (g_fIsMetamod) {
         CALL_GAME_ENTITY(PLID, "player", VARS(pEdict));
      } else {
         player(VARS(pEdict));
      }

      MDLL_ClientConnect(pEdict, "bot", "127.0.0.1", ptr);
      MDLL_ClientPutInServer(pEdict); // let this bot actually spawn into the game

      return pEdict;
   }

   return NULL;
}

// find a pointer to a client with the specified name
CClient *CServer::FindClientByName(const char *netname)
{
   for (int i = 0; i < GetMaxClients(); i++) {
      CClient *pClient = m_rgpClients[i];
      if (pClient && !strcmp(pClient->GetNetName(), netname))
         return pClient;
   }

   return NULL; // client with this name is not found
}

