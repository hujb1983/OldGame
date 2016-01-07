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
    g_PacketHandler.ParsePacket_Database( this, (MSG_BASE*)pMsg, wSize );
}

void DBSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	DEBUG_MSG( LVL_DEBUG, "DBSession::OnConnect. \n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		DEBUG_MSG( LVL_DEBUG, "DBSession::OnConnect success. \n");
		ServerSession::SendServerType();
        g_LobbyUpdate.Init();   // 初始化数据
	}
	else {
	    DEBUG_MSG( LVL_DEBUG, "DBSession::OnConnect fail. \n");
	}

}

void DBSession::OnLogString( char * pszLog)
{

}

