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
// h_export.cpp
//
// main exported DLL functionality
//
// Based on: HPB bot - botman's High Ping Bastard bot
// (http://planethalflife.com/botman/)
//

#include "wpedit.h"

// Holds engine functionality callbacks
enginefuncs_t          g_engfuncs;
globalvars_t          *gpGlobals;

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
C_DLLEXPORT void STDCALL GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
{
   // get the engine functions from the engine...
   memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
   gpGlobals = pGlobals;
}

