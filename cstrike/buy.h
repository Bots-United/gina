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

#ifndef CS_BUY_H
#define CS_BUY_H

class CCSBuyManager {
public:
   CCSBuyManager();
   virtual ~CCSBuyManager();

   int       ChooseWeapon(int iEmotion, int iMoney, int iTeam, int iType = 0);
   void      Personalize(const std::vector<int> &v);

private:
   void      LoadBuyPriority(); // load the buy priority

   std::vector<int>     aggressive_ct; // buy priority for aggressive bot (CT)
   std::vector<int>     normal_ct;     // buy priority for normal bot (CT)
   std::vector<int>     defensive_ct;  // buy priority for defensive bot (CT)

   std::vector<int>     aggressive_t;  // buy priority for aggressive bot (T)
   std::vector<int>     normal_t;      // buy priority for normal bot (T)
   std::vector<int>     defensive_t;   // buy priority for defensive bot (T)

   bool                 m_fBuyPersonalized;
   std::vector<int>     personalized;
};

extern CCSBuyManager *g_pBuyMgrCS;

// define the struct for weapon table.
struct cs_weapontable_s {
   int         iId;         // weapon ID
   int         iPrice;      // weapon price
   char       *szBuyCmd15;  // buy command for CS 1.5
   char       *szBuyCmd16;  // buy command for CS 1.6
};

#endif

