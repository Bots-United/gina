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
// server_gearbox_ctf.cpp
//

#include "../main.h"

CServerOP4CTF::CServerOP4CTF()
{
}

CServerOP4CTF::~CServerOP4CTF()
{
}

int CServerOP4CTF::GetTeam(CClient *pClient)
{
   // returns the team of the client
   // similar to CS, we just need to check what the 1st character is :)
   char t = pClient->GetModel()[0];
   if (t == 'c' || // Ctf_barney, Cl_suit, Ctf_gina, Ctf_gordon, Ctf_scientist
      t == 'o') // Otis (fat dude!)
      return OP4_TEAM_CIVILIAN;
   return OP4_TEAM_MILITARY; // Beret, Drill, Grunt, Recruit, Shephard, Tower
}

bool CServerOP4CTF::ClientIsImportant(CClient *pClient)
{
   // see if this player has the flag...
   CEntity *pEntity = NULL;
   while (pEntity = UTIL_FindEntityInSphere(pEntity, pClient->GetOrigin(), 10)) {
      if (!pEntity->GetOwner())
         continue; // reliability check
      if (*pEntity->GetOwner() == *pClient &&
         pEntity->GetOrigin() == pClient->GetOrigin() &&
         strcmp(pEntity->GetClassname(), "item_ctfflag") == 0)
         return true; // this player has the flag
   }
   return false; // this player doesn't have the flag
}

CBaseBot *CServerOP4CTF::NewBotInstance()
{
   CBaseBot *pBot = new CBotOP4CTF;
   if (!pBot)
      g_General.TerminateOnError("Memory allocation error !");
   return pBot;
}

