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
// Portions based on:
//
// HPB bot - botman's High Ping Bastard bot
// (http://planethalflife.com/botman/)
//

#include "wpedit.h"

WAYPOINT waypoints[MAX_WAYPOINTS]; // waypoints with information bits (flags)
int g_iNumWaypoints = 0; // number of waypoints currently in use
PATH *paths[MAX_WAYPOINTS]; // head pointers to the path structures...
// time that this waypoint was displayed (while editing)
float wp_display_time[MAX_WAYPOINTS];

bool g_fWaypointOn = false;
bool g_fWaypointPaths = false; // have any paths been allocated?
bool g_fAutoWaypoint = false;
bool g_fPathWaypoint = false;
Vector g_vecLastWaypoint(0, 0, 0);

// initialize the waypoint structures...
void WaypointInit()
{
   // have any waypoint path nodes been allocated yet?
   if (g_fWaypointPaths) {
      // free the linked list of waypoint path nodes...
      for (int i = 0; i < MAX_WAYPOINTS; i++) {
         PATH *p = paths[i];
         while (p != NULL) {
            // free the linked list
            PATH *p_next = p->next;  // save the link to next
            free(p);
            p = p_next;
         }
         paths[i] = NULL;
      }
   }

   for (int i = 0; i < MAX_WAYPOINTS; i++) {
      waypoints[i].iFlags = 0;
      waypoints[i].vecOrigin = Vector(0, 0, 0);
      waypoints[i].vecOriginPeak = Vector(0, 0, 0);

      paths[i] = NULL;  // no paths allocated yet
      wp_display_time[i] = 0.0;
   }

   g_iNumWaypoints = 0;
   g_vecLastWaypoint = Vector(0, 0, 0);
}

void WaypointAdd()
{
/*
   if (g_iNumWaypoints >= MAX_WAYPOINTS) {
      SERVER_PRINT("ERROR: There are too many waypoints!\n");
      return;
   }

   int index = g_iNumWaypoints;

   waypoints[index].iFlags = 0;

   // store the origin (location) of this waypoint (use entity origin)
   waypoints[index].vecOrigin = listenserver_edict->v.origin;
   waypoints[index].vecOrigin.z = listenserver_edict->v.absmin.z;

   // store the last used waypoint for the auto waypoint code...
   g_iLastWaypoint = listenserver_edict->v.origin;

   // set the time that this waypoint was originally displayed...
   wp_display_time[index] = gpGlobals->time;

   if (listenserver_edict->v.flags & FL_DUCKING) {
      waypoints[index].iFlags |= TFL_CROUCH; // crouching waypoint
   }

   if (pEntity->v.movetype == MOVETYPE_FLY)
      waypoints[index].iFlags |= TFL_LADDER; // waypoint on a ladder

   // search the area near the waypoint for items (HEALTH, AMMO, WEAPON, etc.)
//   WaypointSearchItems(pEntity, waypoints[index].vecOrigin, index);

   EMIT_SOUND_DYN2(listenserver_edict, CHAN_WEAPON, "weapons/xbow_hit1.wav",
      1.0, ATTN_NORM, 0, 100);

   g_iNumWaypoints++; // increment total number of waypoints...

   // calculate all the paths to this new waypoint
   for (int i = 0; i < g_iNumWaypoints; i++) {
      if (i == index)
         continue;  // skip the waypoint that was just added

      // check if the waypoint is reachable from the new one (one-way)
      if (IsReachable(listenserver_edict->v.origin, waypoints[i].vecOrigin)) {
         WaypointAddPath(index, i);
      }

      // check if the new one is reachable from the waypoint (other way)
      if (WaypointReachable(waypoints[i].vecOrigin, listenserver_edict->v.origin)) {
         WaypointAddPath(i, index);
      }
   }
*/
}

void WaypointAddPath(int add_index, int path_index, int flags)
{
   PATH *p = paths[add_index], *prev = NULL;
   int i;

   // check if this path already exists...
   while (p != NULL) {
      for (i = 0; i < MAX_PATH_INDEX; i++) {
         if (p->index[i] == path_index) {
            // play "cancelled" sound...
            EMIT_SOUND_DYN2(listenserver_edict, CHAN_WEAPON, "common/wpn_moveselect.wav",
                            1.0, ATTN_NORM, 0, 100);

            return;
         }
      }
      p = p->next; // go to next node in linked list
   }

   p = paths[add_index];

   // find an empty slot for new path_index...
   while (p != NULL) {
      for (i = 0; i < MAX_PATH_INDEX; i++) {
         if (p->index[i] == -1) {
            p->index[i] = path_index;
            p->flags[i] = flags;

            // play "start" sound...
            EMIT_SOUND_DYN2(listenserver_edict, CHAN_WEAPON, "common/wpn_hudoff.wav",
                            1.0, ATTN_NORM, 0, 100);

            return;
         }
      }
      prev = p; // save the previous node in linked list
      p = p->next; // go to next node in linked list
   }

   p = (PATH *)malloc(sizeof(PATH));

   if (p == NULL) {
      return; // ERROR ALLOCATING MEMORY!!!
   }

   for (i = 0; i < MAX_PATH_INDEX; i++) {
      p->index[i] = -1;
      p->flags[i] = 0;
   }

   p->index[0] = path_index;
   p->flags[0] = flags;
   p->next = NULL;

   if (prev != NULL)
      prev->next = p; // link new node into existing list

   if (paths[add_index] == NULL)
      paths[add_index] = p; // save head point if necessary

   // play "start" sound...
   EMIT_SOUND_DYN2(listenserver_edict, CHAN_WEAPON, "common/wpn_hudoff.wav",
                   1.0, ATTN_NORM, 0, 100);
}

void WaypointDeletePath(short int del_index)
{
   // search all paths for this index...
   for (int index = 0; index < g_iNumWaypoints; index++) {
      PATH *p = paths[index];

      // search linked list for del_index...
      while (p != NULL) {
         for (int i = 0; i < MAX_PATH_INDEX; i++) {
            if (p->index[i] == del_index) {
               p->index[i] = -1;  // unassign this path
            }
         }
         p = p->next;  // go to next node in linked list
      }
   }
}

void WaypointDeletePath(short int path_index, short int del_index)
{
   PATH *p = paths[path_index];

   // search linked list for del_index...
   while (p != NULL) {
      for (int i = 0; i < MAX_PATH_INDEX; i++) {
         if (p->index[i] == del_index) {
            // play "start" sound...
            EMIT_SOUND_DYN2(listenserver_edict, CHAN_WEAPON, "common/wpn_hudoff.wav",
                            1.0, ATTN_NORM, 0, 100);

            p->index[i] = -1;  // unassign this path
            return;
         }
      }

      p = p->next;  // go to next node in linked list
   }

   // play "cancelled" sound...
   EMIT_SOUND_DYN2(listenserver_edict, CHAN_WEAPON, "common/wpn_moveselect.wav",
                   1.0, ATTN_NORM, 0, 100);
}

void WaypointThink()
{
   if (!g_fWaypointOn)
      return; // waypoint editing is off, do nothing here

   int i, j;

   // Draw all waypoints
   for (i = 0; i < MAX_WAYPOINTS; i++) {
      if (wp_display_time[i] < gpGlobals->time) {
         WaypointDraw(waypoints[i]);

         // Draw all paths if pathwaypoint is on
         if (g_fPathWaypoint) {
            PATH *p = paths[i];
            Vector vecBegin = (waypoints[i].vecOrigin + waypoints[i].vecOriginPeak) / 2;
            while (p != NULL) {
               for (j = 0; j < MAX_PATH_INDEX; j++) {
                  int index = p->index[j], flag = p->flags[j];
                  Vector vecEnd = (waypoints[index].vecOrigin + waypoints[index].vecOriginPeak) / 2;

                  // TODO
               }
               p = p->next;
            }
         }
      }
   }
}

void WaypointLoad()
{
   char str[256];
   int linkcount, viscount, i, j, curlink = 1;

   GET_GAME_DIR(str); // Get the game directory

   // Load the ini file
   CIniFile ini(UTIL_VarArgs("%s/gina/maps/%s.ini", str, STRING(gpGlobals->mapname)));
   if (!ini.Valid()) {
      return; // can't load the waypoint file, give up
   }

   WaypointInit();

   // Get the number of waypoints, links and visibilities
   g_iNumWaypoints = atoi(ini.Get("INFO", "NodeCount"));
   linkcount = atoi(ini.Get("INFO", "LinkCount"));
   viscount = atoi(ini.Get("INFO", "VisCount"));

   for (i = 0; i < g_iNumWaypoints; i++) {
      waypoints[i].vecOrigin.x = atof(ini.Get(UTIL_VarArgs("NODE%d", i + 1), "OriginX"));
      waypoints[i].vecOrigin.y = atof(ini.Get(UTIL_VarArgs("NODE%d", i + 1), "OriginY"));
      waypoints[i].vecOrigin.z = atof(ini.Get(UTIL_VarArgs("NODE%d", i + 1), "OriginZ"));
      waypoints[i].iFlags = atoi(ini.Get(UTIL_VarArgs("NODE%d", i + 1), "Flags"));

      if (waypoints[i].iFlags & TFL_CROUCH) {
         // crouching player is 52 units high
         waypoints[i].vecOriginPeak = waypoints[i].vecOrigin + Vector(0, 0, 52);
      } else {
         // standing player is 74 units high
         waypoints[i].vecOriginPeak = waypoints[i].vecOrigin + Vector(0, 0, 74);
      }

      int c = atoi(ini.Get(UTIL_VarArgs("NODE%d", i + 1), "LinkCount"));
      for (j = 0; j < c; j++) {
         int iIndex = atoi(ini.Get(UTIL_VarArgs("LINK%d", curlink), "DestNode")) - 1;
         unsigned int iFlags = atoi(ini.Get(UTIL_VarArgs("LINK%d", curlink), "TravelFlags"));
         WaypointAddPath(i, iIndex, iFlags);
      }
   }
}

void WaypointSave()
{
   if (g_iNumWaypoints <= 0) {
      SERVER_PRINT("No waypoints to save!\n");
      return;
   }

   CIniFile ini;
   int linkindex = 1, visindex = 1, i, j;

   ini.Set("INFO", "NodeCount", UTIL_VarArgs("%d", g_iNumWaypoints));

   for (i = 0; i < g_iNumWaypoints; i++) {
      int count = 0;

      char key[10];
      strncpy(key, UTIL_VarArgs("NODE%d", i + 1), 10);

      // Store away the basic information of this waypoint...
      ini.Set(key, "OriginX", UTIL_VarArgs("%f", waypoints[i].vecOrigin.x));
      ini.Set(key, "OriginY", UTIL_VarArgs("%f", waypoints[i].vecOrigin.y));
      ini.Set(key, "OriginZ", UTIL_VarArgs("%f", waypoints[i].vecOrigin.z));

      ini.Set(key, "Flags", UTIL_VarArgs("%u", waypoints[i].iFlags));

      // Store away all the connections from this waypoint...
      PATH *path = paths[i];
      while (path != NULL) {
         for (j = 0; j < MAX_PATH_INDEX; j++) {
            if (path->index[j] < 0 || path->index[j] >= g_iNumWaypoints)
               continue;

            char k[10];
            strncpy(k, UTIL_VarArgs("LINK%d", linkindex), 10);

            ini.Set(k, "DestNode", UTIL_VarArgs("%d", path->index[j] + 1));
            ini.Set(k, "TravelFlags", UTIL_VarArgs("%d", path->flags[j]));

            linkindex++;
            count++;
         }

         path = path->next;
      }

      ini.Set(key, "LinkCount", UTIL_VarArgs("%d", count));

      count = 0;

      // calculate the visibilities of this waypoint...
      Vector v_src, v_dest;
      if (waypoints[i].iFlags & TFL_CROUCH) {
         v_src = waypoints[i].vecOrigin + Vector(0, 0, 36);
      } else {
         v_src = waypoints[i].vecOrigin + Vector(0, 0, 31);
      }

      for (j = 0; j < g_iNumWaypoints; j++) {
         TraceResult tr;
         int flag = 0;

         v_dest = waypoints[j].vecOriginPeak;
         UTIL_TraceLine(v_src, v_dest, ignore_monsters, ignore_glass,
            NULL, &tr);
         if (tr.fStartSolid || tr.flFraction == 1.0)
            flag |= VFL_TOP;

         v_dest.z -= 20;
         UTIL_TraceLine(v_src, v_dest, ignore_monsters, ignore_glass,
            NULL, &tr);
         if (tr.fStartSolid || tr.flFraction == 1.0)
            flag |= VFL_HEAD;

         v_dest = waypoints[j].vecOrigin;
         UTIL_TraceLine(v_src, v_dest, ignore_monsters, ignore_glass,
            NULL, &tr);
         if (tr.fStartSolid || tr.flFraction == 1.0)
            flag |= VFL_BOTTOM;

         if (waypoints[j].iFlags & TFL_CROUCH)
            v_dest.z += 19;
         else
            v_dest.z += 37;
         UTIL_TraceLine(v_src, v_dest, ignore_monsters, ignore_glass,
            NULL, &tr);
         if (tr.fStartSolid || tr.flFraction == 1.0)
            flag |= VFL_WAIST;

         if (flag != 0) {
            char k[10];
            strncpy(k, UTIL_VarArgs("VIS%d", linkindex), 10);

            ini.Set(k, "DestNode", UTIL_VarArgs("%d", j + 1));
            ini.Set(k, "VisFlags", UTIL_VarArgs("%d", flag));

            visindex++;
            count++;
         }
      }

      ini.Set(key, "VisCount", UTIL_VarArgs("%d", count));
   }

   ini.Set("INFO", "LinkCount", UTIL_VarArgs("%d", linkindex - 1));
   ini.Set("INFO", "VisCount", UTIL_VarArgs("%d", visindex - 1));

   char str[256];
   GET_GAME_DIR(str); // Get the game directory
   ini.Save(UTIL_VarArgs("%s/gina/maps/%s.ini", str, STRING(gpGlobals->mapname)));

   SERVER_PRINT("Waypoints saved\n");
}

bool IsReachable(const Vector &v_src, const Vector &v_dest)
{
   TraceResult tr;
   float height, last_height;

   float distance = (v_dest - v_src).Length();

   // is the destination close enough?
   if (distance < MAX_REACHABLE) {
      bool bIsVisible = FALSE;
      bool bIsNearDoor = FALSE;

      // check if this waypoint is visible...
      UTIL_TraceHull(v_src, v_dest, ignore_monsters, head_hull, NULL, &tr);
      if (tr.flFraction >= 1.0)
         bIsVisible = TRUE;

      // check if this waypoint is near a door
      UTIL_TraceLine(v_src, v_dest, ignore_monsters, NULL, &tr);
      if (!FNullEnt(tr.pHit)) {
         if (FClassnameIs(tr.pHit, "func_door") ||
            FClassnameIs(tr.pHit, "func_door_rotating"))
            bIsNearDoor = TRUE;
      }

      // if waypoint is visible from current position (even behind head)...
      if (bIsVisible || bIsNearDoor) {
         if (bIsNearDoor) {
            // If it's a door check if nothing blocks behind
            Vector vDoorEnd = tr.vecEndPos;
            UTIL_TraceLine(vDoorEnd, v_dest, ignore_monsters, ignore_glass, tr.pHit, &tr);
            if (tr.flFraction < 1.0)
               return FALSE;
         }

         // check for special case of waypoint being suspended in mid-air...

         // is dest waypoint higher than src? (45 is max jump height)
         if (v_dest.z > v_src.z + 45.0) {
            Vector v_new_src = v_dest;
            Vector v_new_dest = v_dest;

            v_new_dest.z = v_new_dest.z - 50;  // straight down 50 units

            UTIL_TraceLine(v_new_src, v_new_dest, ignore_monsters, ignore_glass, NULL, &tr);

            // check if we didn't hit anything, if not then it's in mid-air
            if (tr.flFraction >= 1.0)
               return FALSE;  // can't reach this one
         }

         // check if distance to ground drops more than step height at points
         // between source and destination...

         Vector v_direction = (v_dest - v_src).Normalize();  // 1 unit long
         Vector v_check = v_src;
         Vector v_down = v_src;

         v_down.z = v_down.z - 1000.0;  // straight down 1000 units

         UTIL_TraceLine(v_check, v_down, ignore_monsters, ignore_glass, NULL, &tr);

         last_height = tr.flFraction * 1000.0;  // height from ground

         distance = (v_dest - v_check).Length();  // distance from goal

         while (distance > 10.0) {
            // move 10 units closer to the goal...
            v_check = v_check + v_direction * 10.0;

            v_down = v_check;
            v_down.z = v_down.z - 1000.0;  // straight down 1000 units

            UTIL_TraceLine(v_check, v_down, ignore_monsters, ignore_glass, NULL, &tr);

            height = tr.flFraction * 1000.0;  // height from ground

            if (height < last_height - 18.0) // is the current height greater than the step height?
               return FALSE; // can't get there without jumping

            last_height = height;

            distance = (v_dest - v_check).Length();  // distance from goal
         }
         return TRUE;
      }
   }
   return FALSE;
}

void WaypointDraw(const WAYPOINT &wp)
{
   if (FNullEnt(listenserver_edict))
      return; // reliability check

   if (wp_display_time[&wp - waypoints] >= gpGlobals->time)
      return; // time not reached for displaying this waypoint

   int r = 0, g = 255, b = 0;

   // Draw the waypoint
   WaypointDrawBeam(listenserver_edict, wp.vecOrigin, wp.vecOriginPeak,
                    30, 0, r, g, b, 250, 5);

   // Remember the time of displaying of this waypoint
   wp_display_time[&wp - waypoints] = gpGlobals->time + 1.0;
}

