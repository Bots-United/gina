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
 * entity.h                                            *
 *******************************************************
 * Purpose: Wrapping all entity handling stuff in a    *
 *          class.                                     *
 * Engine-specific: yes                                *
 *******************************************************/

#ifndef _ENTITY_H
#define _ENTITY_H

class CEntity {
public:
   CEntity() : pev(NULL) {}
   CEntity(edict_t *pEdict) : pev(&pEdict->v) {}
   virtual ~CEntity() {}

   entvars_t    *pev;

   // From Half-Life SDK cbase.h:
   inline edict_t   *edict()     { return ENT(pev); }
   inline int        entindex()  { return ENTINDEX(edict()); }

   inline bool operator==(CEntity &e) { return entindex() == e.entindex(); }

   static CEntity *Instance(edict_t *pent);
   static CEntity *Instance(entvars_t *pev)   { return Instance(ENT(pev)); }

   inline bool   IsValid(void)     { return (this && pev && (FNullEnt(edict()) ? false : (edict()->free == 0))); }
   inline bool   IsMonster(void)   { return (pev->flags & FL_MONSTER) != 0; }
   inline bool   IsPlayer(void)    { return (pev->flags & FL_CLIENT) != 0 && entindex() <= gpGlobals->maxClients; }

   inline bool   HasBoundingBox(void)
   {
      // this function returns TRUE if entity has a valid bounding box, FALSE otherwise
      return (!FZero(pev->absmin.x) || !FZero(pev->absmin.y) || !FZero(pev->absmin.z) ||
              !FZero(pev->absmax.x) || !FZero(pev->absmax.y) || !FZero(pev->absmax.z));
   }

   inline Vector GetAbsMin(void)   { return pev->absmin; }
   inline Vector GetAbsMax(void)   { return pev->absmax; }
   inline Vector GetMins(void)     { return pev->mins; }
   inline Vector GetMaxs(void)     { return pev->maxs; }

   inline const char  *GetClassname(void) { return STRING(pev->classname); }
   inline const char  *GetNetName(void)   { return (char *)STRING(pev->netname); }
   inline const char  *GetModel(void)
   {
      if (IsPlayer())
         return g_engfuncs.pfnInfoKeyValue(g_engfuncs.pfnGetInfoKeyBuffer(edict()), "model");
      return STRING(pev->model);
   }
   inline int          GetGroupInfo(void) { return pev->groupinfo; }
   inline const char  *GetViewModel(void) { return STRING(pev->viewmodel); }
   inline int    GetHealth(void)   { return (int)pev->health; }
   inline Vector Center(void)      { return (pev->absmax + pev->absmin) * 0.5; } // center point of entity
   inline Vector GetOrigin(void)   { return HasBoundingBox() ? Center() : pev->origin; }
   inline Vector EyePosition(void) { return pev->origin + pev->view_ofs; } // position of eyes
   inline Vector EarPosition(void) { return pev->origin + pev->view_ofs; } // position of ears
   inline Vector GetGunPosition(void)  { return pev->origin + pev->view_ofs; }
   inline Vector GetVelocity(void) { return pev->velocity; }
   inline float  GetSpeed(void)    { return pev->velocity.Length(); }
   inline float  GetSpeed2D(void)  { return pev->velocity.Length2D(); }
   inline float  GetArmor(void)    { return pev->armorvalue; }
   inline float  GetArmorType(void)  { return pev->armortype; }
   inline float  GetMaxspeed(void)   { return pev->maxspeed; }
   inline Vector GetBodyAngles(void) { return pev->angles; }
   inline Vector GetViewAngles(void) { return pev->v_angle; }
   inline float  GetFov(void)        { return pev->fov; }

   inline int    GetMoveType(void) { return pev->movetype; }
   inline int    GetSpawnFlags(void) { return pev->spawnflags; }
   inline int    GetFlags(void)      { return pev->flags; }
   inline int    GetWaterLevel(void) { return pev->waterlevel; }

   inline const char *GetTargetName(void) { return STRING(pev->targetname); }
   inline const char *GetTarget(void)     { return STRING(pev->target); }

   inline void  ReleaseAllButtons(void) { pev->button = 0; }
   inline void  PushButton(int iButton) { pev->button |= iButton; }
   inline void  ReleaseButton(int iButton) { pev->button &= ~iButton; }
   inline int   GetButtons(void)        { return pev->button; }

   inline void  SetAngles(const Vector &vecViewAngles)
   {
      pev->angles = pev->v_angle = vecViewAngles;
      pev->angles.x /= -3;
      pev->angles.ClampAngles();
      pev->v_angle.ClampAngles();
   }

   inline int    GetWeaponAnim(void)    { return pev->weaponanim; }
   inline int    GetWeapons(void)       { return pev->weapons; }

   inline CEntity *GetOwner(void)        { return Instance(pev->owner); }
   inline CEntity *GetGroundEntity(void) { return Instance(pev->groundentity); }

   inline bool   IsVisible(void)   { return !(pev->effects & EF_NODRAW) && GetModel()[0]; }
   inline bool   IsAlive(void)     { return pev->deadflag == DEAD_NO && GetHealth() > 0 && !(pev->flags & FL_NOTARGET); }
   inline bool   IsOnLadder(void)  { return pev->movetype == MOVETYPE_FLY; }
   inline bool   IsInWater(void)   { return pev->waterlevel >= 2; }
   inline bool   IsInSlime(void)   { return POINT_CONTENTS((float *)(GetOrigin() - Vector(0, 0, 23))) == CONTENTS_SLIME; }
   inline bool   IsInLava(void)    { return POINT_CONTENTS((float *)(GetOrigin() - Vector(0, 0, 23))) == CONTENTS_LAVA; }
   inline bool   IsOnFloor(void)   { return IsPlayer() ? ((pev->flags & (FL_ONGROUND | FL_PARTIALGROUND)) != 0) : (ENT_IS_ON_FLOOR(edict()) != 0); }
   inline bool   IsMoving(void)    { return (GetSpeed() > 0); }
   inline bool   IsDucking(void)   { return (pev->flags & FL_DUCKING) != 0; }

   bool          FInViewCone(Vector *pOrigin, float fov = 0);
   bool          FInViewCone(CEntity *pEntity, float fov = 0);

   bool          FVisible(const Vector &vecDest);
   bool          FVisible(CEntity *pEntity);

   bool          FBoxVisible(CEntity *pEntity, Vector *pvHit = NULL, unsigned char *ucBodyPart = NULL);

   inline void   SetSpawnFlags(int flag) { pev->spawnflags |= flag; }
   inline void   SetFlags(int flag)      { pev->flags |= flag; }
   inline void   SetBodyAngles(const Vector &a) { pev->angles = a; }
   inline void   SetViewAngles(const Vector &a) { pev->v_angle = a; }

   void          ChangeAngles(const Vector &idealangles, float speed = 1.0); // speed: 0.1 - 1
};

#endif

