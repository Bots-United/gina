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
// meta_api.cpp
//
// Implementation of metamod's plugin interface.
//

#include "main.h"

bool g_fIsMetamod = FALSE;

int GetEntityAPI_Post(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion);

META_FUNCTIONS gMetaFunctionTable = {
   GetEntityAPI, // pfnGetEntityAPI()
   GetEntityAPI_Post, // pfnGetEntityAPI_Post()
   NULL, // pfnGetEntityAPI2()
   NULL, // pfnGetEntityAPI2_Post()
   NULL, // pfnGetNewDLLFunctions()
   NULL, // pfnGetNewDLLFunctions_Post()
   GetEngineFunctions, // pfnGetEngineFunctions()
   NULL, // pfnGetEngineFunctions_Post()
};

plugin_info_t Plugin_info = {
   META_INTERFACE_VERSION, // interface version
   "The GINA bot", // plugin name
   "0", // plugin version
   __DATE__, // date of creation
   "Wei Mingzhi <whistler_wmz@users.sf.net>", // plugin author
   "http://gina.codeplex.com/", // plugin URL
   "GINA", // plugin logtag
   PT_STARTUP, // when loadable
   PT_NEVER, // when unloadable
};

// Global variables from metamod.  These variable names are referenced by
// various macros.
meta_globals_t *gpMetaGlobals;		// metamod globals
gamedll_funcs_t *gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;		// metamod utility functions

// Metamod requesting info about this plugin
//  ifvers              (given) interface_version metamod is using
//  pPlugInfo           (requested) struct with info about plugin
//  pMetaUtilFuncs      (given) table of utility functions provided by metamod
C_DLLEXPORT int Meta_Query(char *ifvers, plugin_info_t **pPlugInfo,
                           mutil_funcs_t *pMetaUtilFuncs)
{
   // Check for valid pMetaUtilFuncs before we continue.
   if (!pMetaUtilFuncs) {
      ALERT(at_logged, "[%s] ERROR: Meta_Query called with null pMetaUtilFuncs\n", Plugin_info.logtag);
      return FALSE;
   }

   gpMetaUtilFuncs = pMetaUtilFuncs;

   // Give metamod our plugin_info struct.
   *pPlugInfo = &Plugin_info;

   // Check for interface version compatibility.
   if (!FStrEq(ifvers, Plugin_info.ifvers)) {
      int mmajor = 0, mminor=0, pmajor=0, pminor=0;
      LOG_MESSAGE(PLID, "WARNING: meta-interface version mismatch; requested=%s ours=%s",
                  Plugin_info.logtag, ifvers);

      // If plugin has later interface version, it's incompatible (update metamod).
      sscanf(ifvers, "%d:%d", &mmajor, &mminor);
      sscanf(META_INTERFACE_VERSION, "%d:%d", &pmajor, &pminor);
      if (pmajor > mmajor || (pmajor==mmajor && pminor > mminor)) {
         LOG_ERROR(PLID, "metamod version is too old for this plugin; update metamod");
         return FALSE;
      }

      // If plugin has older major interface version, it's incompatible (update plugin).
      else if (pmajor < mmajor) {
         LOG_ERROR(PLID, "metamod version is incompatible with this plugin; please find a newer version of this plugin");
         return FALSE;
      }

      // Minor interface is older, but this is guaranteed to be backwards
      // compatible, so we warn, but we still accept it.
      else if (pmajor == mmajor && pminor < mminor)
         LOG_MESSAGE(PLID, "WARNING: metamod version is newer than expected; consider finding a newer version of this plugin");
      else
         LOG_ERROR(PLID, "unexpected version comparison; metavers=%s, mmajor=%d, mminor=%d; plugvers=%s, pmajor=%d, pminor=%d", ifvers, mmajor, mminor, META_INTERFACE_VERSION, pmajor, pminor);
   }

   return TRUE;
}

// Metamod attaching plugin to the server.
//  now                 (given) current phase, ie during map, during changelevel, or at startup
//  pFunctionTable      (requested) table of function tables this plugin catches
//  pMGlobals           (given) global vars from metamod
//  pGamedllFuncs       (given) copy of function tables from game dll
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, 
		meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
   if (now > Plugin_info.loadable) {
      LOG_ERROR(PLID, "Can't load plugin right now");
      return FALSE;
   }

   if (!pMGlobals) {
      LOG_ERROR(PLID, "Meta_Attach called with null pMGlobals");
      return FALSE;
   }

   gpMetaGlobals = pMGlobals;

   if (!pFunctionTable) {
      LOG_ERROR(PLID, "Meta_Attach called with null pFunctionTable");
      return FALSE;
   }

   memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
   gpGamedllFuncs = pGamedllFuncs;

   LOG_MESSAGE(PLID, "%s v%s  %s", Plugin_info.name, Plugin_info.version, 
               Plugin_info.date);
   LOG_MESSAGE(PLID, "by %s", Plugin_info.author);
   LOG_MESSAGE(PLID, "   %s", Plugin_info.url);

   return TRUE; // Let's go.
}

// Metamod detaching plugin from the server.
// now		(given) current phase, ie during map, etc
// reason	(given) why detaching (refresh, console unload, forced unload, etc)
C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
   if (now > Plugin_info.unloadable && reason != PNL_CMD_FORCED) {
      LOG_ERROR(PLID, "Can't unload plugin right now");
      return FALSE;
   }

   return TRUE; // Done!
}

// If this function is called, we're running under Metamod.
C_DLLEXPORT void Meta_Init(void)
{
   // Record that we're running under Metamod.
   g_fIsMetamod = TRUE;
}
