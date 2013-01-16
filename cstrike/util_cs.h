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
// utils_cs.h
//

#ifndef UTILS_CS_H
#define UTILS_CS_H

namespace UtilCS
{
   // returns if a client has the tactical shield
   inline bool HasShield(CClient *pClient)
   {
      return (strncmp(pClient->GetViewModel(), "models/shield/v_shield_", 23) == 0);
   }

   inline bool HasPrimary(CClient *pClient)
   {
      return (pClient->GetWeapons() &
         ((1 << CS_WEAPON_XM1014) | (1 << CS_WEAPON_M3) | (1 << CS_WEAPON_MAC10) |
         (1 << CS_WEAPON_UMP45) | (1 << CS_WEAPON_MP5NAVY) | (1 << CS_WEAPON_TMP) |
         (1 << CS_WEAPON_P90) | (1 << CS_WEAPON_AUG) | (1 << CS_WEAPON_M4A1) |
         (1 << CS_WEAPON_SG552) | (1 << CS_WEAPON_AK47) | (1 << CS_WEAPON_SCOUT) |
         (1 << CS_WEAPON_SG550) | (1 << CS_WEAPON_AWP) | (1 << CS_WEAPON_G3SG1) |
         (1 << CS_WEAPON_M249) | (1 << CS_WEAPON_FAMAS) | (1 << CS_WEAPON_GALIL))) != 0;
   }

   inline bool HasSecondary(CClient *pClient)
   {
      return (pClient->GetWeapons() &
         ((1 << CS_WEAPON_P228) | (1 << CS_WEAPON_DEAGLE) | (1 << CS_WEAPON_FIVESEVEN) |
         (1 << CS_WEAPON_ELITE) | (1 << CS_WEAPON_USP) | (1 << CS_WEAPON_GLOCK18))) != 0;
   }

   // returns if the shield is drawn
   inline bool IsShieldDrawn(CClient *pClient)
   {
      if (!HasShield(pClient))
         return false;

      // 6: already drawn, 7: drawing, 8: taking away
      return (pClient->GetWeaponAnim() == 6 || pClient->GetWeaponAnim() == 7);
   }

   // returns if the player is VIP
   inline bool IsVIP(CClient *pClient)
   {
      return (strcmp(pClient->GetModel(), "vip") == 0);
   }

   // returns if the player has the bomb
   inline bool HasBomb(CClient *pClient)
   {
      return (pClient->GetWeapons() & CS_WEAPON_C4) != 0;
   }

   // returns if the specified weapon is knife
   inline bool WeaponIsKnife(int iId)
   {
      return (iId == CS_WEAPON_KNIFE);
   }

   // returns if the specified weapon is pistol
   inline bool WeaponIsPistol(int iId)
   {
      return (iId == CS_WEAPON_P228 || iId == CS_WEAPON_ELITE ||
         iId == CS_WEAPON_USP || iId == CS_WEAPON_GLOCK18 ||
         iId == CS_WEAPON_DEAGLE || iId == CS_WEAPON_FIVESEVEN);
   }

   // returns if the specified weapon is shotgun
   inline bool WeaponIsShotgun(int iId)
   {
      return (iId == CS_WEAPON_XM1014 || iId == CS_WEAPON_M3);
   }

   // returns if the specified weapon is sub-machinegun
   inline bool WeaponIsSMG(int iId)
   {
      return (iId == CS_WEAPON_MAC10 || iId == CS_WEAPON_UMP45 ||
         iId == CS_WEAPON_MP5NAVY || iId == CS_WEAPON_TMP ||
         iId == CS_WEAPON_P90);
   }

   // returns if the specified weapon is rifle
   inline bool WeaponIsRifle(int iId)
   {
      return (iId == CS_WEAPON_AUG || iId == CS_WEAPON_M4A1 ||
         iId == CS_WEAPON_SG552 || iId == CS_WEAPON_AK47 ||
         iId == CS_WEAPON_FAMAS || iId == CS_WEAPON_GALIL);
   }

   // returns if the specified weapon is sniper rifle
   inline bool WeaponIsSniperRifle(int iId)
   {
      return (iId == CS_WEAPON_SCOUT || iId == CS_WEAPON_SG550 ||
         iId == CS_WEAPON_AWP || iId == CS_WEAPON_G3SG1);
   }

   // returns if the specified weapon is machine gun
   inline bool WeaponIsMachineGun(int iId)
   {
      return (iId == CS_WEAPON_M249);
   }

   // returns if the specified weapon is primary weapon
   inline bool WeaponIsPrimary(int iId)
   {
      return (iId == CS_WEAPON_XM1014 || iId == CS_WEAPON_M3 ||
         iId == CS_WEAPON_MAC10 || iId == CS_WEAPON_UMP45 ||
         iId == CS_WEAPON_MP5NAVY || iId == CS_WEAPON_TMP ||
         iId == CS_WEAPON_P90 || iId == CS_WEAPON_AUG ||
         iId == CS_WEAPON_M4A1 || iId == CS_WEAPON_SG552 ||
         iId == CS_WEAPON_AK47 || iId == CS_WEAPON_SCOUT ||
         iId == CS_WEAPON_SG550 || iId == CS_WEAPON_AWP ||
         iId == CS_WEAPON_G3SG1 || iId == CS_WEAPON_M249 ||
         iId == CS_WEAPON_FAMAS || iId == CS_WEAPON_GALIL);
   }

   // returns the max ammo in clip for the specific weapon
   int MaxAmmoInClip(int iId);
}

#endif

