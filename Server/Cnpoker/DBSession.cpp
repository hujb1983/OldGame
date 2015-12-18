#include "DBSession.h"
#include "PacketHandler.h"

DBSession::DBSession()
{

}

DBSession::~DBSession()
{

}

void DBSession::Clear()
{

}

void DBSession::OnRecv(BYTE *pMsg, WORD wSize)
{
    g_PacketHandler.ParsePacket_Database( (ServerSession*)this, (MSG_BASE*)pMsg, wSize );
}

void DBSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	printf("[GameServer] : DBSession::OnConnect\n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[DBSession::OnConnect] success.\n");
		printf("Send Server Type.\n");
		ServerSession::SendServerType();
	}
	else {
	    printf("[GameServer] : DBSession::OnConnect fail\n");
	}
}

void DBSession::OnLogString( char * pszLog)
{

}

