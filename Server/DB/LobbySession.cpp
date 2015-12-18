#include "LobbySession.h"
#include "PacketHandler.h"

LobbySession::LobbySession()
{

}

LobbySession::~LobbySession()
{

}

void LobbySession::Update()
{
    m_Database.Update();
}

void LobbySession::Clear()
{

}

void LobbySession::OnRecv(BYTE *pMsg, WORD wSize) {
    g_PacketHandler.ParsePacket_Lobby( this, (MSG_BASE*)pMsg, wSize );
}

void LobbySession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void LobbySession::DBResult( WORD cate, WORD ptcl, QueryResult * pData )
{
    MSG_DBPROXY_RESULT msg;
    msg.m_byCategory = cate;
    msg.m_byProtocol = ptcl;
    msg.m_pData = pData;
    g_PacketHandler.ParsePacket_Database( this, (MSG_BASE*)&msg, sizeof(msg) );
}

void LobbySession::OnLogString( char * pszLog)
{

}

