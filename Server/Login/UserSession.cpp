#include "UserSession.h"
#include "LoginServer.h"
#include "LoginFactory.h"
#include "PacketHandler.h"

DWORD UserSession::m_dwClientDelay = 0xFFFFFFFF;
DWORD UserSession::m_dwServerDelay = 0xFFFFFFFF;

UserSession::UserSession() : m_bFirst(TRUE)
{
}

UserSession::~UserSession()
{
}

BOOL UserSession::SendPacket(BYTE *pMsg, WORD wSize)
{
	return Send( (BYTE *)pMsg, wSize );
}

WORD UserSession::GetUserKey() const
{
	return m_wUserKey;
}

void  UserSession::SetUserKey(WORD dwKey)
{
	m_wUserKey = dwKey;
}

/************ protected *************/
void UserSession::Init()
{
	m_wUserKey 		= 0;
	m_bFirst 		= TRUE;

	DWORD dwCurrent = Session::GetTickCount();
	m_dwOvertime    = dwCurrent + UserSession::m_dwClientDelay;
	printf( "[UserSession::Init %d] \n", m_dwOvertime);

	this->NotPackageHeader();
}

BOOL UserSession::Update( DWORD dwDeltaTick )
{
	printf( "[UserSession::Update %d = %d] \n", dwDeltaTick, m_dwOvertime);

	// Count Down;
	if ( dwDeltaTick > m_dwOvertime ) {
		Disconnect(TRUE);
		return TRUE;
	}

	return FALSE;
}

void UserSession::CloseSession()
{
	printf(" [ UserSession::CloseSession ] \n");

	if ( m_pSession != NULL) {
		m_pSession->CloseSocket();
	}

	m_bFirst = TRUE;

}

void UserSession::Release()
{
	printf(" [ UserSession::Release ] \n");

	m_bFirst = TRUE;

	g_pLoginServer->SetUserSession( this->m_wUserKey, NULL);
	LoginFactory::Instance()->FreeUserSession(this);
}

void UserSession::OnAccept( DWORD dwNetworkIndex )
{
	WORD _wUserKey = g_pLoginServer->AllocSessionKey() ;
	if ( _wUserKey == 0 ) {
	    return;
    }

    this->SetUserKey( _wUserKey );
    g_pLoginServer->SetUserSession( _wUserKey, this);


	char buff[1024]  =  {0};
	char format[256] = 	"{\"protocol\":\"%d\",\"data\":{\"type\":\"text/json\"}}  ";

	MSG_ENTERSERVER_ANC msg2;
	sprintf( buff, format, msg2.m_dwProtocol);

	printf("[UserSession::OnAccept %s] \n", buff);
	WORD len = strlen(buff);

	/* CMsgBuff msgBuff;
    char szBuff[256] = {0};
    msgBuff.SetBuff(szBuff, 256);
    msgBuff.Write(len);
    msgBuff.Write( (char*)buff );
    this->Send( msgBuff.GetHead(), msgBuff.GetWriteLen() ); */
	this->Send( (BYTE*) buff,  len );
}

void UserSession::OnDisconnect()
{
	printf("[UserSession::OnDisconnect]\n");
	NetworkObject::OnDisconnect();

	{
	    g_pLoginServer->FreeSessionKey(m_wUserKey);
        NetworkObject::OnDisconnect();
        LoginFactory::Instance()->FreeUserSession( this );
	}
}

void UserSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf(">>>> [UserSession::OnRecv]\n");

	// Alloc Port
	BYTE msgPlus[1024] = {0};
	if ( m_wUserKey != 0 ) {
		g_PacketHandler.ParsePacket_Client(this, (MSG_BASE*)pMsg, wSize);
	}
}

void UserSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void UserSession::OnLogString( char * pszLog)
{

}