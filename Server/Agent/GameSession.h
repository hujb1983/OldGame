#ifndef _GameSession_H_INCLUDED_
#define _GameSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"
#include "ObjKeyGenerator.h"
#include "UserGame.h"

class GameSession : public ServerSession
{
public:
	GameSession();
	virtual ~GameSession();

	eSERVER_TYPE	GetServerType() {	return GAME_SERVER;  }

public:
	virtual void    Clear();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);

private:
	BOOL m_bFirst;

public:

    // ≥ı ºªØ;
    void Init();

	// UserGame
	BOOL SetUserGame( WORD wIndex, UserGame * pUser );
	UserGame * GetUserGame( WORD wIndex );

    WORD AllocGameKey();
    void FreeGameKey(WORD _wIndex);

private:
    UserGame * m_pUserGame[PORT_MAX + 1];
    CObjKeyGenerator m_cObjKey;
};

#endif // _AGENTPLAYER_H_INCLUDED_
