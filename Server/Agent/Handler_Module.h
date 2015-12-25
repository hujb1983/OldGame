#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "AgentFactory.h"
#include "AgentServer.h"


/* 1. ��½��֤��
   2. ... */
void MSG_Handler_Login_REQ      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Login_ANC      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_Logout_REQ     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Logout_ANC     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_JoinTable_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinTable_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_GamePacket_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_GamePacket_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


/* 1. ָ�����ݣ�
   2. ����������
   3. �����У�
   4. �����У� */
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


/* 1. ��ʼ��Ϸ��
   2. ... */
void MSG_Handler_QuitTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitTable_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. ������Ϸ��*/
void MSG_Handler_JoinGame_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinGame_NAK  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitGame_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 2. �˳����ӣ�
   3. ��ʼ��Ϸ��
   4. �е�����
   5. ���ƣ�  */
void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_StartGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_InitCards_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_ShowCards_REQ  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_ShowCards_BRD  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_CalledBank_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


// �������� ���ҷ��ͷ�������
void MSG_Handler_CreateBank_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// ������Ȩ!
void MSG_Handler_CalledLicense_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsLicense_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// ����!
void MSG_Handler_Discards_REQ   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Discards_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

void MSG_Handler_CalledOvertime_NAK  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DiscardsOvertime_NAK( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// ����!
void MSG_Handler_Settlement_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// �������� ���ҷ��ͷ�������
void MSG_Handler_Relogin_ANC   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Relogin_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Offline_BRD   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

// �й���Ϣ����
void MSG_Handler_Trusteeship_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Trusteeship_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

#endif // _AGENTPLAYER_H_INCLUDED_
