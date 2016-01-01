#include "GameSession.h"
#include "PacketHandler.h"

GameSession::GameSession()
{

}

GameSession::~GameSession()
{

}

void GameSession::Init()
{
    m_cObjKey.Create(1, (PORT_MAX+1) );
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

WORD GameSession::AllocGameKey() {
    return m_cObjKey.GetKey();
}

void GameSession::FreeGameKey( WORD _wIndex ) {
    m_cObjKey.RestoreKey( _wIndex );
}

BOOL GameSession::SetUserGame( WORD wIndex, UserGame * pUser )
{
	if ( wIndex == 0 ) {
		return FALSE;
	}
	m_pUserGame[wIndex] = pUser;
	return TRUE;
}

UserGame * GameSession::GetUserGame( WORD wIndex )
{
    if ( wIndex == 0 ) {
		return NULL;
	}
	return m_pUserGame[wIndex];
}
