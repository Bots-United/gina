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
// dllapi.cpp
//
// Implementation of Half-Life DLL routines
//
// Based on: HPB bot - botman's High Ping Bastard bot
// (http://planethalflife.com/botman/)
//
// HL Function descriptions is from RACC by Pierre-Marie Baty
// (http://racc.bots-united.com)
//

#include "main.h"

bool is_opforctf = false; // for Opposing Force
int g_iMapTypeCS = 0; // for Counter-Strike

static char welcome_msg[] = "Welcome to GINA bot by Wei Mingzhi\n"
                     "Visit http://gina.codeplex.com/\n"
                     "Compile date: " __DATE__ "\n";

edict_t *listenserver_edict = NULL;
float welcome_time = 0.0;
bool welcome_sent = FALSE;

/**
 * this function is a one-time call, and appears to be the second function called in the
 * DLL after GiveFnptrsToDll() has been called. Its purpose is to tell the MOD DLL to
 * initialize the game before the engine actually hooks into it with its video frames and
 * clients connecting. Note that it is a different step than the *server* initialization.
 * This one is called once, and only once, when the game process boots up before the first
 * server is enabled. Here is a good place to do our own game session initialization, and
 * to register by the engine side the server commands we need to administrate our bots.
 */
void GameDLLInit( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "GameDllInit()");

   g_General.GameInit();

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnGameInit)();
}

/**
 * This function asks the game DLL to spawn (i.e, give a physical existence in the virtual
 * world, in other words to 'display') the entity pointed to by pEntity in the game. The
 * Spawn() function is one of the functions any entity is supposed to have in the game DLL,
 * and any MOD is supposed to implement one for each of its entities.
 */
int DispatchSpawn( edict_t *pent )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchSpawn(): %s", STRING(pent->v.classname));

   if (strcmp(STRING(pent->v.classname), "worldspawn") == 0) {
      g_General.PrecacheStuff(); // precache resources used in bot code
      is_opforctf = false;
      g_iMapTypeCS = 0;
   }

   // Determine map type for Counter-Strike...
   if (g_General.m_iModid == MOD_CSTRIKE) {
      if (strcmp(STRING(pent->v.classname), "func_vip_safetyzone") == 0 ||
         strcmp(STRING(pent->v.classname), "info_vip_safetyzone") == 0)
         g_iMapTypeCS |= MAP_AS; // assassination map

      else if (strcmp(STRING(pent->v.classname), "hostage_entity") == 0)
         g_iMapTypeCS |= MAP_CS; // rescue map

      else if (strcmp(STRING(pent->v.classname), "func_bomb_target") == 0 ||
         strcmp(STRING(pent->v.classname), "info_bomb_target") == 0)
         g_iMapTypeCS |= MAP_DE; // defusion map
   }

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   int iReturn = (*other_gFunctionTable.pfnSpawn)(pent);

   // solves the bots unable to see through certain types of glass bug.
   // MAPPERS: NEVER EVER ALLOW A TRANSPARENT ENTITY TO WEAR THE FL_WORLDBRUSH FLAG !!!
   if (pent->v.rendermode == kRenderTransTexture)
      pent->v.flags &= ~FL_WORLDBRUSH; // clear the FL_WORLDBRUSH flag out of transparent ents

   return iReturn;
}

int DispatchSpawn_Post(edict_t *pent)
{
   if (pent->v.rendermode == kRenderTransTexture)
      pent->v.flags &= ~FL_WORLDBRUSH; // clear the FL_WORLDBRUSH flag out of transparent ents

   RETURN_META_VALUE(MRES_IGNORED, 0);
}

/**
 * This function asks the game DLL to make the entity pointed to by pEntity 'think' (by
 * calling the appropriate function in the game DLL. The Think() function is another one of
 * the functions any entity is supposed to have in the game DLL, and any MOD is supposed to
 * implement one for each of its entities.
 */
void DispatchThink( edict_t *pent )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchThink(): %s", STRING(pent->v.classname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnThink)(pent);
}

/**
 * This function is called when two entities interact upon another (using a special "use"
 * flag). For example, a player is likely to press a switch for, let's say, opening a door or
 * raising an elevator. When the IN_USE flag is set in the player's input buttons structure,
 * and some usable entity is in range, this function is called. The pentUsed pointer
 * represents the entity being used (button, switch, etc.), whereas the pentOther pointer
 * points to the entity taking the action (usually a player).
 */
void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchUse(): pentUsed = %s, pentOther = %s",
      STRING(pentUsed->v.classname), STRING(pentOther->v.classname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnUse)(pentUsed, pentOther);
}

/**
 * This function is called when two entities' bounding boxes enter in collision. For example,
 * when a player walks upon a gun, the player entity bounding box collides to the gun entity
 * bounding box, and the result is that this function is called. It is used by the game for
 * taking the appropriate action when such an event occurs (in our example, the player who
 * is walking upon the gun will "pick it up"). Entities that "touch" others are usually
 * entities having a velocity, as it is assumed that static entities (entities that don't
 * move) will never touch anything. Hence, in our example, the pentTouched will be the gun
 * (static entity), whereas the pentOther will be the player (as it is the one moving). When
 * the two entities both have velocities, for example two players colliding, this function
 * is called twice, once for each entity moving.
 */
void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchTouch(): pentTouched = %s, pentOther = %s",
      STRING(pentTouched->v.classname), STRING(pentOther->v.classname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnTouch)(pentTouched, pentOther);
}

/**
 * Some entities may be blocked by others. It's the case for elevators, doors, trains. For
 * example, if someone is standing still on the way of a bobbing platform, the platform will
 * hit it and go back in the opposite direction. This function is called when an entity is
 * blocked by another one, for the game DLL to take the appropriate actions. The pentBlocked
 * pointer represents the entity being blocked from movement or action (elevator, door, etc),
 * whereas the pentOther pointer points to the blocking entity (usually a player, but can be
 * a pushable crate or any other entity).
 */
void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchBlocked(): pentBlocked = %s, pentOther = %s",
      STRING(pentBlocked->v.classname), STRING(pentOther->v.classname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnBlocked)(pentBlocked, pentOther);
}

/**
 * This function is called when the game requests a pointer to some entity's keyvalue data.
 * The keyvalue data is held in each entity's infobuffer (basically a char buffer where each
 * game DLL can put the stuff it wants) under - as it says - the form of a key/value pair. A
 * common example of key/value pair is the "model", "(name of player model here)" one which
 * is often used for client DLLs to display player characters with the right model (else they
 * would all have the dull "models/player.mdl" one). The entity for which the keyvalue data
 * pointer is requested is pentKeyvalue, the pointer to the keyvalue data structure pkvd.
 */
void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchKeyValue(): %s %s=%s\n",
      STRING(pentKeyvalue->v.classname), pkvd->szKeyName, pkvd->szValue);

   if (g_General.m_iModid == MOD_GEARBOX) {
      if ((strcmp(pkvd->szKeyName, "classname") == 0) &&
         (strcmp(pkvd->szValue, "info_ctfdetect") == 0))
         is_opforctf = true; // we're running the OpFor CTF
   }

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnKeyValue)(pentKeyvalue, pkvd);
}

/**
 * This function is probably called in single-player games when the player wants to save its
 * game, although I've not had a chance to verify that. It looks explicit, though.
 */
void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchSave(): %s", STRING(pent->v.classname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSave)(pent, pSaveData);
}

/**
 * This function is probably called in single-player games when the player wants to restore
 * a saved game, although I've not had a chance to verify that. It looks a little less
 * explicit than the former one - I have no idea of the purpose of "globalEntity".
 */
int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchRestore(): %s", STRING(pent->v.classname));

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnRestore)(pent, pSaveData, globalEntity);
}

/**
 * This function tells the game DLL to send a network message to clients in order to update
 * the bounding box of the entity pointed to by pent, because something changed it in the
 * engine. If that wasn't done, the physics calculations would be different between the server
 * and the clients (especially the collisions). It's the same for much of the data lying in
 * any entity's entvars: if someone changes it on the server, he must be certain that the
 * clients will get notified of it somehow.
 */
void DispatchObjectCollisionBox( edict_t *pent )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "DispatchObjectCollisionBox(): %s", STRING(pent->v.classname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSetAbsBox)(pent);
}

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "SaveWriteFields()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSaveWriteFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "SaveReadFields()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSaveReadFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveGlobalState( SAVERESTOREDATA *pSaveData )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "SaveGlobalState()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSaveGlobalState)(pSaveData);
}

void RestoreGlobalState( SAVERESTOREDATA *pSaveData )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "RestoreGlobalState()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnRestoreGlobalState)(pSaveData);
}

void ResetGlobalState( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ResetGlobalState()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnResetGlobalState)();
}

BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]  )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ClientConnect(): %d, %s, %s",
      ENTINDEX(pEntity), pszName, pszAddress);

   // check if this client is the listen server client
   if (strcmp(pszAddress, "loopback") == 0)
      listenserver_edict = pEntity; // save the edict of the listen server client...

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnClientConnect)(pEntity, pszName, pszAddress, szRejectReason);
}

void ClientDisconnect( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ClientConnect(): %d", ENTINDEX(pEntity));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnClientDisconnect)(pEntity);
}

void ClientKill( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ClientKill(): %s", STRING(pEntity->v.netname));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnClientKill)(pEntity);
}

void ClientPutInServer( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ClientPutInServer(): %d", ENTINDEX(pEntity));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnClientPutInServer)(pEntity);
}

void ClientCommand( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ClientCommand(): %d(%s), %s %s", ENTINDEX(pEntity), STRING(pEntity->v.netname), g_General.GetArg(0), g_General.GetArgs());

   BotClientCommand(CClient::Instance(pEntity)); // let the bot handle this command

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnClientCommand)(pEntity);
}

void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ClientUserInfoChanged(): %d, %s", ENTINDEX(pEntity), infobuffer);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnClientUserInfoChanged)(pEntity, infobuffer);
}

void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ServerActivate()");

   assert(g_pServer == NULL); // just to make sure

   // allocate memory for the server, also needs to be MOD specific...
   switch (g_General.m_iModid) {
   case MOD_CSTRIKE:
      g_pServer = new CServerCS; // Counter-Strike
      break;

   case MOD_VALVE:
      g_pServer = new CServerHL; // Half-Life Deathmatch
      break;

   case MOD_GEARBOX:
      // Opposing Force
      if (is_opforctf) {
         g_pServer = new CServerOP4CTF; // Capture the flag
      } else {
         g_pServer = new CServerOP4; // Deathmatch
      }
      break;

   case MOD_RICOCHET:
      g_pServer = new CServerRC; // Ricochet
      break;

   default:
      // something really messed up
      g_General.TerminateOnError("ServerActivate(): Unsupported MOD !");
      break;
   }

   SERVER_COMMAND(va("exec %s\n", g_General.BuildFileName("bot.cfg"))); // also execute the bot configuration script

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnServerActivate)(pEdictList, edictCount, clientMax);
}

void ServerDeactivate( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ServerDeactivate()");

   assert(g_pServer != NULL); // just to make sure
   delete g_pServer; // deactivate the server and free the memory
   g_pServer = NULL;

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnServerDeactivate)();
}

void PlayerPreThink( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "PlayerPreThink(): %d", ENTINDEX(pEntity));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnPlayerPreThink)(pEntity);
}

void PlayerPostThink( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "PlayerPostThink(): %d", ENTINDEX(pEntity));

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnPlayerPostThink)(pEntity);
}

void StartFrame( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "StartFrame()");

   if (!IS_DEDICATED_SERVER()) {
      if (listenserver_edict != NULL && !welcome_sent && welcome_time < 1.0) {
         if (listenserver_edict->v.deadflag == DEAD_NO && listenserver_edict->v.health > 0 &&
            !(listenserver_edict->v.flags & FL_NOTARGET))
            welcome_time = gpGlobals->time + 5.0;  // welcome in 5 seconds
      }

      if (welcome_time > 0.0 && welcome_time < gpGlobals->time && !welcome_sent) {
         // send the welcome message to this client
         MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, listenserver_edict);
            WRITE_BYTE(TE_TEXTMESSAGE);
            WRITE_BYTE(1);
            WRITE_SHORT(-1);
            WRITE_SHORT(0);
            WRITE_BYTE(0);
            WRITE_BYTE(RANDOM_LONG(10, 255));
            WRITE_BYTE(RANDOM_LONG(10, 255));
            WRITE_BYTE(RANDOM_LONG(10, 255));
            WRITE_BYTE(1);
            WRITE_BYTE(RANDOM_LONG(10, 255));
            WRITE_BYTE(RANDOM_LONG(10, 255));
            WRITE_BYTE(RANDOM_LONG(10, 255));
            WRITE_BYTE(1);
            WRITE_SHORT(256);
            WRITE_SHORT(512);
            WRITE_SHORT(512);
            WRITE_STRING(welcome_msg);
         MESSAGE_END();

         SERVER_COMMAND("speak barney/guyresponsible\n");

         welcome_sent = true;  // clear this so we only do it once
      }
   }

   if (g_pServer)
      g_pServer->Think(); // make the server 'think'

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnStartFrame)();
}

void ParmsNewLevel( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ParmsNewLevel()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnParmsNewLevel)();
}

void ParmsChangeLevel( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "ParmsChangeLevel()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnParmsChangeLevel)();
}

const char *GetGameDescription( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "GetGameDescription()");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnGetGameDescription)();
}

void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "PlayerCustomization()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnPlayerCustomization)(pEntity, pCust);
}

void SpectatorConnect( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "SpectatorConnect()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSpectatorConnect)(pEntity);
}

void SpectatorDisconnect( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "SpectatorDisconnect()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSpectatorDisconnect)(pEntity);
}

void SpectatorThink( edict_t *pEntity )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "SpectatorThink()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSpectatorThink)(pEntity);
}

void Sys_Error( const char *error_string )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "Sys_Error(): %s", error_string);

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSys_Error)(error_string);
}

void PM_Move( struct playermove_s *ppmove, int server )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnPM_Move)(ppmove, server);
}

void PM_Init( struct playermove_s *ppmove )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnPM_Init)(ppmove);
}

char PM_FindTextureType( char *name )
{
   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnPM_FindTextureType)(name);
}

void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnSetupVisibility)(pViewEntity, pClient, pvs, pas);
}

void UpdateClientData( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnUpdateClientData)(ent, sendweapons, cd);
}

int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnAddToFullPack)(state, e, ent, host, hostflags, player, pSet);
}

void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnCreateBaseline)(player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs);
}

void RegisterEncoders( void )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnRegisterEncoders)();
}

int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnGetWeaponData)(player, info);
}

void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnCmdStart)(player, cmd, random_seed);
}

void CmdEnd( const edict_t *player )
{
   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnCmdEnd)(player);
}

int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnConnectionlessPacket)(net_from, args, response_buffer, response_buffer_size);
}

int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "GetHullBounds(): %d", hullnumber);

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnGetHullBounds)(hullnumber, mins, maxs);
}

void CreateInstancedBaselines( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "CreateInstancedBaselines()");

   if (g_fIsMetamod)
      RETURN_META(MRES_IGNORED);

   (*other_gFunctionTable.pfnCreateInstancedBaselines)();
}

int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "InconsistentFile()");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnInconsistentFile)(player, filename, disconnect_message);
}

int AllowLagCompensation( void )
{
   g_Debug.DebugLog(DEBUG_DLLAPI, "AllowLagCompensation()");

   if (g_fIsMetamod)
      RETURN_META_VALUE(MRES_IGNORED, 0);

   return (*other_gFunctionTable.pfnAllowLagCompensation)();
}

gamedll_funcs_t gGameDLLFunc;

C_DLLEXPORT int GetEntityAPI_Post( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
   memset( pFunctionTable, 0, sizeof( DLL_FUNCTIONS ) );

   pFunctionTable->pfnSpawn = DispatchSpawn_Post;
   return (TRUE);
}

/**
 * This function is called right after GiveFnptrsToDll() by the engine in the game DLL (or
 * what it BELIEVES to be the game DLL), in order to copy the list of MOD functions that can
 * be called by the engine, into a memory block pointed to by the pFunctionTable pointer
 * that is passed into this function (explanation comes straight from botman). This allows
 * the Half-Life engine to call these MOD DLL functions when it needs to spawn an entity,
 * connect or disconnect a player, call Think() functions, Touch() functions, or Use()
 * functions, etc. The bot DLL passes its OWN list of these functions back to the Half-Life
 * engine, and then calls the MOD DLL's version of GetEntityAPI to get the REAL gamedll
 * functions this time (to use in the bot code).
 */
C_DLLEXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
   memset(pFunctionTable, 0, sizeof(DLL_FUNCTIONS));

   if (!g_fIsMetamod) {
      // pass other DLLs engine callbacks to function table...
      if (!(*other_GetEntityAPI)(&other_gFunctionTable, INTERFACE_VERSION))
         return FALSE; // error initializing function table!!!

      gGameDLLFunc.dllapi_table = &other_gFunctionTable;
      gpGamedllFuncs = &gGameDLLFunc;

      memcpy(pFunctionTable, &other_gFunctionTable, sizeof(DLL_FUNCTIONS));
   }

   pFunctionTable->pfnGameInit = GameDLLInit;
   pFunctionTable->pfnSpawn = DispatchSpawn;
   pFunctionTable->pfnThink = DispatchThink;
   pFunctionTable->pfnUse = DispatchUse;
   pFunctionTable->pfnTouch = DispatchTouch;
   pFunctionTable->pfnBlocked = DispatchBlocked;
   pFunctionTable->pfnKeyValue = DispatchKeyValue;
   pFunctionTable->pfnSave = DispatchSave;
   pFunctionTable->pfnRestore = DispatchRestore;
   pFunctionTable->pfnSetAbsBox = DispatchObjectCollisionBox;
   pFunctionTable->pfnSaveWriteFields = SaveWriteFields;
   pFunctionTable->pfnSaveReadFields = SaveReadFields;
   pFunctionTable->pfnSaveGlobalState = SaveGlobalState;
   pFunctionTable->pfnRestoreGlobalState = RestoreGlobalState;
   pFunctionTable->pfnResetGlobalState = ResetGlobalState;
   pFunctionTable->pfnClientConnect = ClientConnect;
   pFunctionTable->pfnClientDisconnect = ClientDisconnect;
   pFunctionTable->pfnClientKill = ClientKill;
   pFunctionTable->pfnClientPutInServer = ClientPutInServer;
   pFunctionTable->pfnClientCommand = ClientCommand;
   pFunctionTable->pfnClientUserInfoChanged = ClientUserInfoChanged;
   pFunctionTable->pfnServerActivate = ServerActivate;
   pFunctionTable->pfnServerDeactivate = ServerDeactivate;
   pFunctionTable->pfnPlayerPreThink = PlayerPreThink;
   pFunctionTable->pfnPlayerPostThink = PlayerPostThink;
   pFunctionTable->pfnStartFrame = StartFrame;
   pFunctionTable->pfnParmsNewLevel = ParmsNewLevel;
   pFunctionTable->pfnParmsChangeLevel = ParmsChangeLevel;
   pFunctionTable->pfnGetGameDescription = GetGameDescription;
   pFunctionTable->pfnPlayerCustomization = PlayerCustomization;
   pFunctionTable->pfnSpectatorConnect = SpectatorConnect;
   pFunctionTable->pfnSpectatorDisconnect = SpectatorDisconnect;
   pFunctionTable->pfnSpectatorThink = SpectatorThink;
   pFunctionTable->pfnSys_Error = Sys_Error;
   pFunctionTable->pfnPM_Move = PM_Move;
   pFunctionTable->pfnPM_Init = PM_Init;
   pFunctionTable->pfnPM_FindTextureType = PM_FindTextureType;
   pFunctionTable->pfnSetupVisibility = SetupVisibility;
   pFunctionTable->pfnUpdateClientData = UpdateClientData;
   pFunctionTable->pfnAddToFullPack = AddToFullPack;
   pFunctionTable->pfnCreateBaseline = CreateBaseline;
   pFunctionTable->pfnRegisterEncoders = RegisterEncoders;
   pFunctionTable->pfnGetWeaponData = GetWeaponData;
   pFunctionTable->pfnCmdStart = CmdStart;
   pFunctionTable->pfnCmdEnd = CmdEnd;
   pFunctionTable->pfnConnectionlessPacket = ConnectionlessPacket;
   pFunctionTable->pfnGetHullBounds = GetHullBounds;
   pFunctionTable->pfnCreateInstancedBaselines = CreateInstancedBaselines;
   pFunctionTable->pfnInconsistentFile = InconsistentFile;
   pFunctionTable->pfnAllowLagCompensation = AllowLagCompensation;

   return TRUE;
}

/**
 * It appears that an extra function table has been added in the engine to gamedll interface
 * since the date where the first enginefuncs table standard was frozen. These ones are
 * facultative and we don't hook them, but since some MODs might be featuring it, we have to
 * pass them too, else the DLL interfacing wouldn't be complete and the game possibly wouldn't
 * run properly.
 */
C_DLLEXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
{
   if (other_GetNewDLLFunctions == NULL) // have we NOT found it?
      return FALSE; // give up

   gGameDLLFunc.newapi_table = pFunctionTable;

   return (*other_GetNewDLLFunctions)(pFunctionTable, interfaceVersion);
}

/**
 * This function synchronizes the studio model animation blending interface (i.e, what parts
 * of the body move, which bones, which hitboxes and how) between the server and the game DLL.
 * some MODs can be using a different hitbox scheme than the standard one.
 */
C_DLLEXPORT int Server_GetBlendingInterface(int version,
   struct sv_blending_interface_s **ppinterface,
   struct engine_studio_api_s *pstudio,
   float (*rotationmatrix)[3][4],
   float (*bonetransform)[MAXSTUDIOBONES][3][4])
{
   if (!other_Server_GetBlendingInterface) // have we NOT found it?
      return FALSE; // give up

   return (*other_Server_GetBlendingInterface)(version, ppinterface, pstudio, rotationmatrix, bonetransform);
}

