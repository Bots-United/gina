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
// mathlib.h
//

#ifndef MATHLIBRARY_H
#define MATHLIBRARY_H

// Use this definition globally
#define ON_EPSILON        0.01
#define EQUAL_EPSILON     0.001

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <math.h>

#define ABS(f)         ((f) > 0 ? (f) : -(f))
#define FZero(v)       (ABS(v) < ON_EPSILON)
#define FEqual(v1, v2) (ABS((v1) - (v2)) < EQUAL_EPSILON)
#define SQUARE(f)      ((f) * (f))

#pragma warning (disable: 4239)
#pragma warning (disable: 4244)

/**
 * This function adds or substracts 360 enough times needed to the given angle in
 * order to set it into the range [0, 360) and returns the resulting angle. Letting
 * the engine have a hand on angles that are outside these bounds may cause the
 * game to freeze by screwing up the engine math code.
 */
inline float AngleMod(float a)
{
   return (360.0 / 65536) * ((int)(a * (65536.0 / 360.0)) & 65535);
}

/**
 * This function adds or substracts 360 enough times needed to the given angle in
 * order to set it into the range [-180, 180) and returns the resulting angle. Letting
 * the engine have a hand on angles that are outside these bounds may cause the game
 * to freeze by screwing up the engine math code.
 */
inline float AngleNormalize(float angle)
{
   return (360.0 / 65536) * ((int)((angle + 180) * (65536.0 / 360.0)) & 65535) - 180;
}

// fast trig routine using inline assembly
void inline SinCos(float rad, float *flSin, float *flCos)
{
#ifdef __GNUC__
   register double __cosr, __sinr;
   __asm __volatile__ ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (rad));
   *flSin = __sinr;
   *flCos = __cosr;
#else
#ifndef _MSC_VER
   *flSin = sinf(rad);
   *flCos = cosf(rad);
#else
   __asm {
      fld DWORD PTR[rad]
      fsincos
      mov edx, DWORD PTR[flCos]
      mov eax, DWORD PTR[flSin]
      fstp DWORD PTR[edx]
      fstp DWORD PTR[eax]
   }
#endif
#endif
}

// faster method to calculate 1.0/sqrt(f)
inline float InvSqrt(float f)
{
   float half = 0.5f * f;

   long lBits = *(long *)&f; // evil floating point bit level hacking
   lBits = 0x5f3759df - (lBits >> 1); // WTF?
   f = *(float *)&lBits;
   f *= 1.5f - half * f * f; // 1st iteration
//   f *= 1.5f - half * f * f; // 2nd iteration, this can be removed

   return f;
}

inline float AngleDiff(float destAngle, float srcAngle)
{
   return AngleNormalize(destAngle - srcAngle);
}

// 2D Vector
class Vector2D
{
public:
   Vector2D() : x(0), y(0) {}
   Vector2D(float a, float b) : x(a), y(b) {}
   Vector2D(float *v) : x(v[0]), y(v[1]) {}

   inline Vector2D operator-(void) const
   {
      return Vector2D(-x, -y);
   }

   inline Vector2D operator+(const Vector2D &v) const
   {
      return Vector2D(x + v.x, y + v.y);
   }

   inline Vector2D operator-(const Vector2D &v) const
   {
      return Vector2D(x - v.x, y - v.y);
   }

   inline Vector2D operator*(float f) const
   {
      return Vector2D(x * f, y * f);
   }

   inline Vector2D operator/(float f) const
   {
      return Vector2D(x / f, y / f);
   }

   inline bool operator==(const Vector2D &v) const
   {
      return FEqual(x, v.x) && FEqual(y, v.y);
   }

   inline bool operator!=(const Vector2D &v) const
   {
      return !FEqual(x, v.x) && !FEqual(y, v.y);
   }

   inline double Length() const
   {
      return sqrt((double)x * (double)x + (double)y * (double)y);
   }

   inline double LengthSquared() const
   {
      return (double)x * (double)x + (double)y * (double)y;
   }

   inline operator float *()
   {
      return &x;
   }

   inline operator const float *() const
   {
      return &x;
   }

   inline Vector2D Normalize(void) const
   {
      if (FZero(x) && FZero(y))
         return Vector2D(0, 0); // reliability check

      double l = 1 / Length();
      return Vector2D(x * l, y * l);
   }

   float         x, y;
};

inline double DotProduct(const Vector2D &a, const Vector2D &b)
{
   return (double)a.x * (double)b.x + (double)a.y + (double)b.y;
}

// 3D Vector
class Vector
{
public:
   Vector() : x(0), y(0), z(0) {}
   Vector(float a, float b, float c) : x(a), y(b), z(c) {}
   Vector(float *v) : x(v[0]), y(v[1]), z(v[2]) {}

   inline Vector operator-(void) const
   {
      return Vector(-x, -y, -z);
   }

   inline Vector operator+(const Vector &v) const
   {
      return Vector(x + v.x, y + v.y, z + v.z);
   }

   inline Vector operator-(const Vector &v) const
   {
      return Vector(x - v.x, y - v.y, z - v.z);
   }

   inline Vector operator*(float f) const
   {
      return Vector(x * f, y * f, z * f);
   }

   inline Vector operator/(float f) const
   {
      return Vector(x / f, y / f, z / f);
   }

   inline bool operator==(const Vector &v) const
   {
      return FEqual(x, v.x) && FEqual(y, v.y) && FEqual(z, v.z);
   }

   inline bool operator!=(const Vector &v) const
   {
      return !FEqual(x, v.x) && !FEqual(y, v.y) && !FEqual(z, v.z);
   }

   inline double Length() const
   {
      return sqrt((double)x * (double)x + (double)y * (double)y +
                  (double)z * (double)z);
   }

   inline double LengthSquared() const
   {
      return (double)x * (double)x + (double)y * (double)y +
             (double)z * (double)z;
   }

   inline double Length2D() const
   {
      return sqrt((double)x * (double)x + (double)y * (double)y);
   }

   inline double LengthSquared2D() const
   {
      return (double)x * (double)x + (double)y * (double)y;
   }

   inline operator float *()
   {
      return &x;
   }

   inline operator const float *() const
   {
      return &x;
   }

   inline Vector Normalize(void) const
   {
      if (FZero(x) && FZero(y) && FZero(z))
         return Vector(0, 0, 1); // reliability check

      double l = 1 / Length();
      return Vector(x * l, y * l, z * l);
   }

   inline Vector2D Make2D(void) const
   {
      return Vector2D(x, y);
   }

   inline void ClampAngles(void)
   {
      x = AngleNormalize(x);
      y = AngleNormalize(y);
      z = 0;
   }

   // The purpose of this function is to convert a spatial location determined by the vector
   // passed in into an absolute Y angle (yaw) from the origin of the world.
   inline float ToYaw(void) const
   {
      if (FZero(x) && FZero(y))
         return 0;
      else
         return atan2(y, x) * (180 / M_PI);
   }

   // The purpose of this function is to convert a spatial location determined by the vector
   // passed in into an absolute X angle (pitch) from the origin of the world.
   inline float ToPitch(void) const
   {
      if (FZero(x) && FZero(y))
         return 0;
      else
         return atan2((double)z, Length2D()) * (180 / M_PI);
   }

   // The purpose of this function is to convert a spatial location determined by the vector
   // passed in into absolute angles from the origin of the world.
   inline Vector ToAngles(void) const
   {
      float yaw, pitch;

      // Is the input vector absolutely vertical?
      if (FZero(x) && FZero(y)) {
         yaw = 0;
         pitch = (z > 0) ? 90 : 270;
      } else {
         // else it's another sort of vector
         // compute individually the pitch and yaw corresponding to this vector
         yaw = atan2(y, x) * (180 / M_PI);
         pitch = atan2(z, (float)Length2D()) * (180 / M_PI);
      }

      return Vector(pitch, yaw, 0);
   }

   // This function builds a 3D referential from a view angle, that is to say, the relative
   // "forward", "right" and "upwards" direction that a player would have if he were facing this
   // view angle. World angles are stored in Vector structs too, the "x" component corresponding
   // to the X angle (horizontal angle), and the "y" component corresponding to the Y angle
   // (vertical angle).
   inline void AngleVectors( Vector *v_forward,
                             Vector *v_right = 0,
                             Vector *v_up = 0 ) const
   {
      float sp = 0, cp = 0, sy = 0, cy = 0, sr = 0, cr = 0;
      float angle = x * (M_PI / 180);
      SinCos(angle, &sp, &cp); // compute the sine and cosine of the pitch component
      angle = y * (M_PI / 180);
      SinCos(angle, &sy, &cy); // compute the sine and cosine of the yaw component
      angle = z * (M_PI / 180);
      SinCos(angle, &sr, &cr); // compute the sine and cosine of the roll component

      // build the FORWARD vector
      if (v_forward) {
         v_forward->x = cp * cy;
         v_forward->y = cp * sy;
         v_forward->z = -sp;
      }

      // build the RIGHT vector
      if (v_right) {
         v_right->x = -sr * sp * cy + cr * sy;
         v_right->y = -sr * sp * sy - cr * cy;
         v_right->z = -sr * cp;
      }

      // build the UPWARDS vector
      if (v_up) {
         v_up->x = cr * sp * cy + sr * sy;
         v_up->y = cr * sp * sy - sr * cy;
         v_up->z = cr * cp;
      }
   }

   // This function returns the angle in degrees between the two vectors, regardless of
   // the axial planes (ie, considering the plane formed by the vectors themselves)
   inline float AngleOf(const Vector &v)
   {
      Vector v1 = Normalize(), v2 = v.Normalize();
      return acos((double)v1.x * (double)v2.x + (double)v1.y * (double)v2.y +
                  (double)v1.z * (double)v2.z);
   }

   float x, y, z;
};

static const Vector NULLVEC(0, 0, 0);

inline double DotProduct(const Vector &a, const Vector &b)
{
   return (double)a.x * (double)b.x + (double)a.y * (double)b.y +
          (double)a.z * (double)b.z;
}

inline Vector CrossProduct(const Vector &a, const Vector &b)
{
   return Vector((double)a.y * (double)b.z - (double)a.z * (double)b.y,
                 (double)a.z * (double)b.x - (double)a.x * (double)b.z,
                 (double)a.x * (double)b.y - (double)a.y * (double)b.x);
}

// Returns if the specified point is inside the bounding box specified
// by origin, mins, maxs.
inline bool PointInsideBoundingBox(const Vector &point,
                                   const Vector &origin,
                                   const Vector &mins,
                                   const Vector &maxs)
{
   float deltaX = origin.x - point.x;
   float deltaY = origin.y - point.y;
   float deltaZ = origin.z - point.z;

   return (deltaX >= mins.x && deltaX <= maxs.x &&
      deltaY >= mins.y && deltaY <= maxs.y &&
      deltaZ >= mins.z && deltaZ <= maxs.z);
}

// Returns if the specified two bounding boxes are touching each other.
inline bool BoundingBoxesTouching(const Vector &origin1,
                                  const Vector &mins1,
                                  const Vector &maxs1,
                                  const Vector &origin2,
                                  const Vector &mins2,
                                  const Vector &maxs2)
{
   return (origin1.x + maxs1.x >= origin2.x + mins2.x &&
           origin1.x + mins1.x <= origin2.x + maxs2.x &&
           origin1.y + maxs1.y >= origin2.y + mins2.y &&
           origin1.y + mins1.y <= origin2.y + maxs2.y &&
           origin1.z + maxs1.z >= origin2.z + mins2.z &&
           origin1.z + mins1.z <= origin2.z + maxs2.z);
}

#endif

