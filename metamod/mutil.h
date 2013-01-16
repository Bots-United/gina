/*
 * Copyright (c) 2001-2003 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

// Simplified version by Wei Mingzhi

#ifndef MUTIL_H
#define MUTIL_H

#include "plinfo.h"
#include "sdk_util.h"

#define MAX_LOGMSG_LEN  1024

typedef enum {
	GINFO_NAME = 0,
	GINFO_DESC,
	GINFO_GAMEDIR,
	GINFO_DLL_FULLPATH,
	GINFO_DLL_FILENAME,
	GINFO_REALDLL_FULLPATH,
} ginfo_t;

typedef struct meta_util_funcs_s {
	void		(*pfnLogConsole)		(plid_t plid, const char *fmt, ...);
	void		(*pfnLogMessage)		(plid_t plid, const char *fmt, ...);
	void		(*pfnLogError)			(plid_t plid, const char *fmt, ...);
	void		(*pfnLogDeveloper)		(plid_t plid, const char *fmt, ...);
	void		(*pfnCenterSay)			(plid_t plid, const char *fmt, ...);
	void		(*pfnCenterSayParms)	(plid_t plid, hudtextparms_t tparms, 
											const char *fmt, ...);
	void		(*pfnCenterSayVarargs)	(plid_t plid, hudtextparms_t tparms, 
											const char *fmt, va_list ap);
	qboolean	(*pfnCallGameEntity)	(plid_t plid, const char *entStr,
											entvars_t *pev);
	int			(*pfnGetUserMsgID)		(plid_t plid, const char *msgname, int *size);
	const char *(*pfnGetUserMsgName)	(plid_t plid, int msgid, int *size);
	const char *(*pfnGetPluginPath)		(plid_t plid);
	const char *(*pfnGetGameInfo)		(plid_t plid, ginfo_t tag);
} mutil_funcs_t;
extern mutil_funcs_t MetaUtilFunctions;

void mutil_LogConsole(plid_t plid, const char *fmt, ...);
void mutil_LogMessage(plid_t plid, const char *fmt, ...);
void mutil_LogError(plid_t plid, const char *fmt, ...);
void mutil_LogDeveloper(plid_t plid, const char *fmt, ...);

void mutil_CenterSay(plid_t plid, const char *fmt, ...);
void mutil_CenterSayParms(plid_t plid, hudtextparms_t tparms, 
		const char *fmt, ...);
void mutil_CenterSayVarargs(plid_t plid, hudtextparms_t tparms, 
		const char *fmt, va_list ap);

qboolean mutil_CallGameEntity(plid_t plid, const char *entStr, entvars_t *pev);

int mutil_GetUserMsgID(plid_t plid, const char *name, int *size);
const char *mutil_GetUserMsgName(plid_t plid, int msgid, int *size);
const char *mutil_GetPluginPath(plid_t plid);
const char *mutil_GetGameInfo(plid_t plid, ginfo_t tag);

#define LOG_CONSOLE			(*gpMetaUtilFuncs->pfnLogConsole)
#define LOG_MESSAGE			(*gpMetaUtilFuncs->pfnLogMessage)
#define LOG_ERROR			(*gpMetaUtilFuncs->pfnLogError)
#define LOG_DEVELOPER		(*gpMetaUtilFuncs->pfnLogDeveloper)
#define CENTER_SAY			(*gpMetaUtilFuncs->pfnCenterSay)
#define CENTER_SAY_PARMS	(*gpMetaUtilFuncs->pfnCenterSayParms)
#define CENTER_SAY_VARARGS	(*gpMetaUtilFuncs->pfnCenterSayVarargs)
#define CALL_GAME_ENTITY	(*gpMetaUtilFuncs->pfnCallGameEntity)
#define GET_USER_MSG_ID		(*gpMetaUtilFuncs->pfnGetUserMsgID)
#define GET_USER_MSG_NAME	(*gpMetaUtilFuncs->pfnGetUserMsgName)
#define GET_PLUGIN_PATH		(*gpMetaUtilFuncs->pfnGetPluginPath)
#define GET_GAME_INFO		(*gpMetaUtilFuncs->pfnGetGameInfo)

#endif
