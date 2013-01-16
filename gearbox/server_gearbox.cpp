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
// server_gearbox.cpp
//

#include "../main.h"

#define MAX_TEAMS                   32
#define MAX_TEAMNAME_LENGTH         16

// default player models for Gearbox's Opposing Force...
const int NUM_GEARBOX_MODELS = 20;
char *gearbox_bot_models[] = {
   "barney", "beret", "cl_suit", "drill", "fassn", "gina", "gman",
   "gordon", "grunt", "helmet", "hgrunt", "massn", "otis", "recon",
   "recruit", "robo", "scientist", "shephard", "tower", "zombie"};

CServerOP4::CServerOP4()
{
}

CServerOP4::~CServerOP4()
{
}

int CServerOP4::GetTeam(CClient *pClient)
{
   static char team_names[MAX_TEAMS][MAX_TEAMNAME_LENGTH];
   static int num_teams = 0;

   if (!m_fTeamChecked) {
      m_fTeamChecked = true;

      char *pName;
      char teamlist[MAX_TEAMS * MAX_TEAMNAME_LENGTH];
      int i;

      num_teams = 0;
      strcpy(teamlist, CVAR_GET_STRING("mp_teamlist"));
      pName = teamlist;
      pName = strtok(pName, ";");

      while (pName != NULL && *pName) {
         // check that team isn't defined twice
         for (i = 0; i < num_teams; i++)
            if (stricmp(pName, team_names[i]) == 0)
               break;

         if (i == num_teams) {
            strcpy(team_names[num_teams], pName);
            num_teams++;
         }

         pName = strtok(NULL, ";");
      }
   }

   const char *model_name = pClient->GetModel();

   for (int index = 0; index < num_teams; index++)
      if (stricmp(model_name, team_names[index]) == 0)
         return index;

   return -1; // team is unknown
}

CBaseBot *CServerOP4::NewBotInstance()
{
   CBaseBot *pBot = new CBotOP4;
   if (!pBot)
      g_General.TerminateOnError("Memory allocation error !");
   return pBot;
}

