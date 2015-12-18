#include <Common.h>
#include <Utility.h>
#include <Network.h>
#include "LobbyServer.h"
#include "Handler_Module.h"


int main(int argc, char ** argv)
{

	g_LobbyServer = new LobbyServer;
	assert(g_LobbyServer);

	if( !g_LobbyServer->Init() ) {
		return 0;
	}

	int nShutdown = 1;
	while( nShutdown ) {
		usleep(20);

		if ( !g_LobbyServer->Update( 0 ) ) {
			break;
		}
	}

	SAFE_DELETE(g_LobbyServer);

	return 0;
}

