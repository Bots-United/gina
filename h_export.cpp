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
// h_export.cpp
//
// main exported DLL functionality
//
// Based on: HPB bot - botman's High Ping Bastard bot
// (http://planethalflife.com/botman/)
//

#include "main.h"

#ifdef WINDOWSNT
HINSTANCE h_Library = NULL;
#else
void *h_Library = NULL;
#endif

enginefuncs_t g_engfuncs;
globalvars_t *gpGlobals;

DLL_FUNCTIONS other_gFunctionTable;

GIVEFNPTRSTODLL other_GiveFnptrsToDll;
GETENTITYAPI other_GetEntityAPI = NULL;
GETNEWDLLFUNCTIONS other_GetNewDLLFunctions = NULL;
SERVER_GETBLENDINGINTERFACE other_Server_GetBlendingInterface = NULL;

#ifdef WINDOWSNT
#ifdef __GNUC__
extern "C" __declspec(dllexport)
#endif
BOOL __stdcall DllMain(HINSTANCE, DWORD fdwReason, LPVOID)
{
   if (fdwReason == DLL_PROCESS_DETACH) {
      if (h_Library)
         FreeLibrary(h_Library);
      FreeNameFuncGlobals();
   }
   return TRUE;
}
#endif

typedef struct game_modinfo_s {
   char   *name;      // name (the game dir)
   char   *linux_so;  // filename of GNU/Linux shared library
   char   *mac_dylib; // filename of Mac OS X shared library
   char   *w32_dll;   // filename of Windows DLL
   int     mod_id;    // mod ID
   bool    official;  // whether this is a Valve's mod or not
} game_modinfo_t;

game_modinfo_t known_games[] = {
   // name        linux_so          mac_dylib         w32_dll        mod_id      official 
   {"cstrike",  "cs_i386.so",       "cs.dylib",       "mp.dll",      MOD_CSTRIKE,  true},  // Counter-Strike
   {"cs13",     "cs_i386.so",       "cs.dylib",       "mp.dll",      MOD_CSTRIKE,  false}, // Counter-Strike 1.3 on Steam
   {"gearbox",  "opfor_i386.so",    "opfor.dylib",    "opfor.dll",   MOD_GEARBOX,  true},  // Opposing Force
   {"valve",    "hl_i386.so",       "hl.dylib",       "hl.dll",      MOD_VALVE,    true},  // Half-Life
   {"ricochet", "ricochet_i386.so", "ricochet.dylib", "mp.dll",      MOD_RICOCHET, true},  // Ricochet
   {"hldm",     "hl_i386.so",       "hl.dylib",       "hl.dll",      MOD_VALVE,    false}, // Half-Life (for CS Retail)
   {"redempt",  "hl_i386.so",       "hl.dylib",       "redempt.dll", MOD_VALVE,    false}, // Maverick's Redemption (same as HLDM)
   {NULL, NULL, NULL, 0, false} // terminator
};

// Find a modinfo corresponding to the given game name.
game_modinfo_t *lookup_game(const char *name)
{
   game_modinfo_t *imod = known_games;
   while (imod->name) {
      if (stricmp(imod->name, name) == 0)
         return imod;
      imod++;
   }
   return NULL; // no match found
}

// If we're using MSVC, we need to specify the export parameter (Thanks Jozef Wagner)...
#if _MSC_VER > 1000
#pragma comment(linker, "/EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8,@1")
#pragma comment(linker, "/SECTION:.data,RW")
#endif

/**
 * This is the very first function that is called in the game DLL by the engine. Its purpose
 * is to set the functions interfacing up, by exchanging the pengfuncsFromEngine function list
 * along with a pointer to the engine's global varibales structure pGlobals, with the game
 * DLL. Also, since there is a known bug on Windows platforms that prevent hook DLLs (such as
 * our bot DLL) to be used in single player games (because they don't export all the stuff
 * they should), we may need to build our own array of exported symbols from the actual game
 * DLL in order to use it as such if necessary. Nothing really bot-related is done in this
 * function. The actual bot initialization stuff will be done later.
 */
void STDCALL GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
{
   // get the engine functions from the engine...
   memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
   gpGlobals = pGlobals;

   char mod_name[256];
   g_General.GetModName(mod_name);
   game_modinfo_t *known = lookup_game(mod_name);

   if (known) {
      g_General.m_iModid = known->mod_id;

      if (g_fIsMetamod)
         return;

#if defined(WINDOWSNT)
      if (known->official) {
         // test if the game DLL file is NOT available outside of the Steam cache
         if (access(va("%s/dlls/%s", mod_name, known->w32_dll), 0) == -1) {
            int filesize;
            unsigned char *filebuf = LOAD_FILE_FOR_ME(va("dlls/%s", known->w32_dll), &filesize);
            if (filebuf) {
               CreateDirectoryA(va("%s/dlls", mod_name), NULL);
               FILE *filep = fopen(va("%s/dlls/%s", mod_name, known->w32_dll), "wb");
               if (filep != NULL) {
                  fwrite(filebuf, 1, filesize, filep); // if in cache, then extract it
                  fclose(filep);
               }
               FREE_FILE(filebuf);
            }
         }
      }

      h_Library = LoadLibrary(va("%s/dlls/%s", mod_name, known->w32_dll));
#elif defined (__linux__)
      h_Library = dlopen(va("%s/dlls/%s", mod_name, known->linux_so), RTLD_NOW);
      if (h_Library == NULL) {
         char buf[256];
         strcpy(buf, known->linux_so);
         char *p = strstr(buf, "_i386.so");
         if (p != NULL) {
            strcpy(p, ".so");
            h_Library = dlopen(va("%s/dlls/%s", mod_name, known->linux_so), RTLD_NOW);
         }
      }
#elif defined (__APPLE__)
      h_Library = dlopen(va("%s/dlls/%s", mod_name, known->mac_dylib), RTLD_NOW);
#endif
   } else {
      g_General.TerminateOnError("Unsupported MOD!");
   }

   if (!h_Library) {
      g_General.TerminateOnError("Fail loading MOD DLL!");
   }

   other_GiveFnptrsToDll = (GIVEFNPTRSTODLL)dlsym(h_Library, "GiveFnptrsToDll");
   other_GetEntityAPI = (GETENTITYAPI)dlsym(h_Library, "GetEntityAPI");
   other_GetNewDLLFunctions = (GETNEWDLLFUNCTIONS)dlsym(h_Library, "GetNewDLLFunctions");
   other_Server_GetBlendingInterface = (SERVER_GETBLENDINGINTERFACE)dlsym(h_Library, "Server_GetBlendingInterface");

#ifdef WINDOWSNT
   LoadSymbols(va("%s/dlls/%s", mod_name, known->w32_dll));
   pengfuncsFromEngine->pfnFunctionFromName = FunctionFromName;
   pengfuncsFromEngine->pfnNameForFunction = NameForFunction;
#endif

   GetEngineFunctions(pengfuncsFromEngine, NULL);

   // give the engine functions to the other DLL...
   (*other_GiveFnptrsToDll)(pengfuncsFromEngine, pGlobals);
}

