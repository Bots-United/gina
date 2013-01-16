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
// bot_manager.h
//

#ifndef BOT_MANAGER_H
#define BOT_MANAGER_H

#include "bot_chat.h"

// bot profile struct
typedef struct bot_profile_s
{
   bool       is_used; // whether this profile is used or not
   char       name[33];  // name of this bot
   char       skin[33];  // skin of this bot
   int        team;      // preferred team for this bot
   int        bot_class; // preferred class for this bot
   int        difficulty; // difficulty (1 = easy - 5 = hard)
   int        skill;      // skill of this bot (1 = unskilled - 100 = skilled)
   float      reaction_time; // reaction time
   float      attack_delay;  // delay after fire
   int        teamplay_percent; // chance to do teamwork
} bot_profile_t;

// bot profile manager
class CProfileManager
{
public:
   CProfileManager();
   virtual ~CProfileManager();

   bot_profile_t     *PickBotProfile(int iDifficulty = -1);
   bot_profile_t     *PickBotProfile(const char *botname);

private:
   bot_profile_t     *profiles;
   int                num_profiles;
   int                current_size;

   void               LoadBotProfiles(void);
   void               FreeBotProfiles(void);
};

// bot manager class
class CBotManager
{
public:
   CBotManager();
   ~CBotManager();

   CChatManager        chat;
   CProfileManager     profiles;

   void          Think(void);

   void          BotAdd(const char *arg);
   void          KickAllBots(void);
   void          KillAllBots(void);

   int           NumBots(void);
};

#endif


