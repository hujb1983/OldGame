#include "TempSession.h"
#include "DBServer.h"

TempSession::TempSession()
	: m_bFirst(TRUE)
{
}

TempSession::~TempSession()
{

}

void TempSession::Clear()
{
	m_bFirst = TRUE;
	ServerSession::Clear();
}

void TempSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	assert( m_bFirst == TRUE);
	if ( !m_bFirst ) {
		return;
	}

    m_bFirst = FALSE;

	printf("Enter TempSession::OnRecv.\n");

	MSG_SERVER_TYPE * pRecvMsg = (MSG_SERVER_TYPE *)pMsg;
	if ( pRecvMsg->m_byServerType == GAME_SERVER ) {
		printf("2.Turn to Game_SERVER\n");
		ServerSession * Obj = g_DBServer->GetGameSession();
		if ( Obj == NULL) {
			printf("\ng_DBServer->GetGameSession Fail.\n");
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}
	}
	else if ( pRecvMsg->m_byServerType == LOBBY_SERVER ) {
		printf("2.Turn to Lobby_SERVER\n");
		ServerSession * Obj = g_DBServer->GetLobbySession();
		if ( Obj == NULL) {
			printf("\ng_DBServer->GetLobbySession Fail.\n");
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}
	}
	else if ( pRecvMsg->m_byServerType == LOGIN_SERVER ) {
		printf("2.Turn to Login_SERVER\n");
		ServerSession * Obj = g_DBServer->GetLoginSession();
		if ( Obj == NULL) {
			printf("\ng_DBServer->GetLobbySession Fail.\n");
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}
	}

	printf("\n>>>> FreeTempSession <<<<\n");
	DBFactory::Instance()->FreeTempSession(this);
}

void TempSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void TempSession::OnLogString( char * pszLog)
{

}

