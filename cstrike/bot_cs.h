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
// bot_cs.h
//

#ifndef BOT_CS_H
#define BOT_CS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "../bot.h"

class CBotCS : public CBaseBot
{
public:
   CBotCS();
   virtual ~CBotCS();

   int        BotAggression(void);

   bool       MessageWrite(void *p);

   int        m_iAccount; // amount of money in CS

   bool       m_fHasDefuser;         // bot has the defuser
   bool       m_fInBombZone;         // bot is in a bomb site
   bool       m_fInBuyZone;          // bot is in buy zone

   inline bool HasPrimary() { return UtilCS::HasPrimary(this); }
   inline bool HasSecondary() { return UtilCS::HasSecondary(this); }
   inline bool HasShield() { return UtilCS::HasShield(this); }
   inline bool IsShieldDrawn() { return UtilCS::IsShieldDrawn(this); }

   // returns if the bot is VIP
   inline bool IsVIP(void) { return (strcmp(GetModel(), "vip") == 0); }

   void       NewRound(void);

   void       HearRadio(CClient *pFrom, int iMsg);
   void       SendRadio(int iMsg);

private:
   virtual void       StartGame(void);

   // for weapon buying
   int        m_iBuyCount; // current count in buying
   float      m_flBuyTime; // time of next buying
   bool       BuyStuff(void); // do the actual buying

protected:
   // For CS we needn't press fire to respawn
   void       DeadThink(void) {}
   void       SpawnInit(void);

   CBotGoal   FindGoal(void);

   bool       IsShootableThruObstacle(const Vector &vecDest);

public:
   /*************************************************************
    *                  AI NODES
    ************************************************************/
   bool       AINode_Normal(void);

   bool       AINode_Buy(void);

   void       AIEnter_PlantBomb(void);
   bool       AINode_PlantBomb(void);

   void       AIEnter_DefuseBomb(void);
   bool       AINode_DefuseBomb(void);

   void       AIEnter_CombatFight(void);
   bool       AINode_CombatFight(void);
};

#endif

