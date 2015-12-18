#include "ServerSession.h"
#include "DBServer.h"

void CallBackQueryResult( DWORD dwIndex, QueryResult * pData)
{
    WORD byPacketCategory = (WORD) LOWORD( dwIndex );
    WORD byPacketType     = (WORD) HIWORD( dwIndex );
    ((ServerSession*)pData->GetVoidObject())->DBResult(byPacketCategory, byPacketType, pData);
}

void CallBackMessage( char * szMsg)
{

}

ServerSession::ServerSession()
{
}

ServerSession::~ServerSession()
{
}

void ServerSession::Init()
{
	this->Clear();

}

BOOL ServerSession::InitDatabase()
{
    HyDatabaseDesc db_descNew;
	db_descNew.DataBaseModuleType			= DBCInterfaceType_MYSQL;
	db_descNew.dwQueryPoolSize				= 2000;
	db_descNew.wQueryProcessorNum			= 1;
	db_descNew.pszDatabaseIP				= "10.60.22.39";
	db_descNew.pszDatabaseName				= "db_passport";
	db_descNew.pszUserName					= "root";
	db_descNew.pszUserPassword				= "123qwe";
	db_descNew.fnErrorMessage				= CallBackMessage;
	db_descNew.fnQueryResult                = CallBackQueryResult;

	if ( !m_Database.Initialize(db_descNew) ) {
        printf( "Connect to db failed!");
        return FALSE;
    }
    printf( "Connect to db succ!");
    return TRUE;
}

void ServerSession::Clear()
{
	m_dwSessionIndex 	    = 0;
	m_wConnectPort 		    = 0;
	m_bForConnect 		    = FALSE;
	m_dwLastHeartbeatTime   = Session::GetTickCount();
	m_bConnection 		    = FALSE;
	m_strConnectIP.clear();
}


void ServerSession::Update()
{
#if 0
	if ( this->IsForConnect() )
	{
		DWORD dwCurTick = Session::GetTickCount();
		if ( dwCurTick - m_dwLastHeartbeatTime > 10000)
		{
			m_dwLastHeartbeatTime = dwCurTick;

			MSG_HEARTBEAT msg;
			msg.m_byCategory	= 0;
	//		msg.m_byProtocol    = SERVERCOMMON_HEARTBEAT;
			Send( (BYTE*)&msg, sizeof(MSG_HEARTBEAT) );
		}
	}
#endif
}

void ServerSession::Release()
{
	m_strConnectIP.clear();
}

void ServerSession::OnAccept( DWORD dwNetworkIndex )
{
	SetForConnect( FALSE );
	SetSessionIndex( dwNetworkIndex );
	m_bConnection = TRUE;
}

void ServerSession::OnConnect( BOOL bSucces, DWORD dwNetworkIndex )
{
	if ( bSucces )
	{
		SetForConnect( TRUE );
		SetSessionIndex( dwNetworkIndex );
		m_bConnection = TRUE;
	}
}

void ServerSession::OnDisconnect()
{
	m_bConnection = FALSE;
}

void ServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
}

void ServerSession::OnLogString( char * pszLog )
{

}

void ServerSession::SendServerType()
{
	MSG_SERVER_TYPE 	msg;
	msg.m_byCategory 	= 0;
	msg.m_byCategory 	= 0;
	msg.m_byServerType 	= GetServerType(); // AGENT_SERVER
	Send( (BYTE *)&msg, sizeof(MSG_SERVER_TYPE) );

}

void ServerSession::SetAddr( char * pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

void ServerSession::TryToConnect()
{
	if ( m_strConnectIP.empty() ) {
		return;
	}

	//g_AgentServer->ConnectToServer( this, (char *)m_strConnectIP.c_str(), m_wConnectPort );
}

void ServerSession::DBQuery( Query * pQuery ) {
    m_Database.DBQuery( pQuery );
}

