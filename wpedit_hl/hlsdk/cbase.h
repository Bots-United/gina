/***
*
*   Copyright (c) 1999, Valve LLC. All rights reserved.
*
*   This product contains software technology licensed from Id 
*   Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*   All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
extern "C" EXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion );
extern "C" EXPORT int GetEntityAPI2( DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion );
extern "C" EXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion );

extern int DispatchSpawn( edict_t *pent );
extern void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd );
extern void DispatchTouch( edict_t *pentTouched, edict_t *pentOther );
extern void DispatchUse( edict_t *pentUsed, edict_t *pentOther );
extern void DispatchThink( edict_t *pent );
extern void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther );
extern void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData );
extern int  DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
extern void DispatchObjectCollsionBox( edict_t *pent );
extern void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveGlobalState( SAVERESTOREDATA *pSaveData );
extern void RestoreGlobalState( SAVERESTOREDATA *pSaveData );
extern void ResetGlobalState( void );

// instant damage

#define DMG_GENERIC      0            // generic damage was done
#define DMG_CRUSH        (1 << 0)    // crushed by falling or moving object
#define DMG_BULLET       (1 << 1)    // shot
#define DMG_SLASH        (1 << 2)    // cut, clawed, stabbed
#define DMG_BURN         (1 << 3)    // heat burned
#define DMG_FREEZE       (1 << 4)    // frozen
#define DMG_FALL         (1 << 5)    // fell too far
#define DMG_BLAST        (1 << 6)    // explosive blast damage
#define DMG_CLUB         (1 << 7)    // crowbar, punch, headbutt
#define DMG_SHOCK        (1 << 8)    // electric shock
#define DMG_SONIC        (1 << 9)    // sound pulse shockwave
#define DMG_ENERGYBEAM   (1 << 10)    // laser or other high energy beam 
#define DMG_NEVERGIB     (1 << 12)    // with this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB    (1 << 13)    // with this bit OR'd in, any damage type can be made to gib victims upon death.
#define DMG_DROWN        (1 << 14)    // Drowning
// time-based damage
#define DMG_TIMEBASED    (~(0x3fff))    // mask for time-based damage

#define DMG_PARALYZE     (1 << 15)    // slows affected creature down
#define DMG_NERVEGAS     (1 << 16)    // nerve toxins, very bad
#define DMG_POISON       (1 << 17)    // blood poisioning
#define DMG_RADIATION    (1 << 18)    // radiation exposure
#define DMG_DROWNRECOVER (1 << 19)    // drowning recovery
#define DMG_ACID         (1 << 20)    // toxic chemicals or acid burns
#define DMG_SLOWBURN     (1 << 21)    // in an oven
#define DMG_SLOWFREEZE   (1 << 22)    // in a subzero freezer
#define DMG_MORTAR       (1 << 23)    // Hit by air raid (done to distinguish grenade from mortar)

// these are the damage types that are allowed to gib corpses
#define DMG_GIB_CORPSE   ( DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB )

// these are the damage types that have client hud art
#define DMG_SHOWNHUD     (DMG_POISON | DMG_ACID | DMG_FREEZE | DMG_SLOWFREEZE | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK)

#define    BAD_WEAPON 0x00007FFF
