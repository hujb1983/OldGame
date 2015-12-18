#include "LoginSession.h"
#include "PacketHandler.h"

LoginSession::LoginSession()
{

}

LoginSession::~LoginSession()
{

}

void LoginSession::Update()
{
    m_Database.Update();
}

void LoginSession::Clear()
{

}

void LoginSession::OnRecv(BYTE *pMsg, WORD wSize) {
    g_PacketHandler.ParsePacket_Login( this, (MSG_BASE*)pMsg, wSize );
}

void LoginSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void LoginSession::DBResult( WORD cate, WORD ptcl, QueryResult * pData )
{
    MSG_DBPROXY_RESULT msg;
    msg.m_byCategory = cate;
    msg.m_byProtocol = ptcl;
    msg.m_pData = pData;
    g_PacketHandler.ParsePacket_Database( this, (MSG_BASE*)&msg, sizeof(msg) );
}

void LoginSession::OnLogString( char * pszLog)
{

}

