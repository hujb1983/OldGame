#include "AgentSession.h"
#include "PacketHandler.h"
#include "LobbyServer.h"
#include "LobbyFactory.h"
#include "LobbyUpdate.h"

AgentSession::AgentSession()
{
    m_next = NULL;
    m_prev = NULL;
}

AgentSession::~AgentSession()
{

}

void AgentSession::AddUser( WORD _userkey, LobbyUser * _pUser )
{
    mapUserObject[_userkey] = _pUser;
    m_pAllUser.push_back( _pUser );
}

LobbyUser * AgentSession::FindUser( WORD _userkey) {
    return mapUserObject[_userkey];
}

void AgentSession::RemoveUser( WORD  _userkey )
{
    if ( mapUserObject[_userkey] ) {
        LobbyFactory::Instance()->FreeUserSession( mapUserObject[_userkey] );
        m_pAllUser.remove( mapUserObject[_userkey] );
        mapUserObject[_userkey] = NULL;
    }
}

void AgentSession::RemoveAllUser()
{
    LobbyFactory * pFactory = LobbyFactory::Instance();
    if ( pFactory ) {
        LobbyUser * pRemove = m_pAllUser.get_frist();
        LobbyUser * pNext;
        for( ; pRemove; ) {
            pNext = pRemove->get_next();
            if ( pRemove ) {
                pFactory->FreeUserSession( pRemove );
            }
            pRemove = pNext;
        }
    }
}

void AgentSession::Clear()
{
	ServerSession::Clear();
}

void AgentSession::OnRecv(BYTE *pMsg, WORD wSize) {
    g_PacketHandler.ParsePacket_Agent( this, (MSG_BASE*)pMsg, wSize );
}

void AgentSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void AgentSession::OnDisconnect()
{
	g_LobbyUpdate.QuitAgentSession( this );
	RemoveAllUser();
}

void AgentSession::OnLogString( char * pszLog)
{

}

int AgentSession::SendAllUser( BYTE * pMsg, WORD wSize )
{
    LobbyUser * pUser = m_pAllUser.get_frist();
    for ( ; pUser ; pUser = pUser->get_next() ) {
        pUser->SendToClient(this, pMsg, wSize);
    }
}

/**********************************************
    队列对象
**********************************************/
AgentSession * AgentSession::set_prev( AgentSession * _prev ) {
    AgentSession * t = m_prev;
    m_prev = _prev;
    return t;
}

AgentSession * AgentSession::set_next( AgentSession * _next ) {
    AgentSession * t = m_next;
    m_next = _next;
    return t;
}

bool AgentSession::is_null_prev() {
    return (m_prev != NULL);
}
bool AgentSession::is_null_next() {
    return (m_next != NULL);
}

AgentSession * AgentSession::get_prev() {
    return (m_prev);
}

AgentSession * AgentSession::get_next() {
    return (m_next);
}


AgentSessionList::AgentSessionList()
{

}

AgentSessionList::~AgentSessionList()
{
}

void AgentSessionList::clear()
{
    m_size = 0;
    m_head = NULL;
    m_last = NULL;
}

void AgentSessionList::push_back(AgentSession * pObj)
{
    if ( !m_head ) {
        m_head = pObj;
        if ( m_head ) {
            m_head->set_prev(NULL);
            m_head->set_next(NULL);
        }
        m_last = NULL;
        ++m_size;
    }
    else if ( !m_last ){
        m_head->set_next( pObj );
        if ( m_head ) {
            pObj->set_prev( m_head );
            pObj->set_next( NULL );
        }
        m_last = pObj;
    }
    else if ( m_last ){
        m_last->set_next( pObj );
        if ( m_head ) {
            pObj->set_prev( m_last );
            pObj->set_next( NULL );
        }
        m_last = pObj;
    }
}

void AgentSessionList::remove(AgentSession * pObj)
{
    AgentSession * _prev = pObj->get_prev();
    AgentSession * _next = pObj->get_next();
    if ( _prev ) {
        _prev->set_next( _next );
    }
    if ( _next ) {
        _next->set_prev( _prev );
    }
    --m_size;
}

AgentSession * AgentSessionList::get_frist() {
    return m_head;
}

