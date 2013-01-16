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
// bot_navigate.h
//

#ifndef BOT_NAVIGATE_H
#define BOT_NAVIGATE_H

#include "bot.h"

// bot waypoint
class CWaypoint {
public:
   CWaypoint() : origin(NULLVEC), flags(0) {};

   Vector         origin;      // waypoint origin
   unsigned long  flags;       // travel flags
};

// bot goal
class CBotGoal {
public:
   CBotGoal() : origin(NULLVEC),
                mins(NULLVEC),
                maxs(NULLVEC),
                pEntity(NULL),
                flags(0) {}

   Vector         origin;      // origin of the goal
   Vector         mins, maxs;  // mins and maxs of the goal
   CEntity       *pEntity;     // goal entity
   unsigned long  flags;       // goal flags
};

class CBotNav
{
public:
   CBotNav(CBaseBot *pBot);
   virtual ~CBotNav();

   CBaseBot        *m_pBot;

   void             Init(void);
   void             Update(void);
   void             Run(void);

   bool             HeadToward(const Vector &vec);
   bool             MoveToward(const Vector &vec);
   bool             IsDeadlyDrop(const Vector &vecTargetPos);
   bool             CantMoveForward(const Vector &move_angles);

   inline bool      IsStuck() { return m_flIsStuck >= 0.5; }

   bool             m_fPrevIsStuck;

protected:
   void             RecordWaterState(void);
   bool             ShouldGoForAir(void);
   Vector           AirGoal(void);

   Vector           m_vecPrevLocation; // previous bot location (for stuck checking)
   float            m_flStuckCheckTime;
   float            m_flIsStuck; // how "stuck" the bot is, >=0.5 = stuck

   std::list<CWaypoint>   wp; // bot waypoints

   float            m_flWptTimeset;  // Time waypoint chosen by bot

   ltg_type         m_LTGType; // long term goal type
   float            m_flLTGTime; // time to search for long term goal
   float            m_flSTGTime; // time to search for short term goal

   std::stack<CBotGoal>       m_LTG;
   std::stack<CBotGoal>       m_STG;

   inline const CWaypoint &CurrentWP(void) { return wp./*top*/front(); }
   inline const CBotGoal &CurrentLTG(void) { return m_LTG.top(); }
   inline const CBotGoal &CurrentSTG(void) { return m_STG.top(); }

//   void             NavigateTo(const Vector &vec);

   bool             HeadTowardLTG(void);
   bool             HeadTowardSTG(void);
   bool             HeadTowardWaypoint(void);

   bool             TouchingGoal(const Vector &vec, const CBotGoal &goal);
   bool             TouchingGoal(const Vector &origin, const Vector &mins, const Vector &maxs);
   bool             TouchingGoal(const CBotGoal &goal);

   bool             ReachedGoal(const CBotGoal &goal);

   void             ClearWP(void);
   void             ClearLTG(void);
   void             ClearSTG(void);

private:
   bool             m_fLastWaterState;
   float            m_flTimeEnterWater;
   Vector           m_vecLastPathFinding; // last pathfinding destination
};

#endif

