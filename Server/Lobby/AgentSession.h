#ifndef _AgentSession_H_INCLUDED_
#define _AgentSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "ServerSession.h"
#include "LobbyUser.h"
#include "ObjKeyGenerator.h"

class AgentSession : public ServerSession
{
public:
	AgentSession();
	virtual ~AgentSession();

	eSERVER_TYPE	GetServerType() {	return LOBBY_SERVER;  }

	void SetIndexKey( WORD _wport ) { m_wIndexKey = _wport; }
	WORD GetIndexKey() { return m_wIndexKey; }

	void AddUser( WORD _userkey, LobbyUser * pUser ) ;
	LobbyUser * FindUser( WORD _userkey);
	void RemoveUser( WORD  _userkey ) ;
    void RemoveAllUser() ;

public:
	virtual void    Clear();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void    OnDisconnect();
	virtual void	OnLogString( char * pszLog);

private:
	WORD m_wIndexKey;

public:
    int SendAllUser( BYTE * pMsg, WORD wSize );

private:
    LobbyUser * mapUserObject[0xFFFF];
    LobbyUserList m_pAllUser;

public:
    AgentSession * set_prev( AgentSession * _prev );
    AgentSession * set_next( AgentSession * _next );

    bool is_null_prev();
    bool is_null_next();

    AgentSession * get_prev();
    AgentSession * get_next();

private:
    AgentSession * m_prev;
    AgentSession * m_next;
};


/**********************************************
    游戏用户队列对象
**********************************************/
class AgentSessionList
{
public:
	AgentSessionList();
	~AgentSessionList();

public:

    void clear();
    void push_back(AgentSession * pUser);
    void remove(AgentSession * pUser);
    WORD size() { return m_size; }

    AgentSession * get_frist();

private:
    WORD m_size;
    AgentSession * m_head;
    AgentSession * m_last;
};


#endif // _AGENTPLAYER_H_INCLUDED_
