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

	eSERVER_TYPE	GetServerType() {	return AGENT_SERVER;   }

public:
	virtual void    Clear();
	virtual void	OnAccept( DWORD dwNetworkIndex );
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);

private:
	BOOL m_bFirst;
};


#endif // _AGENTPLAYER_H_INCLUDED_
