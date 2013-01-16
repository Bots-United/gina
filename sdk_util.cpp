//
// sdk_util.cpp
//
// Utility code from the HLSDK. Functions copied there as needed.
//
/***
*
*   Copyright (c) 1999, Valve LLC. All rights reserved.
*
*   This product contains software technology licensed from Id 
*   Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*   All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#include "main.h"

/*

===== util.cpp ========================================================

  Utility code.  Really not optional after all.

*/

short FixedSigned16( float value, float scale )
{
   int output = (int)(value * scale);
   if ( output > 32767 )
      output = 32767;
   if ( output < -32768 )
      output = -32768;
   return (short)output;
}

unsigned short FixedUnsigned16( float value, float scale )
{
   int output = (int)(value * scale);
   if ( output < 0 )
      output = 0;
   if ( output > 0xFFFF )
      output = 0xFFFF;
   return (unsigned short)output;
}

void ClientPrint(entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4)
{
   static int gmsgTextMsg = 0;

   if (gmsgTextMsg == 0)
      gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );

   MESSAGE_BEGIN( MSG_ONE_UNRELIABLE, gmsgTextMsg, NULL, client );

   WRITE_BYTE( msg_dest );
   WRITE_STRING( msg_name );

   if ( param1 )
      WRITE_STRING( param1 );
   if ( param2 )
      WRITE_STRING( param2 );
   if ( param3 )
      WRITE_STRING( param3 );
   if ( param4 )
      WRITE_STRING( param4 );

   MESSAGE_END();
}

