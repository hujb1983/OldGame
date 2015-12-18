#include "UserSession.h"
#include "PacketHandler.h"
#include "AgentServer.h"
#include "AgentFactory.h"

#include <Public.h>

DWORD UserSession::m_dwClientDelay = 0xFFFFFFFF;
DWORD UserSession::m_dwServerDelay = 0xFFFFFFFF;

UserSession::UserSession()
: m_bFirst(TRUE)
{
	m_eUserType = UT_USER;
}

UserSession::~UserSession()
{
}

BOOL UserSession::SendPacket(BYTE *pMsg, WORD wSize)
{
	return Send( (BYTE *)pMsg, wSize );
}

WORD UserSession::GetUserKey() const        { return m_wUserKey;    }
void UserSession::SetUserKey(WORD dwKey)    { m_wUserKey = dwKey;   }
WORD UserSession::GetBattleKey() const      { return m_wBattleKey;  }
void UserSession::SetBattleKey(WORD dwKey)  { m_wBattleKey = dwKey; }
WORD UserSession::GetUserid() const        { return m_dwUserid;    }
void UserSession::SetUserid(UINT dwID)    { m_dwUserid = dwID;   }

/************ protected *************/
void UserSession::Init()
{
	m_wUserKey 		= 0;
	m_bFirst 		= TRUE;

	DWORD dwCurrent = Session::GetTickCount();
	m_dwOvertime    = dwCurrent + UserSession::m_dwClientDelay;
	printf( "[UserSession::Init %d] \n", m_dwOvertime);

	this->NotPackageHeader();

	m_wUserKey   = 0;       // 用户健值
    m_wBattleKey = 0;       // 房间号
    m_dwUserid   = 0;       // 用户ID
    memset( m_szName, 0x0, sizeof(m_szName) ); // 名称
    m_iMoneys    = 0;       // 点数
    m_uiWons     = 0;       // 赢的次数
	m_uiFaileds  = 0; 	    // 失败次数
	m_uiAways    = 0; 		// 逃跑次数
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

	g_AgentServer->SetUserSession( this->m_wUserKey, NULL);

	AgentFactory::Instance()->FreeUserSession(this);
}

void UserSession::OnAccept( DWORD dwNetworkIndex )
{
	WORD _wUserKey = g_AgentServer->AllocSessionKey() ;
	if ( _wUserKey == 0 ) {
	    return;
    }

    this->SetUserKey( _wUserKey );
    g_AgentServer->SetUserSession( _wUserKey, this);


	char buff[1024]  =  {0};
	char format[256] = 	"{\"protocol\":\"%d\",\"data\":{\"type\":\"text/json\"}}";

	MSG_ENTERSERVER_ANC msg2;
	sprintf( buff, format, msg2.m_dwProtocol);

	printf("[UserSession::OnAccept %s] \n", buff);
	WORD len = strlen(buff);

    CMsgBuff msgBuff;
    char szBuff[256] = {0};
    msgBuff.SetBuff(szBuff, 256);
    msgBuff.Write(len);
    msgBuff.Write( (char*)buff );
    this->Send( msgBuff.GetHead(), msgBuff.GetWriteLen()-1 );

	//this->Send( (BYTE*) buff,  len );
	printf("[UserSession::OnAccept] = %d, %d \n", len, msgBuff.GetWriteLen() );
}

void UserSession::OnDisconnect()
{
	printf("[UserSession::OnDisconnect]\n");

    WORD  _userkey   = this->GetUserKey();
    WORD  _battlekey = this->GetBattleKey();

    {
        char buff[1024]  =  {0};
        char format[256] = 	"{ \"protocol\":\"%d\", \"userkey\":\"%d\", \"battleid\":\"%d\" }";
        sprintf( buff, format, MAKEDWORD( Login_Protocol, Offline_NAK ),
             _userkey, _battlekey);
        int len = strlen(buff);
        g_AgentServer->SendToGameServer( (BYTE*)buff, len );
    }

	{
	    g_AgentServer->FreeSessionKey(_userkey);
        NetworkObject::OnDisconnect();
        AgentFactory::Instance()->FreeUserSession( this );
	}
}

void UserSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	printf(">>>> [UserSession::OnRecv]\n");

	BYTE msgPlus[1024] = {0};

	// Alloc Port
	if ( m_wUserKey != 0 ) {

        /*
		char json_msgs[1024] = {0};
		memcpy(json_msgs, pMsg, wSize);

		JsonMap js_map;
        if ( js_map.set_json( (char *) pMsg ) == -1 ) {
            return;
        }

        int _errmsg;
        js_map.ReadInteger( "protocol", _errmsg );
        */

		g_PacketHandler.ParsePacket_Client(this, (MSG_BASE*)pMsg, wSize);
	}
}

void UserSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void UserSession::OnLogString( char * pszLog)
{

}

void UserSession::DBResult( WORD cate, WORD ptcl, QueryResult * pData )
{
    /*
    MSG_DBPROXY_RESULT msg;
    msg.m_byCategory = cate;
    msg.m_byProtocol = ptcl;
    msg.m_pData = pData;
    g_PacketHandler.ParsePacket_Database( (ServerSession*)this, (MSG_BASE*)&msg, sizeof(msg) );
    */
}


void UserSession::Login( ServerSession * pServerSession )
{
   Session * pSession = pServerSession->GetSesstion();
   if ( pSession != NULL ) {
        m_pSession->UnbindNetworkObject();
        pSession->BindNetworkObject(this);
    }
}

void UserSession::setName( char * _name, int _size ) {
    memset( m_szName, 0x0, sizeof(m_szName) );
    memcpy( m_szName, _name, _size );
}

void UserSession::setGamesinfo( int _point, UINT _wons, UINT _failed, UINT _aways ) {
    m_iMoneys   = _point;           // 点数
    m_uiWons    = _wons;            // 赢的次数
	m_uiFaileds = _failed; 	        // 失败次数
	m_uiAways   = _aways; 		    // 逃跑次数
}

void UserSession::setMoney( int _money ) {
    m_iMoneys = _money;
}

void UserSession::getSeatInfo( char * _seatinfo, int _size )
{
    char _buff[256]   = {0};
    char _format[256] = "{\"show\":\"true\","
                         "\"name\":\"%s\","
                         "\"rate\":\"%.0f\","
                         "\"money\":\"%d\"}";

    float _rate = 0;
    float _sum = (m_uiWons + m_uiFaileds + m_uiAways);

    if (_sum>0) {
        _rate = m_uiWons / _sum * 100;
    }

    snprintf( _buff, sizeof(_buff), _format, m_szName, _rate, m_iMoneys );

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _seatinfo, _buff );
    }
}
