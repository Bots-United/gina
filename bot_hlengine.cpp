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

/*******************************************************
 * bot_hlengine.cpp                                    *
 *******************************************************
 * Purpose: Bot handling for HL engine.                *
 * Engine-specific: yes                                *
 *******************************************************/

#include "main.h"

void CBaseBot::SelectWeapon(int iId)
{
   if (iId != m_iCurrentWeaponId) {
      // is the specified weapon ID valid ?
      if (iId < 0 || iId > MAX_WEAPONS) {
         g_General.TerminateOnError("CBaseBot::SelectWeapon(): Invalid weapon ID !\n");
      }

      // issue a client command to select this weapon...
      g_General.FakeClientCommand(this, g_General.GetWeaponName(iId));
   }
}

void CBaseBot::PerformMovement()
{
   // Translate bot input
   float flMoveSpeed = GetMaxspeed();

   ReleaseAllButtons();

   // if bot is pressing the walk button...
   if (bi.actionflags & ACTION_WALK) {
      PushButton(IN_RUN); // push the walk button
      flMoveSpeed *= 0.3; // reduce his move speed
   }

   // bot want to respawn?
   if (bi.actionflags & ACTION_RESPAWN) {
      // push attack randomly to respawn
      if (RandomLong(0, 1))
         PushButton(IN_ATTACK);
   }

   // bot want to fire?
   if (bi.actionflags & ACTION_ATTACK) {
      PushButton(IN_ATTACK); // push attack to fire
   }

   // bot want to use secondary attack?
   if (bi.actionflags & ACTION_ATTACK2) {
      PushButton(IN_ATTACK2); // push attack to fire
   }

   // bot want to reload?
   if (bi.actionflags & ACTION_RELOAD) {
      PushButton(IN_RELOAD); // push the reload button
   }

   // bot want to use something?
   if (bi.actionflags & ACTION_USE) {
      PushButton(IN_USE);
   }

   // Change bots body angle and view angle
   float turn_skill = 0.3 * GetSkill() / 100, speed;

   // if bot is aiming at something, aim fast, else take our time...
   if (m_pEnemy)
      speed = 0.7 + turn_skill; // fast aim
   else
      speed = 0.2 + turn_skill / 2; // slow aim

   ChangeAngles(bi.idealangles, speed); // change bot angles

   // bot want to move forward?
   if (bi.actionflags & ACTION_MOVEFORWARD) {
      PushButton(IN_FORWARD); // push the forward button
   }

   // bot want to move back?
   if (bi.actionflags & ACTION_MOVEBACK) {
      PushButton(IN_BACK); // push the moveback button
   }

   // bot want to move left?
   if (bi.actionflags & ACTION_MOVELEFT) {
      PushButton(IN_MOVELEFT);
   }

   // bot want to move right?
   if (bi.actionflags & ACTION_MOVERIGHT) {
      PushButton(IN_MOVERIGHT); // press the button
   }

   // bot want to jump?
   if (bi.actionflags & ACTION_JUMP) {
      PushButton(IN_JUMP); // press the button
   }

   // bot want to crouch?
   if (bi.actionflags & ACTION_CROUCH) {
      PushButton(IN_DUCK); // press the button
   }

   // switch to correct weapon
   if (bi.weapon != 0)
      SelectWeapon(bi.weapon);

   // set the view independant movement
   float SpeedForward = 0, SpeedSide = 0, SpeedUp = 0;

   if (fabs(bi.dir.LengthSquared2D()) > ON_EPSILON * ON_EPSILON) {
      float f1 = AngleNormalize(GetViewAngles().y - bi.dir.ToYaw()) * M_PI / 180;

      float flSin, flCos;
      SinCos(f1, &flSin, &flCos);

      SpeedForward = flMoveSpeed * flCos;
      SpeedSide = flMoveSpeed * flSin;
   }

   // pass to the engine
   g_engfuncs.pfnRunPlayerMove(edict(), (float *)GetViewAngles(), SpeedForward,
                               SpeedSide, SpeedUp, pev->button,
                               pev->impulse, g_pServer->GetMsec());
}

void CBaseBot::Say(const char *fmt, ...)
{
   char text[256];
   va_list argptr;

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   _vsnprintf(text, sizeof(text), fmt, argptr);
   va_end(argptr);

   g_General.FakeClientCommand(this, "say \"%s\"\n", text);
}

void CBaseBot::SayTeam(const char *fmt, ...)
{
   char text[256];
   va_list argptr;

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   _vsnprintf(text, sizeof(text), fmt, argptr);
   va_end(argptr);

   g_General.FakeClientCommand(this, "say_team \"%s\"\n", text);
}
