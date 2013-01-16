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
// world.h
//

#ifndef WORLD_H
#define WORLD_H

#include "bot_navigate.h"
#include "entity.h"

#define PATH_FINDING      -1
#define NO_PATH           -2

/*
 *     y (north)
 *     ^
 *     |
 * ----+----> x (east)
 *     |
 *     |
 */
enum { NODE_UP = 0, NODE_DOWN };
enum { NODE_NORTH = 0, NODE_SOUTH };
enum { NODE_EAST = 0, NODE_WEST };

class CWorld
{
public:
   CWorld();
   virtual ~CWorld();

   // Navigation node
   class CNode
   {
   public:
      CNode() : m_vecOrigin(0, 0, 0), m_vecOriginPeak(0, 0, 0),
                m_iFlags(0), m_iLinkCount(0), m_iFirstLink(-1),
                m_iVisCount(0), m_iFirstVis(-1) {}

      Vector        m_vecOrigin;     // origin of this node
      Vector        m_vecOriginPeak; // origin of top of this node
      unsigned int  m_iFlags;        // flags of this node

      int           m_iLinkCount;    // how many links this node has
      int           m_iFirstLink;    // index of the first link

      int           m_iVisCount;     // how many visibilities this node has
      int           m_iFirstVis;     // index of the first visibility item

      inline int    Index(void);

      inline Vector MidPos(void) { return (m_vecOrigin + m_vecOriginPeak) / 2; }
   };

   // a link between nodes
   class CLink
   {
   public:
      CLink() : m_iTravelFlags(0), m_iSrcNode(-1), m_iDestNode(-1),
                m_pLinkEnt(NULL), m_flWeight(0) {}

      unsigned int  m_iTravelFlags; // travel flags of this link
      int           m_iSrcNode, m_iDestNode; // source & destination node

      CEntity      *m_pLinkEnt; // entity which blocks this link
      float         m_flWeight; // weight (length) of the link

      void          UpdateLinkEnt(void); // get the entity which blocks this link
   };

   // visibility flags
   enum tagVisFlags {
      VISIBILITY_TOP             = (1 << 0),
      VISIBILITY_SHOULDER        = (1 << 1),
      VISIBILITY_WAIST           = (1 << 2),
      VISIBILITY_BOTTOM          = (1 << 3),
   };

   // visibility between nodes
   class CVisibility
   {
   public:
      CVisibility() : m_iVisFlags(0), m_iSrcNode(-1), m_iDestNode(-1) {}

      int           m_iVisFlags; // visibility flags
      int           m_iSrcNode, m_iDestNode; // source & destination node
   };

   void           MapInit(void);
   void           FreeAllTheStuff(void);

   void           InitNearNodes(void);

   int            FindPath(const Vector &vecSrc, const Vector &vecDest, std::list<CWaypoint> *pWPList = NULL, int iFlags = 0x7fffffff, float flDangerFactor = 0);
   Vector         FindCover(const Vector &vecSrc, const Vector &vecThreat, const Vector &vecViewOffset = NULLVEC, int iTeam = -1, float flMinDist = 0, float flMaxDist = 784);
   long           PathDistance(const Vector &vecSrc, const Vector &vecDest);
   int            GetDanger(const Vector &vecPos);

   bool           IsAvailable(void) { return m_iNumNodes > 0; }

   void           WaypointLoad(void);
   void           WaypointSave(void);
   CNode         *FindNode(const Vector &vecPos, int radius = 200);
   CNode         *FindNearestNode(const Vector &vecPos);

   int            m_iNumNodes;      // total number of nodes
   CNode         *m_pNodes;         // navigation nodes
   int            m_iNumLinks;      // total number of links
   CLink         *m_pLinks;         // links between nodes
   int            m_iNumVis;        // total number of visibilities
   CVisibility   *m_pVis;           // visibility table

   int            m_iLastCoverSearch;

   void           Think(void);      // called each frame
};

int CWorld::CNode::Index()
{
   return (int)(this - g_pServer->World()->m_pNodes);
}

#endif

