#include "CnpokerServer.h"
#include "InfoParser.h"
#include "CnpokerFactory.h"
#include "GameMgr.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

CnpokerServer * g_pCnpokerServer = NULL;

CnpokerServer::CnpokerServer(void)
{
	m_bShutdown = FALSE;
	m_pIOCPServer = NULL;

	m_pDBSession = NULL;
	m_pAgentSession = NULL;
}

CnpokerServer::~CnpokerServer(void)
{
	if ( m_pIOCPServer )
		delete m_pIOCPServer;
}

BOOL CnpokerServer::Init()
{
	CnpokerFactory::Instance()->Init(); // Factory <GameUser>
	BOOL bRet = g_InfoParser.Init("./ServerInfo.ini");
	if ( !bRet ) {
		printf("Parse Server Info fail\n");
		return FALSE;
	}

	m_pIOCPServer = new IOCPServer;

	SYNCHANDLER_DESC desc;

	desc.dwSyncHandlerKey				= SERVER_SYNCHANDLER;
	desc.dwMaxAcceptSession				= 0;
	desc.dwMaxConnectSession			= 5;
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

	// 主动连接 Agent
	m_pAgentSession = CnpokerFactory::Instance()->AllocAgentSession();
	if ( m_pAgentSession ) {
		SERVER_INFO info = g_InfoParser.GetServerInfo( AGENT_SERVER );
		m_pAgentSession->SetAddr( info.m_strIp, info.m_dwPort ); // Agent Port 8100
	}

	// 主动连接 DB
	m_pDBSession = CnpokerFactory::Instance()->AllocDBSession();
	if ( m_pDBSession ) {
		SERVER_INFO info = g_InfoParser.GetServerInfo( DB_SERVER );
		m_pDBSession->SetAddr( info.m_strIp, info.m_dwPort ); // DB Port 7030
	}

	return TRUE;
}

BOOL CnpokerServer::ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort )
{
	if (pSession == NULL) {
		return FALSE;
	}

	return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, pszIP, wPort );
}

BOOL CnpokerServer::MaintainConnection()
{
	if (m_bShutdown) {
		return FALSE;
	}

	if ( m_pAgentSession ) {
		if ( m_pAgentSession->TryToConnect() ) {
			ConnectToServer( m_pAgentSession, (char *)m_pAgentSession->GetConnnectIP().c_str(), m_pAgentSession->GetConnnectPort() );
		}
	}

	if ( m_pDBSession ) {
		if ( m_pDBSession->TryToConnect() ) {
			ConnectToServer( m_pDBSession, (char *)m_pDBSession->GetConnnectIP().c_str(), m_pDBSession->GetConnnectPort() );
		}
	}
}

BOOL CnpokerServer::Update( DWORD dwDeltaTick )
{
	if ( m_pIOCPServer ) {
		m_pIOCPServer->Update();
	}

	MaintainConnection();
	return TRUE;
}

BOOL CnpokerServer::SendToDBServer( BYTE * pMsg, WORD wSize)
{
	if ( m_pDBSession ) {
		return m_pDBSession->Send( pMsg, wSize );
	}
	return FALSE;
}

BOOL CnpokerServer::SendToAgentServer( BYTE * pMsg, WORD wSize)
{
	if ( m_pAgentSession ) {
		return m_pAgentSession->Send( pMsg, wSize );
	}
	return FALSE;
}

//////////////////////////////////////////////////////

NetworkObject * CreateServerSideAcceptedObject() {
	//ServerSession * obj = GameFactory::Instance()->AllocGameUser();
	//return obj;
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject ) {
	//AgentFactory::Instance()->FreeUserSession(pNetworkObject);
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
}
