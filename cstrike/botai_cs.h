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

#ifndef BOTAI_CS_H
#define BOTAI_CS_H

// AI nodes
DEFINE_AINODE(CAINode_CS_Buy, CBotCS, AINode_Buy); // buy weapons
DEFINE_AINODE_ONENTER(CAINode_CS_PlantBomb, CBotCS, AINode_PlantBomb, AIEnter_PlantBomb); // plant bomb
DEFINE_AINODE_ONENTER(CAINode_CS_DefuseBomb, CBotCS, AINode_DefuseBomb, AIEnter_PlantBomb); // defuse bomb
DEFINE_AINODE_ONENTER(CAINode_CS_CombatFight, CBotCS, AINode_CombatFight, AIEnter_CombatFight); // fight the enemy

#endif

