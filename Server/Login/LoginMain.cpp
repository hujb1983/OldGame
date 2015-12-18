#include "LoginServer.h"

void ProtocolList()
{
    printf("\n");
    printf("PreLogin = %d \n", MAKEDWORD(Login_Protocol,PreLogin_REQ) );
}

void InitOvertime()
{
	LoginServer::m_dwClearDelay 	= 3000;
	UserSession::m_dwClientDelay 	= 6000;
	UserSession::m_dwServerDelay 	= 10000;
}

int main(int argc, char ** argv)
{
    ProtocolList();

	InitOvertime();

	g_pLoginServer = new LoginServer;
	assert(g_pLoginServer);

	if( !g_pLoginServer->Init() ) {
		return 0;
	}

	DWORD dwTicket = 0;
	int nShutdown  = 1;

	while( nShutdown ) {
		usleep(20);

		dwTicket = Session::GetTickCount();

		if ( !g_pLoginServer->Update( dwTicket ) ) {
			break;
		}
	}

	SAFE_DELETE(g_pLoginServer);

	return 0;
}




