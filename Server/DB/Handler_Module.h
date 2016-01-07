#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "DBFactory.h"


/* 1. Ԥ��½��*/
void MSG_Handler_PreLogin       ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_PreLogin_DBR   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. ��½��֤��
   2. ... */
void MSG_Handler_Login     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Login_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. �ص�½��֤��
   2. ... */
void MSG_Handler_Relogin     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Relogin_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. ȡ���ݣ�
   2. ... */
void MSG_Handler_GamePacket     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_GamePacket_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. ָ�����ݣ�
   2. ����������
   3. �����У�
   4. �����У� */
void MSG_Handler_RoomInfo      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_TableInfo     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Onlines       ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_WRankInfo     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DRankInfo     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_RoomInfo_DBR  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_TableInfo_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Onlines_DBR   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_WRankInfo_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_DRankInfo_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/* 1. �������ӣ�
   2. �˳����ӣ�
   3. ��ʼ��Ϸ��
   4. �е�����
   5. ���ƣ�  */
void MSG_Handler_JoinTable      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitTable      ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitGame       ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_JoinTable_DBR  ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_CreateTable_DBR( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_QuitGame_DBR   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


/*  �������� */
void MSG_Handler_UpdateBattle     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_UpdateBattle_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

/*  ���� */
void MSG_Handler_Settlement     ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_Settlement_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

#endif // _AGENTPLAYER_H_INCLUDED_
