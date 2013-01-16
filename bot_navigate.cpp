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
// bot_navigate.cpp
//

#include "main.h"

CBotNav::CBotNav(CBaseBot *pBot) : m_pBot(pBot)
{
   Init();
}

CBotNav::~CBotNav()
{
}

void CBotNav::Init()
{
   ClearWP();
   ClearLTG();
   ClearSTG();

   m_vecPrevLocation = NULLVEC;
   m_flStuckCheckTime = 0;
   m_fPrevIsStuck = false;
   m_flWptTimeset = 0;

   m_fLastWaterState = false;
   m_flTimeEnterWater = 0;
}

void CBotNav::Update()
{
   RecordWaterState(); // record if bot needs to find air

   // if bot is supposed to move, record if bot is stuck
   if (m_pBot->GetMaxspeed() > 10 &&
      (m_pBot->bi.actionflags & (ACTION_MOVEFORWARD | ACTION_MOVEBACK | ACTION_MOVELEFT | ACTION_MOVERIGHT))) {
      // is it the time to check if bot is stuck?
      if (FZero(m_flStuckCheckTime)) {
         // first time, just record bot's current location
         m_flStuckCheckTime = g_pServer->GetTime() + 2;
         m_vecPrevLocation = m_pBot->GetOrigin();
         m_flIsStuck = 0;
      } else if (m_flStuckCheckTime < g_pServer->GetTime()) {
         m_flStuckCheckTime = g_pServer->GetTime() + 0.5; // next stuck checking time

         // if bot is on ground, check 2D Vector only; else check 3D Vector
         float flBotMoved;
         if (m_pBot->IsOnFloor()) {
            flBotMoved = (m_vecPrevLocation - m_pBot->GetOrigin()).LengthSquared2D();
         } else {
            flBotMoved = (m_vecPrevLocation - m_pBot->GetOrigin()).LengthSquared();
         }

         // Did we NOT move enough previously?
         if (flBotMoved < SQUARE(2)) {
            m_flIsStuck += 0.3; // consider being stuck if so
         } else if (m_pBot->IsOnFloor()) {
            m_flIsStuck -= 0.2 * m_pBot->GetSpeed2D() / m_pBot->GetMaxspeed();
         } else {
            m_flIsStuck -= 0.2 * m_pBot->GetSpeed() / m_pBot->GetMaxspeed();
         }

         if (m_flIsStuck > 1.0)
            m_flIsStuck = 1.0;
         else if (m_flIsStuck < 0)
            m_flIsStuck = 0;

         m_vecPrevLocation = m_pBot->GetOrigin(); // record bot's current location

         if (m_fPrevIsStuck && !IsStuck()) {
            m_pBot->DebugMsg(DEBUG_BOTNAV, "Bot is UNSTUCK");
         } else if (!m_fPrevIsStuck && IsStuck()) {
            m_pBot->DebugMsg(DEBUG_BOTNAV, "Bot is STUCK!!!");
         }

         m_fPrevIsStuck = IsStuck();
      }
   } else {
      // bot is supposed not to move
      m_vecPrevLocation = NULLVEC;
      m_flStuckCheckTime = 0;
      m_flIsStuck = 0;
   }
}

// Returns if given location would hurt bot with
// falling damage (Thanks Count Floyd)
bool CBotNav::IsDeadlyDrop(const Vector &vecTargetPos)
{
   traceresult_t tr;

   Vector vecMove(0, (vecTargetPos - m_pBot->GetOrigin()).ToYaw(), 0), forward;
   vecMove.AngleVectors(&forward);

   Vector v_direction = (vecTargetPos - m_pBot->GetOrigin()).Normalize();   // 1 unit long
   Vector v_check = m_pBot->GetOrigin();
   Vector v_down = m_pBot->GetOrigin();

   v_down.z = v_down.z - 1000.0; // straight down 1000 units

   tr = PlayerTestLine(v_check, v_down, true, m_pBot->IsDucking(), m_pBot);

   // We're not on ground anymore?
   if (tr.fraction > 0.036)
      tr.fraction = 0.036;

   float height;
   float last_height = tr.fraction * 1000.0; // height from ground

   float distance = (vecTargetPos - v_check).Length(); // distance from goal

   while (distance > 16.0) {
      // move 10 units closer to the goal...
      v_check = v_check + v_direction * 16.0;

      v_down = v_check;
      v_down.z = v_down.z - 1000.0; // straight down 1000 units

      tr = PlayerTestLine(v_check, v_down, true, m_pBot->IsDucking(), m_pBot);

      // Wall blocking?
      if (tr.startsolid)
         return false;

      height = tr.fraction * 1000.0; // height from ground

      // Drops more than 100 Units?
      if (last_height < height - 100)
         return true;

      last_height = height;

      distance = (vecTargetPos - v_check).Length(); // distance from goal
   }

   return false;
}

bool CBotNav::CantMoveForward(const Vector &move_angles)
{
   // use some TraceLines to determine if anything is blocking the current
   // path of the bot.

   Vector         v_forward, v_right, v_src, v_dest;
   traceresult_t  tr;

   move_angles.AngleVectors(&v_forward, &v_right);

   v_src = m_pBot->EyePosition();
   v_dest = v_src + v_forward * 40;

   // first do a trace from the bot's eyes forward...
   tr = TestLine(v_src, v_dest, false, m_pBot);

   // check if the trace hit something...
   if (tr.fraction < 1.0) {
      if (strncmp(tr.ent->GetClassname(), "func_door", 9) != 0)
         return true;
   }

   // bot's head is clear, check at shoulder level...
   // trace from the bot's shoulder left diagonal forward to the right shoulder...
   v_src = m_pBot->EyePosition() + Vector(0, 0, -16) - v_right * 16;
   v_dest = v_src + v_forward * 40;

   tr = TestLine(v_src, v_dest, false, m_pBot);

   // check if the trace hit something...
   if (tr.fraction < 1.0) {
      if (strncmp(tr.ent->GetClassname(), "func_door", 9) != 0)
         return true;
   }

   // bot's head is clear, check at shoulder level...
   // trace from the bot's shoulder right diagonal forward to the left shoulder...
   v_src = m_pBot->EyePosition() + Vector(0, 0, -16) + v_right * 16;
   v_dest = v_src + v_forward * 40;

   tr = TestLine(v_src, v_dest, false, m_pBot);

   // check if the trace hit something...
   if (tr.fraction < 1.0) {
      if (strncmp(tr.ent->GetClassname(), "func_door", 9) != 0)
         return true;
   }

   // Now check below Waist
   if (m_pBot->IsDucking())  {
      v_src = m_pBot->GetOrigin();
      v_dest = v_src + v_forward * 40;

      tr = TestLine(v_src, v_dest, false, m_pBot);

      // check if the trace hit something...
      if (tr.fraction < 1.0) {
         if (strncmp(tr.ent->GetClassname(), "func_door", 9) != 0)
            return true;
      }
   } else {
      // Trace from the left Waist to the right forward Waist Pos
      v_src = m_pBot->GetOrigin() + Vector(0, 0, -17) + v_right * -16;
      v_dest = m_pBot->GetOrigin() + Vector(0, 0, -17) + v_right * 16 + v_forward * 40;

      tr = TestLine(v_src, v_dest, false, m_pBot);

      // check if the trace hit something...
      if (tr.fraction < 1.0) {
         if (strncmp(tr.ent->GetClassname(), "func_door", 9) != 0)
            return true;
      }

      // Trace from the left Waist to the right forward Waist Pos
      v_src = m_pBot->GetOrigin() + Vector(0, 0, -17) + v_right * 16;
      v_dest = m_pBot->GetOrigin() + Vector(0, 0, -17) + v_right * -16 + v_forward * 40;

      tr = TestLine(v_src, v_dest, false, m_pBot);

      // check if the trace hit something...
      if (tr.fraction < 1.0) {
         if (strncmp(tr.ent->GetClassname(), "func_door", 9) != 0)
            return true;
      }
   }

   return false;  // bot can move forward, return false
}

// record how long bot has been in water
void CBotNav::RecordWaterState()
{
   bool in_water = m_pBot->IsInWater() || m_pBot->IsInLava() || m_pBot->IsInSlime();

   if (m_fLastWaterState && !in_water) {
      m_flTimeEnterWater = 0;
   } else if (!m_fLastWaterState && in_water) {
      m_flTimeEnterWater = g_pServer->GetTime();
   }

   m_fLastWaterState = in_water;
}

// returns if bot should go for air
bool CBotNav::ShouldGoForAir()
{
   if (m_pBot->IsInWater() || m_pBot->IsInLava() || m_pBot->IsInSlime()) {
      // if bot has been in water for more than 6 seconds, go for air
      return g_pServer->GetTime() - m_flTimeEnterWater > 6;
   }

   return false; // bot isn't in water, everything is okay
}

// find the place to go for air when bot is underwater
Vector CBotNav::AirGoal()
{
   if (!m_pBot->IsInWater() && !m_pBot->IsInLava() && !m_pBot->IsInSlime())
      return NULLVEC; // bot isn't underwater, don't bother

   Vector start = m_pBot->GetOrigin(), end = m_pBot->GetOrigin() + Vector(0, 0, 1000);

   // trace up until we hit solid
   traceresult_t tr = TestLine(start, end, true, NULL);

   end = tr.endpos;
   end.z -= 16; // lower a bit

   if (IsLiquid(end) || !PointBelongsToWorld(end))
      return NULLVEC; // no enough space above water, fail

   // find the surface of the water
   while (end.z - start.z > 1) {
      // find the mid point
      Vector mid(start.x, start.y, (start.z + end.z) / 2);
      if (IsLiquid(mid)) {
         start.z = mid.z; // surface is ABOVE the mid point
      } else {
         end.z = mid.z; // surface is BELOW the mid point
      }
   }

   end.z += 16; // raise the point a bit to make sure bot will get air
   return end; // success!
}

// Head toward a LT goal we've already found
// Return true if goal is reached, false if not
bool CBotNav::HeadTowardLTG()
{
/*   // FIXME FIXME FIXME simple & dirty one just to make the bot work
   if (m_LTG.empty())
      return true; // reliability check

   // if the waypoint stack is empty, go finding a path...
   if (wp.empty() || m_vecLastPathFinding != CurrentLTG().origin) {
      ClearWP();
      int result = g_pServer->World()->FindPath(m_pBot->GetOrigin(), CurrentLTG().origin, &wp);
      m_vecLastPathFinding = CurrentLTG().origin;
      if (result == PATH_FINDING) {
         return false; // still finding the path, need to wait a while
      } else if (result == NO_PATH) {
         // No path is found, discard this goal
         m_LTG.pop();
         ClearWP();
         return true;
      }
   }

   if (wp.empty())
      return true;

   // do the actual waypoint navigation
   if (HeadTowardWaypoint()) {
      wp.pop(); // take out a waypoint
   }

   return wp.empty();*/
   return true; // TODO
}

// Head toward a ST goal we've already found
// Return true if goal is reached, false if not
bool CBotNav::HeadTowardSTG()
{
/*
   // FIXME FIXME FIXME simple & dirty one just to make the bot work
   if (m_STG.empty())
      return true; // reliability check

   // if the waypoint stack is empty, go finding a path...
   if (wp.empty() || m_vecLastPathFinding != CurrentSTG().origin) {
      ClearWP();
      int result = g_pServer->World()->FindPath(m_pBot->GetOrigin(), CurrentSTG().origin, &wp);
      m_vecLastPathFinding = CurrentSTG().origin;
      if (result == PATH_FINDING) {
         return false; // still finding the path, need to wait a while
      } else if (result == NO_PATH) {
         // No path is found, discard this goal
         m_STG.pop();
         ClearWP();
         return true;
      }
   }

   if (wp.empty())
      return true;

   // do the actual waypoint navigation
   if (HeadTowardWaypoint()) {
      wp.pop(); // take out a waypoint
   }

   return wp.empty();
*/
   return true; // TODO
}

// Head toward the current waypoint
// Returns true if bot reached the waypoint, false if not
bool CBotNav::HeadTowardWaypoint()
{
/*   // FIXME FIXME FIXME simple & dirty one just to make the bot work
   assert(!wp.empty()); // just to be sure
   HeadToward(CurrentWP().origin);
   g_General.DrawBeam(NULL, m_pBot->GetOrigin(), CurrentWP().origin, 10,
      0, 255, 255, 255, 255, 10, 1);
   if (IsStuck()) {
      m_pBot->bi.actionflags |= (ACTION_JUMP | ACTION_CROUCH);
   } else {
      m_pBot->bi.actionflags &= ~(ACTION_JUMP | ACTION_CROUCH);
   }
   if (TouchingGoal(CurrentWP().origin, Vector(-16, -16, -16), Vector(16, 16, 16)))
      return true;
   return false;*/
   return true; // TODO
}

// Head toward the specified vector, and return if bot is touching it
bool CBotNav::HeadToward(const Vector &vec)
{
   // move toward the specified vector
   bool ret = MoveToward(vec);

   // press the forward key
   m_pBot->bi.actionflags |= ACTION_MOVEFORWARD;

   // also face the position
   m_pBot->FacePosition(vec);

   // Return if bot touched that vector
   return ret;
}

// Move toward the specified vector, and return if bot is touching it
bool CBotNav::MoveToward(const Vector &vec)
{
   // calculate movement angles
   m_pBot->bi.dir = (vec - (m_pBot->GetOrigin() + m_pBot->GetVelocity() * g_pServer->GetFrameTime())).Normalize();

   // press the forward key
   m_pBot->bi.actionflags |= ACTION_MOVEFORWARD;

   // Return if bot touched that vector
   return PointInsideBoundingBox(vec, m_pBot->GetOrigin(), m_pBot->GetMins(), m_pBot->GetMaxs());
}

void CBotNav::Run()
{
   // TODO
/*   // if the goal stack is empty, find one first...
   if (m_LTG.empty()) {
      m_LTG.push(m_pBot->FindGoal());
      ClearWP(); // also clear the waypoint stack
   }

   if (HeadTowardLTG()) {
      m_LTG.pop();
   }*/
}

// Returns if the specified vector is touching the specified goal.
bool CBotNav::TouchingGoal(const Vector &vec, const CBotGoal &goal)
{
   return PointInsideBoundingBox(vec, goal.origin, goal.mins, goal.maxs);
}

// Returns if the bot is touching the goal.
bool CBotNav::TouchingGoal(const Vector &origin, const Vector &mins, const Vector &maxs)
{
   return BoundingBoxesTouching(m_pBot->GetOrigin(), m_pBot->GetMins(), m_pBot->GetMaxs(),
                                origin, mins, maxs);
}

bool CBotNav::TouchingGoal(const CBotGoal &goal)
{
   return TouchingGoal(goal.origin, goal.mins, goal.maxs);
}

// Returns true if bot reached the goal.
bool CBotNav::ReachedGoal(const CBotGoal &goal)
{
   if (goal.flags & GFL_ITEM) {
      // if touching the goal, we've definately reached it.
      if (TouchingGoal(goal)) {
         return true;
      }
      // if we should be able to see the goal but it isn't visible...
      if (goal.pEntity && goal.pEntity->IsValid() &&
          m_pBot->FInViewCone(goal.pEntity) && !m_pBot->FBoxVisible(goal.pEntity) &&
          m_pBot->FVisible(goal.origin)) {
         return true; // discard it
      }
   } else if (goal.flags & GFL_AIR) {
      if (!m_pBot->IsInWater() || !m_pBot->IsInLava() || !m_pBot->IsInSlime()) {
         return true; // if bot is no longer in water, consider we've got air
      }
      return TouchingGoal(goal);
   } else {
      return TouchingGoal(goal);
   }
   return false;
}

// clear the waypoint stack
void CBotNav::ClearWP()
{
   wp.clear();
}

// clear the long term goal stack
void CBotNav::ClearLTG(void)
{
   while (!m_LTG.empty())
      m_LTG.pop();
}

// clear the short term goal stack
void CBotNav::ClearSTG(void)
{
   while (!m_STG.empty())
      m_STG.pop();
}

