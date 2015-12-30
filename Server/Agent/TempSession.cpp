#include "TempSession.h"
#include "AgentFactory.h"
#include "AgentServer.h"

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
	if ( !m_bFirst) {
		return;
	}
	m_bFirst = FALSE;

	DEBUG_MSG( LVL_TRACE, "TempSession::OnRecv.");
	MSG_SERVER_TYPE * recvMsg = (MSG_SERVER_TYPE *)pMsg;
	if ( recvMsg->m_byServerType == GAME_SERVER ) {

		DEBUG_MSG( LVL_TRACE, "g_AgentServer->GetGameSession Succ.");
		ServerSession * Obj = g_AgentServer->GetGameSession();
		if ( Obj == NULL) {
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}
	}
	else if ( recvMsg->m_byServerType == LOBBY_SERVER ) {

        DEBUG_MSG( LVL_TRACE, "g_AgentServer->GetLobbySession Succ.");
		ServerSession * Obj = g_AgentServer->GetLobbySession();
		if ( Obj == NULL) {
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}
	}
	AgentFactory::Instance()->FreeTempSession(this);
}

void TempSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void TempSession::OnLogString( char * pszLog)
{

}
