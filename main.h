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
// main.h
//

#ifndef MAIN_H
#define MAIN_H

#include "mathlib.h"

// HLSDK includes...
#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "entity_state.h"
#include "studio.h"
#include "usercmd.h"

#include "meta_api.h"
extern bool g_fIsMetamod;

// stuff for Windows vs. GNU/Linux builds
#ifdef _WIN32
   #define WINDOWSNT
   #define STDCALL     __stdcall
#else
   #define STDCALL     /* */
#endif

#define MAXSTUDIOBONES 128

C_DLLEXPORT int Server_GetBlendingInterface(int version,
   struct sv_blending_interface_s **ppinterface,
   struct engine_studio_api_s *pstudio,
   float (*rotationmatrix)[3][4],
   float (*bonetransform)[MAXSTUDIOBONES][3][4]);

#ifdef WINDOWSNT

#define dlsym GetProcAddress
typedef void (__stdcall *GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef int (FAR *GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (FAR *GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *);
typedef int (*SERVER_GETBLENDINGINTERFACE)(int, struct sv_blending_interface_s **, struct engine_studio_api_s *, float (*rotationmatrix)[3][4], float (*bonetransform)[MAXSTUDIOBONES][3][4]);
extern HINSTANCE h_Library;
typedef void (FAR *LINK_ENTITY_GAME)(entvars_t *);

#ifdef _MSC_VER

// Remove warnings from warning level 4.
#pragma warning(disable : 4514) // warning C4514: unreferenced inline function has been removed
#pragma warning(disable : 4100) // warning C4100: unreferenced formal parameter
#pragma warning(disable : 4127) // warning C4127: conditional expression is constant
#pragma warning(disable : 4512) // warning C4512: assignment operator could not be generated
#pragma warning(disable : 4611) // warning C4611: interaction between '_setjmp' and C++ object destruction is non-portable
#pragma warning(disable : 4706) // warning C4706: assignment within conditional expression
#pragma warning(disable : 4710) // warning C4710: function 'x' not inlined
#pragma warning(disable : 4702) // warning C4702: unreachable code
#pragma warning(disable : 4505) // unreferenced local function has been removed
#pragma warning(disable : 4239) // nonstandard extension used : 'argument' ( conversion from class Vector to class Vector& )
#pragma warning(disable : 4097) // typedef-name 'BaseClass' used as synonym for class-name 'CFlexCycler::CBaseFlex'
#pragma warning(disable : 4324) // Padding was added at the end of a structure
#pragma warning(disable : 4244) // type conversion warning.
#pragma warning(disable : 4305)	// truncation from 'const double ' to 'float '
#pragma warning(disable : 4786)	// Disable warnings about long symbol names
#pragma warning(disable : 4511)	// Disable warnings about private copy constructors
#pragma warning(disable : 4996) // This function or variable may be unsafe
#pragma warning(disable : 4018) // signed / unsigned mismatch

#endif

#else

#include <dlfcn.h>
#include <unistd.h>
typedef int BOOL;
typedef void (*GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);
typedef int (*GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef int (*GETNEWDLLFUNCTIONS)(NEW_DLL_FUNCTIONS *, int *);
typedef int	(*SERVER_GETBLENDINGINTERFACE)(int, struct sv_blending_interface_s **, struct engine_studio_api_s *, float (*rotationmatrix)[3][4], float (*bonetransform)[MAXSTUDIOBONES][3][4]);
extern void *h_Library;
typedef void (*LINK_ENTITY_GAME)(entvars_t *);

#endif

#ifdef WINDOWSNT
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

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <list>

C_DLLEXPORT void STDCALL GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals);

extern DLL_FUNCTIONS other_gFunctionTable;
extern GETENTITYAPI other_GetEntityAPI;
extern GETNEWDLLFUNCTIONS other_GetNewDLLFunctions;
extern SERVER_GETBLENDINGINTERFACE other_Server_GetBlendingInterface;

extern enginefuncs_t     g_engfuncs;
extern globalvars_t     *gpGlobals;

#define CREATE_FAKE_CLIENT    (*g_engfuncs.pfnCreateFakeClient)
#define GET_INFOBUFFER        (*g_engfuncs.pfnGetInfoKeyBuffer)
#define GET_INFO_KEY_VALUE    (*g_engfuncs.pfnInfoKeyValue)
#define SET_CLIENT_KEY_VALUE  (*g_engfuncs.pfnSetClientKeyValue)

// define constants used to identify which MOD we are playing...
enum {
   MOD_UNKNOWN,
   MOD_VALVE,    // Half-Life Deathmatch
   MOD_CSTRIKE,  // Counter-Strike
   MOD_GEARBOX,  // Opposing Force
   MOD_RICOCHET, // Ricochet
   MOD_DMC,      // Deathmatch Classic
   MOD_TFC,      // Team Fortress Classic
   MOD_DOD,      // Days of Defeat
   MOD_CSCLASSIC,// CS Classic
   MOD_TOD,      // Tours of Duty
   MOD_NS,       // Natural Selection
   MOD_FLF,      // Frontline Force
   MOD_HOLYWARS, // Holy Wars
   MOD_AHL,      // Action Half-Life
   MOD_PVK,      // PVK
   MOD_HI,       // Hostile Intent
   MOD_ASHEEP,   // Azure Sheep
};

// user message record structure definition
typedef struct usermsg_s {
   const char *name; // name of user message as called by the MOD DLL
   int id; // identification number the engine recorded this user message under
} usermsg_t;

// console command structure definition
typedef struct {
   const char *szCmd;     // command name
   void (*pFunc) (void);  // function to execute this command
   bool fInGame;          // whether we need to be in game or not
} consolecmd_t;

#define ITEM_FLAG_SELECTONEMPTY     1
#define ITEM_FLAG_NOAUTORELOAD      2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY 4
#define ITEM_FLAG_LIMITINWORLD      8
// A player can totally exhaust their ammo supply and lose this weapon
#define ITEM_FLAG_EXHAUSTIBLE       16

typedef struct weaponinfo_s {
   int   iSlot;     // HUD slot (0 based)
   int   iPosition; // slot position
   int   iAmmo1;    // ammo index for primary ammo
   int   iMaxAmmo1; // max primary ammo
   int   iAmmo2;    // ammo index for secondary ammo
   int   iMaxAmmo2; // max secondary ammo
   int   iId;       // weapon ID
   char  szClassname[64];
   int   iFlags;
} weaponinfo_t;

// namefunc.cpp functions...
void FreeNameFuncGlobals(void);
void LoadSymbols(const char *filename);
unsigned long FunctionFromName(const char *pName);
const char *NameForFunction(unsigned long function);

char *va(char *format, ...);

void BotServerCommand(void);
void BotClientCommand(class CClient *pClient);

// util.cpp functions...
void trim(char *str);
void NormalizeFilename(char *string);

void Capitalize(char *sz);
void StripTags(char *buffer);
void HumanizeName(char *string);
void HumanizeChat(char *buffer);

long RandomLong(long from, long to);
float RandomFloat(float from, float to);

int compress(char *data_p);

void CRC_Init(unsigned short *crcvalue);
void CRC_ProcessByte(unsigned short *crcvalue, unsigned char data);
unsigned short CRC_Value(unsigned short crcvalue);
unsigned short CRC_Block(unsigned char *start, int count);

void CRC32_Init(unsigned long *crcvalue);
void CRC32_ProcessByte(unsigned long *crcvalue, unsigned char data);
unsigned long CRC32_Value(unsigned long crcvalue);
unsigned long CRC32_Block(unsigned char *start, int count);

#define MAX_PLAYERS        32 // max player allowed

#ifdef _MSC_VER
#include <stdio.h>
#include <cstdio>
#define printf GINA_PRINTF
int GINA_PRINTF(const char *fmt, ...);
#endif

#include "bot_globals.h"
#include "world.h"
#include "entity.h"
#include "client.h"
#include "bot.h"
#include "bot_navigate.h"
#include "bot_chat.h"
#include "general.h"
#include "server.h"
#include "bot_manager.h"
#include "debug.h"
#include "ini.h"

// engine_util.cpp functions...
CEntity *UTIL_FindEntityInSphere(CEntity *pStartEntity, const Vector &vecCenter, float flRadius);
CEntity *UTIL_FindEntityByString(CEntity *pStartEntity, const char *szKeyword, const char *szValue);

inline CEntity *UTIL_FindEntityByClassname(CEntity *pStartEntity, const char *szName)
{
   return UTIL_FindEntityByString( pStartEntity, "classname", szName );
}

inline CEntity *UTIL_FindEntityByTargetname(CEntity *pStartEntity, const char *szName)
{
   return UTIL_FindEntityByString( pStartEntity, "targetname", szName );
}

typedef struct traceresult_s
{
   bool allsolid;    // if true, plane is not valid
   bool startsolid;  // if true, the initial point was in a solid area
   bool inopen, inwater;
   float fraction;   // time completed, 1.0 = didn't hit anything
   Vector endpos;    // final position
   plane_t plane;    // surface normal at impact
   CEntity *ent;     // entity the surface is on
   int hitgroup;     // 0 == generic, non zero is specific body part
} traceresult_t;

traceresult_t TestLine(const Vector &vecStart, const Vector &vecEnd,
                       bool igmon = true, CEntity *pentIgnore = NULL);
traceresult_t PlayerTestLine(const Vector &vecStart, const Vector &vecEnd, bool igmon = true,
                             bool isDuckPlayer = false, CEntity *pentIgnore = NULL);

bool IsLiquid(const Vector &v_location);
bool IsThinAir(const Vector &v_location);
bool PointBelongsToWorld(const Vector &v_location);

void AddNodeEntity(const Vector &origin, int effects = EF_NODRAW);
void ShowNodeEntity(bool show = true);
void RemoveAllNodeEntity(void);

// Include MOD specific headers
#include "cstrike/mod_specific.h"
#include "gearbox/mod_specific.h"
#include "valve/mod_specific.h"
#include "ricochet/mod_specific.h"

#endif // MAIN_H
