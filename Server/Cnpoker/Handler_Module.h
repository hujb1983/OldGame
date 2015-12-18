#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "CnpokerFactory.h"
#include "GameMgr.h"

/* 1. ȡ���ݣ�
   2. ... */
void MSG_Handler_GamePacket_ANC     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. �������ӣ�*/
void MSG_Handler_JoinGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   2. �˳����ӣ�*/
void MSG_Handler_QuitGame_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   2. ��ʼ��Ϸ��*/
void MSG_Handler_StartGame_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_InitCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   3. ��ʾ�ƣ� */
void MSG_Handler_ShowCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   4. �е����� */
void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   5. ���������� */
void MSG_Handler_CreateBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   6. ���ƣ�   */
void MSG_Handler_Discards_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   7. ���㣻   */
void MSG_Handler_Settlement_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   8. ��Ȩ��   */
void MSG_Handler_CalledLicense_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*   ���ߺ����� --- ֪ͨ   */
void MSG_Handler_Relogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Offline_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


#endif // _AGENTPLAYER_H_INCLUDED_
