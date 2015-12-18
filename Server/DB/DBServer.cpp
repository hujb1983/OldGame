#include "DBServer.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

DBServer * g_DBServer = NULL;

DBServer::DBServer(void)
{
	m_bShutdown = FALSE;
	m_pIOCPServer = NULL;

	m_pGameServer = NULL;
	m_pLobbyServer = NULL;
}

DBServer::~DBServer(void)
{
	if ( m_pIOCPServer )
		delete m_pIOCPServer;
}

BOOL DBServer::Init()
{
	DBFactory::Instance()->Init();

	m_pIOCPServer = new IOCPServer;

	SYNCHANDLER_DESC desc[1];

	desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	desc[0].dwMaxAcceptSession			= 9;
	desc[0].dwMaxConnectSession			= 9;
	desc[0].dwSendBufferSize			= 2000000;
	desc[0].dwRecvBufferSize		 	= 2000000;
	desc[0].dwTimeOut					= 0;
	desc[0].dwNumberOfAcceptThreads		= 1;
	desc[0].dwNumberOfIoThreads			= 1;
	desc[0].dwNumberOfConnectThreads	= 0; // DB Server don't Active connect.
	desc[0].dwMaxPacketSize				= 60000; //4096
	desc[0].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;

	if ( !m_pIOCPServer->Init(desc, 1) ) {
		return FALSE;
	}

	StartServerSideListen();

	m_pGameServer = DBFactory::Instance()->AllocGameSession();
	if ( m_pGameServer == NULL) {
		printf("[DBFactory::Instance()->AllocGameSession] fail\n");
		return FALSE;
	}
    if ( !m_pGameServer->InitDatabase() ) {
        printf("[DBServer] m_pGameServer->InitDatabase() fail\n");
        return FALSE;
    }

	m_pLobbyServer = DBFactory::Instance()->AllocLobbySession();
	if ( m_pLobbyServer == NULL) {
		printf("[DBFactory::Instance()->AllocLobbySession] fail\n");
		return FALSE;
	}
    if ( !m_pLobbyServer->InitDatabase() ) {
        printf("[DBServer] m_pLobbyServer->InitDatabase() fail\n");
        return FALSE;
    }

    m_pLoginServer = DBFactory::Instance()->AllocLoginSession();
	if ( m_pLoginServer == NULL) {
		printf("[DBFactory::Instance()->AllocLobbySession] fail\n");
		return FALSE;
	}
    if ( !m_pLoginServer->InitDatabase() ) {
        printf("[DBServer] m_pLobbyServer->InitDatabase() fail\n");
        return FALSE;
    }
	return TRUE;
}

void DBServer::StartServerSideListen()
{
	if( !m_pIOCPServer->IsListening( SERVER_SYNCHANDLER) )
    {
		if ( !m_pIOCPServer->StartListen(SERVER_SYNCHANDLER, "127.0.0.1", 8900) ) 	// DB Port 7300
		{
			return;
		}
	}
}

BOOL DBServer::MaintainConnection()
{
	if (m_bShutdown) {
		return TRUE;
	}
}

BOOL DBServer::Update( DWORD dwDeltaTick )
{
	if(m_pIOCPServer) {
		m_pIOCPServer->Update();
	}

	if ( m_pLobbyServer ) {
		m_pLobbyServer->Update();
	}

	if ( m_pGameServer ) {
		m_pGameServer->Update();
	}

	if ( m_pLoginServer ) {
		m_pLoginServer->Update();
	}
	// MaintainConnection();
	return TRUE;
}

BOOL DBServer::SendToGameServer( BYTE * pMsg, WORD wSize)
{
	printf("[DBServer::SendToGameServer]\n");

	if ( m_pGameServer ) {
		return m_pGameServer->Send( pMsg, wSize );
	}
	return FALSE;
}

BOOL DBServer::SendToLobbyServer( BYTE * pMsg, WORD wSize)
{
	printf("[DBServer::SendToLobbyServer]\n");

	if ( m_pLobbyServer ) {
		return m_pLobbyServer->Send( pMsg, wSize );
	}
	return FALSE;
}

BOOL DBServer::SendToLoginServer( BYTE * pMsg, WORD wSize)
{
	printf("[DBServer::SendToGameServer]\n");

	if ( m_pGameServer ) {
		return m_pGameServer->Send( pMsg, wSize );
	}
	return FALSE;
}

ServerSession * DBServer::GetGameSession() const {
    return m_pGameServer;
}

ServerSession * DBServer::GetLobbySession() const {
	return m_pLobbyServer;
}

ServerSession * DBServer::GetLoginSession() const {
	return m_pLoginServer;
}

///////////////////////////////////////////////////////////////
NetworkObject * CreateServerSideAcceptedObject() {
	TempSession * pObj = DBFactory::Instance()->AllocTempSession();
	if ( pObj == NULL) {
		printf("\nDBServer::Instance()->AllocTempSession() Fail.\n");
		return NULL;
	}
	printf("[DBServer::CreateServerSideAcceptedObject]: AllocTempSession.\n");
	return (NetworkObject *)(pObj);
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs ) {
	ServerSession * pSession = (ServerSession *)pObjs;
	eSERVER_TYPE eType = pSession->GetServerType();
	if ( eType == GAME_SERVER ) {
		printf(">>>FreeGameSession( %x )\n", pObjs);
		GameSession * obj = (GameSession *)pObjs;
		DBFactory::Instance()->FreeGameSession(obj);
	}
	else if ( eType == LOBBY_SERVER )
    {
		printf(">>>FreeLobbySession( %x )\n", pObjs);
		LobbySession * obj = (LobbySession *)pObjs;
		DBFactory::Instance()->FreeLobbySession(obj);
    }
    else if ( eType == TEMP_SERVER )
    {
		printf(">>>FreeTempSession( %x )\n", pObjs);
		TempSession * obj = (TempSession *)pObjs;
		DBFactory::Instance()->FreeTempSession(obj);
    }
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
	printf("[AgentServer::DestroyServerSideConnectedObject]: Not Used.\n");
}
