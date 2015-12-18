#include "LobbyUser.h"
#include "LobbyServer.h"

LobbyUser::LobbyUser()
{

}

LobbyUser::~LobbyUser()
{

}

void LobbyUser::SendToClient( ServerSession *pSession, BYTE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int  _userkey  = 0;
    int  _pid   = 0;
    char _data[256] = {0};

    js_map.ReadInteger( "protocol",  _pid );
    js_map.ReadString ( "data", _data, sizeof (_data) );

    char buff[1024] = {0};
    char format[1024] = "{\"protocol\":\"%d\","
                        " \"userkey\":%d,"
						" \"data\":\"%s\"}";

	snprintf( buff, sizeof(buff), format, _pid,
         _pid, _userkey, _data);

    pSession->Send( pMsg, wSize );
}

LobbyUser * LobbyUser::set_prev( LobbyUser * _prev ) {
    LobbyUser * t = m_prev;
    m_prev = _prev;
    return t;
}

LobbyUser * LobbyUser::set_next( LobbyUser * _next ) {
    LobbyUser * t = m_next;
    m_next = _next;
    return t;
}

bool LobbyUser::is_null_prev() {
    return (m_prev != NULL);
}
bool LobbyUser::is_null_next() {
    return (m_next != NULL);
}

LobbyUser * LobbyUser::get_prev() {
    return (m_prev);
}

LobbyUser * LobbyUser::get_next() {
    return (m_next);
}


/**********************************************
    游戏用户队列对象
**********************************************/
LobbyUserList::LobbyUserList()
{

}

LobbyUserList::~LobbyUserList()
{
}

void LobbyUserList::clear()
{
    m_size = 0;
    m_head = NULL;
    m_last = NULL;
}

void LobbyUserList::push_back(LobbyUser * pUser)
{
     if ( !m_head ) {
        m_head = pUser;
        m_head->set_prev(NULL);
        m_head->set_next(NULL);
        m_last = NULL;
        ++m_size;
    }
    else if ( !m_last ){
        m_head->set_next( pUser );
        pUser->set_prev( m_head );
        pUser->set_next( NULL );
        m_last = pUser;
    }
    else if ( m_last ){
        m_last->set_next( pUser );
        pUser->set_prev( m_last );
        pUser->set_next( NULL );
        m_last = pUser;
    }
}

void LobbyUserList::remove(LobbyUser * pUser)
{
    LobbyUser * _prev = pUser->get_prev();
    LobbyUser * _next = pUser->get_next();
    if ( _prev ) {
        _prev->set_next( _next );
    }
    if ( _next ) {
        _next->set_prev( _prev );
    }
    --m_size;
}

LobbyUser * LobbyUserList::get_frist() {
    return m_head;
}
