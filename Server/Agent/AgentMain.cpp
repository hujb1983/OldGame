#include <Common.h>
#include <Utility.h>
#include <Network.h>
#include "AgentServer.h"
#include "UserSession.h"
#include "Handler_Module.h"


int main(int argc, char ** argv)
{

	g_AgentServer = new AgentServer;
	assert(g_AgentServer);

	if( !g_AgentServer->Init() ) {
		return 0;
	}

	/*
	UserSession pSession;
	char * pMsg = "{\"userid\":555, \"gameid\":6}";
	int nLen = sizeof( pMsg );
	MSG_Handler_GamePacket( (ServerSession*)&pSession, (MSG_BASE*)pMsg, nLen ); */

	int nShutdown = 1;
	while( nShutdown ) {
		usleep(20);

		if ( !g_AgentServer->Update( 0 ) ) {
			break;
		}
	}

	SAFE_DELETE(g_AgentServer);

	return 0;
}

