#include "LobbyServer.h"
#include "InfoParser.h"
#include "LobbyUpdate.h"
//#include "GameFactory.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

LobbyServer * g_LobbyServer = NULL;

LobbyServer::LobbyServer(void)
{
	m_bShutdown = FALSE;
	m_pIOCPServer = NULL;

	// ...
	m_pDBSession = NULL;
}

LobbyServer::~LobbyServer(void)
{
	if ( m_pIOCPServer )
		delete m_pIOCPServer;
}

BOOL LobbyServer::Init()
{
	LobbyFactory::Instance()->Init();

	char szInfo[256] = "./ServerInfo.ini";
	BOOL bRet = g_InfoParser.Init( szInfo );
	if ( !bRet ) {
		printf("Parse Server Info fail\n");
		return FALSE;
	}

	m_pIOCPServer = new IOCPServer;

	SYNCHANDLER_DESC desc;

	desc.dwSyncHandlerKey				= SERVER_SYNCHANDLER;
	desc.dwMaxAcceptSession				= 9;
	desc.dwMaxConnectSession			= 2;
	desc.dwSendBufferSize				= 2000000;
	desc.dwRecvBufferSize		 		= 2000000;
	desc.dwTimeOut						= 0;
	desc.dwNumberOfAcceptThreads		= 1; // 0
	desc.dwNumberOfIoThreads			= 8; // 开辟8个线程
	desc.dwNumberOfConnectThreads		= 1;
	desc.dwMaxPacketSize				= 60000; // 4096
	desc.fnCreateAcceptedObject			= CreateServerSideAcceptedObject;
	desc.fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc.fnDestroyConnectedObject		= DestroyServerSideConnectedObject;

	if ( !m_pIOCPServer->Init(&desc, 1) ) {
		return FALSE;
	}

	StartServerSideListen();

	// 主动连接 DB
	m_pDBSession = LobbyFactory::Instance()->AllocDBSession();
	if ( m_pDBSession ) {
		SERVER_INFO info = g_InfoParser.GetServerInfo( DB_SERVER );
		m_pDBSession->SetAddr( info.m_strIp, info.m_dwPort ); // DB Port 7030
	}

	return TRUE;
}

void LobbyServer::StartServerSideListen()
{
	if ( !m_pIOCPServer->IsListening( SERVER_SYNCHANDLER) ) {
		if ( !m_pIOCPServer->StartListen(SERVER_SYNCHANDLER, "", 9100) ) // Clinet Port 1234
		{
			return;
		}
	}
}

BOOL LobbyServer::ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort )
{
	if (pSession == NULL) {
		return FALSE;
	}
	return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, pszIP, wPort );
}

BOOL LobbyServer::MaintainConnection()
{
	if (m_bShutdown) {
		return FALSE;
	}

	if ( m_pDBSession ) {
		if ( m_pDBSession->TryToConnect() ) {
			ConnectToServer( m_pDBSession, (char *)m_pDBSession->GetConnnectIP().c_str(), m_pDBSession->GetConnnectPort() );
		}
	}
}

BOOL LobbyServer::Update( DWORD dwDeltaTick )
{
	if ( m_pIOCPServer ) {
		m_pIOCPServer->Update();
	}

	// Connect other Servers
	MaintainConnection();

	return TRUE;
}

BOOL LobbyServer::SendToDBServer( BYTE * pMsg, WORD wSize)
{
	printf("[LobbyServer::SendToDBServer]\n");

	if ( m_pDBSession ) {
		return m_pDBSession->Send( pMsg, wSize );
	}
	return FALSE;
}

//////////////////////////////////////////////////////
NetworkObject * CreateServerSideAcceptedObject() {
	printf("LobbyServer::CreateServerSideAcceptedObject \n");
	TempSession * obj = LobbyFactory::Instance()->AllocTempSession();
	if ( obj == NULL) {
		printf("LobbyFactory::Instance()->AllocTempSession() Fail.\n");
		return NULL;
	}
	return (NetworkObject *)(obj);
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs ) {
	printf("LobbyServer::DestroyServerSideAcceptedObject \n");

	ServerSession * pSession = (ServerSession *)pObjs;
	eSERVER_TYPE eType = pSession->GetServerType();

	if ( eType == AGENT_SERVER ) {
		printf(">>>FreeAgentSession( %x )\n", pObjs);
		AgentSession * obj = (AgentSession *)pObjs;
		LobbyFactory::Instance()->FreeAgentSession(obj);
	}
	else if ( eType == TEMP_SERVER ) {
		printf(">>>FreeTempSession( %x )\n", pObjs);
		TempSession * obj = (TempSession *)pObjs;
		LobbyFactory::Instance()->FreeTempSession(obj);
    }
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
	printf("LobbyServer::DestroyServerSideConnectedObject\n");
}
