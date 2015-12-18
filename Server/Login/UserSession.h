#ifndef _UserSession_H_INCLUDED_
#define _UserSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"

class UserSession : public ServerSession
{
public:
	UserSession();
	virtual ~UserSession();

	virtual BOOL Update( DWORD dwDeltaTick );
	virtual BOOL SendPacket(BYTE *pMsg, WORD wSize);

	// UserKey = Accept Port;
	WORD GetUserKey() const;
	void SetUserKey(WORD dwKey);

	void CloseSession();

public:
	virtual void 	Init();
	virtual void 	Release();
	virtual void	OnAccept( DWORD dwNetworkIndex );
	virtual void	OnDisconnect();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);

private:
	BOOL            m_bFirst;
	WORD 			m_wUserKey;

public:
	// static DWORD    m_byCloseDelay;
	static DWORD    m_dwClientDelay;
	static DWORD    m_dwServerDelay;
	DWORD 			m_dwOvertime;  // 超时时间
};

#endif // _AGENTPLAYER_H_INCLUDED_
