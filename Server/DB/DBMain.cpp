#include <Common.h>
#include <Utility.h>
#include <Network.h>
#include "DBServer.h"
#include "Handler_Module.h"


int main(int argc, char ** argv)
{

	g_DBServer = new DBServer;
	assert(g_DBServer);

	if( !g_DBServer->Init() ) {
		return 0;
	}

	int nShutdown = 1;
	while( nShutdown ) {
		usleep(20);

		if ( !g_DBServer->Update( 0 ) ) {
			break;
		}
	}

	SAFE_DELETE(g_DBServer);

	return 0;
}

