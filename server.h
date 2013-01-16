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
// server.h
//

#ifndef _SERVER_H
#define _SERVER_H

#include "client.h"

class CBotManager;
class CWorld;

class CServer
{
public:
   CServer();
   virtual ~CServer();

   CClient *m_rgpClients[MAX_PLAYERS];

   inline float GetTime(void)      { return gpGlobals->time; }
   inline float GetFrameTime(void) { return gpGlobals->frametime; }
   inline int GetMaxClients(void)  { return gpGlobals->maxClients; }
   inline int GetMaxEntities(void) { return gpGlobals->maxEntities; }
   inline const char *GetMapName(void) { return STRING(gpGlobals->mapname); }

   void Think(void);

   // for round based game like Counter-Strike
   virtual void NewRound(void)     { return; }

   // Returns the team of the player
   // This should be MOD dependant so it's virtual function
   virtual int GetTeam(CClient *pClient) { return pClient->pev->team - 1; }
   virtual bool ClientIsImportant(CClient *pClient) { return false; }

   inline int GetMsec(void) { return m_iMsecval; }

   // For HL network message handling:
   void          MessageBegin(int iType, edict_t *ed);
   virtual bool  MessageWrite(void *p);

   inline int    GetCurMsgType(void)  { return m_iCurrMsgType; }
   inline int    GetCurMsgState(void) { return m_iCurrMsgState; }

   inline bool   IsMultiplay(void)    { return ((int)gpGlobals->deathmatch != 0); }
   virtual bool  IsTeamplay(void);

   inline bool   IsDedicated(void)    { return (IS_DEDICATED_SERVER() != 0); }

   // host client is always the first one in this server
   inline CClient *GetHostClient(void)   { return IsDedicated() ? NULL : (m_rgpClients[0]->IsValid() ? m_rgpClients[0] : NULL); }

   void              BotCreate(struct bot_profile_s *profile);
   virtual CBaseBot *NewBotInstance(void) { return NULL; }

   CClient        *FindClientByName(const char *netname);

   inline float CvarGetFloat(const char *szVarName) { return CVAR_GET_FLOAT(szVarName); }
   inline const char *CvarGetString(const char *szVarName) { return CVAR_GET_STRING(szVarName); }

   inline CBotManager *BotManager(void) { return m_pBotManager; }
   inline CWorld      *World(void)      { return m_pWorld; }

   virtual bool   IsWorldRequired(void) { return true; }

protected:
   int           m_iCurrMsgType;
   int           m_iCurrMsgState;
   CClient      *m_pCurrMsgClient;

   bool          m_fTeamChecked;

   void          CheckClients(void);
   void          AddBot(CBaseBot *pBot);
   edict_t      *CreateFakeClient(const char *name);

private:
   void          UpdateMsec(void);

   // for msec computing
   float         m_flMsecvalRest;
   float         m_flPrevTime;
   int           m_iMsecval;

   CBotManager  *m_pBotManager;
   CWorld       *m_pWorld;
};

extern CServer *g_pServer;

#endif
