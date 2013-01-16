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

int m_spriteTexture = 0;
edict_t *listenserver_edict = NULL;

int DispatchSpawn(edict_t *pent)
{
   if (strcmp(STRING(pent->v.classname), "worldspawn") == 0) {
      PRECACHE_SOUND("weapons/xbow_hit1.wav");
      PRECACHE_SOUND("weapons/mine_activate.wav");
      PRECACHE_SOUND("common/wpn_hudoff.wav");
      PRECACHE_SOUND("common/wpn_hudon.wav");
      PRECACHE_SOUND("common/wpn_moveselect.wav");
      PRECACHE_SOUND("common/wpn_denyselect.wav");

      m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr");
   }

   RETURN_META_VALUE(MRES_IGNORED, 0);
}

BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{ 
   // check if this client is the listen server client
   if (strcmp(pszAddress, "loopback") == 0) {
      // save the edict of the listen server client...
      listenserver_edict = pEntity;
   }

   RETURN_META_VALUE(MRES_IGNORED, 0);
}

void ClientCommand( edict_t *pEntity )
{
   // only allow custom commands if NOT dedicated server and
   // client sending command is the listen server client...
   if (!IS_DEDICATED_SERVER() && pEntity == listenserver_edict) {
   }

   RETURN_META(MRES_IGNORED);
}

void StartFrame()
{
   WaypointThink();
   RETURN_META(MRES_IGNORED);
}

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable,
    int *interfaceVersion)
{
   pFunctionTable->pfnSpawn = DispatchSpawn;
   pFunctionTable->pfnClientConnect = ClientConnect;
   pFunctionTable->pfnClientCommand = ClientCommand;
   pFunctionTable->pfnStartFrame = StartFrame;

   return TRUE;
}

