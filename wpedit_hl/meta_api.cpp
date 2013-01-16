/*
 * The GINA Bot - a computer opponent for Valve's FPS game Half-Life
 * Waypoint Editor Metamod Plugin
 *
 * Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>
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

#include "wpedit.h"

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
   NULL,                // pfnGetEntityAPI
   NULL,                // pfnGetEntityAPI_Post
   GetEntityAPI2,       // pfnGetEntityAPI2
   NULL,                // pfnGetEntityAPI2_Post
   NULL,                // pfnGetNewDLLFunctions
   NULL,                // pfnGetNewDLLFunctions_Post
   GetEngineFunctions,  // pfnGetEngineFunctions
   NULL,                // pfnGetEngineFunctions_Post
};

// Description of plugin
plugin_info_t Plugin_info = {
   META_INTERFACE_VERSION, // ifvers
   "GINA Waypoint Editor", // name
   "0", // version
   __DATE__, // date
   "Wei Mingzhi <whistler_wmz@users.sf.net>", // author
   "http://gina.sf.net", // url
   "GWPE", // logtag, all caps please
   PT_ANYTIME, // (when) loadable
   PT_ANYPAUSE, // (when) unloadable
};

// Global vars from metamod:
meta_globals_t       *gpMetaGlobals;   // metamod globals
gamedll_funcs_t      *gpGamedllFuncs;  // gameDLL function tables
mutil_funcs_t        *gpMetaUtilFuncs; // metamod utility functions

// Metamod requesting info about this plugin:
//  ifvers          (given) interface_version metamod is using
//  pPlugInfo       (requested) struct with info about plugin
//  pMetaUtilFuncs  (given) table of utility functions provided by metamod
C_DLLEXPORT int Meta_Query(char *ifvers, plugin_info_t **pPlugInfo,
   mutil_funcs_t *pMetaUtilFuncs)
{
   // Give metamod our plugin_info struct
   *pPlugInfo = &Plugin_info;

   // Get metamod utility function table.
   gpMetaUtilFuncs = pMetaUtilFuncs;
   return TRUE;
}

// Metamod attaching plugin to the server.
//  now             (given) current phase, ie during map, during changelevel, or at startup
//  pFunctionTable  (requested) table of function tables this plugin catches
//  pMGlobals       (given) global vars from metamod
//  pGamedllFuncs   (given) copy of function tables from game dll
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now,
   META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals,
   gamedll_funcs_t *pGamedllFuncs) 
{
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

   return TRUE;
}

// Metamod detaching plugin from the server.
// now      (given) current phase, ie during map, etc
// reason   (given) why detaching (refresh, console unload, forced unload, etc)
C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason) 
{
   if (now > Plugin_info.unloadable && reason != PNL_CMD_FORCED) {
      LOG_ERROR(PLID, "Can't unload plugin right now");
      return(FALSE);
   }

   return TRUE;
}

