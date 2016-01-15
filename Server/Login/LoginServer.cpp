#include "MsgBuff.h"
#include "LoginServer.h"
#include "InfoParser.h"
#include "ObjKeyGenerator.h"

NetworkObject * CreateServerSideAcceptedObject();
VOID DestroyServerSideAcceptedObject( NetworkObject *pNetworkObject );
VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject );

NetworkObject * CreateClientSideAcceptedObject();
VOID DestroyClientSideAcceptedObject( NetworkObject * pNetworkObject );
VOID DestroyClientSideConnectedObject( NetworkObject * pNetworkObject );

LoginServer * g_pLoginServer 		= NULL;
DWORD LoginServer::m_dwClearDelay 	= 0xFFFFFFFF;

LoginServer::LoginServer(void)
{
    m_pDBServer     = NULL;

	m_bShutdown		= FALSE;
	m_pIOCPServer 	= NULL;

	memset( m_pUserSession, 0x0, PORT_MAX);

	m_lsOvertimeTable.clear();
}

LoginServer::~LoginServer(void)
{
	if ( m_pIOCPServer ) {
		delete m_pIOCPServer;
	}
}

BOOL LoginServer::Init()
{
    BOOL bRet = g_InfoParser.Init("./ServerInfo.ini");
	if ( !bRet ) {
		printf("Parse Server Info fail\n");
		return FALSE;
	}

    m_cObjKey.Create(1, 0xFFFF);

	//m_lsOvertimeTable.reserve(999);

	LoginFactory::Instance()->Init();

	m_pIOCPServer = new IOCPServer;

	SYNCHANDLER_DESC desc[2];

	desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	desc[0].dwMaxAcceptSession			= 0;
	desc[0].dwMaxConnectSession			= 1;
	desc[0].dwSendBufferSize			= 2000000;
	desc[0].dwRecvBufferSize		 	= 2000000;
	desc[0].dwTimeOut					= 0;
	desc[0].dwNumberOfAcceptThreads		= 0; // Accept
	desc[0].dwNumberOfIoThreads			= 1;
	desc[0].dwNumberOfConnectThreads	= 1; // Login Server don't Active connect.
	desc[0].dwMaxPacketSize				= 60000; //4096
	desc[0].fnCreateAcceptedObject		= CreateServerSideAcceptedObject;
	desc[0].fnDestroyAcceptedObject		= DestroyServerSideAcceptedObject;
	desc[0].fnDestroyConnectedObject	= DestroyServerSideConnectedObject;

	desc[1].dwSyncHandlerKey 			= CLIENT_SYNCHANDLER;
	desc[1].dwMaxAcceptSession			= 700;
	desc[1].dwMaxConnectSession			= 0;
	desc[1].dwSendBufferSize			= 1024 * 60; //60000
	desc[1].dwRecvBufferSize			= 1024 * 60; //60000
	desc[1].dwTimeOut					= 0;
	desc[1].dwNumberOfAcceptThreads		= 1; // Accept
	desc[1].dwNumberOfIoThreads			= 9;
	desc[1].dwNumberOfConnectThreads	= 0; // Connect
	desc[1].dwMaxPacketSize				= 1024 * 10;	//4096
	desc[1].fnCreateAcceptedObject		= CreateClientSideAcceptedObject;
	desc[1].fnDestroyAcceptedObject		= DestroyClientSideAcceptedObject;
	desc[1].fnDestroyConnectedObject	= DestroyClientSideConnectedObject;

	if ( !m_pIOCPServer->Init(desc, 2) ) {
		return FALSE;
	}

	StartClientSideListen();

	m_pDBServer = LoginFactory::Instance()->AllocDBSession();
	if ( m_pDBServer == NULL) {
		printf("[LoginFactory::Instance()->AllocDBSession] fail\n");
		return FALSE;
	}
	else {
        SERVER_INFO info = g_InfoParser.GetServerInfo( DB_SERVER );
        m_pDBServer->SetAddr( info.m_strIp, info.m_dwPort ); // Agent Port 8100
    }

	return TRUE;
}

void LoginServer::StartClientSideListen()
{
	if ( !m_pIOCPServer->IsListening( CLIENT_SYNCHANDLER) )
	{
		if ( !m_pIOCPServer->StartListen(CLIENT_SYNCHANDLER, "", 6010) ) // Clinet Port 1234
		{
			return;
		}
	}
}

BOOL LoginServer::ConnectToServer( ServerSession * pSession, char * pszIP, WORD wPort )
{
	if (pSession == NULL) {
		return FALSE;
	}
	return m_pIOCPServer->Connect( SERVER_SYNCHANDLER, (NetworkObject *)pSession, pszIP, wPort );
}

BOOL LoginServer::MaintainConnection()
{
	if ( m_bShutdown ) {
		return TRUE;
	}

	// 主动连接 DB Server
	if ( m_pDBServer ) {
		if ( m_pDBServer->TryToConnect() ) {
			// ConnectToServer( m_pDBServer, (char *)m_pDBServer->GetConnnectIP().c_str(), m_pDBServer->GetConnnectPort() );
			ConnectToServer( m_pDBServer, "127.0.0.1", 8900 );
		}
	}
}

BOOL LoginServer::Update( DWORD dwDeltaTick )
{
	if(m_pIOCPServer)
	{
		m_pIOCPServer->Update();
	}

	if ( dwDeltaTick > m_dwClearOvertime )
	{
		// OvertimeClear(dwDeltaTick);
		m_dwClearOvertime = dwDeltaTick + m_dwClearDelay;
		//printf( "[LoginServer::Update %d = %d] \n", m_dwClearOvertime, dwDeltaTick);
	}

	MaintainConnection();
	return TRUE;
}

BOOL LoginServer::OvertimeClear( DWORD dwDeltaTick )
{
	printf(" [ LoginServer::OvertimeClear dwDeltaTick= %d ] \n", dwDeltaTick);
	printf(" [ LoginServer::OvertimeClear size = %d ] \n", m_lsOvertimeTable.size());

	WORD wIndex = 0;
	UserSession * pSession = NULL;
	for (int i = 0; i< m_lsOvertimeTable.size(); ++i) {
		wIndex = m_lsOvertimeTable.front();
		if ( wIndex==0 ) {
			m_lsOvertimeTable.pop_front();
			continue;
		}
		printf(" [ LoginServer::OvertimeClear wIndex = %d ] \n", wIndex);
		pSession = m_pUserSession[wIndex];
		if ( pSession != NULL ) {
			printf(" [ LoginServer::OvertimeClear pSession = %d ] \n", pSession);
			if ( pSession->Update( dwDeltaTick ) ) {
				pSession->CloseSession();
			}
		}
		m_lsOvertimeTable.pop_front();
	}
}

BOOL LoginServer::SendToDBServer( BYTE * pMsg, WORD wSize)
{
    if ( m_pDBServer ) {
		return m_pDBServer->Send( pMsg, wSize );
	}
	return FALSE;
}

//ServerSession * LoginServer::GetKeyvSession() const {
//	return m_pKeyvServer;
//}

ServerSession * LoginServer::GetDBSession() const {
	return m_pDBServer;
}

// User Server;
BOOL LoginServer::SendToClient( WORD wIndex, BYTE * pMsg, WORD wSize )
{
	UserSession * pSession = m_pUserSession[wIndex];
	if ( pSession != NULL ) {
        /*CMsgBuff msgBuff;
        char szBuff[1024] = {0};
        msgBuff.SetBuff(szBuff, 1024);
        msgBuff.Write(wSize);
        msgBuff.Write((char*)pMsg);
		pSession->Send( msgBuff.GetHead(), msgBuff.GetWriteLen() );*/
        pSession->Send( pMsg, wSize);
		return TRUE;
	}
    return FALSE;
}

BOOL LoginServer::SendToClient( BYTE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return FALSE;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );
	WORD wIndex = (WORD) _userport;

	UserSession * pSession = m_pUserSession[wIndex];
	if ( pSession != NULL ) {
        /*CMsgBuff msgBuff;
        char szBuff[1024] = {0};
        msgBuff.SetBuff(szBuff, 1024);
        msgBuff.Write(wSize);
        msgBuff.Write((char*)pMsg);
		pSession->Send( msgBuff.GetHead(), msgBuff.GetWriteLen() );*/

        pSession->Send( pMsg, wSize);
		return TRUE;
	}
    return FALSE;
}

BOOL LoginServer::SetUserSession( WORD wIndex, UserSession * pSession )
{
	printf("[LoginServer::SetUserSession]\n");
	if ( wIndex == 0 ) {
		return FALSE;
	}

	m_pUserSession[wIndex] = pSession;
	return TRUE;
}

UserSession * LoginServer::GetUserSession( WORD wIndex )
{
	printf("[LoginServer::GetUserSession]\n");
	if ( wIndex == 0 ) {
		return NULL;
	}

	return m_pUserSession[wIndex];
}

WORD LoginServer::AllocSessionKey() {
    return m_cObjKey.GetKey();
}

void LoginServer::FreeSessionKey( WORD _wIndex ) {
    m_cObjKey.RestoreKey( _wIndex );
}


///////////////////////////////////////////////////////////////
// 服务器端
NetworkObject * CreateServerSideAcceptedObject() {
	printf("[LoginServer::CreateServerSideAcceptedObject]: Alloc TempServerSession.\n");
	return (NetworkObject *)(NULL);
}

VOID DestroyServerSideAcceptedObject( NetworkObject *pObjs ) {
	printf("[LoginServer::DestroyServerSideAcceptedObject] Function\n");
}

VOID DestroyServerSideConnectedObject( NetworkObject *pNetworkObject ) {
	printf("[LoginServer::DestroyServerSideConnectedObject]: Not Used.\n");
}

// 客户端
NetworkObject * CreateClientSideAcceptedObject() {
	printf("[LoginServer::CreateClientSideAcceptedObject]: Alloc UserSession.\n");

	UserSession * obj = LoginFactory::Instance()->AllocUserSession();
	if ( obj == NULL) {
		printf("\nLoginFactory::Instance()->AllocTempUserSession() Fail.\n");
		return NULL;
	}

	obj->Init();
	return (NetworkObject *)(obj);
}

VOID DestroyClientSideAcceptedObject( NetworkObject * pObjs ) {
	printf("[LoginServer::DestroyClientSideAcceptedObject]: Function\n");
	UserSession * pSession = (UserSession *)pObjs;
	pSession->Release();
}

VOID DestroyClientSideConnectedObject( NetworkObject * pNetworkObject ) {
	printf("[LoginServer::DestroyClientSideConnectedObject]: Not Used.\n");
}
