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
// bot_manager.cpp
//

#include "main.h"

CBotManager::CBotManager()
{
}

CBotManager::~CBotManager()
{
}

void CBotManager::Think()
{
}

// add a bot
void CBotManager::BotAdd(const char *arg)
{
   if (g_pServer->IsWorldRequired() && !g_pServer->World()->IsAvailable()) {
      printf("Navigation map is unavailable - can't add bot\n");
      return;
   }

   bot_profile_t *profile = NULL;

   if (arg && *arg) {
      // check if the parameter is skill...
      if (strncmp(arg, "skill=", 6) == 0) {
         profile = profiles.PickBotProfile(atoi(&arg[6]));
      } else {
         profile = profiles.PickBotProfile(arg);
      }
   } else {
      // pick a profile randomly...
      profile = profiles.PickBotProfile();
   }

   if (!profile) {
      printf("BOT: bot profile is not found or no profiles left\n");
      return;
   }

   g_pServer->BotCreate(profile); // add this bot to the server
}

// kill all bots
void CBotManager::KillAllBots()
{
   for (int i = 0; i < g_pServer->GetMaxClients(); i++) {
      CClient *pClient = g_pServer->m_rgpClients[i];
      // if this is a bot...
      if (pClient && pClient->IsValid() && pClient->IsBot()) {
         pClient->Kill(); // kill him/her!
      }
   }
}

// kick all bots
void CBotManager::KickAllBots()
{
   for (int i = 0; i < g_pServer->GetMaxClients(); i++) {
      CClient *pClient = g_pServer->m_rgpClients[i];
      // if this is a bot...
      if (pClient && pClient->IsValid() && pClient->IsBot()) {
         pClient->Kick(); // kick him/her!
      }
   }
}

// get the number of bots
int CBotManager::NumBots()
{
   int count = 0;

   // enumerate all the clients in the server...
   for (int i = 0; i < g_pServer->GetMaxClients(); i++) {
      CClient *pClient = g_pServer->m_rgpClients[i];
      if (pClient && pClient->IsValid() && pClient->IsBot()) {
         count++; // this is a bot
      }
   }

   return count; // return the number of bots
}

//==============================================================
// Bot profile manager
//==============================================================

static const int PROFILE_SIZE_INCREMENT = 10;

CProfileManager::CProfileManager():
profiles(NULL), num_profiles(0), current_size(0)
{
   LoadBotProfiles();
}

CProfileManager::~CProfileManager()
{
   FreeBotProfiles();
}

// load bot profiles from the disk
void CProfileManager::LoadBotProfiles()
{
   char str[256];

   FILE *fp = fopen(g_General.BuildFileName("profiles.ini"), "r");

   if (!fp) {
      printf("BOT: cannot load profiles.ini !\n");
      return;
   }

   while (fgets(str, 256, fp)) {
      trim(str); // trim all the blanks or linefeeds

      // skip all comment lines or empty lines
      if (!str[0] || str[0] == ';' || str[0] == '/' || str[0] == '#')
         continue;

      char *p;

      if (strcmpi(str, "[PROFILE]") == 0) {
         num_profiles++; // a new profile item has started...

         // if we don't have enough room, try to allocate memory...
         if (num_profiles > current_size) {
            current_size += PROFILE_SIZE_INCREMENT;
            if (profiles) {
               profiles = (bot_profile_t *)realloc(profiles, sizeof(bot_profile_t) * current_size);
            } else {
               profiles = (bot_profile_t *)malloc(sizeof(bot_profile_t) * current_size);
            }

            if (!profiles) {
               g_General.TerminateOnError("Memory allocation error !");
            }
         }

         // set all items of this profile to default value...
         profiles[num_profiles - 1].is_used = false;
         profiles[num_profiles - 1].difficulty = 2;
         strcpy(profiles[num_profiles - 1].name, "bot");
         strcpy(profiles[num_profiles - 1].skin, "gordon");
         profiles[num_profiles - 1].team = -1; // random team
         profiles[num_profiles - 1].bot_class = -1; // random class
         profiles[num_profiles - 1].reaction_time = 0.4;
         profiles[num_profiles - 1].skill = 75;
         profiles[num_profiles - 1].teamplay_percent = 75;
         profiles[num_profiles - 1].attack_delay = 0.35;
      } else if (p = strchr(str, '=')) {
         int slot = num_profiles - 1;
         if (slot >= 0) {
            *(p++) = '\0';
            trim(str);
            trim(p);

            bot_profile_t *pProfile = &profiles[slot];

            if (strcmpi(str, "name") == 0) {
               strncpy(pProfile->name, p, 32); // avoid buffer overrun

               // check if this name already exists...
               char normalized_name[33];
               strncpy(normalized_name, p, 32);
               NormalizeFilename(normalized_name);

               for (int i = 0; i < slot; i++) {
                  char n[33];
                  strcpy(n, profiles[i].name);
                  NormalizeFilename(n);

                  if (strcmpi(n, normalized_name) == 0) {
                     g_General.TerminateOnError("Duplicate name in profiles.ini - \"%s\"", profiles[slot].name);
                  }
               }
            } else if (strcmpi(str, "skin") == 0) {
               strncpy(pProfile->skin, p, 32);
            } else if (strcmpi(str, "team") == 0) {
               pProfile->team = atoi(p);
            } else if (strcmpi(str, "class") == 0) {
               pProfile->bot_class = atoi(p);
            } else if (strcmpi(str, "difficulty") == 0) {
               int difficulty = atoi(p);
               if (difficulty >= 1 && difficulty <= 5)
                  pProfile->difficulty = difficulty;
            } else if (strcmpi(str, "skill") == 0) {
               int skill = atoi(p);
               if (skill >= 1 && skill <= 100)
                  pProfile->skill = skill;
            } else if (strcmpi(str, "reactiontime") == 0) {
               pProfile->reaction_time = atof(p);
            } else if (strcmpi(str, "attackdelay") == 0) {
               pProfile->attack_delay = atof(p);
            } else if (strcmpi(str, "teamplay") == 0) {
               int teamplay = atoi(p);
               if (teamplay >= 0 && teamplay <= 100)
                  pProfile->teamplay_percent = teamplay;
            }
         }
      }
   }

   fclose(fp); // load finished; close the file
}

// free all the memory allocated for bot profiles
void CProfileManager::FreeBotProfiles()
{
   if (profiles)
      free(profiles);
   profiles = NULL;
}

// pick a bot profile which is same as the specified difficulty
bot_profile_t *CProfileManager::PickBotProfile(int iDifficulty)
{
   if (num_profiles <= 0)
      return NULL;

   int num = RandomLong(0, num_profiles - 1);

   for (int attempt = 0; attempt <= num_profiles; attempt++) {
      bot_profile_t *p = &profiles[num];

      // check if this profile is NOT used AND
      // difficulty is NOT specified OR
      // specified difficulty is the same as this profile
      if (!p->is_used &&
         (iDifficulty <= 0 || p->difficulty == iDifficulty))
         return p; // this is a good one

      num++;
      if (num >= num_profiles)
         num = 0;
   }

   return NULL; // no profile is good to use, can't add bots
}

// pick a bot profile which is same as the specified name
bot_profile_t *CProfileManager::PickBotProfile(const char *name)
{
   if (num_profiles <= 0)
      return NULL;

   int num = RandomLong(0, num_profiles - 1);

   for (int attempt = 0; attempt <= num_profiles; attempt++) {
      bot_profile_t *p = &profiles[num];

      // check if this profile is NOT used AND
      // specified name is the same as this profile
      if (!p->is_used && strcmp(name, p->name) == 0)
         return p; // this is a good one

      num++;
      if (num >= num_profiles)
         num = 0;
   }

   return NULL; // no profile is good to use, can't add bots
}
