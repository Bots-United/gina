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
// console.cpp
//
// handle console commands.
//

#include "main.h"

int g_iRadioCommandCS[MAX_PLAYERS]; // for Counter-Strike

inline const char *argument(int number)
{
   return g_General.GetArg(number + 1);
}


/////////////////////////////////////////////////////////////
//          C O N S O L E       C O M M A N D S            //
/////////////////////////////////////////////////////////////

void CMD_AddBot()
{
   // Create a bot. Usage: bot add [name | skill=n]
   g_pServer->BotManager()->BotAdd(argument(1));
}

void CMD_KickAllBots()
{
   // kick all bots away from the server
   g_pServer->BotManager()->KickAllBots();
}

void CMD_KillAllBots()
{
   // kill all bots
   g_pServer->BotManager()->KillAllBots();
}

void CMD_ShowHelp()
{
   // display the help message
   printf("*** The GINA bot (BUILD %d) *** \n", g_General.GetBuildNumber());
   printf("gina help - Show this message.\n");
   printf("gina debug <item> <on|off> - Turn on/off debugging for the specified item.\n");
   printf("gina add [name | skill=n] - Add a bot using the specified name or skill.\n");
   printf("gina killall - Kill all bots.\n");
   printf("gina kickall - Remove all bots from the server.\n");
}

void CMD_Debug()
{
#define DEBUG_COMMAND(cmd, flag, desc)                  \
{                                                       \
   if (!strcmp(argument(1), cmd)) {                     \
      if (!strcmp(argument(2), "on")) {                 \
         g_Debug.DebugOn(flag);                         \
      } else if (!strcmp(argument(2), "off")) {         \
         g_Debug.DebugOff(flag);                        \
      }                                                 \
      printf("DEBUG: " desc " is %s\n",                 \
         g_Debug.IsDebugOn(flag) ? "on" : "off");       \
      return;                                           \
   }                                                    \
}

   DEBUG_COMMAND("general", DEBUG_GENERAL, "General debugging");
   DEBUG_COMMAND("engine", DEBUG_ENGINEAPI, "Engine debugging");
   DEBUG_COMMAND("dll", DEBUG_DLLAPI, "DLL API debugging");
   DEBUG_COMMAND("error", DEBUG_ERROR, "Error logging");
   DEBUG_COMMAND("netmsg", DEBUG_NETMSG, "Net message tracing");
   DEBUG_COMMAND("ai", DEBUG_BOTAI, "Bot general AI debugging");
   DEBUG_COMMAND("nav", DEBUG_BOTNAV, "Bot navigation debugging");
   DEBUG_COMMAND("move", DEBUG_BOTMOVE, "Bot move debugging");
   DEBUG_COMMAND("combat", DEBUG_BOTCOMBAT, "Bot combat debugging");
}

// list of console commands
consolecmd_t commands[] = {
   {"add",         CMD_AddBot,          true},
   {"killall",     CMD_KillAllBots,     true},
   {"kickall",     CMD_KickAllBots,     true},
   {"help",        CMD_ShowHelp,        false},
   {"debug",       CMD_Debug,           false},
   {NULL, NULL, false} // terminator
};

// This is the real function to handle the console commands.
void BotServerCommand()
{
   const char *cmd = argument(0);
   consolecmd_t *pCommand = commands;

   while (pCommand->szCmd) {
      if (strcmpi(cmd, pCommand->szCmd) == 0) {
         // make sure the game has started if we need
         if (!pCommand->fInGame || g_pServer)
            pCommand->pFunc();
         else
            printf("BOT: cannot '%s', server is not running\n", cmd);
         return;
      }
      pCommand++;
   }

   printf("BOT: Unknown command. Type 'gina help' for more info.\n");
}

// Handle the client-side commands.
void BotClientCommand(CClient *pClient)
{
   if (!pClient || !pClient->IsValid())
      return; // reliablility check

   // detect chat messages here...
   const char *pcmd = g_General.GetArg(0);

   if (strcmpi(pcmd, "say") == 0 || strcmpi(pcmd, "say_team") == 0) {
      if (g_pServer) {
         bool is_teammsg = (strcmpi(pcmd, "say_team") == 0);
         for (int i = 0; i < g_pServer->GetMaxClients(); i++) {
            CClient *pTo = g_pServer->m_rgpClients[i];
            if (!pTo || !pTo->IsValid())
               continue; // reliability check
            if (!is_teammsg || (pClient->GetTeam() == pTo->GetTeam())) {
               if (g_General.m_iModid == MOD_CSTRIKE) {
                  // for Counter-Strike we also need to care about bot is
                  // alive or not
                  if (pClient->IsAlive() != pTo->IsAlive())
                     continue;
               }

               // let the chat manager handle this chat message
               // NOTE: as some engines other than HL may allow sending a message
               //       to a specific player, I'm handling this one by one
               g_pServer->BotManager()->chat.ChatMessage(pClient, pTo, g_General.GetArgs());
            }
         }
      }
   }

   // If we are running Counter-Strike...
   if (g_General.m_iModid == MOD_CSTRIKE && pClient->IsAlive()) {
      // detect radio messages here...
      int iIndex = pClient->entindex() - 1;

      if (strcmpi(pcmd, "radio1")) {
         g_iRadioCommandCS[iIndex] = 10;
      } else if (strcmpi(pcmd, "radio2")) {
         g_iRadioCommandCS[iIndex] = 20;
      } else if (strcmpi(pcmd, "radio3")) {
         g_iRadioCommandCS[iIndex] = 30;
      } else if (strcmpi(pcmd, "menuselect")) {
         // if this player's radio message menu is shown
         if (g_iRadioCommandCS[iIndex] != 0) {
            // this player has issued a message; handle this message
            g_iRadioCommandCS[iIndex] += atoi(g_General.GetArg(1));

            // handle this message actually...
            CServerCS *pServerCS = dynamic_cast<CServerCS *>(g_pServer);
            pServerCS->RadioMessage(pClient, g_iRadioCommandCS[iIndex]);

            g_iRadioCommandCS[iIndex] = 0;
         }
      }
   }
}

