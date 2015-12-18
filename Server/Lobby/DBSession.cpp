#include "DBSession.h"
#include "PacketHandler.h"
#include "LobbyUpdate.h"
#include "LobbyServer.h"

DBSession::DBSession()
{
}

DBSession::~DBSession()
{

}

void DBSession::Clear()
{
	m_bFirst = TRUE;
	ServerSession::Clear();
}

void DBSession::OnRecv(BYTE *pMsg, WORD wSize) {
    printf( "[DBSession::OnRecv] pMsg = %s \n", pMsg );
    g_PacketHandler.ParsePacket_Database( this, (MSG_BASE*)pMsg, wSize );
}

void DBSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	printf("[GameServer] : DBSession::OnConnect\n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[DBServerSession::OnConnect] success.\n");
		printf("Send Server Type.\n");
		ServerSession::SendServerType();
        g_LobbyUpdate.Init();   // 初始化数据
	}
	else {
	    printf("[GameServer] : DBServerSession::OnConnect fail\n");
	}

}

void DBSession::OnLogString( char * pszLog)
{

}

