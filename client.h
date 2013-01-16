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
// client.h
//

#ifndef _CLIENT_H
#define _CLIENT_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "general.h"
#include "entity.h"

class CBaseBot;

enum msg_dest {
   TEXT_CENTER,
   TEXT_CONSOLE,
   TEXT_SAY,
};

class CClient : public CEntity
{
public:
   CClient();
   virtual ~CClient();

   inline bool   IsBot(void)            { return (GetBotPointer() != NULL); }

   int           GetTeam(void);

   inline bool   HasWeapon(int iId)        { return (GetWeapons() & (1 << iId)) != 0; }
   inline int    GetCurrentWeapon(void)    { return m_iCurrentWeaponId; }
   inline int    GetAmmoPrimary(void)      { return m_rgAmmo[g_General.m_rgWeaponDefs[m_iCurrentWeaponId].iAmmo1]; }
   inline int    GetAmmoSecondary(void)    { return m_rgAmmo[g_General.m_rgWeaponDefs[m_iCurrentWeaponId].iAmmo2]; }
   inline int    GetAmmoPrimary(int iId)   { return m_rgAmmo[g_General.m_rgWeaponDefs[iId].iAmmo1]; }
   inline int    GetAmmoSecondary(int iId) { return m_rgAmmo[g_General.m_rgWeaponDefs[iId].iAmmo2]; }
   inline int    GetAmmoClip(void)         { return m_rgAmmoInClip[m_iCurrentWeaponId]; }
   inline int    GetAmmoClip(int iId)      { return m_rgAmmoInClip[iId]; }
   inline int    GetAmmoTotal(void)        { return GetAmmoPrimary() + GetAmmoClip(); }
   inline int    GetFrags(void)            { return (int)(pev->frags); }

   inline bool   HasLongJump(void)         { return atoi(g_engfuncs.pfnGetPhysicsKeyValue(edict(), "slj")) != 0; }

   inline bool   IsShooting(void)          { return (pev->button & IN_ATTACK) != 0; }
 
   virtual bool      MessageWrite(void *p);

   virtual CBaseBot *GetBotPointer(void) { return NULL; }

   static CClient *Instance(edict_t *pent);
   static CClient *Instance(entvars_t *pev)   { return Instance(ENT(pev)); }
   static CClient *Instance(CEntity *pEntity) { return Instance(pEntity->edict()); }

   inline CEntity *GetDamageInflictor(void)   { return CEntity::Instance(pev->dmg_inflictor); }

   void            TextMessage(msg_dest iMsgDest, const char *fmt, ...);

   void            Kill(void);
   void            Kick(void);

protected:
   int            m_rgAmmo[MAX_AMMO_SLOTS];    // total ammo amounts
   int            m_iCurrentWeaponId;
   int            m_rgAmmoInClip[MAX_WEAPONS];

   float          m_flLastShootTime;
};

#endif

