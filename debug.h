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
// debug.h
//

#ifndef DEBUG_H
#define DEBUG_H

// debug flags
enum {
   DEBUG_GENERAL       = (1 << 0),
   DEBUG_ENGINEAPI     = (1 << 1),
   DEBUG_DLLAPI        = (1 << 2),
   DEBUG_ERROR         = (1 << 3),
   DEBUG_NETMSG        = (1 << 4),
   DEBUG_BOTAI         = (1 << 5),
   DEBUG_BOTNAV        = (1 << 6),
   DEBUG_BOTMOVE       = (1 << 7),
   DEBUG_BOTCOMBAT     = (1 << 8),
   DEBUG_BOTTEAM       = (1 << 9),
};

class CDebug
{
public:
   CDebug() : m_iDebugFlag(0) {}
   virtual ~CDebug() {}

   inline void     DebugOn(int iFlag)   { m_iDebugFlag |= iFlag; }
   inline void     DebugOff(int iFlag)  { m_iDebugFlag &= ~iFlag; }

   void            DebugLog(int iFlag, const char *fmt, ...);

   inline bool     IsDebugOn(int iFlag)  { return (m_iDebugFlag & iFlag) != 0; }

private:
   int             m_iDebugFlag;
};

extern CDebug g_Debug;

#endif

