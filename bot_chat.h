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
 * bot_chat.h                                          *
 *******************************************************
 * Purpose: Bots chatting code                         *
 * Engine-specific: no                                 *
 *******************************************************/

#ifndef BOT_CHAT_H
#define BOT_CHAT_H

// bot chat class
class CBotChat {
public:
   CBotChat(CBaseBot *pBot);
   virtual ~CBotChat();

   CBaseBot     *m_pBot;
   std::string   m_strLastKiller;
   std::string   m_strLastVictim;    // last victim of this bot

   bool          IsGoodChatPosition(void);

   void          ReplyToMessage(CClient *pFrom, const char *msg);
   void          PrepareMessage(const char *msg, bool is_team_msg = false);

   void          Think(void);

private:
   bool          m_fIsTeamMsg;
   float         m_flChatTime;       // time to say message
   std::string   m_strMsgBuffer;     // message to say
};

// chat item
class CChatItem {
public:
   CChatItem(unsigned short t) { type = t; }

   unsigned short             type; // message type (CRC'ed)
   std::vector<std::string>   msg; // messages

   void          AddMessage(char *sz);
   const char   *PickMessage(void);
};

// chat reply item
class CChatReply {
public:
   std::vector<std::string>          key; // keywords
   std::vector<std::string>          msg; // messages

   void         AddKeyword(char *sz);
   void         AddMessage(char *sz);

   const char  *IsValidReply(const char *word);
   const char  *PickMessage(void);
};

// chat manager class
class CChatManager {
public:
   CChatManager();
   virtual ~CChatManager();

   void LoadBotChat(void);

   void ChatMessage(CClient *from, CClient *to, const char *message);
   char *GenerateChat(CBaseBot *pClient, const char *type);
   char *GenerateReply(CBaseBot *pClient, CClient *pFrom, const char *msg);

   int  NumChatItem(void) { return m_ChatItem.size(); }
   int  NumChatReply(void) { return m_ChatReply.size(); }

private:
   std::vector<CChatItem>         m_ChatItem;
   std::vector<CChatReply>        m_ChatReply;
};

#endif

