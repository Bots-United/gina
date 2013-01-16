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

#include "wpedit.h"

C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine,
    int *interfaceVersion)
{
   if (!pengfuncsFromEngine) {
      ALERT(at_error, "GetEngineFunctions called with null pengfuncsFromEngine");
      return FALSE;
   } else if (*interfaceVersion != ENGINE_INTERFACE_VERSION) {
      ALERT(at_error, "GetEngineFunctions version mismatch; requested=%d ours=%d", *interfaceVersion, ENGINE_INTERFACE_VERSION);
      // Tell metamod what version we had, so it can figure out who is out of date.
      *interfaceVersion = ENGINE_INTERFACE_VERSION;
      return FALSE;
   }

   return TRUE;
}

