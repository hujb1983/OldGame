#ifndef _AGENTSERVER_H_INCLUDED_
#define _AGENTSERVER_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "AgentFactory.h"
#include "ObjKeyGenerator.h"

using namespace std;


class AgentServer
{
	enum { CLIENT_SYNCHANDLER = 0, SERVER_SYNCHANDLER = 1, };

public:
	AgentServer(void);
	~AgentServer(void);

	BOOL Init();

	void StartServerSideListen();
	void StartClientSideListen();

	BOOL Update( DWORD dwDeltaTick );

	BOOL ConnectToServer(ServerSession * pSession, char * pszIP, WORD wPort);
    BOOL MaintainConnection();

public: // 对其他服务器操作
	BOOL SendToGameServer( BYTE * pMsg, WORD wSize);
	ServerSession * GetGameSession() const;

	BOOL SendToLobbyServer( BYTE * pMsg, WORD wSize);
	ServerSession * GetLobbySession() const;

public: // 对其游戏用户操作

	// User Session
    BOOL SendToClient( WORD wIndex, BYTE * pMsg, WORD wSize );
	BOOL SendToClient( BYTE * pMsg, WORD wSize );
	BOOL SetUserSession( WORD wIndex, UserSession * pSession );
	UserSession * GetUserSession( WORD wIndex );

	// User Session
	BOOL SetUserGame( WORD wIndex, UserSession * pSession );
	UserGame * GetUserGame( WORD wIndex );

private:
	BOOL 	        m_bShutdown;
	IOCPServer *    m_pIOCPServer;

	ServerSession * m_pGameServer;
	ServerSession * m_pLobbyServer;

	// UserKey = Key;  maxsize = 0xFFFF+1;
	UserSession * m_pUserSession[PORT_MAX + 1];
    UserGame * m_pUserGame[PORT_MAX + 1];

public: /* ·ÖÅä¹Ø¼ü×Ö */
    WORD AllocSessionKey();
    void FreeSessionKey(WORD _wIndex);

private:
    CObjKeyGenerator m_cObjKey;
};

extern AgentServer * g_AgentServer;

#endif // _AGENTSERVER_H_INCLUDED_
