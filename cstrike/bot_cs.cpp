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
// bot_cs.cpp
//

#include "../main.h"

CBotCS::CBotCS():
m_iAccount(0),
m_fHasDefuser(false),
m_fInBombZone(false),
m_fInBuyZone(false),
m_iBuyCount(0),
m_flBuyTime(0)
{
}

CBotCS::~CBotCS()
{
}

void CBotCS::NewRound()
{
   m_fNeedToInitialize = true;
}

bool CBotCS::MessageWrite(void *p)
{
   // return true when the message is handled, false if not.

   // handles net messages sent to this bot...
   static int message_VGUI = -1;
   static int message_ShowMenu = -1;
   static int message_Money = -1;
   static int message_StatusIcon = -1;
   static int message_ScreenFade = -1;

   if (message_VGUI == -1) {
      // cache the message ID in local static variables for speedup
      message_VGUI = g_General.GetUserMsgId("VGUIMenu");
      message_ShowMenu = g_General.GetUserMsgId("ShowMenu");
      message_Money = g_General.GetUserMsgId("Money");
      message_StatusIcon = g_General.GetUserMsgId("StatusIcon");
      message_ScreenFade = g_General.GetUserMsgId("ScreenFade");
   }

   if (g_pServer->GetCurMsgType() == message_VGUI) {
      // This message is sent when the VGUI menu is displayed.

      // only take the first packet of a VGUI message (only want menu codes)
      if (g_pServer->GetCurMsgState() == 0) {
         if ((*(int *)p) == MENU_CS_TEAM) {
            m_iStartAction = MSG_CS_TEAM_SELECT; // team select menu
         } else if ((*(int *)p) == MENU_CS_TERRORIST || (*(int *)p) == MENU_CS_CT) {
            m_iStartAction = MSG_CS_CLASS_SELECT; // model select menu
         }
      }
   } else if (g_pServer->GetCurMsgType() == message_ShowMenu) {
      // This message is sent when a menu is being displayed in Counter-Strike

      // only take the fourth packet of a ShowMenu message (only want menu codes)
      if (g_pServer->GetCurMsgState() >= 3) {
         if (strcmp((char *)p, "#Team_Select") == 0) {
            // team select menu?
            m_iStartAction = MSG_CS_TEAM_SELECT;
         } else if (strcmp ((char *) p, "#Terrorist_Select") == 0 ||
            strcmp((char *) p, "#CT_Select") == 0) {
            // model select menu?
            m_iStartAction = MSG_CS_CLASS_SELECT;
         }
      }
   } else if (g_pServer->GetCurMsgType() == message_Money) {
      // This message gets sent when bots money amount is changed

      if (g_pServer->GetCurMsgState() == 0) {
         m_iAccount = *(int *)p;
      }
   } else if (g_pServer->GetCurMsgType() == message_StatusIcon) {
      // This message gets sent when an icon appears on the bot's HUD

      static unsigned char byEnable;
      if (g_pServer->GetCurMsgState() == 0) {
         // get the messagestate of the icon (either off, lit, or blinking)
         byEnable = *(unsigned char *)p;
      } else if (g_pServer->GetCurMsgState() == 1) {
         if (strcmp((char *)p, "c4") == 0) {
            // is it the C4 icon?
            m_fInBombZone = (byEnable == 2); // remember bomb icon state
         } else if (strcmp((char *)p, "defuser") == 0) {
            // else is it the defuse kit icon?
            m_fHasDefuser = (byEnable != 0); // remember defuse kit bomb icon state
         } else if (strcmp((char *)p, "buyzone") == 0) {
            // else is it the buy zone cart icon?
            m_fInBuyZone = (byEnable != 0); // remember buy zone cart icon state
         }
      }
   } else if (g_pServer->GetCurMsgType() == message_ScreenFade) {
      // This message gets sent when the bots are flashbang'ed

   } else {
      return CClient::MessageWrite(p); // message not handled
   }

   return true; // message handled
}

/**
 * The purpose of this function is to make the bot browse down through
 * the team and class selection menus of Counter-Strike.
 */
void CBotCS::StartGame()
{
   int select;

   switch (m_iStartAction) {
   case MSG_CS_TEAM_SELECT:
      // the team selection menu...
      select = m_pProfile->team;

      if (select < 1 || select > 2)
         select = 5; // use auto team assignment

      // issue the command for choosing the team
      m_iStartAction = MSG_CS_IDLE; // switch back to idle
      g_General.FakeClientCommand(this, "menuselect %d", select);
      break;

   case MSG_CS_CLASS_SELECT:
      // the class selection menu...
      select = m_pProfile->bot_class;

      if (select < 1 || select > 4)
         select = RandomLong(1, 4); // if invalid use random

      // issue the command for choosing the class
      m_iStartAction = MSG_CS_IDLE; // switch back to idle
      g_General.FakeClientCommand(this, "menuselect %d", select);
      m_fIsStarted = true; // bot should be ready to fight now
      break;
   }
}

void CBotCS::SpawnInit()
{
   m_flBuyTime = g_pServer->GetTime() + 1.5 + RandomFloat(0, 1.0);
   m_iBuyCount = 0; // zero out the buy count

   CBaseBot::SpawnInit(); // call initialize function in base class

   AIEnter(new CAINode_CS_Buy(this)); // buy weapons first
}

// Find a long term goal
CBotGoal CBotCS::FindGoal()
{
   // TODO TODO TODO
   CBotGoal goal;
   goal.flags = 0;
   goal.maxs = goal.mins = goal.origin = NULLVEC;
   goal.pEntity = NULL;
   return goal;
}

// This function gets called when bot hears a radio message
void CBotCS::HearRadio(CClient *pFrom, int iMsg)
{
   DebugMsg(DEBUG_BOTTEAM, "Heard radio message %d from %s", iMsg, pFrom->GetNetName());
}

// Send a radio message
void CBotCS::SendRadio(int iMsg)
{
   g_General.FakeClientCommand(this, "radio%d", iMsg / 10 + 1); // open the menu
   g_General.FakeClientCommand(this, "menuselect %d", iMsg % 10); // select the item
}

// Returns if enemy can be shoot through some obstacle
// adapted from POD-Bot by Count Floyd (http://podbot.nuclearbox.com)
bool CBotCS::IsShootableThruObstacle(const Vector &vecDest)
{
   Vector vecSrc = GetGunPosition();
   Vector vecDir = (vecDest - vecSrc).Normalize(); // 1 unit long
   Vector vecPoint;
   int iThickness = 0;
   int iHits = 0;

   traceresult_t tr = TestLine(vecSrc, vecDest, true, this);

   while (tr.fraction != 1.0 && iHits < 3) {
      iHits++;
      iThickness++;
      vecPoint = tr.endpos + vecDir;

      while (!PointBelongsToWorld(vecPoint) && iThickness < 64) {
         vecPoint = vecPoint + vecDir;
         iThickness++;
      }

      tr = TestLine(vecPoint, vecDest, true, this);
   }

   if (iHits < 3 && iThickness < 64) {
      float f_distance = (vecDest - vecPoint).Length();
      if (f_distance < 112)
         return true;
   }

   return false;
}

// Get the aggression of the bot
int CBotCS::BotAggression()
{
   // If the enemy is located way higher than the bot...
   if (m_pEnemy && m_pEnemy->IsValid()) {
      if (m_pEnemy->GetOrigin().z - GetOrigin().z > 200) {
         return 0; // bot is feeling no good
      }
   }

   // If the bot is very low on health...
   if (GetHealth() < 30) {
      return 0; // don't be aggressive
   }

   // If the bot is low on health, and has insufficient armor...
   if (GetHealth() < 50 && GetArmor() < 60) {
      return 0;
   }

   // check if we have enough ammo...

   // First check Assault Rifles
   // If the bot can use the AK47...
   if (HasWeapon(CS_WEAPON_AK47) && GetAmmoClip(CS_WEAPON_AK47) > 10)
      return 100;

   // If the bot can use the M4A1...
   if (HasWeapon(CS_WEAPON_M4A1) && GetAmmoClip(CS_WEAPON_M4A1) > 10)
      return 100;

   // If the bot can use the FAMAS...
   if (HasWeapon(CS_WEAPON_FAMAS) && GetAmmoClip(CS_WEAPON_FAMAS) > 10)
      return 100;

   // If the bot can use the GALIL...
   if (HasWeapon(CS_WEAPON_GALIL) && GetAmmoClip(CS_WEAPON_GALIL) > 10)
      return 100;

   // If the bot can use the SG552...
   if (HasWeapon(CS_WEAPON_SG552) && GetAmmoClip(CS_WEAPON_SG552) > 10)
      return 95;

   // If the bot can use the AUG...
   if (HasWeapon(CS_WEAPON_AUG) && GetAmmoClip(CS_WEAPON_AUG) > 10)
      return 95;

   // Then check shotguns
   // If the bot can use the M3...
   if (HasWeapon(CS_WEAPON_M3) &&
      (GetAmmoClip(CS_WEAPON_M3) > 3 || GetAmmoPrimary(CS_WEAPON_M3) > 5))
      return 95;

   // If the bot can use the XM1014...
   if (HasWeapon(CS_WEAPON_M3) &&
      (GetAmmoClip(CS_WEAPON_M3) > 3 || GetAmmoPrimary(CS_WEAPON_M3) > 5))
      return 100;

   // then check SMGs
   // If the bot can use the MP5...
   if (HasWeapon(CS_WEAPON_MP5NAVY) && GetAmmoClip(CS_WEAPON_MP5NAVY) > 10)
      return 100;

   // If the bot can use the TMP...
   if (HasWeapon(CS_WEAPON_TMP) && GetAmmoClip(CS_WEAPON_TMP) > 10)
      return 80;

   // If the bot can use the P90...
   if (HasWeapon(CS_WEAPON_P90) && GetAmmoClip(CS_WEAPON_P90) > 10)
      return 90;

   // If the bot can use the MAC10...
   if (HasWeapon(CS_WEAPON_MAC10) && GetAmmoClip(CS_WEAPON_MAC10) > 10)
      return 90;

   // If the bot can use the UMP45...
   if (HasWeapon(CS_WEAPON_UMP45) && GetAmmoClip(CS_WEAPON_UMP45) > 10)
      return 90;

   // then check sniper rifles
   // If the bot can use SCOUT...
   if (HasWeapon(CS_WEAPON_SCOUT) && GetAmmoClip(CS_WEAPON_SCOUT) > 2)
      return 40;

   // If the bot can use AWP...
   if (HasWeapon(CS_WEAPON_AWP) && GetAmmoClip(CS_WEAPON_AWP) > 2)
      return 30;

   // If the bot can use G3SG1...
   if (HasWeapon(CS_WEAPON_G3SG1) && GetAmmoClip(CS_WEAPON_G3SG1) > 8)
      return 50;

   // If the bot can use SG550...
   if (HasWeapon(CS_WEAPON_SG550) && GetAmmoClip(CS_WEAPON_SG550) > 8)
      return 50;

   // then check machine guns...
   // If the bot can use M249...
   if (HasWeapon(CS_WEAPON_M249) && GetAmmoClip(CS_WEAPON_M249) > 10)
      return 90;

   // then check grenades...
   // If the bot can use HE Grenade...
   if (HasWeapon(CS_WEAPON_HEGRENADE))
      return 80;

   // If the bot can use FlashBang...
   if (HasWeapon(CS_WEAPON_FLASHBANG))
      return 75;

   // If the bot can use Smoke Grenade...
   if (HasWeapon(CS_WEAPON_SMOKEGRENADE))
      return 80;

   // at last, check pistols...
   // If the bot can use USP...
   if (HasWeapon(CS_WEAPON_USP) && GetAmmoClip(CS_WEAPON_USP) > 3)
      return 50;

   // If the bot can use GLOCK18...
   if (HasWeapon(CS_WEAPON_GLOCK18) && GetAmmoClip(CS_WEAPON_GLOCK18) > 5)
      return 50;

   // If the bot can use DEAGLE...
   if (HasWeapon(CS_WEAPON_DEAGLE) && GetAmmoClip(CS_WEAPON_DEAGLE) > 2)
      return 60;

   // If the bot can use P228...
   if (HasWeapon(CS_WEAPON_P228) && GetAmmoClip(CS_WEAPON_P228) > 3)
      return 55;

   // If the bot can use ELITE...
   if (HasWeapon(CS_WEAPON_ELITE) && GetAmmoClip(CS_WEAPON_ELITE) > 10)
      return 65;

   // If the bot can use FIVESEVEN...
   if (HasWeapon(CS_WEAPON_FIVESEVEN) && GetAmmoClip(CS_WEAPON_FIVESEVEN) > 3)
      return 55;

   return 0; // Otherwise bot is feeling no good

   // FIXME: For other MODs we should take cover & go finding weapons here; but
   // for CS, as there's usually NO weapons in the map, probably we have to rush
   // with the knife at all cost.
}

