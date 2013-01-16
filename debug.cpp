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
// debug.cpp
//

#include "main.h"
#include <time.h> // time(), localtime()

CDebug g_Debug;

const static char *debug_filename = "log.txt";

void CDebug::DebugLog(int iFlag, const char *fmt, ...)
{
   if (!(m_iDebugFlag & iFlag))
      return;

   // log a message in the file
   char logbuf[256];
   va_list va_alist;

   va_start(va_alist, fmt);
   _vsnprintf(logbuf, sizeof(logbuf), fmt, va_alist);
   va_end(va_alist);

   time_t ticktime = time(NULL);
   struct tm *timestruct = localtime(&ticktime);

   char desc[200] = "";
   if (iFlag & DEBUG_GENERAL)
      strcat(desc, "GENERAL ");
   if (iFlag & DEBUG_ENGINEAPI)
      strcat(desc, "ENGINE ");
   if (iFlag & DEBUG_DLLAPI)
      strcat(desc, "DLL ");
   if (iFlag & DEBUG_ERROR)
      strcat(desc, "ERROR ");
   if (iFlag & DEBUG_NETMSG)
      strcat(desc, "NETMSG ");
   if (iFlag & DEBUG_BOTAI)
      strcat(desc, "BOTAI ");
   if (iFlag & DEBUG_BOTNAV)
      strcat(desc, "BOTNAV ");
   if (iFlag & DEBUG_BOTMOVE)
      strcat(desc, "BOTMOVE ");
   if (iFlag & DEBUG_BOTCOMBAT)
      strcat(desc, "BOTCOMBAT ");
   if (iFlag & DEBUG_BOTTEAM)
      strcat(desc, "BOTTEAM ");

   FILE *fp = fopen(g_General.BuildFileName(debug_filename), "a");

   if (fp) {
      fprintf(fp, "[%s%02d:%02d:%02d] %s\n", desc, timestruct->tm_hour,
         timestruct->tm_min, timestruct->tm_sec, logbuf);
      fclose(fp);
   }
}

