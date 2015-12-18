#include "AgentSession.h"
#include "PacketHandler.h"
#include "CnpokerServer.h"

AgentSession::AgentSession()
{

}

AgentSession::~AgentSession()
{

}

void AgentSession::Clear()
{

}

void AgentSession::OnRecv(BYTE *pMsg, WORD wSize)
{
    g_PacketHandler.ParsePacket_Agent( (ServerSession*)this, (MSG_BASE*)pMsg, wSize );
}

void AgentSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
	if ( bSuccess ) {
		printf("[GameServer] : AgentSession::OnConnect success.\n");
		ServerSession::SendServerType();
	}
	else {
	    printf("[GameServer] : AgentSession::OnConnect fail\n");
	}
}

void AgentSession::OnLogString( char * pszLog)
{

}


