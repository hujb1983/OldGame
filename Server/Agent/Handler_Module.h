#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "AgentFactory.h"
#include "AgentServer.h"


/* 1. 登陆验证；
   2. ... */
void MSG_Handler_Login_REQ      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Login_ANC      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_Logout_REQ     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Logout_ANC     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_JoinTable_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinTable_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

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
void MSG_Handler_DRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_WRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


/* 1. 开始游戏；
   2. ... */
void MSG_Handler_QuitTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitTable_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. 加入游戏；*/
void MSG_Handler_JoinGame_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinGame_NAK  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitGame_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 2. 退出桌子；
   3. 开始游戏；
   4. 叫地主；
   5. 出牌；  */
void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_StartGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_InitCards_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_ShowCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_ShowCards_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_CalledBank_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


// 创建地主 并且发送发牌命令
void MSG_Handler_CreateBank_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// 牌照授权!
void MSG_Handler_CalledLicense_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsLicense_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// 出牌!
void MSG_Handler_Discards_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Discards_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_CalledOvertime_NAK  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsOvertime_NAK( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// 结算!
void MSG_Handler_Settlement_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// 创建地主 并且发送发牌命令
void MSG_Handler_Relogin_ANC   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Relogin_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Offline_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// 托管消息处理
void MSG_Handler_Trusteeship_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Trusteeship_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

#endif // _AGENTPLAYER_H_INCLUDED_
