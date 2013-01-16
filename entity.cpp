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
// entity.cpp
//

#include "main.h"

//=========================================================
// FInViewCone - returns true is the passed vector is in
// the caller's forward view cone.
//=========================================================
bool CEntity::FInViewCone(Vector *pOrigin, float fov)
{
   if (fov <= 0)
      fov = GetFov();

   Vector forward;
   GetViewAngles().AngleVectors(&forward);

   Vector vecLOS = (*pOrigin - GetGunPosition()).Normalize();
   float flDot = DotProduct(vecLOS, forward);

   return (flDot >= cos((fov / 2) * (M_PI / 180)));
}

bool CEntity::FInViewCone(CEntity *pEntity, float fov)
{
   Vector v_origin = pEntity->GetOrigin();
   return FInViewCone(&v_origin, fov);
}

//=========================================================
// FVisible - returns true if a line can be traced from
// the caller's eyes to the target
//=========================================================
bool CEntity::FVisible(const Vector &vecDest)
{
   // don't look through water
   if (IsLiquid(GetGunPosition()) != IsLiquid(vecDest))
      return false;

   // check if line of sight to object is not blocked (i.e. visible)
   return (TestLine(EyePosition(), vecDest).fraction >= 1.0);
}

bool CEntity::FVisible(CEntity *pEntity)
{
   // don't look through water
   if (IsInWater() != pEntity->IsInWater())
      return false;

   return (TestLine(EyePosition(), pEntity->EyePosition(), true, pEntity).fraction >= 1.0);
}

bool CEntity::FBoxVisible(CEntity *pEntity, Vector *pvHit, unsigned char *ucBodyPart)
{
   if (ucBodyPart)
      *ucBodyPart = 0;

   // don't look through water
   if (IsInWater() != pEntity->IsInWater())
      return false;

   bool            fVisible = false;
   traceresult_t   tr;

   // Check direct Line to waist
   Vector vecLookerOrigin = GetGunPosition();
   Vector vecTarget = pEntity->GetOrigin();
   tr = TestLine(vecLookerOrigin, vecTarget, true, NULL);
   if (tr.fraction >= 1.0) {
      if (pvHit)
         *pvHit = tr.endpos;
      if (ucBodyPart)
         *ucBodyPart |= WAIST_VISIBLE;
      fVisible = true;
   }

   // Check direct Line to head
   vecTarget = pEntity->GetGunPosition();
   tr = TestLine(vecLookerOrigin, vecTarget, true, NULL);
   if (tr.fraction >= 1.0) {
      if (pvHit)
         *pvHit = tr.endpos;
      if (ucBodyPart)
         *ucBodyPart |= HEAD_VISIBLE;
      fVisible = true;
   }

   if (fVisible)
      return true;

   // Nothing visible - check randomly other parts of body
   for (int i = 0; i < 5; i++) {
      vecTarget.x = RandomFloat(pEntity->GetAbsMin().x, pEntity->GetAbsMax().x);
      vecTarget.y = RandomFloat(pEntity->GetAbsMin().y, pEntity->GetAbsMax().y);
      vecTarget.z = RandomFloat(pEntity->GetAbsMin().z, pEntity->GetAbsMax().z);

      tr = TestLine(vecLookerOrigin, vecTarget, true, NULL);

      if (tr.fraction == 1.0) {
         // Return seen position
         if (pvHit)
            *pvHit = tr.endpos;
         if (ucBodyPart)
            *ucBodyPart |= CUSTOM_VISIBLE;
         return true;
      }
   }

   return false; // it's invisible
}

void CEntity::ChangeAngles(const Vector &idealangles, float speed)
{
#if 0
   pev->v_angle = pev->angles = idealangles;
   pev->v_angle.x *= -1;
   pev->angles.x /= 3;
#else
   Vector v_deviation = Vector(-idealangles.x, idealangles.y, idealangles.z) - GetViewAngles();
   v_deviation.ClampAngles();

   float da_deadly_math = exp(log(speed / 2) * g_pServer->GetMsec() / 50);

   // Thanks Tobias Heimann and Johannes Lampel for this one
   pev->yaw_speed = (pev->yaw_speed * da_deadly_math + speed * v_deviation.y * (1 - da_deadly_math)) * g_pServer->GetMsec() / 50;
   pev->pitch_speed = (pev->pitch_speed * da_deadly_math + speed * v_deviation.x * (1 - da_deadly_math)) * g_pServer->GetMsec() / 50;

   // influence of y movement on x axis and vice versa (less influence than x on y since it's
   // easier and more natural for the bot to "move its mouse" horizontally than vertically)
   pev->pitch_speed += pev->yaw_speed / 4.5;
   pev->yaw_speed += pev->pitch_speed / 3;

   if (fabs(pev->pitch_speed) >= fabs(v_deviation.x) &&
      pev->pitch_speed * v_deviation.x >= 0)
      pev->pitch_speed = v_deviation.x;

   if (fabs(pev->yaw_speed) >= fabs(v_deviation.y) &&
      pev->yaw_speed * v_deviation.y >= 0)
      pev->yaw_speed = v_deviation.y;

   pev->v_angle.y += pev->yaw_speed;
   pev->v_angle.x += pev->pitch_speed;

   // set the body angles to point the gun correctly
   pev->angles.x = -pev->v_angle.x / 3;
   pev->angles.y = pev->v_angle.y;
#endif
   pev->v_angle.ClampAngles();
   pev->angles.ClampAngles();
}

#define MAX_ENT_INSTANCE 10

CEntity *CEntity::Instance(edict_t *pent)
{
   static CEntity entity[MAX_ENT_INSTANCE];
   static int iCur = 0;

   CEntity *pEntity = CClient::Instance(pent);
   if (pEntity != NULL)
      return pEntity;

   int index = iCur;

   iCur++;
   if (iCur >= MAX_ENT_INSTANCE)
      iCur = 0;

   entity[index].pev = &pent->v;
   return &entity[index];
}

