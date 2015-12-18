#include "GameSession.h"
#include "PacketHandler.h"

GameSession::GameSession()
{

}

GameSession::~GameSession()
{

}

void GameSession::Clear()
{

}

void GameSession::OnRecv(BYTE *pMsg, WORD wSize)
{
    g_PacketHandler.ParsePacket_Games(this, (MSG_BASE*)pMsg, wSize);
}

void GameSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void GameSession::OnLogString( char * pszLog)
{

}

