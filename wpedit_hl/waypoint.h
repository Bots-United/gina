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

#ifndef WAYPOINT_H
#define WAYPOINT_H

const int MAX_WAYPOINTS = 4096; // max of 4096 waypoints
const int MAX_PATH_INDEX = 4;

// define the structure for waypoints...
typedef struct {
   Vector        vecOrigin;     // origin of this node
   Vector        vecOriginPeak; // origin of top of this node
   unsigned int  iFlags;        // flags of this node
} WAYPOINT;

// define the structure for waypoint paths (paths are connections between
// two waypoint nodes that indicates the bot can get from point A to point B.
// note that paths DON'T have to be two-way.  sometimes they are just one-way
// connections between two points.  There is an array called "paths" that
// contains head pointers to these structures for each waypoint index.
typedef struct path {
   short int index[MAX_PATH_INDEX];  // indices of waypoints (index -1 means not used)
   unsigned int flags[MAX_PATH_INDEX]; // flags of the paths
   struct path *next;   // link to next structure
} PATH;

extern WAYPOINT waypoints[MAX_WAYPOINTS];
extern bool g_fWaypointOn;
extern bool g_fWaypointPaths;
extern bool g_fAutoWaypoint;
extern bool g_fPathWaypoint;

void WaypointInit(void);
void WaypointAddPath(int from, int to, int flags);
void WaypointDeletePath(short int del_index);
void WaypointDeletePath(short int path_index, short int del_index);
void WaypointThink(void);
void WaypointLoad(void);
void WaypointSave(void);
bool IsReachable(const Vector &v_src, const Vector &v_dest);
void WaypointDraw(const WAYPOINT &wp);

const float MAX_REACHABLE = 400.0;

#endif

