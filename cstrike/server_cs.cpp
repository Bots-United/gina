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
// server_cs.cpp
//

#include "../main.h"

CServerCS::CServerCS()
{
   VersionCheck(); // check CS version
   g_pBuyMgrCS = new CCSBuyManager;
   if (!g_pBuyMgrCS)
      g_General.TerminateOnError("Memory allocation error !");
}

CServerCS::~CServerCS()
{
   if (g_pBuyMgrCS)
      delete g_pBuyMgrCS;
   g_pBuyMgrCS = NULL;
}

void CServerCS::VersionCheck()
{
   // see if we are running Counter-Strike 1.6...
   m_iCSVersion = CS_VERSION_WON; // assume we're running CS 1.5

   // only CS v1.6 has this file
   if (CGeneral::DoesFileExist("events/famas.sc"))
      m_iCSVersion = CS_VERSION_STEAM; // we're running CS 1.6

   g_Debug.DebugLog(DEBUG_GENERAL, "Note: assuming we are running CS %s",
      m_iCSVersion == CS_VERSION_STEAM ? "1.6" : "1.5");
}

int CServerCS::GetTeam(CClient *pClient)
{
   // returns the team of the client
   // we actually needn't check the full string, we just need
   // to check what the 3rd character is :)
   char t = (pClient->GetModel())[2];
   if (t == 'r' || // teRror
      t == 'e' || // leEt, guErilla
      t == 'c') // arCtic
      return CS_TEAM_TERRORIST;
   return CS_TEAM_CT; // urBan, gsG9, giGn, saS, viP
}

bool CServerCS::ClientIsImportant(CClient *pClient)
{
   // returns if the specified client is "important"
   if (!pClient)
      return false; // reliability check

   if (pClient->HasWeapon(CS_WEAPON_C4) || strcmp(pClient->GetModel(), "vip") == 0)
      return true; // always treats bomb carrier or VIP as important

   // see if there's any hostages around him
   CEntity *pEntity = NULL;
   while (pEntity = UTIL_FindEntityInSphere(pEntity, pClient->GetOrigin(), 256)) {
      if (strcmp(pEntity->GetClassname(), "hostage_entity") == 0)
         if (pClient->FVisible(pEntity))
            return true; // assume he's leading or approaching hostages
   }

   return false; // this client is not important
}

bool CServerCS::MessageWrite(void *p)
{
   static int message_HLTV = -1;
   static int message_TextMsg = -1;

   if (message_HLTV == -1) {
      message_HLTV = g_General.GetUserMsgId("HLTV");
      message_TextMsg = g_General.GetUserMsgId("TextMsg");
   }

   // return true when the message is handled, false if not.
   if (m_iCurrMsgType == message_HLTV) {
      if (m_iCurrMsgState == 0 && (*(int *)p) == 0)
         NewRound(); // new round for CS 1.6
   } else if (m_iCurrMsgType == message_TextMsg) {
      // is this message for all clients?
      if (!m_pCurrMsgClient) {
         if (strcmp((char *)p, "#Bomb_Planted") == 0) {
            // bomb is planted
            g_Debug.DebugLog(DEBUG_GENERAL, "*** The bomb is now PLANTED!!! ***");

            m_fBombPlanted = true; // remember it
            m_flTimeBombPlanted = GetTime();
         }
      }
   } else {
      return CServer::MessageWrite(p);
   }

   m_iCurrMsgState++;
   return true;
}

// this function gets called when a new round has started
void CServerCS::NewRound()
{
   g_Debug.DebugLog(DEBUG_GENERAL, "*** A new round has started ***");

   int i;

   // call "new round" function for all bots
   for (i = 0; i < GetMaxClients(); i++) {
      if (m_rgpClients[i]) {
         if (m_rgpClients[i]->IsBot()) {
            ((CBotCS *)m_rgpClients[i])->NewRound();
         }
      }
   }

   // ...also reset the array of radio commands
   extern int g_iRadioCommandCS[MAX_PLAYERS];
   for (i = 0; i < MAX_PLAYERS; i++) {
      g_iRadioCommandCS[i] = RADIO_NONE;
   }

   m_fBombPlanted = false;
}

CBaseBot *CServerCS::NewBotInstance()
{
   CBaseBot *pBot = new CBotCS;
   if (!pBot)
      g_General.TerminateOnError("Memory allocation error !");
   return pBot;
}

// This function gets called when the client pFrom has issued a radio
// message iRadioMsg
void CServerCS::RadioMessage(CClient *pFrom, int iRadioMsg)
{
   int iTeam = pFrom->GetTeam();

   for (int i = 0; i < GetMaxClients(); i++) {
      CClient *pTo = m_rgpClients[i];

      // if this client is NOT a bot OR this client is dead OR
      // this client is on the different side from who sent this radio message
      if (!pTo || !pTo->IsBot() || !pTo->IsAlive() || pTo->GetTeam() != iTeam)
         continue;

      ((CBotCS *)pTo)->HearRadio(pFrom, iRadioMsg);
   }
}

