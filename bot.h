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

/*******************************************************
 * bot.h                                               *
 *******************************************************
 * Purpose: Base bot handling.                         *
 * Engine-specific: no                                 *
 *******************************************************/

#ifndef _BOT_H
#define _BOT_H

// action flags (32 bits available)
enum tagActionFlag {
   ACTION_ATTACK         = (1 << 0),
   ACTION_ATTACK2        = (1 << 1),
   ACTION_USE            = (1 << 2),
   ACTION_WALK           = (1 << 3),
   ACTION_JUMP           = (1 << 4),
   ACTION_MOVEUP         = (1 << 5),
   ACTION_CROUCH         = (1 << 6),
   ACTION_MOVEDOWN       = (1 << 7),
   ACTION_MOVEFORWARD    = (1 << 8),
   ACTION_MOVEBACK       = (1 << 9),
   ACTION_MOVELEFT       = (1 << 10),
   ACTION_MOVERIGHT      = (1 << 11),
   ACTION_RELOAD         = (1 << 12),
   ACTION_RESPAWN        = (1 << 13),
};

// travel flags
enum tagTravelFlag {
   TFL_NONE          = 0,
   TFL_INVALID       = (1 << 0), // traveling temporary not possible
   TFL_WALK          = (1 << 1), // walking
   TFL_CROUCH        = (1 << 2), // crouching
   TFL_BARRIERJUMP   = (1 << 3), // jumping onto a barrier
   TFL_JUMP          = (1 << 4), // jumping
   TFL_LONGJUMP      = (1 << 5), // longjumping
   TFL_LADDER        = (1 << 6), // climbing a ladder
   TFL_WALKOFFLEDGE  = (1 << 7), // walking of a ledge
   TFL_SWIM          = (1 << 8), // swimming
   TFL_WATERJUMP     = (1 << 9), // jumping out of the water
   TFL_TELEPORT      = (1 << 10), // teleporting
   TFL_ELEVATOR      = (1 << 11), // elevator
   TFL_GRAPPLEHOOK   = (1 << 12), // grappling hook
   TFL_DOUBLEJUMP    = (1 << 13), // double jump
   TFL_RAMPJUMP      = (1 << 14), // ramp jump
   TFL_STRAFEJUMP    = (1 << 15), // strafe jump
   TFL_JUMPPAD       = (1 << 16), // jump pad
   TFL_AIR           = (1 << 17), // travel through air
   TFL_WATER         = (1 << 18), // travel through water
   TFL_DONOTENTER    = (1 << 19), // travel through donotenter area
   TFL_FUNCBOB       = (1 << 20), // func bobbing
   TFL_FLIGHT        = (1 << 21), // flight
};

// visibility flags
enum tagVisFlag {
   VFL_TOP           = (1 << 0), // top
   VFL_HEAD          = (1 << 1), // head
   VFL_WAIST         = (1 << 2), // waist
   VFL_BOTTOM        = (1 << 3), // bottom
};

// goal flags
enum tagGoalFlag {
   GFL_NONE          = 0,
   GFL_ITEM          = (1 << 0), // item goal
   GFL_ROAM          = (1 << 1), // roaming goal
   GFL_DROPPED       = (1 << 2), // dropped goal
   GFL_AIR           = (1 << 3), // air goal (when swimming)
};

// long term goal types
typedef enum tagLTGType {
   LTG_TEAMHELP,                  // help a team mate
   LTG_TEAMACCOMPANY,             // accompany a team mate
   LTG_DEFENDKEYAREA,             // defend a key area
   LTG_MISSIONGOAL,               // mission goal (enemy flag, hostage, bomb site, etc)
   LTG_RUSHBASE,                  // rush to the base
   LTG_RETURN,                    // flag return point, hostage rescue point, bomb safety area, etc
   LTG_CAMP,                      // camp somewhere
   LTG_CAMPORDER,                 // ordered to camp somewhere
   LTG_PATROL,                    // patrol
   LTG_GETITEM,                   // get an item
   LTG_KILL,                      // kill someone
} ltg_type;

// some goal dedication times
#define TEAM_HELP_TIME              60  // 1 minute teamplay help time
#define TEAM_ACCOMPANY_TIME         600 // 10 minutes teamplay accompany time
#define TEAM_DEFENDKEYAREA_TIME     300 // 5 minutes ctf defend base time
#define TEAM_CAMP_TIME              600 // 10 minutes camping time
#define TEAM_PATROL_TIME            600 // 10 minutes patrolling time
#define TEAM_LEAD_TIME              600 // 10 minutes taking the lead
#define TEAM_GETITEM_TIME           60  // 1 minute
#define	TEAM_KILL_SOMEONE           180 // 3 minute to kill someone
#define CTF_GETFLAG_TIME            300 // 5 minutes ctf get flag time
#define CTF_RUSHBASE_TIME           120 // 2 minutes ctf rush base time
#define CTF_RETURNFLAG_TIME         180 // 3 minutes to return the flag
#define CTF_ROAM_TIME               60  // 1 minute ctf roam time

// patrol flags
#define PATROL_LOOP                 (1 << 0)
#define PATROL_REVERSE              (1 << 1)
#define PATROL_BACK                 (1 << 2)

// task preference
#define TP_ATTACKER              1
#define TP_DEFENDER              2

// Enemy Body Parts Seen
#define HEAD_VISIBLE          (1 << 0)
#define WAIST_VISIBLE         (1 << 1)
#define CUSTOM_VISIBLE        (1 << 2)

//============================================================

#ifndef WPEDIT_PLUGIN

#include "client.h"
#include "server.h"
#include "general.h"

#include "bot_chat.h"
#include "bot_manager.h"

// the bot input
typedef struct bot_input_s {
   Vector   dir;         // movement direction
   Vector   idealangles; // the ideal view angles
   int      actionflags; // one of the ACTION_? flags
   int      weapon;      // weapon to use
} bot_input_t;

// AI Node class
class CAINode
{
public:
   virtual void              OnEnter(void) {}
   virtual void              OnExit(void)  {}
   virtual bool              Run(void) = 0;
   virtual const char       *GetDescription(void) = 0;

   CBaseBot                 *m_pBot; // pointer to the bot
};

class CBotNav;
class CBotChat;

// main bot class
class CBaseBot : public CClient
{
   friend class CBotNav;
   friend class CBotChat;

public:
   CBaseBot();
   virtual ~CBaseBot();

   void         BotThink(void);
   CBaseBot    *GetBotPointer(void) { return this; }

   int                      m_iStartAction;
   struct bot_profile_s    *m_pProfile;

   inline CBotNav   *Nav(void)    { return nav; }
   inline CBotChat  *Chat(void)   { return chat; }

   inline int     GetDifficulty(void) { return m_pProfile->difficulty; }
   inline int     GetSkill(void)      { return m_pProfile->skill; }
   inline float   GetReactionTime(void)  { return m_pProfile->reaction_time; }
   inline float   GetAttackDelay(void)   { return m_pProfile->attack_delay; }
   inline int     GetTeamplayPercent(void) { return m_pProfile->teamplay_percent; }

   void           TakeDamage(CEntity *entInflictor, const Vector &vecOrigin, float flDamage, float flArmor, int bitsDamageType);

   virtual void   ResetState(void);

private:
   void         BotAI(void);

   void         PerformMovement(void);

   float        m_flTimeThink;
   float        m_flTimePrevThink;

   int          m_iPrevHealth; // health point of last thinking cycle

   inline float ThinkInterval(void) { return g_pServer->GetTime() - m_flTimePrevThink; }

protected:
   bool         m_fNeedToInitialize;
   bool         m_fIsStarted;

   CEntity        *m_pEnemy;
   Vector          m_vecEnemy;
   unsigned char   m_ucVisibility;
   CClient        *EnemyClient(void) { return CClient::Instance(m_pEnemy); }

   virtual bool             FindEnemy(void);

   virtual bool             IsShootableThruObstacle(const Vector &vecDest) { return false; }

   bot_input_t              bi; // Bot Input

   CBotNav                 *nav;
   CBotChat                *chat;

   CAINode                 *m_pAINode;

   void                     AIEnter(CAINode *pNode);
   void                     AINet_Run(void);

   void         FacePosition(const Vector &vecPos);

   virtual void StartGame(void)        { m_fIsStarted = true; }
   virtual void SpawnInit(void);
   virtual void DeadThink(void);

   virtual class CBotGoal      FindGoal(void) = 0;

   virtual void SelectWeapon(int iId);

   virtual int  BotAggression(void) { return 0; }

   void         DebugMsg(int flag, const char *fmt, ...);

   void         Say(const char *fmt, ...);
   void         SayTeam(const char *fmt, ...);

public:
   /*************************************************************
    *                  AI NODES
    ************************************************************/
   virtual bool       AINode_Normal(void) = 0;
};

const int MAX_NODESWITCHES = 50;

#define DEFINE_AINODE(ClassName, BotClass, FuncRun)            \
class ClassName : public CAINode                               \
{                                                              \
public:                                                        \
   ClassName(CBaseBot *pBot) { m_pBot = pBot; }                \
                                                               \
   bool Run(void)                                              \
   {                                                           \
      return ((BotClass *)m_pBot)->FuncRun();                  \
   }                                                           \
                                                               \
   const char *GetDescription(void) { return #ClassName; }     \
}

#define DEFINE_AINODE_ONENTER(ClassName, BotClass, FuncRun, FuncEnter) \
class ClassName : public CAINode                               \
{                                                              \
public:                                                        \
   ClassName(CBaseBot *pBot) { m_pBot = pBot; }                \
                                                               \
   bool Run(void)                                              \
   {                                                           \
      return ((BotClass *)m_pBot)->FuncRun();                  \
   }                                                           \
                                                               \
   void OnEnter(void)                                          \
   {                                                           \
      ((BotClass *)m_pBot)->FuncEnter();                       \
   }                                                           \
                                                               \
   const char *GetDescription(void) { return #ClassName; }     \
}

#define DEFINE_AINODE_ONEXIT(ClassName, BotClass, FuncRun, FuncExit) \
class ClassName : public CAINode                               \
{                                                              \
public:                                                        \
   ClassName(CBaseBot *pBot) { m_pBot = pBot; }                \
                                                               \
   bool Run(void)                                              \
   {                                                           \
      return ((BotClass *)m_pBot)->FuncRun();                  \
   }                                                           \
                                                               \
   void OnExit(void)                                           \
   {                                                           \
      ((BotClass *)m_pBot)->FuncExit();                        \
   }                                                           \
                                                               \
   const char *GetDescription(void) { return #ClassName; }     \
}

#define DEFINE_AINODE_ONENTEREXIT(ClassName, BotClass, FuncRun, FuncEnter, FuncExit) \
class ClassName : public CAINode                               \
{                                                              \
public:                                                        \
   ClassName(CBaseBot *pBot) { m_pBot = pBot; }                \
                                                               \
   bool Run(void)                                              \
   {                                                           \
      return ((BotClass *)m_pBot)->FuncRun();                  \
   }                                                           \
                                                               \
   void OnEnter(void)                                          \
   {                                                           \
      ((BotClass *)m_pBot)->FuncEnter();                       \
   }                                                           \
                                                               \
   void OnExit(void)                                           \
   {                                                           \
      ((BotClass *)m_pBot)->FuncExit();                        \
   }                                                           \
                                                               \
   const char *GetDescription(void) { return #ClassName; }     \
}

#define AINODE_RETURN    do { return false; } while (0)
#define AINODE_SWITCH(NodeClass)                               \
do {                                                           \
   AIEnter(new NodeClass(this));                               \
   return true;                                                \
} while (0)

DEFINE_AINODE(CAINode_Normal, CBaseBot, AINode_Normal); // Normal AI Node

#endif

#endif

