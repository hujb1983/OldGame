#ifndef _DBServer_H_INCLUDED_
#define _DBServer_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "DBFactory.h"
#include "ObjKeyGenerator.h"

using namespace std;

class DBServer
{
	enum { CLIENT_SYNCHANDLER = 0, SERVER_SYNCHANDLER = 1, };

public:
	DBServer(void);

	~DBServer(void);

	BOOL Init();

	void StartServerSideListen();

	BOOL MaintainConnection();

	BOOL Update( DWORD dwDeltaTick );

	BOOL SendToGameServer( BYTE * pMsg, WORD wSize);
	BOOL SendToLobbyServer( BYTE * pMsg, WORD wSize);
	BOOL SendToLoginServer( BYTE * pMsg, WORD wSize);

	ServerSession * GetGameSession() const;
	ServerSession * GetLobbySession() const;
	ServerSession * GetLoginSession() const;

	UINT GetTableKey(); // ·¿¼äºÅ
    void FreeTableKey( UINT _uiIndex );

private:
	BOOL m_bShutdown;
	IOCPServer * m_pIOCPServer;

	ServerSession * m_pGameServer;
	ServerSession * m_pLobbyServer;
	ServerSession * m_pLoginServer;

	CObjKeyGenerator m_cTablesKey;
};

extern DBServer * g_DBServer;

#endif // _AGENTSERVER_H_INCLUDED_
