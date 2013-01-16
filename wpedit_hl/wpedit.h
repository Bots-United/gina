/*
 * The GINA Bot - a computer opponent for Valve's FPS game Half-Life
 * Waypoint Editor Metamod Plugin
 *
 * Copyright (c) 2005, Wei Mingzhi <whistler_wmz@users.sf.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (version 2 or any later
 * version) as published by the Free Software Foundation.
 *
 * [In other words, you are welcome to use, share and improve this
 * program. You are forbidden to forbid anyone else to use, share
 * and improve what you give them.]
 *
 * As a special exception, you have permission to link this program with
 * the Half-Life 1, as long as you follow the requirements of the GNU GPL
 * in regard to all of the software aside from HL1.
 */

#ifndef WPEDIT_H
#define WPEDIT_H

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "entity_state.h"
#include "studio.h"
#include "usercmd.h"

#include "meta_api.h"

#ifdef _WIN32
#include <io.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <assert.h>

#ifndef WPEDIT_PLUGIN
#define WPEDIT_PLUGIN /* */
#endif

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);
C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion);

void UTIL_TraceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr);
void UTIL_TraceLine(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr);
void UTIL_TraceHull(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr);
void UTIL_TraceModel(const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr);
edict_t *UTIL_FindEntityInSphere(edict_t *pentStart, const Vector &vecCenter, float flRadius);
edict_t *UTIL_FindEntityByString(edict_t *pentStart, const char *szKeyword, const char *szValue);
edict_t *UTIL_FindEntityByClassname(edict_t *pentStart, const char *szName);
edict_t *UTIL_FindEntityByTargetname(edict_t *pentStart, const char *szName);
void UTIL_ShowMenu(edict_t *pEdict, int slots, int displaytime, bool needmore, char *pText);
void UTIL_SayText(const char *pText, edict_t *pEdict);
Vector VecBModelOrigin(edict_t *pEdict);
Vector GetGunPosition(edict_t *pEdict);
bool FInViewCone(Vector *pOrigin, edict_t *pEdict);
void WaypointDrawBeam(edict_t *pEntity, const Vector &start, const Vector &end,
                      int width, int noise, int red, int green, int blue,
                      int brightness, int speed);
char *UTIL_VarArgs(char *format, ...);

extern edict_t *listenserver_edict;

#include "../bot.h"
#include "../ini.h"
#include "waypoint.h"

#endif

