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

/*******************************************************
 * bot.cpp                                             *
 *******************************************************
 * Purpose: Base bot handling.                         *
 * Engine-specific: no                                 *
 *******************************************************/

#include "main.h"

CBaseBot::CBaseBot():
m_iStartAction(0),
m_fIsStarted(false),
m_fNeedToInitialize(true),
m_flTimeThink(0),
m_flTimePrevThink(0),
m_iPrevHealth(0),
m_pAINode(NULL)
{
   bi.actionflags = 0;
   bi.dir = NULLVEC;
   bi.idealangles = NULLVEC;
   bi.weapon = 0;

   // initialize subsystems...
   nav = new CBotNav(this);
   chat = new CBotChat(this);
   if (!nav || !chat)
      g_General.TerminateOnError("Memory allocation error !");
}

CBaseBot::~CBaseBot()
{
   // delete subsystems...
   delete nav;
   delete chat;

   if (m_pAINode)
      delete m_pAINode; // free memory allocated for current AI node

   m_pProfile->is_used = false; // this profile is not used now
}

void CBaseBot::BotThink()
{
   if (!IsValid() || !GetNetName()[0]) {
      g_General.TerminateOnError("BotThink(): Bot entity is invalid!");
   }

   if (!m_fIsStarted) {
      // if the bot hasn't selected stuff to start the game yet, go do that...
      StartGame();
   } else {
      Chat()->Think(); // update the chatting subsystem

      if (!IsAlive()) {
         // if the bot is dead, run the dead think code (MOD specific)
         DeadThink();
         m_fNeedToInitialize = true;
         // delete the AI node data if exist
         if (m_pAINode)
            delete m_pAINode;
         m_pAINode = NULL;
      } else {
         if (m_fNeedToInitialize) {
            SpawnInit(); // initialize if we need to
            m_fNeedToInitialize = false;
         }

         if (m_flTimeThink < g_pServer->GetTime()) {
            BotAI(); // run the main bot AI routine at 10Hz

            m_flTimePrevThink = g_pServer->GetTime();
            m_flTimeThink = g_pServer->GetTime() + 0.1;
         }
      }
   }

   PerformMovement(); // Pass through the engine
}

void CBaseBot::DeadThink()
{
   // press fire to respawn...
   // NOTE: For some MODs like CS, we needn't press fire.
   // so leave this virtual...
   bi.actionflags |= ACTION_RESPAWN;
}

void CBaseBot::SpawnInit()
{
   ResetState();

   AIEnter(new CAINode_Normal(this)); // enter the normal AI node
}

void CBaseBot::FacePosition(const Vector &vecPos)
{
   // Adjust bots ideal angles to face an absolute vector
   bi.idealangles = (vecPos - GetGunPosition()).ToAngles();
   bi.idealangles.ClampAngles();
}

void CBaseBot::ResetState()
{
   // reset bot input parameters
   bi.actionflags = 0;
   bi.dir = NULLVEC;
   bi.idealangles = NULLVEC;
   bi.weapon = 0;

   m_iPrevHealth = 0;

   m_pEnemy = NULL; // null out the enemy pointer
   m_vecEnemy = NULLVEC;
   m_ucVisibility = 0;

   Nav()->Init(); // initialize the navigation system
}

void CBaseBot::DebugMsg(int flag, const char *fmt, ...)
{
#ifndef _DEBUG
   if (!g_Debug.IsDebugOn(flag))
      return; // debug for this flag is off; don't proceed
#endif

   va_list     argptr;
   char        string[256];

   va_start(argptr, fmt);
   _vsnprintf(string, sizeof(string), fmt, argptr);
   va_end(argptr);

   printf("[BOT DEBUG] %s: %s\n", GetNetName(), string); // print this message to the console
   g_Debug.DebugLog(flag, "(%s) %s", GetNetName(), string); // also store it to the log file
}

