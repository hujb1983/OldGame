#include "GameSession.h"
#include "PacketHandler.h"

GameSession::GameSession()
{

}

GameSession::~GameSession()
{

}

void GameSession::Update()
{
    m_Database.Update();
}

void GameSession::Clear()
{

}

void GameSession::OnRecv(BYTE *pMsg, WORD wSize) {
    g_PacketHandler.ParsePacket_Games( this, (MSG_BASE*)pMsg, wSize );
}

void GameSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void GameSession::DBResult( WORD cate, WORD ptcl, QueryResult * pData )
{
    MSG_DBPROXY_RESULT msg;
    msg.m_byCategory = cate;
    msg.m_byProtocol = ptcl;
    msg.m_pData = pData;
    g_PacketHandler.ParsePacket_Database( this, (MSG_BASE*)&msg, sizeof(msg) );
}

void GameSession::OnLogString( char * pszLog)
{

}

