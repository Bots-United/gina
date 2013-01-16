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
// bot_gearbox_ctf.h
//

#ifndef BOT_GEARBOX_CTF_H
#define BOT_GEARBOX_CTF_H

#include "bot_gearbox.h"

class CBotOP4CTF : public CBotOP4
{
public:
   CBotOP4CTF();
   virtual ~CBotOP4CTF();

   bool      MessageWrite(void *p);

   void      StartGame(void);
   void      SpawnInit(void);

   // powerups in OpFor CTF
   bool      m_fHasRegen; // Ammo Regen Power-Up
   bool      m_fHasLongJump; // Jump Pack Power-Up
   bool      m_fHasPhev;  // Shield Power-Up ("Portable HEV")
   bool      m_fHasAccel; // Accelerator (Death) Power-Up
   bool      m_fHasBpack; // Health Power-Up

   int       m_iFlagState;
   int       m_iEnemyFlagState;

   inline bool     HasFlag(void) { return m_iFlagState == FLAG_CARRIED; }

protected:
   class CBotGoal      FindGoal(void);

public:
   /*************************************************************
    *                  AI NODES
    ************************************************************/
   bool                AINode_Normal(void);
};

#endif


