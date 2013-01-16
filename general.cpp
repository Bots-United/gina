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
// general.cpp
//
// Portions based on:
//
// RACC - AI development project for first-person shooter games
// derivated from Valve's Half-Life
// (http://racc.bots-united.com/)
//

#include "main.h"

CGeneral g_General;

CGeneral::CGeneral():
m_iModid(MOD_UNKNOWN),
m_iUserMsgsCount(0),
m_fIsFakeClientCommand(false),
m_iCmdArgc(0),
m_pszCmdArgs(NULL)
{
}

CGeneral::~CGeneral()
{
   if (g_pServer) {
      delete g_pServer;
   }
}

void CGeneral::GameInit()
{
   g_Globals.Load(); // load the global parameters

   g_engfuncs.pfnAddServerCommand("gina", BotServerCommand);

   // print the copyright message
   printf("\n");
   printf("   === The GINA bot [%s - BUILD %d] ===\n", __DATE__, GetBuildNumber());
   printf("   Copyright (c) 2011, Wei Mingzhi. All rights reserved.\n");
   printf("\n");
}

void CGeneral::PrecacheStuff()
{
   m_sModelIndexLaser = PRECACHE_MODEL("sprites/laserbeam.spr");
}

void CGeneral::DrawBeam(CClient *pClient, Vector start, Vector end, int width,
        int noise, int red, int green, int blue, int brightness, int speed, int life)
{
   int msg_dest = MSG_ONE_UNRELIABLE;
   edict_t *pEdict = NULL;

   if (!pClient || !pClient->IsValid()) {
      // if we haven't specified which client to send, send to all clients
      msg_dest = MSG_BROADCAST;
   } else {
      pEdict = pClient->edict();
   }

   MESSAGE_BEGIN(msg_dest, SVC_TEMPENTITY, NULL, pEdict);
      WRITE_BYTE(TE_BEAMPOINTS);
      WRITE_COORD(start.x);
      WRITE_COORD(start.y);
      WRITE_COORD(start.z);
      WRITE_COORD(end.x);
      WRITE_COORD(end.y);
      WRITE_COORD(end.z);
      WRITE_SHORT(m_sModelIndexLaser);
      WRITE_BYTE(0);       // framestart
      WRITE_BYTE(10);      // framerate
      WRITE_BYTE(life);    // life in 0.1's
      WRITE_BYTE(width);   // width
      WRITE_BYTE(noise);   // noise
      WRITE_BYTE(red);     // r, g, b
      WRITE_BYTE(green);   // r, g, b
      WRITE_BYTE(blue);    // r, g, b
      WRITE_BYTE(brightness);   // brightness
      WRITE_BYTE(speed);        // speed
   MESSAGE_END();
}

// load a file into the memory
unsigned char *CGeneral::LoadFile(char *filename, int *pLength)
{
   // use HL engine function to make sure it also reads the
   // files inside the PAK or Steam GCF cache
   return LOAD_FILE_FOR_ME(filename, pLength);
}

// free the memory allocated for a file
void CGeneral::UnloadFile(void *buffer)
{
   FREE_FILE(buffer);
}

// check if the file exists either on disk or on PAK/Steam GCF
bool CGeneral::DoesFileExist(char *filename)
{
   int i;
   unsigned char *tempbuf = LoadFile(filename, &i);

   if (tempbuf) {
      UnloadFile(tempbuf);
      return true;
   }

   return false;
}

void CGeneral::AddUserMsg(const char *msg_name, int msg_type)
{
   // is this message NOT already registered?
   int index;
   for (index = 0; index < m_iUserMsgsCount; index++)
      if (strcmp(m_rgUsermsgs[index].name, msg_name) == 0)
         break; // cycle through usermsgs types array and break when a similar record is found

   // now keep track (or update if necessary) this user message in our own array...
   m_rgUsermsgs[index].name = msg_name; // record this message's name
   m_rgUsermsgs[index].id = msg_type; // record this message's index

   // are we certain this message has not been registered in the past?
   if (index == m_iUserMsgsCount)
      m_iUserMsgsCount++; // we know now one user message more
}

// This function returns the user message id of the recorded message named msg_name.
int CGeneral::GetUserMsgId(const char *msg_name)
{
   // cycle through our known user message types array
   for (int i = 0; i < m_iUserMsgsCount; i++)
      if (strcmp(m_rgUsermsgs[i].name, msg_name) == 0)
         return m_rgUsermsgs[i].id; // return the id of the user message named msg_name

   // unregistered user message, have the engine register it
   return REG_USER_MSG(msg_name, -1); // ask the engine to register this new message
}

// This function returns the user message name of the recorded message index msg_type
const char *CGeneral::GetUserMsgName(int msg_type)
{
   // cycle through our known user message types array
   for (int i = 0; i < m_iUserMsgsCount; i++)
      if (m_rgUsermsgs[i].id == msg_type)
         return m_rgUsermsgs[i].name; // return the name of the user message having the msg_type id

   return NULL; // unregistered user message
}

/**
 * The purpose of this function is to provide fakeclients (bots) with the same client
 * command-scripting advantages (putting multiple commands in one line between semicolons)
 * as real players. It is an improved version of botman's FakeClientCommand, in which you
 * supply directly the whole string as if you were typing it in the bot's "console". It
 * is supposed to work exactly like the pfnClientCommand (server-sided client command). [PMB]
 */
void CGeneral::FakeClientCommand(CClient *pClient, const char *fmt, ...)
{
   edict_t *pFakeClient = pClient->edict();

   if (FNullEnt(pFakeClient))
      return; // reliability check

   va_list argptr;
   char command[256];
   int length, fieldstart, fieldstop, i, index, stringindex = 0;

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   _vsnprintf(command, sizeof(command), fmt, argptr);
   va_end(argptr);

   if (*command == 0)
      return; // if nothing in the command buffer, return

   g_Debug.DebugLog(DEBUG_GENERAL, "CGeneral::FakeClientCommand(): %s(#%d) - %s", pClient->GetNetName(), pClient->entindex(), command);

   m_fIsFakeClientCommand = true; // set the "fakeclient command" flag
   length = strlen(command); // get the total length of the command string

   // process all individual commands (separated by a semicolon) one each a time
   while (stringindex < length) {
      fieldstart = stringindex; // save field start position (first character)
      while (stringindex < length && command[stringindex] != ';')
         stringindex++; // reach end of field
      if (command[stringindex - 1] == '\n')
         fieldstop = stringindex - 2; // discard any trailing '\n' if needed
      else
         fieldstop = stringindex - 1; // save field stop position (last character before semicolon or end)
      for (i = fieldstart; i <= fieldstop; i++)
         m_szCmdArgv[i - fieldstart] = command[i]; // store the field value in the g_argv global string
      m_szCmdArgv[i - fieldstart] = 0; // terminate the string
      stringindex++; // move the overall string index one step further to bypass the semicolon

      index = 0;
      m_iCmdArgc = 0; // let's now parse that command and count the different arguments
      m_pszCmdArgs = NULL;

      // count the number of arguments
      while (index < i - fieldstart) {
         while (index < i - fieldstart && m_szCmdArgv[index] == ' ')
            index++; // ignore spaces

         if (m_iCmdArgc == 1) {
            m_pszCmdArgs = &m_szCmdArgv[index];
         }

         // is this field a group of words between quotes or a single word ?
         if (m_szCmdArgv[index] == '"') {
            index++; // move one step further to bypass the quote
            while (index < i - fieldstart && m_szCmdArgv[index] != '"')
               index++; // reach end of field
            index++; // move one step further to bypass the quote
         } else {
            while (index < i - fieldstart && m_szCmdArgv[index] != ' ')
               index++; // this is a single word, so reach the end of field
         }

         m_iCmdArgc++; // we have processed one argument more
      }

      BotClientCommand(CClient::Instance(pFakeClient)); // let the bot handle this command
      MDLL_ClientCommand(pFakeClient); // tell now the MOD DLL to execute this ClientCommand...
   }

   m_szCmdArgv[0] = 0; // when it's done, reset the g_argv field
   m_fIsFakeClientCommand = FALSE; // reset the "fakeclient command" flag
   m_iCmdArgc = 0; // and the argument count
}

const char *CGeneral::GetArgs()
{
   if (m_fIsFakeClientCommand) {
      return m_pszCmdArgs;
   } else {
      return g_engfuncs.pfnCmd_Args();
   }
}

/**
 * The purpose of this function is to provide fakeclients (bots) with the same Cmd_Argv
 * convenience the engine provides to real clients. This way the handling of real client
 * commands and bot client commands is exactly the same, just have a look in engine.cpp
 * for the hooking of pfnCmd_Argc, pfnCmd_Args and pfnCmd_Argv, which redirects the call
 * either to the actual engine functions (when the caller is a real client), either on
 * our function here, which does the same thing, when the caller is a bot. [PMB]
 */
const char *CGeneral::GetArg(int field_number)
{
   if (m_fIsFakeClientCommand) {
      static char field[256];

      int index = 0;
      int field_count = 0;
      field[0] = 0; // reset field
      int length = strlen(m_szCmdArgv); // get length of string

      // while we have not reached end of line
      while (index < length && field_count <= field_number) {
         while (index < length && (m_szCmdArgv[index] == ' ' || m_szCmdArgv[index] == '\t'))
            index++; // ignore spaces or tabs

         int fieldstart, fieldstop;

         // is this field multi-word between quotes or single word ?
         if (m_szCmdArgv[index] == '"') {
            index++; // move one step further to bypass the quote
            fieldstart = index; // save field start position
            while (index < length && m_szCmdArgv[index] != '"')
               index++; // reach end of field
            fieldstop = index - 1; // save field stop position
            index++; // move one step further to bypass the quote
         } else {
            fieldstart = index; // save field start position
            while (index < length && m_szCmdArgv[index] != ' ' && m_szCmdArgv[index] != '\t')
               index++; // reach end of field
            fieldstop = index - 1; // save field stop position
         }

         // is this field we just processed the wanted one ?
         if (field_count == field_number) {
            int i;
            for (i = fieldstart; i <= fieldstop; i++)
               field[i - fieldstart] = m_szCmdArgv[i]; // store the field value in a string
            field[i - fieldstart] = 0; // terminate the string
            break; // and stop parsing
         }

         field_count++; // we have parsed one field more
      }
      return field;
   } else {
      return g_engfuncs.pfnCmd_Argv(field_number);
   }
}

void CGeneral::GetModName(char *mod_name)
{
   GET_GAME_DIR(mod_name); // ask the engine for the MOD directory path
   int length = strlen(mod_name); // get the length of the returned string

   // format the returned string to get the last directory name
   int fieldstop = length - 1;
   while ((mod_name[fieldstop] == '\\' || mod_name[fieldstop] == '/') && fieldstop > 0)
      fieldstop--; // shift back any trailing separator

   int fieldstart = fieldstop;
   while (mod_name[fieldstart] != '\\' && mod_name[fieldstart] != '/' && fieldstart > 0)
      fieldstart--; // shift back to the start of the last subdirectory name

   if (mod_name[fieldstart] == '\\' || mod_name[fieldstart] == '/')
      fieldstart++; // if we reached a separator, step over it

   // now copy the formatted string back onto itself character per character
   for (length = fieldstart; length <= fieldstop; length++)
      mod_name[length - fieldstart] = mod_name[length];
   mod_name[length - fieldstart] = 0; // terminate the string
}

char *CGeneral::BuildFileName(const char *filename)
{
   static char sz[256];

   GetModName(sz); // get the MOD name
   strncat(sz, "/gina/", 256);
   strncat(sz, filename, 256);

   return sz;
}

/**
 * This function terminates the game because of an error and prints the message string pointed
 * to by fmt both in the server console and in a messagebox.
 */
void CGeneral::TerminateOnError(const char *fmt, ...)
{
   va_list argptr;
   char string[256];

   // concatenate all the arguments in one string
   va_start(argptr, fmt);
   _vsnprintf(string, sizeof(string), fmt, argptr);
   va_end(argptr);

   g_Debug.DebugLog(DEBUG_ERROR, string);

   ALERT(at_error, "%s\n", string);
   assert(false); // allows jumping to debugger

#ifdef WINDOWSNT
   MessageBox(0, string, "ERROR", MB_ICONERROR); // popup a messagebox in Windows
#endif

   exit(1);
}

// compute the version number based on the compile date
unsigned short CGeneral::GetBuildNumber()
{
   const char *date = __DATE__;

   char *mon[12] =
   { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
   unsigned char mond[12] = 
   { 31,    28,    31,    30,    31,    30,    31,    31,    30,    31,    30,    31 };

   int m = 0, d = 0, y = 0;

   for (m = 0; m < 11; m++) {
      if (strncmp(&date[0], mon[m], 3) == 0)
         break;
      d += mond[m];
   }

   d += atoi( &date[4] ) - 1;
   y = atoi( &date[7] ) - 2000;

   unsigned short usBuildNumber = d + (int)((y - 1) * 365.25);

   // Is it a leap year?
   if ((y % 4) == 0 && m > 1) {
      usBuildNumber += 1;
   }

   return usBuildNumber - 1267;
}

