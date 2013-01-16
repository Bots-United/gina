/***
*
*  Copyright (c) 1999, Valve LLC. All rights reserved.
*
*  This product contains software technology licensed from Id 
*  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*  All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

//
// HPB_bot - botman's High Ping Bastard bot
//
// (http://planethalflife.com/botman/)
//
// util.cpp
//

#include "wpedit.h"

int gmsgShowMenu = 0;
int gmsgSayText = 0;

extern int m_spriteTexture;

// Overloaded to add IGNORE_GLASS
void UTIL_TraceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr)
{
   TRACE_LINE(vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0), pentIgnore, ptr );
}

void UTIL_TraceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr)
{
   TRACE_LINE(vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), pentIgnore, ptr);
}

void UTIL_TraceHull(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr)
{
   TRACE_HULL(vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), hullNumber, pentIgnore, ptr);
}

void UTIL_TraceModel(const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr)
{
   g_engfuncs.pfnTraceModel(vecStart, vecEnd, hullNumber, pentModel, ptr );
}

edict_t *UTIL_FindEntityInSphere(edict_t *pentStart, const Vector &vecCenter, float flRadius)
{
   edict_t *pentEntity = FIND_ENTITY_IN_SPHERE(pentStart, vecCenter, flRadius);

   if (!FNullEnt(pentEntity))
      return pentEntity;

   return NULL;
}

edict_t *UTIL_FindEntityByString(edict_t *pentStart, const char *szKeyword, const char *szValue)
{
   edict_t *pentEntity = FIND_ENTITY_BY_STRING(pentStart, szKeyword, szValue);

   if (!FNullEnt(pentEntity))
      return pentEntity;
   return NULL;
}

edict_t *UTIL_FindEntityByClassname(edict_t *pentStart, const char *szName)
{
   return UTIL_FindEntityByString(pentStart, "classname", szName);
}

edict_t *UTIL_FindEntityByTargetname(edict_t *pentStart, const char *szName)
{
   return UTIL_FindEntityByString(pentStart, "targetname", szName);
}

void UTIL_ShowMenu(edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText)
{
   if (gmsgShowMenu == 0)
      gmsgShowMenu = REG_USER_MSG("ShowMenu", -1);

   MESSAGE_BEGIN(MSG_ONE, gmsgShowMenu, NULL, pEdict);
   WRITE_SHORT(slots);
   WRITE_CHAR(displaytime);
   WRITE_BYTE(needmore);
   WRITE_STRING(pText);
   MESSAGE_END();
}

void UTIL_SayText(const char *pText, edict_t *pEdict)
{
   if (gmsgSayText == 0)
      gmsgSayText = REG_USER_MSG( "SayText", -1 );

   MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, gmsgSayText, NULL, pEdict);
   WRITE_BYTE(ENTINDEX(pEdict));
   WRITE_STRING(pText);
   MESSAGE_END();
}

Vector VecBModelOrigin(edict_t *pEdict)
{
   return pEdict->v.absmin + pEdict->v.size * 0.5;
}

Vector GetGunPosition(edict_t *pEdict)
{
   return (pEdict->v.origin + pEdict->v.view_ofs);
}

bool FInViewCone(Vector *pOrigin, edict_t *pEdict)
{
   MAKE_VECTORS(pEdict->v.angles);
   Vector vecLOS = (*pOrigin - pEdict->v.origin).Normalize();
   return (DotProduct(vecLOS, gpGlobals->v_forward) > 0.747);
}

void WaypointDrawBeam(edict_t *pEntity, const Vector &start, const Vector &end,
                      int width, int noise, int red, int green, int blue,
                      int brightness, int speed)
{
   MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, pEntity);
   WRITE_BYTE(TE_BEAMPOINTS);
   WRITE_COORD(start.x);
   WRITE_COORD(start.y);
   WRITE_COORD(start.z);
   WRITE_COORD(end.x);
   WRITE_COORD(end.y);
   WRITE_COORD(end.z);
   WRITE_SHORT(m_spriteTexture);
   WRITE_BYTE(1);        // framestart
   WRITE_BYTE(10);       // framerate
   WRITE_BYTE(10);       // life in 0.1's
   WRITE_BYTE(width);    // width
   WRITE_BYTE(noise);    // noise

   WRITE_BYTE(red);      // r, g, b
   WRITE_BYTE(green);    // r, g, b
   WRITE_BYTE(blue);     // r, g, b

   WRITE_BYTE(brightness);   // brightness
   WRITE_BYTE(speed);        // speed
   MESSAGE_END();
}

char *UTIL_VarArgs(char *format, ...)
{
   va_list        argptr;
   static char    string[1024];

   va_start(argptr, format);
   _vsnprintf(string, sizeof(string), format, argptr);
   va_end(argptr);

   return string;
}

