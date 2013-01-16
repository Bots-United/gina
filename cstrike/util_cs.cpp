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

#include "../main.h"

// returns the max ammo in clip for the specific weapon
int UtilCS::MaxAmmoInClip(int iId)
{
   switch (iId) {
      case CS_WEAPON_M249:
         return 100;

      case CS_WEAPON_P90:
         return 50;

      case CS_WEAPON_GALIL:
         return 35;

      case CS_WEAPON_ELITE:
      case CS_WEAPON_MP5NAVY:
      case CS_WEAPON_TMP:
      case CS_WEAPON_MAC10:
      case CS_WEAPON_M4A1:
      case CS_WEAPON_AK47:
      case CS_WEAPON_SG552:
      case CS_WEAPON_AUG:
      case CS_WEAPON_SG550:
         return 30;

      case CS_WEAPON_UMP45:
      case CS_WEAPON_FAMAS:
         return 25;

      case CS_WEAPON_GLOCK18:
      case CS_WEAPON_FIVESEVEN:
      case CS_WEAPON_G3SG1:
         return 20;

      case CS_WEAPON_P228:
         return 13;

      case CS_WEAPON_USP:
         return 12;

      case CS_WEAPON_AWP:
      case CS_WEAPON_SCOUT:
         return 10;

      case CS_WEAPON_M3:
         return 8;

      case CS_WEAPON_DEAGLE:
      case CS_WEAPON_XM1014:
         return 7;

      default:  // for knives & grenades
         return 0;
   }
}

