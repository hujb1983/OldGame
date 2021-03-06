#ifndef _LobbySession_H_INCLUDED_
#define _LobbySession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class LobbySession : public ServerSession
{
public:
	LobbySession();
	virtual ~LobbySession();

	eSERVER_TYPE	GetServerType() {	return LOBBY_SERVER;  }

	virtual void Update();
    virtual void DBResult( WORD cate, WORD ptcl, QueryResult * pData );

public:
	virtual void    Clear();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);

private:
	BOOL m_bFirst;
};


#endif // _AGENTPLAYER_H_INCLUDED_
