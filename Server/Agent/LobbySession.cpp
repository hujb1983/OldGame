#include "LobbySession.h"
#include "PacketHandler.h"

LobbySession::LobbySession()
{

}

LobbySession::~LobbySession()
{

}

void LobbySession::Clear()
{

}

void LobbySession::OnRecv(BYTE *pMsg, WORD wSize) {
    g_PacketHandler.ParsePacket_Lobby(this, (MSG_BASE*)pMsg, wSize);
}

void LobbySession::OnAccept( DWORD dwNetworkIndex )
{

}

void LobbySession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	DEBUG_MSG( LVL_DEBUG, "LobbySession::OnConnect. \n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		DEBUG_MSG( LVL_DEBUG, "LobbySession::OnConnect success. \n");
		printf("Send Server Type.\n");
		ServerSession::SendServerType();
	}
	else {
	    DEBUG_MSG( LVL_DEBUG, "LobbySession::OnConnect fail. \n");
	}
}

void LobbySession::OnLogString( char * pszLog)
{

}

