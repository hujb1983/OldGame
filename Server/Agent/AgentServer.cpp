#include "AgentServer.h"
#include "InfoParser.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

NetworkObject * CreateClientSideAcceptedObject();
VOID DestroyClientSideAcceptedObject( NetworkObject * pNetworkObject );
VOID DestroyClientSideConnectedObject( NetworkObject * pNetworkObject );

AgentServer * g_AgentServer = NULL;

AgentServer::AgentServer(void)
{
	m_bShutdown		= FALSE;
	m_pIOCPServer 	= NULL;
	m_pGameServer 	= NULL;
	m_pLobbyServer  = NULL;
}

AgentServer::~AgentServer(void)
{
	if ( m_pIOCPServer ) {
		delete m_pIOCPServer;
	}

	if ( m_pLobbyServer ) {
		delete m_pLobbyServer;
	}
}

BOOL AgentServer::Init()
{
    m_cObjKey.Create(1, 0xFFFF);

	AgentFactory::Instance()->Init();

	BOOL bRet = g_InfoParser.Init("./ServerInfo.ini");
	if ( !bRet ) {
		printf("Parse Server Info fail\n");
		return FALSE;
	}

	m_pIOCPServer = new IOCPServer;
	SYNCHANDLER_DESC desc[2];
	desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	desc[0].dwMaxAcceptSession			= 3;
	desc[0].dwMaxConnectSession			= 3;
	desc[0].dwSendBufferSize			= 2000000;
	desc[0].dwRecvBufferSize		 	= 2000000;
	desc[0].dwTimeOut				    = 0;
	desc[0].dwNumberOfAcceptThreads		= 1;
	desc[0].dwNumberOfIoThreads			= 8;
	desc[0].dwNumberOfConnectThreads	= 1; // Agent Server don't Active connect.
	desc[0].dwMaxPacketSize				= 60000; //4096
	desc[0].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;
	desc[1].dwSyncHandlerKey 			= CLIENT_SYNCHANDLER;
	desc[1].dwMaxAcceptSession			= 700;
	desc[1].dwMaxConnectSession			= 0;
	desc[1].dwSendBufferSize			= 1024 * 60; //60000
	desc[1].dwRecvBufferSize			= 1024 * 60; //60000
	desc[1].dwTimeOut				    = 0;
	desc[1].dwNumberOfAcceptThreads		= 1;
	desc[1].dwNumberOfIoThreads			= 8;
	desc[1].dwNumberOfConnectThreads	= 0;
	desc[1].dwMaxPacketSize				= 1024 * 10;	//4096
	desc[1].fnCreateAcceptedObject		= CreateClientSideAcceptedObject;
	desc[1].fnDestroyAcceptedObject		= DestroyClientSideAcceptedObject;
	desc[1].fnDestroyConnectedObject	= DestroyClientSideConnectedObject;

	if ( !m_pIOCPServer->Init(desc, 2) ) {
		return FALSE;
	}

	StartServerSideListen();
	StartClientSideListen();

	m_pGameServer = AgentFactory::Instance()->AllocGameSession();
	if ( m_pGameServer == NULL) {
		DEBUG_MSG( LVL_DEBUG, "AllocGameSession fail.");
		return FALSE;
	}

	m_pLobbyServer = AgentFactory::Instance()->AllocLobbySession();
	if ( m_pLobbyServer == NULL) {
		DEBUG_MSG( LVL_DEBUG, "AllocLobbySession fail.");
		return FALSE;
	}
    else {
        SERVER_INFO info = g_InfoParser.GetServerInfo( LOBBY_SERVER );
        m_pLobbyServer->SetAddr( info.m_strIp, info.m_dwPort ); // Agent Port 8100
    }

	return TRUE;
}

void AgentServer::StartServerSideListen()
{
	if( !m_pIOCPServer->IsListening( SERVER_SYNCHANDLER) ) {

		if ( !m_pIOCPServer->StartListen(SERVER_SYNCHANDLER, "", 8100) ) // AGENT Port 8100
		{
			return;
		}
	}
}

void AgentServer::StartClientSideListen()
{
	if ( !m_pIOCPServer->IsListening( CLIENT_SYNCHANDLER) ) {

		if ( !m_pIOCPServer->StartListen(CLIENT_SYNCHANDLER, "", 1234) ) // Clinet Port 1234
		{
			return;
		}
	}
}

BOOL AgentServer::ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort )
{
	if (pSession == NULL) {
		return FALSE;
	}
	return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, pszIP, wPort );
	//return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, "127.0.0.1", 7000 );
}

BOOL AgentServer::MaintainConnection()
{
	if (m_bShutdown) {
		return TRUE;
	}

	// 主动连接 Game Server
	if ( m_pLobbyServer ) {
		if ( m_pLobbyServer->TryToConnect() ) {
			ConnectToServer( m_pLobbyServer, (char *)m_pLobbyServer->GetConnnectIP().c_str(), m_pLobbyServer->GetConnnectPort() );
		}
	}
}

BOOL AgentServer::Update( DWORD dwDeltaTick )
{
	if(m_pIOCPServer) {
		m_pIOCPServer->Update();
	}

	MaintainConnection();
	return TRUE;
}

BOOL AgentServer::SendToGameServer( BYTE * pMsg, WORD wSize) {
	if ( m_pGameServer ) {
		return m_pGameServer->Send( pMsg, wSize );
	}
	return FALSE;
}
ServerSession * AgentServer::GetGameSession() const {
	return m_pGameServer;
}

BOOL AgentServer::SendToLobbyServer( BYTE * pMsg, WORD wSize) {
	if ( m_pLobbyServer ) {
		return m_pLobbyServer->Send( pMsg, wSize );
	}
	return FALSE;
}
ServerSession * AgentServer::GetLobbySession() const {
    return m_pLobbyServer;
}

// User Server;
BOOL AgentServer::SendToClient( WORD wIndex, BYTE * pMsg, WORD wSize )
{
	UserSession * pSession = m_pUserSession[wIndex];
	if ( pSession != NULL ) {
        char szBuff[40960] = {0};
        CMsgBuff msgBuff;
        msgBuff.SetBuff(szBuff, sizeof(szBuff) );
        msgBuff.Write(wSize);
        msgBuff.Write((char*)pMsg);
		pSession->Send( msgBuff.GetHead(), msgBuff.GetWriteLen()-1 );
		return TRUE;
	}
    return FALSE;
}

BOOL AgentServer::SendToClient( BYTE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return FALSE;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );
	WORD wIndex = (WORD) _userport;

    return SendToClient( wIndex, pMsg, wSize );
}
BOOL AgentServer::SetUserSession( WORD wIndex, UserSession * pSession )
{
	if ( wIndex == 0 ) {
		return FALSE;
	}
	m_pUserSession[wIndex] = pSession;
	return TRUE;
}
UserSession * AgentServer::GetUserSession( WORD wIndex )
{
	if ( wIndex == 0 ) {
		return NULL;
	}
	return m_pUserSession[wIndex];
}
WORD AgentServer::AllocSessionKey() {
    return m_cObjKey.GetKey();
}
void AgentServer::FreeSessionKey( WORD _wIndex ) {
    m_cObjKey.RestoreKey( _wIndex );
}


#if 0
BOOL AgentServer::ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort )
{
	//printf("AgentServer::ConnectToServer Function\n");
	if (pSession == NULL) {
		return FALSE;
	}
	//printf("IP = %s, Port = %d\n", pszIP, wPort);
	return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, pszIP, wPort );
	//return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, "127.0.0.1", 3456 );
}
#endif

///////////////////////////////////////////////////////////////

NetworkObject * CreateServerSideAcceptedObject() {
    DEBUG_MSG( LVL_TRACE, "CreateServerSideAcceptedObject. \n");
	TempSession * obj = AgentFactory::Instance()->AllocTempSession();
	if ( obj == NULL) {
        DEBUG_MSG( LVL_ERROR, "AllocTempSession Fail . \n");
		return NULL;
	}
	return (NetworkObject *)(obj);
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs ) {
	DEBUG_MSG( LVL_TRACE, "DestroyServerSideAcceptedObject. \n");

	ServerSession * pSession = (ServerSession *)pObjs;
	eSERVER_TYPE eType = pSession->GetServerType();
	if ( eType == GAME_SERVER ) {
		DEBUG_MSG( LVL_TRACE, ">>>FreeGameSession( %x )\n", pObjs);
		GameSession * obj = (GameSession *)pObjs;
		AgentFactory::Instance()->FreeGameSession(obj);
	}
	else if ( eType == TEMP_SERVER ) {
		DEBUG_MSG( LVL_TRACE, ">>>FreeTempSession( %x )\n", pObjs);
		TempSession * obj = (TempSession *)pObjs;
		AgentFactory::Instance()->FreeTempSession(obj);
    }
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
     DEBUG_MSG( LVL_TRACE, "DestroyServerSideConnectedObject. \n");
}

// 客户端
NetworkObject * CreateClientSideAcceptedObject() {
    DEBUG_MSG( LVL_TRACE, "CreateClientSideAcceptedObject. \n");
	UserSession * obj = AgentFactory::Instance()->AllocUserSession();
	if ( obj == NULL) {
		DEBUG_MSG( LVL_TRACE, "AllocUserSession Fail.\n");
		return NULL;
	}
	obj->Init();
	return (NetworkObject *)(obj);
}

VOID DestroyClientSideAcceptedObject( NetworkObject * pObjs ) {
    DEBUG_MSG( LVL_TRACE, "DestroyClientSideAcceptedObject. \n");
	UserSession * pSession = (UserSession *)pObjs;
	assert( pSession );
	pSession->Release();
}

VOID DestroyClientSideConnectedObject( NetworkObject * pNetworkObject ) {
	DEBUG_MSG( LVL_TRACE, "DestroyClientSideConnectedObject. \n");
}
