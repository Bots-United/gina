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
// bot_gearbox_ctf.cpp
//

#include "../main.h"

CBotOP4CTF::CBotOP4CTF():
m_fHasRegen(false),
m_fHasLongJump(false),
m_fHasPhev(false),
m_fHasAccel(false),
m_fHasBpack(false),
m_iFlagState(FLAG_INBASE),
m_iEnemyFlagState(FLAG_INBASE)
{
}

CBotOP4CTF::~CBotOP4CTF()
{
}

bool CBotOP4CTF::MessageWrite(void *p)
{
   // return true when the message is handled, false if not.

   // handles net messages sent to this bot...
   static int message_VGUI = -1;
   static int message_StatusIcon = -1;

   if (message_VGUI == -1) {
      // cache the message ID in local static variables for speedup
      message_VGUI = g_General.GetUserMsgId("VGUIMenu");
      message_StatusIcon = g_General.GetUserMsgId("StatusIcon");
   }

   if (g_pServer->GetCurMsgType() == message_VGUI) {
      // This message is sent when the VGUI menu is displayed.

      // only take the first packet of a VGUI message (only want menu codes)
      if (g_pServer->GetCurMsgState() == 0) {
         if ((*(int *)p) == MENU_OPFOR_TEAM) {
            m_iStartAction = MSG_OPFOR_TEAM_SELECT; // team select menu
         } else if ((*(int *)p) == MENU_OPFOR_CLASS) {
            m_iStartAction = MSG_OPFOR_CLASS_SELECT; // model select menu
         }
      }
   } else if (g_pServer->GetCurMsgType() == message_StatusIcon) {
      // This message gets sent when an icon appears on the bot's HUD

      static unsigned char byEnable;
      if (g_pServer->GetCurMsgState() == 0) {
         // get the messagestate of the icon (either off, lit, or blinking)
         byEnable = *(unsigned char *)p;
      } else if (g_pServer->GetCurMsgState() == 1) {
         if (strcmp((char *)p, "item_ctfregen") == 0) {
            // is it the regen icon ?
            m_fHasRegen = (byEnable != 0); // remember regen icon state
         } else if (strcmp((char *)p, "item_ctfljump") == 0) {
            // else is it the long jump icon ?
            m_fHasLongJump = (byEnable != 0); // remember long jump icon state
         } else if (strcmp((char *)p, "item_ctfaccel") == 0) {
            // else is it the accelerator icon ?
            m_fHasAccel = (byEnable != 0); // remember accelerator icon state
         } else if (strcmp((char *)p, "item_ctfbpack") == 0) {
            // else is it the backpack icon ?
            m_fHasBpack = (byEnable != 0); // remember backpack icon state
         } else if (strcmp((char *)p, "item_ctfphev") == 0) {
            // else is it the shield icon ?
            m_fHasPhev = (byEnable != 0); // remember shield icon state
         }
      }
   } else if (g_pServer->GetCurMsgType() == g_General.GetUserMsgId("FlagIcon")){
      // This message gets sent when the flag icon is changed

      static int flag_state;
      if (g_pServer->GetCurMsgState() == 1) {
         if (!strcmp((char *)p, "item_ctfflagh"))
            flag_state = FLAG_INBASE; // flag is in base
         else if (!strcmp((char *)p, "item_ctfflagd"))
            flag_state = FLAG_LOST; // flag is lost
         else if (!strcmp((char *)p, "item_ctfflagg"))
            flag_state = FLAG_CARRIED; // flag is being carried by this bot
         else if (!strcmp((char *)p, "item_ctfflagc"))
            flag_state = FLAG_STOLEN; // flag is being carried by another player
      } else if (g_pServer->GetCurMsgState() == 2) {
         if (*(int *)p == 0) {
            // this message is for the scientist team
            // remember the flag state
            if (GetTeam() == OP4_TEAM_CIVILIAN)
               m_iFlagState = flag_state;
            else
               m_iEnemyFlagState = flag_state;
         } else if (*(int *)p == 1) {
            // this message is for the soldier team
            // remember the flag state
            if (GetTeam() == OP4_TEAM_MILITARY)
               m_iFlagState = flag_state;
            else
               m_iEnemyFlagState = flag_state;
         }
      }
   } else {
      return CClient::MessageWrite(p); // message not handled
   }

   return true; // message handled
}

/**
 * The purpose of this function is to make the bot browse down through
 * the team and class selection menus of Counter-Strike.
 */
void CBotOP4CTF::StartGame()
{
   int select;

   switch (m_iStartAction) {
   case MSG_OPFOR_TEAM_SELECT:
      // the team selection menu...
      select = m_pProfile->team;

      if (select < 1 || select > 2)
         select = 3; // use auto team assignment

      // issue the command for choosing the team
      m_iStartAction = MSG_OPFOR_IDLE; // switch back to idle
      g_General.FakeClientCommand(this, "jointeam %d", select);
      break;

   case MSG_OPFOR_CLASS_SELECT:
      select = m_pProfile->bot_class;

      // the class selection menu...
      if (select < 1 || select > 6)
         select = RandomLong(1, 6); // if invalid use random

      // issue the command for choosing the class
      m_iStartAction = MSG_OPFOR_IDLE; // switch back to idle
      g_General.FakeClientCommand(this, "selectchar %d", select);
      m_fIsStarted = true; // bot should be ready to fight now
      break;
   }
}

void CBotOP4CTF::SpawnInit()
{
   CBaseBot::SpawnInit(); // call initialize function in base class
}

CBotGoal CBotOP4CTF::FindGoal()
{
   // TODO TODO TODO
   CBotGoal goal;
   goal.flags = 0;
   goal.maxs = goal.mins = goal.origin = NULLVEC;
   goal.pEntity = NULL;
   return goal;
}

bool CBotOP4CTF::AINode_Normal()
{
   return false; // TODO TODO TODO
}

