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
// general.h
//

#ifndef _GENERAL_H
#define _GENERAL_H

// maximum number of user messages the engine can register (hardcoded in the engine)
#define MAX_USERMSG_TYPES 256

class CClient;

class CGeneral
{
public:
   CGeneral();
   virtual ~CGeneral();

   void         AddUserMsg(const char *msg_name, int msg_type);
   int          GetUserMsgId(const char *msg_name);
   const char  *GetUserMsgName(int msg_type);

   void         GameInit(void);
   void         PrecacheStuff(void);

   void         GetModName(char *mod_name);

   char        *BuildFileName(const char *filename);

   void         FakeClientCommand(CClient *pClient, const char *fmt, ...);
   inline bool  IsFakeClientCommand(void) { return m_fIsFakeClientCommand; }
   inline int   FakeArgCount(void)        { return m_iCmdArgc; }
   const char  *GetArgs(void);
   const char  *GetArg(int field_number);

   void         DrawBeam(CClient *pClient, Vector start, Vector end, int width,
                         int noise, int red, int green, int blue, int brightness,
                         int speed, int life);

   inline char *GetWeaponName(int iId)    { return m_rgWeaponDefs[iId].szClassname; }

   static unsigned char     *LoadFile(char *filename, int *pLength);
   static void               UnloadFile(void *buffer);
   static bool               DoesFileExist(char *filename);

   static unsigned short     GetBuildNumber(void);

   static void  TerminateOnError(const char *fmt, ...);

   int          m_iModid;
   weaponinfo_t m_rgWeaponDefs[MAX_WEAPONS];

private:
   usermsg_t    m_rgUsermsgs[MAX_USERMSG_TYPES];
   int          m_iUserMsgsCount;

   int          m_iCmdArgc;
   char         m_szCmdArgv[256];
   char        *m_pszCmdArgs;
   bool         m_fIsFakeClientCommand;

   short        m_sModelIndexLaser; // holds the index for the laser beam
};

extern CGeneral g_General;

#endif

