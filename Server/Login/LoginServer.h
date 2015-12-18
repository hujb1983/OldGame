#ifndef _LOGINSERVER_H_INCLUDED_
#define _LOGINSERVER_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "LoginFactory.h"
#include "ObjKeyGenerator.h"
#include "JsonParser.h"
#include "MsgBuff.h"

using namespace std;

class LoginServer
{
	enum { CLIENT_SYNCHANDLER = 0, SERVER_SYNCHANDLER = 1, };

public:
	LoginServer(void);
	~LoginServer(void);

	BOOL Init();

	// Start Listen;
	void StartServerSideListen();
	void StartClientSideListen();

	BOOL ConnectToServer(ServerSession * pSession, char * pszIP, WORD wPort);
    BOOL MaintainConnection();

	BOOL Update( DWORD dwDeltaTick );

	// DB Server;
	BOOL SendToDBServer( BYTE * pMsg, WORD wSize);
	ServerSession * GetDBSession() const;

	// User Session
    BOOL SendToClient( WORD wIndex, BYTE * pMsg, WORD wSize );
	BOOL SendToClient( BYTE * pMsg, WORD wSize );
	BOOL SetUserSession( WORD wIndex, UserSession * pSession );
	UserSession * GetUserSession( WORD wIndex );
	BOOL OvertimeClear( DWORD dwDeltaTick );

	// User Key
	WORD AllocSessionKey();
    void FreeSessionKey(WORD _wIndex);

private:
	BOOL m_bShutdown;

	// Main Server;
	IOCPServer *m_pIOCPServer;

	// DB Server;
	DBSession *m_pDBServer;

	// UserKey = Key;  maxsize = 0xFFFF+1;
	UserSession * m_pUserSession[PORT_MAX + 1];

	// User Key
    CObjKeyGenerator m_cObjKey;

public:
	// Overtime Process;
	static DWORD 		m_dwClearDelay;
	DWORD 				m_dwClearOvertime;
	std::list<WORD> 	m_lsOvertimeTable;
};

extern LoginServer * g_pLoginServer;

#endif // _AGENTSERVER_H_INCLUDED_
