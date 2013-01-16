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
// world.cpp
//

#include "main.h"

CWorld::CWorld():
m_pNodes(NULL),
m_pLinks(NULL),
m_pVis(NULL),
m_iNumNodes(0),
m_iLastCoverSearch(0)
{
   MapInit();

   WaypointLoad(); // load waypoints at map start
}

CWorld::~CWorld()
{
   FreeAllTheStuff();
}

// initialize the navigation map.
void CWorld::MapInit()
{
   m_iLastCoverSearch = 0;

   FreeAllTheStuff();
}

// free all allocated memory for the navigation map.
void CWorld::FreeAllTheStuff()
{
   // free all nodes
   if (m_pNodes) {
      delete []m_pNodes;
      m_pNodes = NULL;
   }

   // free all links
   if (m_pLinks) {
      delete []m_pLinks;
      m_pLinks = NULL;
   }

   // free all visibilities
   if (m_pVis) {
      delete []m_pVis;
      m_pVis = NULL;
   }

   m_iNumNodes = 0;
   m_iNumLinks = 0;
   m_iNumVis = 0;
}

// initialize the nearest nodes lookup table for each nodes
void CWorld::InitNearNodes()
{
	// TODO
}

// find a path from the source position to the dest position.
int CWorld::FindPath(const Vector &vecSrc, const Vector &vecDest, std::list<CWaypoint> *pWPList,
                     int iFlags, float flDangerFactor)
{
   return NO_PATH; // TODO
}

// get the danger value of the specified position.
int CWorld::GetDanger(const Vector &vecPos)
{
   return 0; // TODO
}

// get the estimated path distance from the source to the destination.
long CWorld::PathDistance(const Vector &vecSrc, const Vector &vecDest)
{
   return 0; // TODO
}

// find a position to take cover from the specified position.
Vector CWorld::FindCover(const Vector &vecSrc, const Vector &vecThreat,
                         const Vector &vecViewOffset, int iTeam,
                         float flMinDist, float flMaxDist)
{
   if (!IsAvailable())
      return NULLVEC; // reliability check

   // TODO
/*
   Vector vecLookersOffset = vecThreat + vecViewOffset; // calculate location of enemy's eyes
   if (flMinDist > flMaxDist / 2) {
      flMinDist = flMaxDist / 2; // MinDist too close to MaxDist
   }

   // we'll do a rough sample to find nodes that are relatively nearby
   for (int i = 0; i < m_iNumNodes; i++) {
      int iIndex = (i + m_iLastCoverSearch) % m_iNumNodes;
      m_iLastCoverSearch = iIndex + 1; // next search for cover node will start where we left off here

      PATH *p = &m_rgNodes[iIndex];
      Vector vecTarget = p->origin;

      float flDist = (vecSrc - vecTarget).Length();
      if (flDist < flMinDist || flDist > flMaxDist) {
         continue; // distance invalid; skip this point
      }

      bool fValid = true;
      if (iTeam != -1) {
         for (int j = 0; j < g_pServer->GetMaxClients(); j++) {
            CClient *pClient = g_pServer->m_rgpClients[j];
            if (pClient && pClient->IsValid() && pClient->IsAlive() && pClient->GetTeam() == iTeam) {
               if ((pClient->GetOrigin() - vecSrc).Length() < 128)
                  fValid = false;
            }
         }
      }

      if (!fValid) {
         continue; // too many teammates there; skip this point
      }

      // check if this waypoint blocks the line of sight of the threat
      if (TestLine(vecTarget, vecLookersOffset, true, NULL).fraction >= 1.0) {
         continue; // enemy can see this position; not a good place
      }

      if (PathDistance(vecSrc, vecTarget) <= PathDistance(vecThreat, vecTarget)) {
         return vecTarget; // this is a good place to take cover
      }
   }
*/
   return NULLVEC; // no good place to take cover
}

// load waypoints
void CWorld::WaypointLoad()
{
   int i, j;

   MapInit();

   CIniFile ini(g_General.BuildFileName(va("maps/%s.ini", g_pServer->GetMapName())));

   if (!ini.Valid()) {
      return; // cannot load waypoint ini file
   }

   // get number of nodes, links and visibilities
   m_iNumNodes = atoi(ini.Get("INFO", "NodeCount"));
   m_iNumLinks = atoi(ini.Get("INFO", "LinkCount"));
   m_iNumVis = atoi(ini.Get("INFO", "VisCount"));

   if (m_iNumNodes <= 0 || m_iNumLinks <= 0 || m_iNumVis <= 0) {
      printf("ERROR: Invalid waypoint file %s.ini!\n", g_pServer->GetMapName());
      MapInit();
      return;
   }

   m_pNodes = new CNode[m_iNumNodes];
   m_pLinks = new CLink[m_iNumLinks];
   m_pVis = new CVisibility[m_iNumVis];
   if (m_pNodes == NULL || m_pLinks == NULL || m_pVis == NULL) {
      CGeneral::TerminateOnError("Memory allocation error !");
   }

   int iCurrLinkIndex = 0, iCurrVisIndex = 0;

   // load all the nodes
   for (i = 0; i < m_iNumNodes; i++) {
      m_pNodes[i].m_vecOrigin.x = atof(ini.Get(va("NODE%d", i + 1), "OriginX"));
      m_pNodes[i].m_vecOrigin.y = atof(ini.Get(va(NULL), "OriginY"));
      m_pNodes[i].m_vecOrigin.z = atof(ini.Get(va(NULL), "OriginZ"));
      m_pNodes[i].m_iFlags = atoi(ini.Get(va(NULL), "Flags"));

      // FIXME: not all the engines are like this
      if (m_pNodes[i].m_iFlags & TFL_CROUCH) {
         // crouching player is 52 units high
         m_pNodes[i].m_vecOriginPeak = m_pNodes[i].m_vecOrigin + Vector(0, 0, 52);
      } else {
         // standing player is 74 units high
         m_pNodes[i].m_vecOriginPeak = m_pNodes[i].m_vecOrigin + Vector(0, 0, 74);
      }

      // load links from this node
      m_pNodes[i].m_iLinkCount = atoi(ini.Get(va(NULL), "LinkCount"));
      m_pNodes[i].m_iFirstLink = iCurrLinkIndex;
      for (j = 0; j < m_pNodes[i].m_iLinkCount; j++) {
         if (iCurrLinkIndex >= m_iNumLinks) {
            // reliability check
            printf("ERROR: Invalid waypoint file %s.ini!\n", g_pServer->GetMapName());
            MapInit();
            return;
         }

         m_pLinks[iCurrLinkIndex].m_iSrcNode = i;
         m_pLinks[iCurrLinkIndex].m_iDestNode = atoi(ini.Get(va("LINK%d", iCurrLinkIndex + 1), "DestNode")) - 1;
         m_pLinks[iCurrLinkIndex].m_iTravelFlags  = atoi(ini.Get(va(NULL), "TravelFlags"));

         // check if anything is blocking this link
         m_pLinks[iCurrLinkIndex].UpdateLinkEnt();

         iCurrLinkIndex++;
      }

      // load visibilities from this node
      m_pNodes[i].m_iVisCount = atoi(ini.Get(va("NODE%d", i), "VisCount"));
      m_pNodes[i].m_iFirstVis = iCurrVisIndex;
      for (j = 0; j < m_pNodes[i].m_iVisCount; j++) {
         if (iCurrVisIndex >= m_iNumVis) {
            // reliability check
            printf("ERROR: Invalid waypoint file %s.ini!\n", g_pServer->GetMapName());
            MapInit();
            return;
         }

         m_pVis[iCurrVisIndex].m_iSrcNode = i;
         m_pVis[iCurrVisIndex].m_iDestNode = atoi(ini.Get(va("VIS%d", iCurrVisIndex), "DestNode")) - 1;
         m_pVis[iCurrVisIndex].m_iVisFlags  = atoi(ini.Get(va(NULL), "VisFlags"));

         iCurrVisIndex++;
      }
   }

   InitNearNodes(); // initialize the nearest nodes lookup table
}

// save waypoints
void CWorld::WaypointSave()
{
   if (m_pNodes == NULL || m_iNumNodes <= 0) {
      printf("ERROR: No nodes to save\n");
      return;
   }

   CIniFile ini;
   int i;

   // get the basic info of waypoints
   ini.Set("INFO", "NodeCount", va("%d", m_iNumNodes));
   ini.Set("INFO", "LinkCount", va("%d", m_iNumLinks));
   ini.Set("INFO", "VisCount", va("%d", m_iNumVis));

   // get the info of all nodes
   for (i = 0; i < m_iNumNodes; i++) {
      ini.Set(va("NODE%d", i + 1), "OriginX", va("%f", m_pNodes[i].m_vecOrigin.x));
      ini.Set(va("NODE%d", i + 1), "OriginY", va("%f", m_pNodes[i].m_vecOrigin.y));
      ini.Set(va("NODE%d", i + 1), "OriginZ", va("%f", m_pNodes[i].m_vecOrigin.z));

      ini.Set(va("NODE%d", i + 1), "Flags", va("%d", m_pNodes[i].m_iFlags));
      ini.Set(va("NODE%d", i + 1), "LinkCount", va("%d", m_pNodes[i].m_iLinkCount));
      ini.Set(va("NODE%d", i + 1), "VisCount", va("%d", m_pNodes[i].m_iVisCount));
   }

   // get the info of all links
   for (i = 0; i < m_iNumLinks; i++) {
      ini.Set(va("LINK%d", i + 1), "DestNode", va("%d", m_pLinks[i].m_iDestNode + 1));
      ini.Set(va("LINK%d", i + 1), "TravelFlags", va("%d", m_pLinks[i].m_iTravelFlags));
   }

   // get the info of all visibilities
   for (i = 0; i < m_iNumVis; i++) {
      ini.Set(va("VIS%d", i + 1), "DestNode", va("%d", m_pVis[i].m_iDestNode + 1));
      ini.Set(va("VIS%d", i + 1), "VisFlags", va("%d", m_pVis[i].m_iVisFlags));
   }

   // write out the ini file
   ini.Save(g_General.BuildFileName(va("maps/%s.ini", g_pServer->GetMapName())));

   printf("Waypoints saved to maps\\%s.ini\n", g_pServer->GetMapName());
}

CWorld::CNode *CWorld::FindNode(const Vector &vecPos, int radius)
{
   return NULL; // TODO
}

// find the nearest node to the vecPos
CWorld::CNode *CWorld::FindNearestNode(const Vector &vecPos)
{
   return NULL; // TODO
}

// Called each frame
void CWorld::Think()
{
}

// get the entity which blocks this link
void CWorld::CLink::UpdateLinkEnt()
{
   traceresult_t tr;

   // use a traceline to find the entity which blocks this link
   tr = TestLine(g_pServer->World()->m_pNodes[m_iSrcNode].MidPos(),
                 g_pServer->World()->m_pNodes[m_iDestNode].MidPos(),
                 false, NULL);

   m_pLinkEnt = tr.ent; // store away the entity
}

