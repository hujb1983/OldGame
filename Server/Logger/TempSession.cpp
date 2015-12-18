#include "DBSession.h"
#include "LoginServer.h"
#include "PacketHandler.h"

DBSession::DBSession()
	: m_bFirst(TRUE)
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
	g_PacketHandler.ParsePacket_Database( (ServerSession*)this, (MSG_BASE*)pMsg, wSize );
}

void DBSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[GameServer] : DBSession::OnConnect success.\n");
		ServerSession::SendServerType();
	}
	else {
	    printf("[LoginServer] : DBSession::OnConnect fail\n");
	}
}

void DBSession::OnLogString( char * pszLog)
{

}
