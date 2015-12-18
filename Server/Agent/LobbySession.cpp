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
	printf("[AgentServer] : LobbySession::OnConnect\n");
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[LobbySession::OnConnect] success.\n");
		printf("Send Server Type.\n");
		ServerSession::SendServerType();
	}
	else {
	    printf("[AgentServer] : LobbySession::OnConnect fail\n");
	}
}

void LobbySession::OnLogString( char * pszLog)
{

}

