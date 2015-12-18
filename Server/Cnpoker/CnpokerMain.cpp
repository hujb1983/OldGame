#include <Common.h>
#include <Utility.h>
#include <Network.h>
#include "CnpokerServer.h"
#include "Handler_Module.h"


int main(int argc, char ** argv)
{

	g_pCnpokerServer = new CnpokerServer;
	assert(g_pCnpokerServer);

	if( !g_pCnpokerServer->Init() ) {
		return 0;
	}

	DWORD dwTicket = 0;
	int nShutdown = 1;
	while( nShutdown ) {

		usleep(20);

		if ( !g_pCnpokerServer->Update( dwTicket ) ) {
			break;
		}
	}

	SAFE_DELETE(g_pCnpokerServer);

	return 0;
}

