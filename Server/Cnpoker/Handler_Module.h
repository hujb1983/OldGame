#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "CnpokerFactory.h"
#include "GameMgr.h"


/**************************************
        ȡ����
***************************************/
void MSG_Handler_GamePacket_ANC     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ��������
***************************************/
void MSG_Handler_JoinTable_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        �˳�����
***************************************/
void MSG_Handler_QuitTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ��ʼ��Ϸ��
***************************************/
void MSG_Handler_StartGame_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_InitCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ��ʾ��
***************************************/
void MSG_Handler_ShowCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        �е���
***************************************/
void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ����������
***************************************/
void MSG_Handler_CreateBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ����
***************************************/
void MSG_Handler_Discards_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ������ʾ
***************************************/
void MSG_Handler_Reminder_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ����
***************************************/
void MSG_Handler_Settlement_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ��Ȩ ���� �� ����
***************************************/
void MSG_Handler_CalledLicense_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
        ���ߺ����� --- ֪ͨ
***************************************/
void MSG_Handler_Relogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Offline_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/**************************************
       �й�
***************************************/
void MSG_Handler_Trusteeship_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
#endif // _AGENTPLAYER_H_INCLUDED_
