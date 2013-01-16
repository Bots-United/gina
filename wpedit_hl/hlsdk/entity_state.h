/***
*
*   Copyright (c) 1999, Valve LLC. All rights reserved.
*
*   This product contains software technology licensed from Id 
*   Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*   All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#if !defined( ENTITY_STATEH )
#define ENTITY_STATEH
#ifdef _WIN32
#pragma once
#endif

#define MAX_PHYSINFO_STRING 256

typedef struct entity_state_s entity_state_t;

typedef struct clientdata_s
{
   vec3_t origin;
   vec3_t velocity;
   int viewmodel;
   vec3_t punchangle;
   int flags;
   int waterlevel;
   int watertype;
   vec3_t view_ofs;
   float health;
   int bInDuck;
   int weapons;
   int flTimeStepSound;
   int flDuckTime;
   int flSwimTime;
   int waterjumptime;
   float maxspeed;
   float fov;
   int weaponanim;
   int m_iId;
   int ammo_shells;
   int ammo_nails;
   int ammo_cells;
   int ammo_rockets;
   float m_flNextAttack;
   int tfstate;
   int pushmsec;
   int deadflag;
   char physinfo[ MAX_PHYSINFO_STRING ];
   int iuser1;
   int iuser2;
   int iuser3;
   int iuser4;
   float fuser1;
   float fuser2;
   float fuser3;
   float fuser4;
   vec3_t vuser1;
   vec3_t vuser2;
   vec3_t vuser3;
   vec3_t vuser4;
} clientdata_t;

#endif