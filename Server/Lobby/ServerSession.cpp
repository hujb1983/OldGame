#include "ServerSession.h"

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
    printf("OnConnect(%d)! \n", bSucces);
	if ( bSucces )
	{
		SetForConnect( TRUE );
		SetSessionIndex( dwNetworkIndex );
		m_bConnection = TRUE;
	}
}

void ServerSession::OnDisconnect()
{
    printf("OnDisconnect(%d)! \n", m_bConnection);
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
    printf("ServerSession::SendServerType(%d,%d) \n", GetServerType(), LOBBY_SERVER);
}

void ServerSession::SetAddr( char * pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

BOOL ServerSession::TryToConnect()
{
	if ( m_strConnectIP.empty() ) {
		return FALSE;
	}

	if ( m_bConnection )
		return FALSE;

	return TRUE;
}

void ServerSession::DBQuery( Query * pQuery ) {
}

