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
#ifndef SDKUTIL_H
#define SDKUTIL_H

#ifndef ENGINECALLBACK_H
#include "enginecallback.h"
#endif

extern globalvars_t                *gpGlobals;

#define DLL_GLOBAL

#define STRING(offset)        ((const char *)gpGlobals->pStringBase + (int)offset)
#define MAKE_STRING(str)    ((int)str - (int)STRING(0))

inline edict_t *FIND_ENTITY_BY_CLASSNAME(edict_t *entStart, const char *pszName) 
{
    return FIND_ENTITY_BY_STRING(entStart, "classname", pszName);
}

inline edict_t *FIND_ENTITY_BY_TARGETNAME(edict_t *entStart, const char *pszName) 
{
    return FIND_ENTITY_BY_STRING(entStart, "targetname", pszName);
}

// for doing a reverse lookup. Say you have a door, and want to find its button.
inline edict_t *FIND_ENTITY_BY_TARGET(edict_t *entStart, const char *pszName) 
{
    return FIND_ENTITY_BY_STRING(entStart, "target", pszName);
}

// Keeps clutter down a bit, when writing key-value pairs
#define WRITEKEY_INT(pf, szKeyName, iKeyValue) ENGINE_FPRINTF(pf, "\"%s\" \"%d\"\n", szKeyName, iKeyValue)
#define WRITEKEY_FLOAT(pf, szKeyName, flKeyValue)                                \
        ENGINE_FPRINTF(pf, "\"%s\" \"%f\"\n", szKeyName, flKeyValue)
#define WRITEKEY_STRING(pf, szKeyName, szKeyValue)                                \
        ENGINE_FPRINTF(pf, "\"%s\" \"%s\"\n", szKeyName, szKeyValue)
#define WRITEKEY_VECTOR(pf, szKeyName, flX, flY, flZ)                            \
        ENGINE_FPRINTF(pf, "\"%s\" \"%f %f %f\"\n", szKeyName, flX, flY, flZ)

// More explicit than "int"
typedef int EOFFSET;

// In case this ever changes
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//
// Conversion among the three types of "entity", including identity-conversions.
//
inline edict_t *ENT(const entvars_t *pev)    { return pev->pContainingEntity; }
inline edict_t *ENT(edict_t *pent)               { return pent; }
inline edict_t *ENT(EOFFSET eoffset)             { return (*g_engfuncs.pfnPEntityOfEntOffset)(eoffset); }
inline EOFFSET OFFSET(EOFFSET eoffset)           { return eoffset; }
inline EOFFSET OFFSET(const edict_t *pent)    
{ 
#if _DEBUG
    if ( !pent )
        ALERT( at_error, "Bad ent in OFFSET()\n" );
#endif
    return (*g_engfuncs.pfnEntOffsetOfPEntity)(pent); 
}
inline EOFFSET OFFSET(entvars_t *pev)                
{ 
#if _DEBUG
    if ( !pev )
        ALERT( at_error, "Bad pev in OFFSET()\n" );
#endif
    return OFFSET(ENT(pev)); 
}
inline entvars_t *VARS(entvars_t *pev)                    { return pev; }

inline entvars_t *VARS(edict_t *pent)            
{ 
    if ( !pent )
        return NULL;

    return &pent->v; 
}

inline entvars_t* VARS(EOFFSET eoffset)              { return VARS(ENT(eoffset)); }
inline int      ENTINDEX(edict_t *pEdict)            { return (*g_engfuncs.pfnIndexOfEdict)(pEdict); }
inline edict_t* INDEXENT( int iEdictNum )            { return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum); }
inline void MESSAGE_BEGIN( int msg_dest, int msg_type, const float *pOrigin, entvars_t *ent ) {
    (*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ENT(ent));
}

// Testing the three types of "entity" for nullity
#define eoNullEntity 0
inline BOOL FNullEnt(EOFFSET eoffset)            { return eoffset == 0; }
inline BOOL FNullEnt(const edict_t* pent)        { return pent == NULL || FNullEnt(OFFSET(pent)); }
inline BOOL FNullEnt(entvars_t* pev)             { return pev == NULL || FNullEnt(OFFSET(pev)); }

// Testing strings for nullity
#define iStringNull 0
inline BOOL FStringNull(int iString)             { return iString == iStringNull; }

#define cchMapNameMost 32

// Dot products for view cone checking
#define VIEW_FIELD_FULL             (float)-1.0 // +-180 degrees
#define    VIEW_FIELD_WIDE          (float)-0.7 // +-135 degrees 0.1 // +-85 degrees, used for full FOV checks 
#define    VIEW_FIELD_NARROW        (float)0.7 // +-45 degrees, more narrow check used to set up ranged attacks
#define    VIEW_FIELD_ULTRA_NARROW  (float)0.9 // +-25 degrees, more narrow check used to set up ranged attacks

// Misc useful
inline BOOL FStrEq(const char*sz1, const char*sz2)
    { return (strcmp(sz1, sz2) == 0); }
inline BOOL FClassnameIs(edict_t* pent, const char* szClassname)
    { return FStrEq(STRING(VARS(pent)->classname), szClassname); }
inline BOOL FClassnameIs(entvars_t* pev, const char* szClassname)
    { return FStrEq(STRING(pev->classname), szClassname); }

#define UTIL_EntitiesInPVS(pent)           (*g_engfuncs.pfnEntitiesInPVS)(pent)

typedef enum { ignore_monsters=1, dont_ignore_monsters=0, missile=2 } IGNORE_MONSTERS;
typedef enum { ignore_glass=1, dont_ignore_glass=0 } IGNORE_GLASS;
typedef enum { point_hull=0, human_hull=1, large_hull=2, head_hull=3 } HULL;

// prints messages through the HUD
extern void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );

typedef struct hudtextparms_s
{
    float x;
    float y;
    int   effect;
    byte  r1, g1, b1, a1;
    byte  r2, g2, b2, a2;
    float fadeinTime;
    float fadeoutTime;
    float holdTime;
    float fxTime;
    int   channel;
} hudtextparms_t;

// func_rotating
#define SF_BRUSH_ROTATE_Y_AXIS      0
#define SF_BRUSH_ROTATE_INSTANT     1
#define SF_BRUSH_ROTATE_BACKWARDS   2
#define SF_BRUSH_ROTATE_Z_AXIS      4
#define SF_BRUSH_ROTATE_X_AXIS      8
#define SF_PENDULUM_AUTO_RETURN     16
#define SF_PENDULUM_PASSABLE        32

#define SF_BRUSH_ROTATE_SMALLRADIUS  128
#define SF_BRUSH_ROTATE_MEDIUMRADIUS 256
#define SF_BRUSH_ROTATE_LARGERADIUS  512

#define PUSH_BLOCK_ONLY_X    1
#define PUSH_BLOCK_ONLY_Y    2

#define VEC_HULL_MIN         Vector(-16, -16, -36)
#define VEC_HULL_MAX         Vector( 16,  16,  36)
#define VEC_HUMAN_HULL_MIN   Vector( -16, -16, 0 )
#define VEC_HUMAN_HULL_MAX   Vector( 16, 16, 72 )
#define VEC_HUMAN_HULL_DUCK  Vector( 16, 16, 36 )

#define VEC_VIEW             Vector( 0, 0, 28 )

#define VEC_DUCK_HULL_MIN    Vector(-16, -16, -18 )
#define VEC_DUCK_HULL_MAX    Vector( 16,  16,  18)
#define VEC_DUCK_VIEW        Vector( 0, 0, 12 )

#define SVC_TEMPENTITY      23
#define SVC_INTERMISSION    30
#define SVC_CDTRACK         32
#define SVC_WEAPONANIM      35
#define SVC_ROOMTYPE        37
#define SVC_DIRECTOR        51

// triggers
#define SF_TRIGGER_ALLOWMONSTERS    1// monsters allowed to fire this trigger
#define SF_TRIGGER_NOCLIENTS        2// players not allowed to fire this trigger
#define SF_TRIGGER_PUSHABLES        4// only pushables can fire this trigger

// func breakable
#define SF_BREAK_TRIGGER_ONLY   1// may only be broken by trigger
#define SF_BREAK_TOUCH          2// can be 'crashed through' by running player (plate glass)
#define SF_BREAK_PRESSURE       4// can be broken by a player standing on it
#define SF_BREAK_CROWBAR        256// instant break if hit with crowbar

// func_pushable (it's also func_breakable, so don't collide with those flags)
#define SF_PUSH_BREAKABLE       128

#define SF_LIGHT_START_OFF     1

#define SPAWNFLAG_NOMESSAGE    1
#define SPAWNFLAG_NOTOUCH      1
#define SPAWNFLAG_DROIDONLY    4

#define SPAWNFLAG_USEONLY   1        // can't be touched, must be used (buttons)

#define TELE_PLAYER_ONLY    1
#define TELE_SILENT         2

#define SF_TRIG_PUSH_ONCE   1

#endif

