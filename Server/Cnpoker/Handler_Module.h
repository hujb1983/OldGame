#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "CnpokerFactory.h"
#include "GameMgr.h"

/* 1. 取数据；
   2. ... */
void MSG_Handler_GamePacket_ANC     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. 加入桌子；*/
void MSG_Handler_JoinGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   2. 退出桌子；*/
void MSG_Handler_QuitGame_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   2. 开始游戏；*/
void MSG_Handler_StartGame_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_InitCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   3. 显示牌； */
void MSG_Handler_ShowCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   4. 叫地主； */
void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   5. 创建地主； */
void MSG_Handler_CreateBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   6. 出牌；   */
void MSG_Handler_Discards_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   7. 结算；   */
void MSG_Handler_Settlement_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   8. 授权；   */
void MSG_Handler_CalledLicense_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   在线和离线 --- 通知   */
void MSG_Handler_Relogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Offline_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


#endif // _AGENTPLAYER_H_INCLUDED_
