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

/***********************************************************
 * engine_api.cpp                                          *
 ***********************************************************
 * Purpose: Implementation of Half-Life engine functions.  *
 * Engine-specific: yes                                    *
 ***********************************************************
 * Based on: HPB bot - botman's High Ping Bastard bot      *
 * (http://planethalflife.com/botman/)                     *
 *                                                         *
 * Partially based on: RACC by Pierre-Marie Baty           *
 * (http://racc.bots-united.com)                           *
 ***********************************************************/

#include "main.h"

/**
 * The purpose of this function is to precache (load in memory) the 3D models that can have
 * to be spawned during the game. This involves player skins, weapons, items, etc. The
 * precaching of items can only be done at each loading of a new map.
 */
int pfnPrecacheModel(char* s)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PrecacheModel(): %s", s);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPrecacheModel)(s);
}

/**
 * The purpose of this function is to precache (load in memory) the sounds that can have to
 * be played during the game. The precaching of items is done at each loading of a new map.
 * We hook this function in order to compute at precache time the average loudness of the sound
 * being precached, for having a quick reference when the bots have to hear one of them.
 */
int pfnPrecacheSound(char* s)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PrecacheSound(): %s", s);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPrecacheSound)(s);
}

/**
 * The purpose of this function is to ask the engine to set a model m (like "models/
 * mechgibs.mdl") to entity e. The engine itself will provide the necessary changes
 * in the edict's structure for it.
 */
void pfnSetModel(edict_t *e, const char *m)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetModel(): %x %s", e, m);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetModel)(e, m);
}

int pfnModelIndex(const char *m)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ModelIndex(): %s", m);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnModelIndex)(m);
}

int pfnModelFrames(int modelIndex)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ModelFrames(): %d", modelIndex);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnModelFrames)(modelIndex);
}

/**
 * The purpose of this function is to ask the engine to set the bounding box size of the
 * entity pointed to by e. The vector values passed in rgflMin and rgflMax as arrays of
 * floats correspond to the lower left and upper right corners of the bounding box.
 * Note that bounding boxes are always axial, i.e. parallel to the map axises.
 */
void pfnSetSize(edict_t *e, const float *rgflMin, const float *rgflMax)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetSize():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetSize)(e, rgflMin, rgflMax);
}

/**
 * The purpose of this function is to ask the engine to shutdown the server and restart a
 * new one running the map whose name is s1. It is used ONLY IN SINGLE PLAYER MODE and is
 * transparent to the user, because it saves the player state and equipment and restores it
 * back in the new level. The "changelevel trigger point" in the old level is linked to the
 * new level's spawn point using the s2 string, which is formatted as follows: "trigger_name
 * to spawnpoint_name", without spaces (for example, "tr_1atotr_2lm" would tell the engine
 * the player has reached the trigger point "tr_1a" and has to spawn in the next level on the
 * spawn point named "tr_2lm".
 */
void pfnChangeLevel(char* s1, char* s2)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ChangeLevel(): %s, %s", s1, s2);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnChangeLevel)(s1, s2);
}

void pfnGetSpawnParms(edict_t *ent)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetSpawnParms(): %x", ent);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnGetSpawnParms)(ent);
}

void pfnSaveSpawnParms(edict_t *ent)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SaveSpawnParms(): %x", ent);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSaveSpawnParms)(ent);
}

float pfnVecToYaw(const float *rgflVector)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "VecToYaw():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnVecToYaw)(rgflVector);
}

/**
 * The purpose of this function is to convert a spatial location determined by the vector
 * passed in by rgflVectorIn as an array of float into absolute angles from the origin of
 * the world that are written back in the rgflVectorOut array of float (which can be easily
 * converted to angle vector).
 */
void pfnVecToAngles(const float *rgflVectorIn, float *rgflVectorOut)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "VecToAngles():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnVecToAngles)(rgflVectorIn, rgflVectorOut);
}

void pfnMoveToOrigin(edict_t *ent, const float *pflGoal, float dist, int iMoveType)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "MoveToOrigin():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnMoveToOrigin)(ent, pflGoal, dist, iMoveType);
}

/**
 * The purpose of this function is to ask the engine to change the body angle yaw (horizontal
 * angle) of a monster entity pointed to by ent towards this entity's ideal body angles
 * (stored in the entity's entvars_t field in the idealpitch and ideal_yaw variables), by
 * increment of the value of this entity's yaw speed (which is also stored in its entity
 * variables as yaw_speed, this value setting the maximal amount of degrees the monster entity
 * is allowed to turn in ONE frame).
 */
void pfnChangeYaw(edict_t* ent)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ChangeYaw():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnChangeYaw)(ent);
}

/**
 * The purpose of this function is to ask the engine to change the body angle pitch (vertical
 * angle) of a monster entity pointed to by ent towards this entity's ideal body angles
 * (stored in the entity's entvars_t field in the idealpitch and ideal_yaw variables), by
 * increment of the value of this entity's pitch speed (which is also stored in its entity
 * variables as pitch_speed, this value setting the maximal amount of degrees the monster
 * entity is allowed to turn in ONE frame).
 */
void pfnChangePitch(edict_t* ent)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ChangePitch():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnChangePitch)(ent);
}

edict_t* pfnFindEntityByString(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FindEntityByString(): %x, %s, %s", pEdictStartSearchAfter, pszField, pszValue);

   // if we're running Counter-Strike...
   if (g_General.m_iModid == MOD_CSTRIKE) {
      // New round for CS 1.5 or earlier
      if (strcmp(pszValue, "func_bomb_target") == 0) {
         if (g_pServer) {
            g_pServer->NewRound();
         }
      }
   }

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnFindEntityByString)(pEdictStartSearchAfter, pszField, pszValue);
}

int pfnGetEntityIllum(edict_t* pEnt)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetEntityIllum():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnGetEntityIllum)(pEnt);
}

edict_t* pfnFindEntityInSphere(edict_t *pEdictStartSearchAfter, const float *org, float rad)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FindEntityInSphere():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnFindEntityInSphere)(pEdictStartSearchAfter, org, rad);
}

edict_t* pfnFindClientInPVS(edict_t *pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FindClientInPVS():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnFindClientInPVS)(pEdict);
}

edict_t* pfnEntitiesInPVS(edict_t *pplayer)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EntitiesInPVS():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnEntitiesInPVS)(pplayer);
}

void pfnMakeVectors(const float *rgflVector)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "MakeVectors():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnMakeVectors)(rgflVector);
}

void pfnAngleVectors(const float *rgflVector, float *forward, float *right, float *up)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "AngleVectors():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnAngleVectors)(rgflVector, forward, right, up);
}

edict_t* pfnCreateEntity(void)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CreateEntity():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCreateEntity)();
}

void pfnRemoveEntity(edict_t* e)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "RemoveEntity():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnRemoveEntity)(e);
}

edict_t* pfnCreateNamedEntity(int className)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CreateNamedEntity(): %s", STRING(className));

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCreateNamedEntity)(className);
}

void pfnMakeStatic(edict_t *ent)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "MakeStatic():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnMakeStatic)(ent);
}

/**
 * This function asks the engine to check if the entity e is in mid-air, or touches the floor.
 * It returns a positive value (1) if entity e is touching the ground, 0 otherwise. Player
 * entities are handled differently though, because such entities store their "ground state"
 * directly in their entvars_t entity variables, under the flags bitmap variable (either
 * FL_ONGROUND, or FL_PARTIALGROUND). That's why this function should only be used for non
 * player entities.
 */
int pfnEntIsOnFloor(edict_t *e)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EntIsOnFloor():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnEntIsOnFloor)(e);
}

/**
 * This function asks the engine to change the vector location (origin) of entity e so that
 * it will touch the ground. Most map makers place entities slightly in mid-air, to avoid
 * the moving ones to get stuck in case they would slightly overlap each other with the
 * world. Engine programmers got it fine: at each start of a new level, the appropriate
 * entities are dropped to floor. Note we are not using engine physics for this, the result
 * is immediate, like a teleport.
 */
int pfnDropToFloor(edict_t* e)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "DropToFloor():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnDropToFloor)(e);
}

/**
 * This function is the equivalent of pfnRunPlayerMove, but for monsters. It tells the engine
 * to play the movement animation of the monster character ent, which will face the direction
 * yaw (yaw is a view angle), using mode iMode (determining whether the monster is allowed to
 * pass through entities, for example WALKMOVE_NORMAL means the monster will collide with the
 * world and activate triggers like every player would instead). dist is the distance that
 * will have been walked once the animation is over, i.e. for most monsters, the step size.
 * Setting it to a low value could turn your monster into a Michael Jackson "moonwalking" :)
 * Note that whereas a RunPlayerMove movement lasts strictly the duration of the frame, a
 * monster WalkMove extends during several frames, until the animation is completed.
 */
int pfnWalkMove(edict_t *ent, float yaw, float dist, int iMode)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "WalkMove():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnWalkMove)(ent, yaw, dist, iMode);
}

/**
 * This function tells the engine to change the origin (vector location in the virtual world)
 * of entity e to the vector location rgflOrigin (which must point to a vector).
 */
void pfnSetOrigin(edict_t *e, const float *rgflOrigin)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetOrigin():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetOrigin)(e, rgflOrigin);
}

/**
 * This function tells the engine that the entity pointed to by "entity", is emitting a sound
 * which filename is "sample", at level "channel" (CHAN_VOICE, etc...), with "volume" as
 * loudness multiplicator (normal volume VOL_NORM is 1.0), with a pitch of "pitch" (normal
 * pitch PITCH_NORM is 100.0), and that this sound has to be attenuated by distance in air
 * according to the value of "attenuation" (normal attenuation ATTN_NORM is 0.8 ; ATTN_NONE
 * means no attenuation with distance).
 */
void pfnEmitSound(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EmitSound(): %x, %d, %s, %f, %f, %d, %d", entity, channel, sample, volume, attenuation, fFlags, pitch);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnEmitSound)(entity, channel, sample, volume, attenuation, fFlags, pitch);
}

/**
 * This function tells the engine that the entity pointed to by "entity", is emitting a sound
 * which filename is "samp", at position "pos" (pos must point to a vector), with "vol" as
 * loudness multiplicator (normal volume VOL_NORM is 1.0), with a pitch of "pitch" (normal
 * pitch PITCH_NORM is 100.0), and that this sound has to be attenuated by distance in air
 * according to the value of "attenuation" (normal attenuation ATTN_NORM is 0.8 ; ATTN_NONE
 * means no attenuation with distance).
 */
void pfnEmitAmbientSound(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EmitAmbientSound(): %x, %s, %f, %f, %d, %d", entity, samp, vol, attenuation, fFlags, pitch);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnEmitAmbientSound)(entity, pos, samp, vol, attenuation, fFlags, pitch);
}

/**
 * This function is one of the most important of the game. It is a test line. By calling
 * TraceLine the engine is told to check the direct reachability between two points, and to
 * return the answer in a special dedicated structure. In detail, it is about starting from
 * spatial location v1, going in direct line to v2 (v1 and v2 point to vectors), ignoring or
 * not entities that are monsters along the way (depending whether fNoMonsters is set to
 * enumerated type ignore_monsters or dont_ignore_monsters), ignoring if needed the entity it
 * starts within, pentToSkip, and returning the results of the trace in structure ptr, which
 * results may contain lots of useful info. The most important is the flFraction field, a
 * fraction of 1 proportionnal to the distance that has been successfully reached without
 * obstacles. I.e, if flFraction is 0.5, the trace has covered half the distance successfully.
 */
void pfnTraceLine(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceLine():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnTraceLine)(v1, v2, fNoMonsters, pentToSkip, ptr);
}

void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceToss():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnTraceToss)(pent, pentToIgnore, ptr);
}

int pfnTraceMonsterHull(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceMonsterHull():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnTraceMonsterHull)(pEdict, v1, v2, fNoMonsters, pentToSkip, ptr);
}

void pfnTraceHull(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceHull():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnTraceHull)(v1, v2, fNoMonsters, hullNumber, pentToSkip, ptr);
}

void pfnTraceModel(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceModel():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnTraceModel)(v1, v2, hullNumber, pent, ptr);
}

const char *pfnTraceTexture(edict_t *pTextureEntity, const float *v1, const float *v2 )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceTexture():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnTraceTexture)(pTextureEntity, v1, v2);
}

void pfnTraceSphere(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "TraceSphere():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnTraceSphere)(v1, v2, fNoMonsters, radius, pentToSkip, ptr);
}

void pfnGetAimVector(edict_t* ent, float speed, float *rgflReturn)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetAimVector():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnGetAimVector)(ent, speed, rgflReturn);
}

void pfnServerCommand(char* str)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ServerCommand(): %s", str);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnServerCommand)(str);
}

void pfnServerExecute(void)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ServerExecute():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnServerExecute)();
}

/**
 * This function forces the client whose player entity is pEntity to issue a client command.
 * How it works is that clients all have a g_argv global string in their client DLL that
 * stores the command string; if ever that string is filled with characters, the client DLL
 * sends it to the engine as a command to be executed. When the engine has executed that
 * command, this g_argv string is reset to zero. Here is somehow a curious implementation of
 * ClientCommand: the engine sets the command it wants the client to issue in his g_argv, then
 * the client DLL sends it back to the engine, the engine receives it then executes the
 * command therein. Don't ask me why we need all this complicated crap. Anyhow since bots have
 * no client DLL, be certain never to call this function upon a bot entity, else it will just
 * make the server crash. Since hordes of uncautious, not to say stupid, programmers don't
 * even imagine some players on their servers could be bots, this check is performed less than
 * sometimes actually by their side, that's why we strongly recommend to check it here too. In
 * case it's a bot asking for a client command, we handle it like we do for bot commands, ie
 * using FakeClientCommand().
 */
void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
   va_list argptr;
   char string[256];

   va_start(argptr, szFmt);
   _vsnprintf(string, sizeof(string), szFmt, argptr);
   va_end(argptr);

   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ClientCommand(): %s", string);

   CClient *pClient = CClient::Instance(pEdict);
   if (pClient) {
      if (pClient->IsBot()) {
         g_General.FakeClientCommand(pClient, string);

         if (g_fIsMetamod)
            RETURN_META(MRES_SUPERCEDE);

         return;
      }
   }

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnClientCommand)(pEdict, string);
}

void pfnParticleEffect(const float *org, const float *dir, float color, float count)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ParticleEffect():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnParticleEffect)(org, dir, color, count);
}

void pfnLightStyle(int style, char* val)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "LightStyle():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnLightStyle)(style, val);
}

int pfnDecalIndex(const char *name)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "DecalIndex(): %s", name);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnDecalIndex)(name);
}

int pfnPointContents(const float *rgflVector)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PointContents():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPointContents)(rgflVector);
}

void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
   const char *type = g_General.GetUserMsgName(msg_type);
   g_Debug.DebugLog(DEBUG_NETMSG, "MessageBegin(): %d, %d (%s), %x", msg_dest, msg_type, type ? type : "UNKNOWN" , ed);

   if (g_pServer)
      g_pServer->MessageBegin(msg_type, ed);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed);
}

void pfnMessageEnd(void)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "MessageEnd():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnMessageEnd)();
}

void pfnWriteByte(int iValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteByte(): %d", iValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&iValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteByte)(iValue);
}

void pfnWriteChar(int iValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteChar(): %d", iValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&iValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteChar)(iValue);
}

void pfnWriteShort(int iValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteShort(): %d", iValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&iValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteShort)(iValue);
}

void pfnWriteLong(int iValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteLong(): %d", iValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&iValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteLong)(iValue);
}

void pfnWriteAngle(float flValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteAngle(): %f", flValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&flValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteAngle)(flValue);
}

void pfnWriteCoord(float flValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteCoord(): %f", flValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&flValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteCoord)(flValue);
}

void pfnWriteString(const char *sz)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteString(): %s", sz);

   if (g_pServer)
      g_pServer->MessageWrite((void *)sz);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteString)(sz);
}

void pfnWriteEntity(int iValue)
{
   g_Debug.DebugLog(DEBUG_NETMSG, "WriteEntity(): %d", iValue);

   if (g_pServer)
      g_pServer->MessageWrite((void *)&iValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnWriteEntity)(iValue);
}

void pfnCVarRegister(cvar_t *pCvar)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CVarRegister():", pCvar->name);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCVarRegister)(pCvar);
}

float pfnCVarGetFloat(const char *szVarName)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CVarGetFloat(): %s", szVarName);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCVarGetFloat)(szVarName);
}

const char* pfnCVarGetString(const char *szVarName)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CVarGetString(): %s", szVarName);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCVarGetString)(szVarName);
}

void pfnCVarSetFloat(const char *szVarName, float flValue)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CVarSetFloat(): %s, %f", szVarName, flValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCVarSetFloat)(szVarName, flValue);
}

void pfnCVarSetString(const char *szVarName, const char *szValue)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CVarSetString(): %s %s", szVarName, szValue);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCVarSetString)(szVarName, szValue);
}

void pfnEngineFprintf(struct _iobuf *pfile, char *szFmt, ...)
{
   va_list argptr;
   char string[256];

   va_start(argptr, szFmt);
   _vsnprintf(string, sizeof(string), szFmt, argptr);
   va_end(argptr);

   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EngineFprintf(): %s", string);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnEngineFprintf)(pfile, string);
}

void pfnAlertMessage(ALERT_TYPE atype, char *szFmt, ...)
{
   va_list argptr;
   char string[256];

   va_start(argptr, szFmt);
   _vsnprintf(string, sizeof(string), szFmt, argptr);
   va_end(argptr);

   g_Debug.DebugLog(DEBUG_ENGINEAPI, "AlertMessage(): %d, %s", atype, string);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnAlertMessage)(atype, string);
}

void* pfnPvAllocEntPrivateData(edict_t *pEdict, int32 cb)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PvAllocEntPrivateData():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPvAllocEntPrivateData)(pEdict, cb);
}

void* pfnPvEntPrivateData(edict_t *pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PvEntPrivateData():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPvEntPrivateData)(pEdict);
}

void pfnFreeEntPrivateData(edict_t *pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FreeEntPrivateData():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnFreeEntPrivateData)(pEdict);
}

const char* pfnSzFromIndex(int iString)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SzFromIndex():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnSzFromIndex)(iString);
}

int pfnAllocString(const char *szValue)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "AllocString(): %s", szValue);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnAllocString)(szValue);
}

entvars_t* pfnGetVarsOfEnt(edict_t *pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetVarsOfEnt():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnGetVarsOfEnt)(pEdict);
}

edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PEntityOfEntOffset():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPEntityOfEntOffset)(iEntOffset);
}

int pfnEntOffsetOfPEntity(const edict_t *pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EntOffsetOfPEntity():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnEntOffsetOfPEntity)(pEdict);
}

int pfnIndexOfEdict(const edict_t *pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "IndexOfEdict():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnIndexOfEdict)(pEdict);
}

edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PEntityOfEntIndex():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPEntityOfEntIndex)(iEntIndex);
}

edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FindEntityByVars():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnFindEntityByVars)(pvars);
}

void* pfnGetModelPtr(edict_t* pEdict)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetModelPtr():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnGetModelPtr)(pEdict);
}

/**
 * This function registers a "user message" by the engine side. User messages are network
 * messages the game DLL asks the engine to send to clients. Since many MODs have completely
 * different client features (Counter-Strike has a radar and a timer, for example), network
 * messages just can't be the same for every MOD. Hence here the MOD DLL tells the engine,
 * "Hey, you have to know that I use a network message whose name is pszName and it is iSize
 * packets long". The engine books it, and returns the ID number under which he recorded that
 * custom message. Thus every time the MOD DLL will be wanting to send a message named pszName
 * using pfnMessageBegin(), it will know what message ID number to send, and the engine will
 * know what to do.
 */
int pfnRegUserMsg(const char *pszName, int iSize)
{
   int iMsg = (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);
   g_Debug.DebugLog(DEBUG_NETMSG, "RegUserMsg(): %s, %d", pszName, iMsg);
   g_General.AddUserMsg(pszName, iMsg);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_SUPERCEDE, iMsg);

   return iMsg;
}

void pfnAnimationAutomove(const edict_t* pEdict, float flTime)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "AnimationAutomove():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnAnimationAutomove)(pEdict, flTime);
}

void pfnGetBonePosition(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetBonePosition():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnGetBonePosition)(pEdict, iBone, rgflOrigin, rgflAngles);
}

/**
 * This function prints the text message string pointed to by szMsg by the client side of
 * the client entity pointed to by pEntity, in a manner depending of ptype (print_console,
 * print_center or print_chat). Be certain never to try to feed a bot with this function,
 * as it will crash your server. Why would you, anyway ? bots have no client DLL as far as
 * we know, right ? But since stupidity rules this world, we do a preventive check :)
 */
void pfnClientPrintf(edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ClientPrintf(): %x, %d, %s", pEdict, ptype, szMsg);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnClientPrintf)(pEdict, ptype, szMsg);
}

void pfnServerPrint( const char *szMsg )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "ServerPrint(): %s", szMsg);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnServerPrint)(szMsg);
}

/*
 * This function returns the number of arguments the current client command string has. Since
 * bots have no client DLL and we may want a bot to execute a client command, we had to
 * implement a g_argv string in the bot DLL for holding the bots' commands, and also keep
 * track of the argument count. Hence this hook not to let the engine ask an unexistent client
 * DLL for a command we are holding here. Of course, real clients commands are still retrieved
 * the normal way, by asking the engine.
 */
int pfnCmd_Argc(void)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "Cmd_Argc():");

   // is this a bot issuing that client command ?
   if (g_General.IsFakeClientCommand()) {
      if (g_fIsMetamod)
         RETURN_META_VALUE(MRES_SUPERCEDE, g_General.FakeArgCount());

      return g_General.FakeArgCount(); // if so, then return the argument count we know
   }

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCmd_Argc)();
}

/**
 * This function returns a pointer to the whole current client command string. Since bots
 * have no client DLL and we may want a bot to execute a client command, we had to implement
 * a m_szArgv string in CGeneral class for holding the bots' commands, and also keep track of the
 * argument count. Hence this hook not to let the engine ask an unexistent client DLL for a
 * command we are holding here. Of course, real clients commands are still retrieved the
 * normal way, by asking the engine.
 */
const char *pfnCmd_Args(void)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "Cmd_Args():");

   // is this a bot issuing that client command ?
   if (g_General.IsFakeClientCommand()) {
      if (g_fIsMetamod)
         RETURN_META_VALUE(MRES_SUPERCEDE, g_General.GetArgs());

      return g_General.GetArgs();
   }

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCmd_Args)();
}

/**
 * This function returns a pointer to a certain argument of the current client command. Since
 * bots have no client DLL and we may want a bot to execute a client command, we had to
 * implement a g_argv string in the bot DLL for holding the bots' commands, and also keep
 * track of the argument count. Hence this hook not to let the engine ask an unexistent client
 * DLL for a command we are holding here. Of course, real clients commands are still retrieved
 * the normal way, by asking the engine.
 */
const char *pfnCmd_Argv(int argc)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "Cmd_Argv(): %d", argc);

   // is this a bot issuing that client command?
   if (g_General.IsFakeClientCommand()) {
      if (g_fIsMetamod)
         RETURN_META_VALUE(MRES_SUPERCEDE, g_General.GetArg(argc));
      return g_General.GetArg(argc);
   }

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCmd_Argv)(argc);
}

void pfnGetAttachment(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetAttachment():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnGetAttachment)(pEdict, iAttachment, rgflOrigin, rgflAngles);
}

void pfnCRC32_Init(CRC32_t *pulCRC)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CRC32_Init():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCRC32_Init)(pulCRC);
}

void pfnCRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CRC32_ProcessBuffer():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCRC32_ProcessBuffer)(pulCRC, p, len);
}

void pfnCRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CRC32_ProcessByte():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCRC32_ProcessByte)(pulCRC, ch);
}

CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CRC32_Final():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCRC32_Final)(pulCRC);
}

int32 pfnRandomLong(int32 lLow, int32 lHigh)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "RandomLong(): %d %d", lLow, lHigh);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnRandomLong)(lLow, lHigh);
}

float pfnRandomFloat(float flLow, float flHigh)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "RandomFloat(): %f %f", flLow, flHigh);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnRandomFloat)(flLow, flHigh);
}

void pfnSetView(const edict_t *pClient, const edict_t *pViewent )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetView(): %x %x", pClient, pViewent);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetView)(pClient, pViewent);
}

float pfnTime( void )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "Time():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnTime)();
}

void pfnCrosshairAngle(const edict_t *pClient, float pitch, float yaw)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CrosshairAngle():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCrosshairAngle)(pClient, pitch, yaw);
}

byte *pfnLoadFileForMe(char *filename, int *pLength)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "LoadFileForMe(): %s", filename);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnLoadFileForMe)(filename, pLength);
}

void pfnFreeFile(void *buffer)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FreeFile(): %x", buffer);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnFreeFile)(buffer);
}

void pfnEndSection(const char *pszSectionName)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "EndSection(): %s", pszSectionName);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnEndSection)(pszSectionName);
}

int pfnCompareFileTime(char *filename1, char *filename2, int *iCompare)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CompareFileTime(): %s, %s", filename1, filename2);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCompareFileTime)(filename1, filename2, iCompare);
}

void pfnGetGameDir(char *szGetGameDir)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetGameDir()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnGetGameDir)(szGetGameDir);
}

void pfnCvar_RegisterVariable(cvar_t *variable)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "Cvar_RegisterVariable(): %s", variable->name);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnCvar_RegisterVariable)(variable);
}

void pfnFadeClientVolume(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "FadeClientVolume(): %x, %d, %d, %d, %f", pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnFadeClientVolume)(pEdict, fadePercent, fadeOutSeconds, holdTime, fadeInSeconds);
}

void pfnSetClientMaxspeed(const edict_t *pEdict, float fNewMaxspeed)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetClientMaxspeed(): %x, %f", pEdict, fNewMaxspeed);

   ((edict_t *)pEdict)->v.maxspeed = fNewMaxspeed;

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetClientMaxspeed)(pEdict, fNewMaxspeed);
}

edict_t * pfnCreateFakeClient(const char *netname)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "CreateFakeClient(): %s", netname);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnCreateFakeClient)(netname);
}

void pfnRunPlayerMove(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "RunPlayerMove(): %x, (%f, %f, %f), %f, %f, %f, %u, %u, %u", fakeclient, viewangles[0], viewangles[1], viewangles[2], forwardmove, sidemove, upmove, buttons, impulse, msec);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnRunPlayerMove)(fakeclient, viewangles, forwardmove, sidemove, upmove, buttons, impulse, msec);
}

int pfnNumberOfEntities(void)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "NumberOfEntities():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnNumberOfEntities)();
}

char* pfnGetInfoKeyBuffer(edict_t *e)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "GetInfoKeyBuffer():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnGetInfoKeyBuffer)(e);
}

char* pfnInfoKeyValue(char *infobuffer, char *key)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "InfoKeyValue():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnInfoKeyValue)(infobuffer, key);
}

void pfnSetKeyValue(char *infobuffer, char *key, char *value)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetKeyValue():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetKeyValue)(infobuffer, key, value);
}

void pfnSetClientKeyValue(int clientIndex, char *infobuffer, char *key, char *value)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "SetClientKeyValue():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnSetClientKeyValue)(clientIndex, infobuffer, key, value);
}

int pfnIsMapValid(char *filename)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "IsMapValid():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnIsMapValid)(filename);
}

void pfnStaticDecal( const float *origin, int decalIndex, int entityIndex, int modelIndex )
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "StaticDecal():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnStaticDecal)(origin, decalIndex, entityIndex, modelIndex);
}

int pfnPrecacheGeneric(char* s)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "PrecacheGeneric(): %s", s);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnPrecacheGeneric)(s);
}

int pfnGetPlayerUserId(edict_t *e)
{
   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnGetPlayerUserId)(e);
}

void pfnBuildSoundMsg(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "BuildSoundMsg():");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*g_engfuncs.pfnBuildSoundMsg)(entity, channel, sample, volume, attenuation, fFlags, pitch, msg_dest, msg_type, pOrigin, ed);
}

int pfnIsDedicatedServer(void)
{
   g_Debug.DebugLog(DEBUG_ENGINEAPI, "IsDedicatedServer():");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*g_engfuncs.pfnIsDedicatedServer)();
}

// Pass engine functions table to the game DLL or Metamod
C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine,
		                           int *interfaceVersion)
{
   if (g_fIsMetamod)
      memset(pengfuncsFromEngine, 0, sizeof(pengfuncsFromEngine));

   pengfuncsFromEngine->pfnPrecacheModel = pfnPrecacheModel;
   pengfuncsFromEngine->pfnPrecacheSound = pfnPrecacheSound;
   pengfuncsFromEngine->pfnSetModel = pfnSetModel;
   pengfuncsFromEngine->pfnModelIndex = pfnModelIndex;
   pengfuncsFromEngine->pfnModelFrames = pfnModelFrames;
   pengfuncsFromEngine->pfnSetSize = pfnSetSize;
   pengfuncsFromEngine->pfnChangeLevel = pfnChangeLevel;
   pengfuncsFromEngine->pfnGetSpawnParms = pfnGetSpawnParms;
   pengfuncsFromEngine->pfnSaveSpawnParms = pfnSaveSpawnParms;
   pengfuncsFromEngine->pfnVecToYaw = pfnVecToYaw;
   pengfuncsFromEngine->pfnVecToAngles = pfnVecToAngles;
   pengfuncsFromEngine->pfnMoveToOrigin = pfnMoveToOrigin;
   pengfuncsFromEngine->pfnChangeYaw = pfnChangeYaw;
   pengfuncsFromEngine->pfnChangePitch = pfnChangePitch;
   pengfuncsFromEngine->pfnFindEntityByString = pfnFindEntityByString;
   pengfuncsFromEngine->pfnGetEntityIllum = pfnGetEntityIllum;
   pengfuncsFromEngine->pfnFindEntityInSphere = pfnFindEntityInSphere;
   pengfuncsFromEngine->pfnFindClientInPVS = pfnFindClientInPVS;
   pengfuncsFromEngine->pfnEntitiesInPVS = pfnEntitiesInPVS;
   pengfuncsFromEngine->pfnMakeVectors = pfnMakeVectors;
   pengfuncsFromEngine->pfnAngleVectors = pfnAngleVectors;
   pengfuncsFromEngine->pfnCreateEntity = pfnCreateEntity;
   pengfuncsFromEngine->pfnRemoveEntity = pfnRemoveEntity;
   pengfuncsFromEngine->pfnCreateNamedEntity = pfnCreateNamedEntity;
   pengfuncsFromEngine->pfnMakeStatic = pfnMakeStatic;
   pengfuncsFromEngine->pfnEntIsOnFloor = pfnEntIsOnFloor;
   pengfuncsFromEngine->pfnDropToFloor = pfnDropToFloor;
   pengfuncsFromEngine->pfnWalkMove = pfnWalkMove;
   pengfuncsFromEngine->pfnSetOrigin = pfnSetOrigin;
   pengfuncsFromEngine->pfnEmitSound = pfnEmitSound;
   pengfuncsFromEngine->pfnEmitAmbientSound = pfnEmitAmbientSound;
   pengfuncsFromEngine->pfnTraceLine = pfnTraceLine;
   pengfuncsFromEngine->pfnTraceToss = pfnTraceToss;
   pengfuncsFromEngine->pfnTraceMonsterHull = pfnTraceMonsterHull;
   pengfuncsFromEngine->pfnTraceHull = pfnTraceHull;
   pengfuncsFromEngine->pfnTraceModel = pfnTraceModel;
   pengfuncsFromEngine->pfnTraceTexture = pfnTraceTexture;
   pengfuncsFromEngine->pfnTraceSphere = pfnTraceSphere;
   pengfuncsFromEngine->pfnGetAimVector = pfnGetAimVector;
   pengfuncsFromEngine->pfnServerCommand = pfnServerCommand;
   pengfuncsFromEngine->pfnServerExecute = pfnServerExecute;
   pengfuncsFromEngine->pfnClientCommand = pfnClientCommand;
   pengfuncsFromEngine->pfnParticleEffect = pfnParticleEffect;
   pengfuncsFromEngine->pfnLightStyle = pfnLightStyle;
   pengfuncsFromEngine->pfnDecalIndex = pfnDecalIndex;
   pengfuncsFromEngine->pfnPointContents = pfnPointContents;
   pengfuncsFromEngine->pfnMessageBegin = pfnMessageBegin;
   pengfuncsFromEngine->pfnMessageEnd = pfnMessageEnd;
   pengfuncsFromEngine->pfnWriteByte = pfnWriteByte;
   pengfuncsFromEngine->pfnWriteChar = pfnWriteChar;
   pengfuncsFromEngine->pfnWriteShort = pfnWriteShort;
   pengfuncsFromEngine->pfnWriteLong = pfnWriteLong;
   pengfuncsFromEngine->pfnWriteAngle = pfnWriteAngle;
   pengfuncsFromEngine->pfnWriteCoord = pfnWriteCoord;
   pengfuncsFromEngine->pfnWriteString = pfnWriteString;
   pengfuncsFromEngine->pfnWriteEntity = pfnWriteEntity;
   pengfuncsFromEngine->pfnCVarRegister = pfnCVarRegister;
   pengfuncsFromEngine->pfnCVarGetFloat = pfnCVarGetFloat;
   pengfuncsFromEngine->pfnCVarGetString = pfnCVarGetString;
   pengfuncsFromEngine->pfnCVarSetFloat = pfnCVarSetFloat;
   pengfuncsFromEngine->pfnCVarSetString = pfnCVarSetString;
   pengfuncsFromEngine->pfnPvAllocEntPrivateData = pfnPvAllocEntPrivateData;
   pengfuncsFromEngine->pfnPvEntPrivateData = pfnPvEntPrivateData;
   pengfuncsFromEngine->pfnFreeEntPrivateData = pfnFreeEntPrivateData;
   pengfuncsFromEngine->pfnSzFromIndex = pfnSzFromIndex;
   pengfuncsFromEngine->pfnAllocString = pfnAllocString;
   pengfuncsFromEngine->pfnGetVarsOfEnt = pfnGetVarsOfEnt;
   pengfuncsFromEngine->pfnPEntityOfEntOffset = pfnPEntityOfEntOffset;
   pengfuncsFromEngine->pfnEntOffsetOfPEntity = pfnEntOffsetOfPEntity;
   pengfuncsFromEngine->pfnIndexOfEdict = pfnIndexOfEdict;
   pengfuncsFromEngine->pfnPEntityOfEntIndex = pfnPEntityOfEntIndex;
   pengfuncsFromEngine->pfnFindEntityByVars = pfnFindEntityByVars;
   pengfuncsFromEngine->pfnGetModelPtr = pfnGetModelPtr;
   pengfuncsFromEngine->pfnRegUserMsg = pfnRegUserMsg;
   pengfuncsFromEngine->pfnAnimationAutomove = pfnAnimationAutomove;
   pengfuncsFromEngine->pfnGetBonePosition = pfnGetBonePosition;
   pengfuncsFromEngine->pfnClientPrintf = pfnClientPrintf;
   pengfuncsFromEngine->pfnServerPrint = pfnServerPrint;
   pengfuncsFromEngine->pfnCmd_Args = pfnCmd_Args;
   pengfuncsFromEngine->pfnCmd_Argv = pfnCmd_Argv;
   pengfuncsFromEngine->pfnCmd_Argc = pfnCmd_Argc;
   pengfuncsFromEngine->pfnGetAttachment = pfnGetAttachment;
   pengfuncsFromEngine->pfnCRC32_Init = pfnCRC32_Init;
   pengfuncsFromEngine->pfnCRC32_ProcessBuffer = pfnCRC32_ProcessBuffer;
   pengfuncsFromEngine->pfnCRC32_ProcessByte = pfnCRC32_ProcessByte;
   pengfuncsFromEngine->pfnCRC32_Final = pfnCRC32_Final;
   pengfuncsFromEngine->pfnRandomLong = pfnRandomLong;
   pengfuncsFromEngine->pfnRandomFloat = pfnRandomFloat;
   pengfuncsFromEngine->pfnSetView = pfnSetView;
   pengfuncsFromEngine->pfnTime = pfnTime;
   pengfuncsFromEngine->pfnCrosshairAngle = pfnCrosshairAngle;
   pengfuncsFromEngine->pfnLoadFileForMe = pfnLoadFileForMe;
   pengfuncsFromEngine->pfnFreeFile = pfnFreeFile;
   pengfuncsFromEngine->pfnEndSection = pfnEndSection;
   pengfuncsFromEngine->pfnCompareFileTime = pfnCompareFileTime;
   pengfuncsFromEngine->pfnGetGameDir = pfnGetGameDir;
   pengfuncsFromEngine->pfnCvar_RegisterVariable = pfnCvar_RegisterVariable;
   pengfuncsFromEngine->pfnFadeClientVolume = pfnFadeClientVolume;
   pengfuncsFromEngine->pfnSetClientMaxspeed = pfnSetClientMaxspeed;
   pengfuncsFromEngine->pfnCreateFakeClient = pfnCreateFakeClient;
   pengfuncsFromEngine->pfnRunPlayerMove = pfnRunPlayerMove;
   pengfuncsFromEngine->pfnNumberOfEntities = pfnNumberOfEntities;
   pengfuncsFromEngine->pfnGetInfoKeyBuffer = pfnGetInfoKeyBuffer;
   pengfuncsFromEngine->pfnInfoKeyValue = pfnInfoKeyValue;
   pengfuncsFromEngine->pfnSetKeyValue = pfnSetKeyValue;
   pengfuncsFromEngine->pfnSetClientKeyValue = pfnSetClientKeyValue;
   pengfuncsFromEngine->pfnIsMapValid = pfnIsMapValid;
   pengfuncsFromEngine->pfnStaticDecal = pfnStaticDecal;
   pengfuncsFromEngine->pfnPrecacheGeneric = pfnPrecacheGeneric;
   pengfuncsFromEngine->pfnGetPlayerUserId = pfnGetPlayerUserId;
   pengfuncsFromEngine->pfnBuildSoundMsg = pfnBuildSoundMsg;
   pengfuncsFromEngine->pfnIsDedicatedServer = pfnIsDedicatedServer;

   return TRUE;
}

