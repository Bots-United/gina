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

#ifndef INI_H
#define INI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#if defined(_MSC_VER)
// Remove warnings from Level 4
#pragma warning (disable : 4244)
#pragma warning (disable : 4514)
#pragma warning (disable : 4706)
#pragma warning (disable : 4996)
#endif

#ifdef _WIN32
#ifndef strcasecmp
#define strcasecmp strcmpi
#endif
#endif

static const int INI_HASH_KEY_SIZE = 1024; // size of the key hash table
static const int INI_HASH_VALUE_SIZE = 1024; // size of the value hash table of each key

static const int INI_SIZE_INCREMENT = 10; // increment of memory allocation

typedef struct {
   char           *value_name;
   char           *value;
} ini_value_t;

typedef struct {
   char               *key_name;
   int                 value_count;
   int                 current_size;
   ini_value_t        *values;
   ini_value_t        *hash[INI_HASH_VALUE_SIZE];
} ini_key_t;

class CIniFile
{
public:
   CIniFile();
   CIniFile(const char *filename);
   virtual ~CIniFile();

   void Load(const char *filename);
   void Save(const char *filename);

   const char *Get(const char *key, const char *value, const char *def = "");
   void Set(const char *key, const char *value, const char *set);

   inline bool Valid(void) { return (ini != NULL); }

private:
   ini_key_t      *ini;
   int             key_count;
   int             current_size;

   void            FreeAllTheStuff(void);

   // utility functions
   void            trim(char *str);
   unsigned short  GetHashValue(const char *sz);

   ini_key_t     **m_Hash;
};

#endif

