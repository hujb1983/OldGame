#include "TempSession.h"
#include "LobbyServer.h"
#include "LobbyUpdate.h"


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

	printf("[Enter TempSession::OnRecv].\n");
    LobbyFactory * pFactory = LobbyFactory::Instance();
    if ( pFactory ) {

        MSG_SERVER_TYPE * recvMsg = (MSG_SERVER_TYPE *)pMsg;
        if ( recvMsg->m_byServerType == AGENT_SERVER ) {

            ServerSession * Obj = pFactory->AllocAgentSession();
            if ( Obj == NULL) {
                return;
            }

            printf("[Factory->AllocAgentSession] Succ.\n");
            Session * pSession = this->m_pSession;
            if ( pSession != NULL ) {
                m_pSession->UnbindNetworkObject();
                pSession->BindNetworkObject(Obj);
            }

            g_LobbyUpdate.JoinAgentSession( (AgentSession*) Obj );

            /* char szMsg[256] = {" [AGENT_SERVER] Connected Succ!"};
            printf( "%s \n", szMsg );
            Obj->Send( (BYTE*)szMsg, strlen(szMsg) ); */
        }
	}

	LobbyFactory::Instance()->FreeTempSession(this);
}

void TempSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void TempSession::OnLogString( char * pszLog)
{

}

