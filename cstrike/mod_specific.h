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
// mod_specific.h
//

#ifndef CS_MOD_SPECIFIC_H
#define CS_MOD_SPECIFIC_H

enum cs_version {
   CS_VERSION_WON, // CS 1.5 or earlier
   CS_VERSION_STEAM, // CS 1.6 Steam
};

enum team_cs {
   CS_TEAM_TERRORIST = 0,
   CS_TEAM_CT,
};

// weapon ID values for Counter-Strike
enum weaponid_cs {
   CS_WEAPON_P228 = 1,
   CS_WEAPON_SHIELDGUN = 2, // not real, just for easy handling
   CS_WEAPON_SCOUT = 3,
   CS_WEAPON_HEGRENADE = 4,
   CS_WEAPON_XM1014 = 5,
   CS_WEAPON_C4 = 6,
   CS_WEAPON_MAC10 = 7,
   CS_WEAPON_AUG = 8,
   CS_WEAPON_SMOKEGRENADE = 9,
   CS_WEAPON_ELITE = 10,
   CS_WEAPON_FIVESEVEN = 11,
   CS_WEAPON_UMP45 = 12,
   CS_WEAPON_SG550 = 13,
   CS_WEAPON_GALIL = 14,
   CS_WEAPON_FAMAS = 15,
   CS_WEAPON_USP = 16,
   CS_WEAPON_GLOCK18 = 17,
   CS_WEAPON_AWP = 18,
   CS_WEAPON_MP5NAVY = 19,
   CS_WEAPON_M249 = 20,
   CS_WEAPON_M3 = 21,
   CS_WEAPON_M4A1 = 22,
   CS_WEAPON_TMP = 23,
   CS_WEAPON_G3SG1 = 24,
   CS_WEAPON_FLASHBANG = 25,
   CS_WEAPON_DEAGLE = 26,
   CS_WEAPON_SG552 = 27,
   CS_WEAPON_AK47 = 28,
   CS_WEAPON_KNIFE = 29,
   CS_WEAPON_P90 = 30,
};

// Game start messages for CS...
enum msg_cs {
   MSG_CS_IDLE = 0,
   MSG_CS_TEAM_SELECT,
   MSG_CS_CLASS_SELECT,
};

// VGUI menu
enum vgui_cs {
   MENU_CS_TEAM = 2,
   MENU_CS_TERRORIST = 26,
   MENU_CS_CT = 27,
};

// Radio Messages
enum radiomsg_cs {
   RADIO_NONE            = 0,

   // Radio Commands
   RADIO_COVERME         = 1,
   RADIO_YOUTAKEPOINT    = 2,
   RADIO_HOLDPOSITION    = 3,
   RADIO_REGROUPTEAM     = 4,
   RADIO_FOLLOWME        = 5,
   RADIO_TAKINGFIRE      = 6,

   // Group Radio Commands
   RADIO_GOGOGO          = 11,
   RADIO_FALLBACK        = 12,
   RADIO_STICKTOGETHER   = 13,
   RADIO_GETINPOSITION   = 14,
   RADIO_STORMTHEFRONT   = 15,
   RADIO_REPORTTEAM      = 16,

   // Radio Responses/Reports
   RADIO_AFFIRMATIVE     = 21,
   RADIO_ENEMYSPOTTED    = 22,
   RADIO_NEEDBACKUP      = 23,
   RADIO_SECTORCLEAR     = 24,
   RADIO_IMINPOSITION    = 25,
   RADIO_REPORTINGIN     = 26,
   RADIO_SHESGONNABLOW   = 27,
   RADIO_NEGATIVE        = 28,
   RADIO_ENEMYDOWN       = 29,
};

// map type
enum maptype_cs {
   MAP_AS = (1 << 0),
   MAP_CS = (1 << 1),
   MAP_DE = (1 << 2),
};

#include "util_cs.h"
#include "bot_cs.h"
#include "botai_cs.h"
#include "server_cs.h"
#include "buy.h"

#endif

