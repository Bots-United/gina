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
// engine_util.cpp
//

#include "main.h"

/**
 * Does a varargs printf into a temp buffer, so we don't need to have
 * varargs versions of all text functions.
 */
char *va(char *format, ...)
{
   static char string[2][256], c = 0, *p;

   if (format == NULL) {
      return string[c];
   }

   va_list     argptr;

   // we need this because this may be called twice in one function
   p = string[c ^= 1];

   va_start(argptr, format);
   _vsnprintf(p, 256, format, argptr);
   va_end(argptr);

   return p;
}

/**
 * This function prints a message on the screen. The text will be printed
 * on the server console. Since it's basically a redefinition of the
 * standard C libraries printf() function, it has to be the same type,
 * hence the integer return value.
 */
int printf(const char *fmt, ...)
{
   va_list argptr;
   char string[256];

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   _vsnprintf(string, sizeof(string), fmt, argptr);
   va_end(argptr);

   g_engfuncs.pfnServerPrint(string); // print to console
   return strlen(string); // printf() HAS to return a value
}

CEntity *UTIL_FindEntityInSphere(CEntity *pStartEntity, const Vector &vecCenter, float flRadius)
{
   edict_t *pentEntity;

   if (pStartEntity && pStartEntity->IsValid())
      pentEntity = pStartEntity->edict();
   else
      pentEntity = NULL;

   pentEntity = FIND_ENTITY_IN_SPHERE(pentEntity, vecCenter, flRadius);

   if (!FNullEnt(pentEntity)) {
      return CEntity::Instance(pentEntity);
   }

   return NULL;
}

CEntity *UTIL_FindEntityByString(CEntity *pStartEntity, const char *szKeyword, const char *szValue )
{
   edict_t *pentEntity;

   if (pStartEntity && pStartEntity->IsValid())
      pentEntity = pStartEntity->edict();
   else
      pentEntity = NULL;

   pentEntity = FIND_ENTITY_BY_STRING(pentEntity, szKeyword, szValue);

   if (!FNullEnt(pentEntity)) {
      return CEntity::Instance(pentEntity);
   }

   return NULL;
}

traceresult_t TestLine(const Vector &vecStart, const Vector &vecEnd,
                       bool igmon, CEntity *pentIgnore)
{
   // local variables are made static for speedup reason
   static TraceResult tr; // Valve's TraceResult struct
   static traceresult_t trace;  // Our own struct

   // Use Valve's traceline function to perform the actual operation...
   TRACE_LINE(vecStart, vecEnd, (0x100 | (igmon ? 1 : 0)), (pentIgnore ? pentIgnore->edict() : NULL), &tr);

   // convert Valve's TraceResult to our own traceresult_t
   trace.allsolid = tr.fAllSolid != 0;
   trace.endpos = tr.vecEndPos;
   trace.ent = CEntity::Instance(tr.pHit);
   trace.fraction = tr.flFraction;
   trace.hitgroup = tr.iHitgroup;
   trace.inopen = tr.fInOpen != 0;
   trace.inwater = tr.fInWater != 0;
   trace.plane.dist = tr.flPlaneDist;
   trace.plane.normal = tr.vecPlaneNormal;
   trace.startsolid = tr.fStartSolid != 0;

   return trace; // okay, result converted, return it. Traceline done.
}

traceresult_t PlayerTestLine(const Vector &vecStart, const Vector &vecEnd, bool igmon,
                             bool isDuckPlayer, CEntity *pentIgnore)
{
   // local variables are made static for speedup reason
   static TraceResult tr; // Valve's TraceResult struct
   static traceresult_t trace;  // our own traceresult_t struct

   // Use Valve's tracehull function to perform the actual operation...
   TRACE_HULL(vecStart, vecEnd, (igmon ? 1 : 0), isDuckPlayer ? head_hull : human_hull,
              (pentIgnore ? pentIgnore->edict() : NULL), &tr);

   // convert Valve's TraceResult to our own traceresult_t
   trace.allsolid = tr.fAllSolid != 0;
   trace.endpos = tr.vecEndPos;
   trace.ent = CEntity::Instance(tr.pHit);
   trace.fraction = tr.flFraction;
   trace.hitgroup = tr.iHitgroup;
   trace.inopen = tr.fInOpen != 0;
   trace.inwater = tr.fInWater != 0;
   trace.plane.dist = tr.flPlaneDist;
   trace.plane.normal = tr.vecPlaneNormal;
   trace.startsolid = tr.fStartSolid != 0;

   return trace; // okay, result converted, return it. Tracehull done.
}

/**
 * Probes the world space at v_location and returns whether or not the point
 * belongs to swimmable liquid.
 */
bool IsLiquid(const Vector &v_location)
{
   // do the actual probe
   int contents = POINT_CONTENTS(v_location);

   // point belongs to liquid in either of these cases
   return (contents == CONTENTS_WATER || contents == CONTENTS_SLIME ||
           contents == CONTENTS_LAVA);
}

/**
 * Probes the world space at v_location and returns whether or not the point
 * is in mid-air.
 */
bool IsThinAir(const Vector &v_location)
{
   // do the actual probe
   int contents = POINT_CONTENTS(v_location);

   // point belongs to mid-air in either of these cases
   return (contents == CONTENTS_EMPTY || contents == CONTENT_FOG ||
           contents == CONTENT_FLYFIELD || contents == CONTENT_GRAVITY_FLYFIELD);
}

/**
 * Probes the world space at v_location and returns whether or not the point
 * belongs to the reachable world - thin air or swimmable liquid - or the void.
 * Concrete materials and skyboxes are assimilated to the void because of their
 * "unreachable" state.
 */
bool PointBelongsToWorld(const Vector &v_location)
{
   // do the actual probe
   int contents = POINT_CONTENTS(v_location);

   // point belongs to reachable world only if it's NOT inside concrete material,
   // NOT in the sky and NOT inside a ladder entity
   return !(contents == CONTENTS_SOLID || contents == CONTENTS_SKY ||
            contents == CONTENTS_LADDER);
}

extern "C" void info_node(entvars_t *pev);

void AddNodeEntity(const Vector &origin, int effects)
{
   edict_t *pNode = CREATE_ENTITY();
   if (g_fIsMetamod) {
      CALL_GAME_ENTITY(PLID, "info_node", VARS(pNode));
   } else {
      info_node(VARS(pNode));
   }

   pNode->v.classname = MAKE_STRING("info_node");
   SET_ORIGIN(pNode, origin);
   pNode->v.rendercolor.x = 255;
   pNode->v.rendercolor.y = 255;
   pNode->v.rendercolor.z = 0;
   pNode->v.rendermode = kRenderTransAdd;
   pNode->v.movetype = MOVETYPE_NONE;
   pNode->v.solid = SOLID_TRIGGER;
   SET_SIZE(ENT(&pNode->v), Vector(-8, -8, -8), Vector(8, 8, 8));
   pNode->v.renderfx = kRenderFxNoDissipation;
   pNode->v.renderamt = 255;
   pNode->v.scale = 0.2;
   SET_MODEL(ENT(pNode), "sprites/glow02.spr");
   pNode->v.effects = effects;
}

void ShowNodeEntity(bool show)
{
   CEntity *pFoundNode = NULL;
   while ((pFoundNode = UTIL_FindEntityByClassname(pFoundNode, "info_node")) != NULL) {
      if (show)
         pFoundNode->pev->effects &= ~EF_NODRAW;
      else
         pFoundNode->pev->effects |= EF_NODRAW;
   }
}

void RemoveAllNodeEntity()
{
   CEntity *pFoundNode = UTIL_FindEntityByClassname(NULL, "info_node");
   if (pFoundNode == NULL)
      return; // No node entities

   do {
      CEntity *pNextNode = UTIL_FindEntityByClassname(pFoundNode, "info_node");
      REMOVE_ENTITY(pFoundNode->edict());
      pFoundNode = pNextNode;
   } while (pFoundNode != NULL);
}

