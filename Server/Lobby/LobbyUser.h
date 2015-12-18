#ifndef __LobbyUser_H_INCLUDED__
#define __LobbyUser_H_INCLUDED__

#include <Common.h>
#include <Network.h>
#include <Utility.h>
#include <Public.h>
#include "ServerSession.h"

/**********************************************
    游戏用户对象
**********************************************/
class LobbyUser
{
public:
    LobbyUser();
    ~LobbyUser();

    void SetHashKey ( WORD _userkey) { m_wUserkey = _userkey; }
    WORD GetHashKey () const { return m_wUserkey; }

    void SetDB ( UINT _userdb) { m_wUserDB = _userdb; }
    WORD GetDB () const { return m_wUserDB; }

    void SendToClient( ServerSession *pSession, BYTE * pMsg, WORD wSize );

public:
    LobbyUser * set_prev( LobbyUser * _prev );
    LobbyUser * set_next( LobbyUser * _next );

    bool is_null_prev();
    bool is_null_next();

    LobbyUser * get_prev();
    LobbyUser * get_next();

private:
    WORD m_wUserkey;
    UINT m_wUserDB;     // DB id

private:
    LobbyUser * m_prev;
    LobbyUser * m_next;
};


/**********************************************
    游戏用户队列对象
**********************************************/
class LobbyUserList
{
public:
	LobbyUserList();
	~LobbyUserList();

public:

    void clear();
    void push_back(LobbyUser * pUser);
    void remove(LobbyUser * pUser);
    WORD size() { return m_size; }

    LobbyUser * get_frist();

private:
    WORD m_size;
    LobbyUser * m_head;
    LobbyUser * m_last;
};

#endif // __PRELOGIN_QUERY_H_INCLUDED__
