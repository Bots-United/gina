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
// buy.cpp
//

#include "../main.h"

CCSBuyManager *g_pBuyMgrCS = NULL;

cs_weapontable_s weapon_prices[] = {
   // secondary weapons
   {CS_WEAPON_GLOCK18,   400,  "buy;menuselect 1;menuselect 2", "glock"},
   {CS_WEAPON_USP,       500,  "buy;menuselect 1;menuselect 1", "usp"},
   {CS_WEAPON_P228,      600,  "buy;menuselect 1;menuselect 4", "p228"},
   {CS_WEAPON_DEAGLE,    650,  "buy;menuselect 1;menuselect 3", "deagle"},
   {CS_WEAPON_FIVESEVEN, 750,  "buy;menuselect 1;menuselect 6", "fn57"},
   {CS_WEAPON_ELITE,    1000,  "buy;menuselect 1;menuselect 5", "elites"},
   // primary weapons
   {CS_WEAPON_TMP,      1250,  "buy;menuselect 3;menuselect 2", "tmp"},
   {CS_WEAPON_MAC10,    1400,  "buy;menuselect 3;menuselect 4", "mac10"},
   {CS_WEAPON_MP5NAVY,  1500,  "buy;menuselect 3;menuselect 1", "mp5"},
   {CS_WEAPON_M3,       1700,  "buy;menuselect 2;menuselect 1", "m3"},
   {CS_WEAPON_UMP45,    1700,  "buy;menuselect 3;menuselect 5", "ump45"},
   {CS_WEAPON_P90,      2350,  "buy;menuselect 3;menuselect 3", "p90"},
   {CS_WEAPON_AK47,     2500,  "buy;menuselect 4;menuselect 1", "ak47"},
   {CS_WEAPON_GALIL,    2500,  "",                              "galil"}, // CS 1.6 only
   {CS_WEAPON_SCOUT,    2750,  "buy;menuselect 4;menuselect 5", "scout"},
   {CS_WEAPON_XM1014,   3000,  "buy;menuselect 2;menuselect 2", "xm1014"},
   {CS_WEAPON_FAMAS,    3100,  "",                              "famas"}, // CS 1.6 only
   {CS_WEAPON_M4A1,     3100,  "buy;menuselect 4;menuselect 3", "m4a1"},
   {CS_WEAPON_AUG,      3500,  "buy;menuselect 4;menuselect 4", "aug"},
   {CS_WEAPON_SG552,    3500,  "buy;menuselect 4;menuselect 2", "sg552"},
   {CS_WEAPON_SG550,    4200,  "buy;menuselect 4;menuselect 8", "sg550"},
   {CS_WEAPON_AWP,      4750,  "buy;menuselect 4;menuselect 6", "awp"},
   {CS_WEAPON_G3SG1,    5000,  "buy;menuselect 4;menuselect 7", "g3sg1"},
   {CS_WEAPON_M249,     5750,  "buy;menuselect 5;menuselect 1", "m249"},
   {0, 0, "", ""} // terminator
};

cs_weapontable_s *get_weapon(int iId)
{
   cs_weapontable_s *p = weapon_prices;
   while (p->iId) {
      if (p->iId == iId)
         return p;
      p++;
   }
   return NULL;
}

CCSBuyManager::CCSBuyManager()
{
   LoadBuyPriority();
}

CCSBuyManager::~CCSBuyManager()
{
}

// choose a weapon to buy and returns the weapon ID
// emotion: 0 (defensive) - 100 (aggressive)
// type: 0 - primary, 1 - secondary
int CCSBuyManager::ChooseWeapon(int iEmotion, int iMoney, int iTeam, int iType)
{
   int iPersonality = 1; // personality: 0 - defensive, 1 - normal, 2 - aggressive
   CServerCS *pServerCS = dynamic_cast<CServerCS *>(g_pServer);

   int iRandom = RandomLong(0, 100); // choose a random value
   int delta = iRandom - iEmotion;
   if (abs(delta) > 33) {
      if (delta > 0) {
         iPersonality = 0; // defensive
      } else {
         iPersonality = 2; // aggressive
      }
   }

   std::vector<int> &p = personalized;
   if (m_fBuyPersonalized) {
      p = personalized;
      m_fBuyPersonalized = false;
   } else if (iTeam == CS_TEAM_TERRORIST) {
      if (iPersonality == 0)
         p = defensive_t;
      else if (iPersonality == 1)
         p = normal_t;
      else
         p = aggressive_t;
   } else {
      if (iPersonality == 0)
         p = defensive_ct;
      else if (iPersonality == 1)
         p = normal_ct;
      else
         p = aggressive_ct;
   }

   int iSize = p.size();
   std::vector<int> chosen; // weapons which has been chosen

   int iCount = 0, i, j;

   for (i = 0; i < iSize; i++) {
      int iId = p[i];

      iCount++;

      // see if this weapon can be bought

      // if we're NOT using CS 1.6 and this weapon is only for CS 1.6
      if (pServerCS->GetCSVersion() == CS_VERSION_WON &&
         (iId == CS_WEAPON_GALIL || iId == CS_WEAPON_FAMAS || iId == CS_WEAPON_SHIELDGUN))
         continue; // skip this weapon

      // are we on an assasssination map?
      if (pServerCS->GetMapType() & MAP_AS) {
         // skip certain weapons which is not available on as_* maps
         if (iTeam == CS_TEAM_TERRORIST) {
            if (iId == CS_WEAPON_M3 || iId == CS_WEAPON_XM1014 ||
               iId == CS_WEAPON_MP5NAVY || iId == CS_WEAPON_P90 ||
               iId == CS_WEAPON_M249)
               continue; // skip this weapon
         } else {
            if (iId == CS_WEAPON_AWP)
               continue; // skip this weapon
         }
      }

      cs_weapontable_s *pWeapon = get_weapon(iId);
      if (!pWeapon) // check if this weapon is in our list
         continue; // skip this weapon if not

      // check if we have enough money (also add some money for ammo)
      if (iMoney < pWeapon->iPrice + 150)
         continue; // skip this weapon if not

      if ((iType == 1 && (UtilCS::WeaponIsPrimary(iId) || iId == CS_WEAPON_SHIELDGUN)) ||
         (iType == 0 && UtilCS::WeaponIsPistol(iId)))
         continue; // skip this weapon

      // this is a good weapon, add it to our chosen list
      for (j = 0; j < iCount; j++) {
         // add multiple instances of the weapon depends on number of weapon skipped
         // so that better weapons have more possibility to get chosen
         chosen.push_back(iId);
      }
      iCount = 0;
   }

   if (chosen.empty())
      return 0; // no weapon is chosen, can't buy weapon

   // select the weapon to buy in the table
   float flFactor = (iMoney - 3000.0) / (16000 - 3000) * 3;
   if (flFactor < 1)
      flFactor = 1;

   // 1) better weapons has more chances to get chosen
   // 2) be even more aggressive if we're rich
   float f = log10(RandomFloat(1, pow(10, flFactor)));
   return chosen[(int)((float)(chosen.size() - 1) * f / flFactor + 0.5)];
}

// pass in the personalized buy priority
void CCSBuyManager::Personalize(const std::vector<int> &v)
{
   m_fBuyPersonalized = true; // mark the flag
   personalized = v; // load the data
}

// load the buy priority configuration file
void CCSBuyManager::LoadBuyPriority()
{
   // load the ini file
   CIniFile ini;

   // try to load the map specific buy configuration file
   ini.Load(g_General.BuildFileName(va("buy_cs(%s).ini", g_pServer->GetMapName())));

   // if the map specific buy configuration file doesn't exist,
   // load the general buy configuration file.
   if (!ini.Valid())
      ini.Load(g_General.BuildFileName("buy_cs.ini"));

#define PARSE_PRIORITY(szString, vecTable)                   \
{                                                            \
   char *pszStart = (char *)szString;                        \
                                                             \
   while (pszStart && *pszStart) {                           \
      char *pszEnd = strchr(pszStart, ',');                  \
      vecTable.push_back(atoi(pszStart));                    \
                                                             \
      if (pszEnd == NULL)                                    \
         break;                                              \
                                                             \
      pszStart = pszEnd + 1;                                 \
   }                                                         \
}

   // parse each line in the file...
   PARSE_PRIORITY(ini.Get("CT", "Aggressive"), aggressive_ct);
   PARSE_PRIORITY(ini.Get("CT", "Normal"), normal_ct);
   PARSE_PRIORITY(ini.Get("CT", "Defensive"), defensive_ct);

   PARSE_PRIORITY(ini.Get("T", "Aggressive"), aggressive_t);
   PARSE_PRIORITY(ini.Get("T", "Normal"), normal_t);
   PARSE_PRIORITY(ini.Get("T", "Defensive"), defensive_t);
}

// returns true if buying is completed, false if not
bool CBotCS::BuyStuff()
{
   int iId;
   CServerCS *pServerCS = dynamic_cast<CServerCS *>(g_pServer);

   switch (m_iBuyCount)
   {
   case 0: // if bot is rich, buy armor+helmet; else buy armor
      if (m_iAccount > RandomLong(3000, 5000)) {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying armor + helmet");
         g_General.FakeClientCommand(this, "buy;menuselect 8;menuselect 2");
      } else {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying armor");
         g_General.FakeClientCommand(this, "buy;menuselect 8;menuselect 1");
      }
      break;

   case 1: // if bot doesn't have a primary weapon, buy one
      if (HasPrimary() || HasShield())
      {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): already have a primary weapon; exiting");
         break; // don't proceed if we already have one
      }

      // TODO: emotion
      // select a primary weapon
      iId = g_pBuyMgrCS->ChooseWeapon(RandomLong(0, 100)/*FIXME*/, m_iAccount, GetTeam());
      if (iId != 0) {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): Buying weapon: %s", g_General.GetWeaponName(iId));
         cs_weapontable_s *pWeapon = get_weapon(iId);
         assert(pWeapon);
         if (pServerCS->GetCSVersion() == CS_VERSION_STEAM)
            g_General.FakeClientCommand(this, pWeapon->szBuyCmd16);
         else
            g_General.FakeClientCommand(this, pWeapon->szBuyCmd15);
      } else {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): no primary weapon is selected");
      }
      break;

   case 2: // if bot doesn't have a secondary weapon, buy one
      if (GetWeapons() & ((1 << CS_WEAPON_P228) | (1 << CS_WEAPON_DEAGLE) |
         (1 << CS_WEAPON_FIVESEVEN) | (1 << CS_WEAPON_ELITE)))
      {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): already have a secondary weapon; exiting");
         break; // don't proceed if we already have a good secondary weapon
      }

      // TODO: emotion
      // select a primary weapon
      iId = g_pBuyMgrCS->ChooseWeapon(RandomLong(0, 100)/*FIXME*/, m_iAccount, GetTeam(), 1);
      if (iId != 0) {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): Buying weapon: %s", g_General.GetWeaponName(iId));
         if (HasShield() && iId == CS_WEAPON_ELITE)
            break; // if we have a shield, we can't buy the dual elites
         cs_weapontable_s *pWeapon = get_weapon(iId);
         assert(pWeapon);
         if (pServerCS->GetCSVersion() == CS_VERSION_STEAM)
            g_General.FakeClientCommand(this, pWeapon->szBuyCmd16);
         else
            g_General.FakeClientCommand(this, pWeapon->szBuyCmd15);
      } else {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): no secondary weapon is selected");
      }
      break;

   case 3: // buy enough ammo
      DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying ammo");
      g_General.FakeClientCommand(this, "buy;menuselect 6"); // prim. ammo
      g_General.FakeClientCommand(this, "buy;menuselect 7"); // sec. ammo
      break;

   case 4: // buy grenades, defusers, etc.
      // if bot is CT, randomly buy the defuser
      if (GetTeam() == CS_TEAM_CT &&
         RandomLong(1, 100) < 33 && m_iAccount > 200)
      {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying defuser");
         if (pServerCS->GetCSVersion() == CS_VERSION_STEAM)
            g_General.FakeClientCommand(this, "defuser"); // use alias in CS 1.6
         else
            g_General.FakeClientCommand(this, "buyequip;menuselect 6");
      }

      if (RandomLong(1, 100) < 75 && m_iAccount >= 300 &&
         !(GetWeapons() & CS_WEAPON_HEGRENADE))
      {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying HE Grenade");
         // Buy a HE Grenade
         g_General.FakeClientCommand(this, "buyequip");
         g_General.FakeClientCommand(this, "menuselect 4");
      }

      if (RandomLong(1, 100) < 50 && m_iAccount >= 200 &&
         !(GetWeapons() & CS_WEAPON_FLASHBANG))
      {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying Concussion Grenade");
         // Buy a Concussion Grenade, i.e., 'FlashBang'
         g_General.FakeClientCommand(this, "buyequip");
         g_General.FakeClientCommand(this, "menuselect 3");
      }

      if (RandomLong(1, 100) < 30 && m_iAccount >= 300 &&
         !(GetWeapons() & CS_WEAPON_SMOKEGRENADE))
      {
         DebugMsg(DEBUG_BOTAI, "CBotCS::BuyStuff(): buying Smoke Grenade");
         // Buy a Smoke Grenade
         g_General.FakeClientCommand(this, "buyequip");
         g_General.FakeClientCommand(this, "menuselect 5");
      }

      break;

   case 5: // buying finished
      return true; // no more buying
      break;

   default:
      CGeneral::TerminateOnError("CBotCS::BuyStuff(): m_iBuyCount error");
      break;
   }

   m_iBuyCount++; // add the buying counter
   return false;
}

