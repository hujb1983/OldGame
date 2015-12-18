#ifndef _CnpokerServer_H_INCLUDED_
#define _CnpokerServer_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "CnpokerFactory.h"

using namespace std;

class CnpokerServer
{
	enum { CLIENT_SYNCHANDLER = 0, SERVER_SYNCHANDLER = 1, };

public:
	CnpokerServer(void);
	~CnpokerServer(void);

	BOOL Init();

	BOOL ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort );

	//void StartListen();

	BOOL MaintainConnection();

	BOOL Update( DWORD dwDeltaTick );

	BOOL SendToDBServer( BYTE * pMsg, WORD wSize);
	BOOL SendToAgentServer( BYTE * pMsg, WORD wSize);

private:
	BOOL m_bShutdown;

	IOCPServer * m_pIOCPServer;

	//
	ServerSession * m_pDBSession;
	ServerSession * m_pAgentSession;

};

extern CnpokerServer * g_pCnpokerServer;

#endif // _AGENTSERVER_H_INCLUDED_
