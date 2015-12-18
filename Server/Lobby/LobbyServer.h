#ifndef _LobbyServer_H_INCLUDED_
#define _LobbyServer_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "LobbyFactory.h"

using namespace std;

class LobbyServer
{
	enum { CLIENT_SYNCHANDLER = 0, SERVER_SYNCHANDLER = 1, };

public:
	LobbyServer(void);
	~LobbyServer(void);

	BOOL Init();

	BOOL ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort );

	void StartServerSideListen();

	BOOL MaintainConnection();

	BOOL Update( DWORD dwDeltaTick );

	BOOL SendToDBServer( BYTE * pMsg, WORD wSize);

private:
	BOOL m_bShutdown;

	IOCPServer * m_pIOCPServer;

	ServerSession * m_pDBSession;
};

extern LobbyServer * g_LobbyServer;

#endif
