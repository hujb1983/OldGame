#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include "LobbyUpdate.h"


/* 1. 登陆验证；
   2. ... */
void MSG_Handler_Login_REQ      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Login_ANC      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_Logout_REQ     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Logout_ANC     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_Relogin_REQ    ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Relogin_ANC    ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_JoinTable_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinTable_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_QuitTable_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitTable_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_GamePacket_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_GamePacket_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


/* 1. 指定数据；
   2. 在线人数；
   3. 周排行；
   4. 月排行； */
void MSG_Handler_RoomInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_RoomInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_TableInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_TableInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_Onlines_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Onlines_ANC   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_WRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_WRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


#endif // _AGENTPLAYER_H_INCLUDED_
