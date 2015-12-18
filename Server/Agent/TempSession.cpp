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

	printf("Enter TempSession::OnRecv.\n");
	MSG_SERVER_TYPE * recvMsg = (MSG_SERVER_TYPE *)pMsg;
	if ( recvMsg->m_byServerType == GAME_SERVER ) {

		printf("\ng_AgentServer->GetGameSession Succ.\n");
		ServerSession * Obj = g_AgentServer->GetGameSession();
		if ( Obj == NULL) {
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}

        char szMsg[256] = {"Hello World!"};
        printf( "%s \n", szMsg );
        Obj->Send( (BYTE*)szMsg, strlen(szMsg) );
	}
	else if ( recvMsg->m_byServerType == LOBBY_SERVER ) {

        printf("\ng_AgentServer->GetLobbySession Succ.\n");
		ServerSession * Obj = g_AgentServer->GetLobbySession();
		if ( Obj == NULL) {
			return;
		}

		Session * pSession = this->m_pSession;
		if ( pSession != NULL ) {
			m_pSession->UnbindNetworkObject();
			pSession->BindNetworkObject(Obj);
		}

        char szMsg[256] = {"Hello World!"};
        printf( "%s \n", szMsg );
        Obj->Send( (BYTE*)szMsg, strlen(szMsg) );
	}
	AgentFactory::Instance()->FreeTempSession(this);
}

void TempSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void TempSession::OnLogString( char * pszLog)
{

}
