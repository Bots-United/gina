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

/*******************************************************
 * bot_chat.cpp                                        *
 *******************************************************
 * Purpose: Bots chatting code                         *
 * Engine-specific: no                                 *
 *******************************************************/

#include "main.h"

CBotChat::CBotChat(CBaseBot *pBot):
m_pBot(pBot),
m_flChatTime(0),
m_strMsgBuffer("")
{
}

CBotChat::~CBotChat()
{
}

// Returns if the bot is at a good position to chat.
bool CBotChat::IsGoodChatPosition()
{
   if (!m_pBot->IsAlive())
      return true; // if the bot is dead all positions are valid

   if (m_pBot->IsInWater())
      return false; // do not chat if under water

   return m_pBot->IsOnFloor(); // must be standing on the ground
}

void CBotChat::ReplyToMessage(CClient *pFrom, const char *msg)
{
   if (m_flChatTime > g_pServer->GetTime())
      return; // we already have something to say

   // generate a reply for this message
   char *p = g_pServer->BotManager()->chat.GenerateReply(m_pBot, pFrom, msg);
   // if we have found a reply
   if (p) {
      PrepareMessage(p); // reply to the message
   }
}

void CBotChat::PrepareMessage(const char *msg, bool is_team_msg)
{
   if (m_flChatTime > g_pServer->GetTime())
      return; // we already have something to say

   m_strMsgBuffer = msg; // store it into our buffer

   // and delay sometime to say it based on length of the message and bot skill
   m_flChatTime = g_pServer->GetTime() + (0.005 * m_pBot->m_pProfile->skill * strlen(msg));

   m_fIsTeamMsg = is_team_msg;
}

void CBotChat::Think()
{
   if (m_flChatTime > 0 && m_flChatTime < g_pServer->GetTime()) {
      // It's the time to say the message now
      // if we are at the good position, send the message in the buffer
      if (IsGoodChatPosition()) {
         if (m_fIsTeamMsg)
            m_pBot->SayTeam(m_strMsgBuffer.c_str());
         else
            m_pBot->Say(m_strMsgBuffer.c_str());

         m_flChatTime = 0; // don't say it again
      }
   }
}

void CChatItem::AddMessage(char *sz)
{
   msg.push_back(std::string(sz)); // add it to the list
}

const char *CChatItem::PickMessage()
{
   assert(msg.size());

   // just pick a random message from the list
   return msg[RandomLong(0, msg.size() - 1)].c_str();
}

const char *CChatReply::IsValidReply(const char *sentence)
{
   char upper_sentence[256], *p = upper_sentence;
   strncpy(upper_sentence, sentence, 256);

   // convert the sentence to upper case
   while (*p) {
      *p = toupper(*p);
      p++;
   }

   // search in the keyword list
   for (int i = 0; i < key.size(); i++) {
      const char *p = strstr(upper_sentence, key[i].c_str());
      if (p) {
         return p;
      }
   }
   return NULL;
}

void CChatReply::AddKeyword(char *sz)
{
   char upper_sz[256], *p = upper_sz;
   strncpy(upper_sz, sz, 256);

   // convert the string to upper case
   while (*p) {
      *p = toupper(*p);
      p++;
   }

   trim(upper_sz); // remove the leading and trailing blanks
   key.push_back(std::string(upper_sz)); // add it to the list
}

void CChatReply::AddMessage(char *sz)
{
   msg.push_back(std::string(sz));
}

const char *CChatReply::PickMessage(void)
{
   if (msg.size() <= 0)
      return NULL; // reliability check

   // just pick a random message from the list
   return msg[RandomLong(0, msg.size() - 1)].c_str();
}

//==============================================================
// Chat Manager
//==============================================================
CChatManager::CChatManager()
{
   LoadBotChat();
}

CChatManager::~CChatManager()
{
}

// load the bot chat data file
void CChatManager::LoadBotChat()
{
   char str[256];

   FILE *fp = fopen(g_General.BuildFileName("botchat.txt"), "r");
   if (!fp) {
      printf("BOT: cannot load file botchat.txt !\n");
      return;
   }

   bool fIsReply = false, fLoadingKeywords = false;
   while (fgets(str, 256, fp)) {
      trim(str); // trim all the blanks or linefeeds

      // skip all comment lines or empty lines
      if (!str[0] || str[0] == ';' || str[0] == '/' || str[0] == '#')
         continue;

      // check if this is a section line (e.g., [SECTION])
      int length = strlen(str);

      if (str[0] == '[' && str[length - 1] == ']') {
         int i;

         char section[256];
         section[0] = '\0';

         strcpy(section, &str[1]);
         section[length - 2] = 0; // remove the ]

         trim(section);

         // convert the key name to upper case
         for (i = 0; i < length - 2; i++) {
            section[i] = toupper(section[i]);
         }

         if (strcmpi(section, "REPLIES") == 0) {
            fIsReply = true;
         } else {
            fIsReply = false;
            unsigned short usCrc = CRC_Block((unsigned char *)section, length - 2); // CRC the section name

            // add a session
            m_ChatItem.push_back(CChatItem(usCrc));
         }
      } else if (fIsReply) {
         if (strncmp(str, "@KEY ", 5) == 0) {
            // This is a keyword line. Add the keyword to our list
            if (!fLoadingKeywords) {
               m_ChatReply.push_back(CChatReply());
               fLoadingKeywords = true;
            }
            m_ChatReply[m_ChatReply.size() - 1].AddKeyword(&str[5]);
         } else if (m_ChatReply.size() > 0) {
            // This is a reply message line. Add it to our list
            m_ChatReply[m_ChatReply.size() - 1].AddMessage(str);
            fLoadingKeywords = false;
         }
      } else if (m_ChatItem.size() > 0) {
         // This is a chat message line. Add it to our list
         m_ChatItem[m_ChatItem.size() - 1].AddMessage(str);
      }
   }

   fclose(fp); // close the file
}

// this function gets called when the client "from" sends a chat message "message"
// to the client "to"
void CChatManager::ChatMessage(CClient *from, CClient *to, const char *message)
{
   if (!to->IsBot())
      return; // ignore this message if the destination is NOT a bot

   CBaseBot *pBot = (CBaseBot *)to;
   if (!pBot->Chat()->IsGoodChatPosition())
      return; // not a good chat position; don't proceed

   // only process the message randomly, don't reply to every message
   if (RandomLong(1, 100) < 50)
      pBot->Chat()->ReplyToMessage(from, message);
}

// Generate a chat message from a specified message type, and return
// the pointer of the generated message
char *CChatManager::GenerateChat(CBaseBot *pClient, const char *type)
{
   static char msg[256]; // for the chat message
   msg[0] = '\0'; // initialize the string

   unsigned short usCrc = CRC_Block((unsigned char *)type, strlen(type));

   // find if we already have this section
   int found;
   for (found = 0; found < m_ChatItem.size(); found++) {
      if (m_ChatItem[found].type == usCrc)
         break;
   }

   if (found < m_ChatItem.size()) {
      char sz[256];
      strncpy(sz, m_ChatItem[found].PickMessage(), 256);

      int length = strlen(sz);

      char *p = sz;
      while (p != NULL && *p) {
         if (*p == '%') {
            char type = *(++p); // skip to next character to get the type
            int hiscore = -9999, j, count;
            CClient *pPickedClient = NULL;
            char buffer[32];

            switch (type) {
            case '%':
               strncat(msg, "%", 256);
               break;

            case 'n': // bot name
               strncpy(buffer, pClient->GetNetName(), 32);
               HumanizeName(buffer);
               strncat(msg, buffer, 256); // append it to the message
               break;

            case 'r': // random opponent
               j = RandomLong(0, g_pServer->GetMaxClients());

               for (count = 0; count < g_pServer->GetMaxClients(); count++, j++) {
                  CClient *p = g_pServer->m_rgpClients[j];
                  if (p != pClient && p->IsValid()) {
                     strncpy(buffer, p->GetNetName(), 32);
                     HumanizeName(buffer);
                     strncat(msg, buffer, 256); // append it to the message
                     break;
                  }
               }

               if (count >= g_pServer->GetMaxClients())
                  return NULL; // can't find any player, don't say anything

               break;

            case 'f': // opponent in first place
               for (j = 0; j < g_pServer->GetMaxClients(); j++) {
                  CClient *p = g_pServer->m_rgpClients[j];
                  if (p == pClient || !p->IsValid())
                     continue;
                  if (p->GetFrags() >= hiscore) {
                     hiscore = p->GetFrags();
                     pPickedClient = p;
                  }
               }
               if (pPickedClient) {
                  strncpy(buffer, pPickedClient->GetNetName(), 32);
                  HumanizeName(buffer); // humanize the name
                  strncat(msg, buffer, 256); // append it to the message
               } else {
                  return NULL; // can't find such a player, don't say anything
               }
               break;

            case 'l': // opponent in last place
               hiscore = 9999;
               for (j = 0; j < g_pServer->GetMaxClients(); j++) {
                  CClient *p = g_pServer->m_rgpClients[j];
                  if (p == pClient || !p->IsValid())
                     continue;
                  if (p->GetFrags() <= hiscore) {
                     hiscore = p->GetFrags();
                     pPickedClient = p;
                  }
               }
               if (pPickedClient) {
                  strncpy(buffer, pPickedClient->GetNetName(), 32);
                  HumanizeName(buffer); // humanize the name
                  strncat(msg, buffer, 256); // append it to the message
               } else {
                  return NULL; // can't find such a player, don't say anything
               }
               break;

            case 'm': // map title
               strncat(msg, g_pServer->GetMapName(), 256); // get the map name
               break;

            case 'v': // last victim of this bot
               if (pClient->Chat()->m_strLastVictim.length() <= 0)
                  return NULL; // we don't have a victim yet, don't say anything
               strncat(msg, pClient->Chat()->m_strLastVictim.c_str(), 256);
               break;

            case 'k': // last player killed this bot
               if (pClient->Chat()->m_strLastKiller.length() <= 0)
                  return NULL; // this bot hasn't been killed yet, don't say anything
               strncat(msg, pClient->Chat()->m_strLastKiller.c_str(), 256);
               break;
            }

            p++; // skip to the next character
            continue;
         }

         char *prev = p;
         p = strchr(p, '%'); // search for the next "%" sign

         if (p) {
            *p = '\0'; // if found, terminate the string
         }

         strncat(msg, prev, 256); // append the remaining text

         if (p) {
            *p = '%'; // restore the '%'
         }
      }

      HumanizeChat(msg); // 'Humanize' the chat message
      return msg;
   }

   return NULL; // not found
}

// Generate a reply message from the input message, and return the
// pointer of the reply message
char *CChatManager::GenerateReply(CBaseBot *pClient, CClient *pFrom,
                                  const char *msg)
{
   static char message[256]; // for the reply message
   message[0] = '\0'; // initialize the string

   char *p;
   int i;

   // find a reply block with the correct keyword
   for (i = 0; i < m_ChatReply.size(); i++) {
      if (m_ChatReply[i].IsValidReply(msg))
         break;
   }

   if (i >= m_ChatReply.size()) {
      // no replies for this sentence are found
      if (RandomLong(1, 100) > 50) {
         // about half the time use a general reply
         if ((p = GenerateChat(pClient, "GENERAL_REPLIES")) != NULL) {
            strncpy(message, p, 256);
         } else {
            return NULL; // No general replies
         }
      } else {
         return NULL; // don't reply to this message
      }
   } else {
      // we have replies for this sentence, generate the message now
      p = (char *)m_ChatReply[i].PickMessage();
      if (!p) {
         return NULL; // no message is available, don't proceed
      }
      while (p && *p) {
         if (*p == '%') {
            char type = *(++p); // skip to next character to get the type
            int hiscore = -9999, j, count;
            CClient *pPickedClient = NULL;
            char buffer[32];

            switch (type) {
            case '%':
               strncat(message, "%", 256);
               break;

            case 'n': // bot name
               strncpy(buffer, pClient->GetNetName(), 32);
               HumanizeName(buffer);
               strncat(message, buffer, 256); // append it to the message
               break;

            case 'r': // random opponent
               j = RandomLong(0, g_pServer->GetMaxClients());

               for (count = 0; count < g_pServer->GetMaxClients(); count++, j++) {
                  CClient *p = g_pServer->m_rgpClients[j];
                  if (p != pClient && p->IsValid()) {
                     strncpy(buffer, p->GetNetName(), 32);
                     HumanizeName(buffer);
                     strncat(message, buffer, 256); // append it to the message
                     break;
                  }
               }

               if (count >= g_pServer->GetMaxClients())
                  return NULL; // can't find any player, don't say anything

               break;

            case 'f': // opponent in first place
               for (j = 0; j < g_pServer->GetMaxClients(); j++) {
                  CClient *p = g_pServer->m_rgpClients[j];
                  if (p == pClient || !p->IsValid())
                     continue;
                  if (p->GetFrags() >= hiscore) {
                     hiscore = p->GetFrags();
                     pPickedClient = p;
                  }
               }
               if (pPickedClient) {
                  strncpy(buffer, pPickedClient->GetNetName(), 32);
                  HumanizeName(buffer); // humanize the name
                  strncat(message, buffer, 256); // append it to the message
               } else {
                  return NULL; // can't find such a player, don't say anything
               }
               break;

            case 'l': // opponent in last place
               hiscore = 9999;
               for (j = 0; j < g_pServer->GetMaxClients(); j++) {
                  CClient *p = g_pServer->m_rgpClients[j];
                  if (p == pClient || !p->IsValid())
                     continue;
                  if (p->GetFrags() <= hiscore) {
                     hiscore = p->GetFrags();
                     pPickedClient = p;
                  }
               }
               if (pPickedClient) {
                  strncpy(buffer, pPickedClient->GetNetName(), 32);
                  HumanizeName(buffer); // humanize the name
                  strncat(message, buffer, 256); // append it to the message
               } else {
                  return NULL; // can't find such a player, don't say anything
               }
               break;

            case 'v': // last victim of this bot
               if (pClient->Chat()->m_strLastVictim.length() <= 0)
                  return NULL; // we don't have a victim yet
               strncat(message, pClient->Chat()->m_strLastVictim.c_str(), 256);
               break;

            case 'k': // last player killed this bot
               if (pClient->Chat()->m_strLastKiller.length() <= 0)
                  return NULL; // this bot hasn't been killed yet
               strncat(message, pClient->Chat()->m_strLastKiller.c_str(), 256);
               break;

            case 'm': // map title
               strncat(message, g_pServer->GetMapName(), 256); // get the map name
               break;

            // reply specific items
            case 's': // player to reply
               strncat(message, pFrom->GetNetName(), 256);
               break;
            }

            p++; // skip to the next character
            continue;
         }

         char *prev = p;
         p = strchr(p, '%'); // search for the next "%" sign

         if (p) {
            *p = '\0'; // if found, terminate the string
         }

         strncat(message, prev, 256); // append the remaining text

         if (p) {
            *p = '%'; // restore the '%'
         }
      }

      HumanizeChat(message); // 'humanize' the chat message
   }

   return message; // return the message
}

