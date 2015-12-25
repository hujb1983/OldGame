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
BYTE UserSession::GetSeatKey() const        { return m_bySeatKey;   }
void UserSession::SetSeatKey(BYTE dwKey)    { m_bySeatKey = dwKey;  }
WORD UserSession::GetBattleKey() const      { return m_wBattleKey;  }
void UserSession::SetBattleKey(WORD dwKey)  { m_wBattleKey = dwKey; }
WORD UserSession::GetUserid() const         { return m_dwUserid;    }
void UserSession::SetUserid(UINT dwID)      { m_dwUserid = dwID;    }

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

	m_bReady     = FALSE;
    m_bShowcards = FALSE;
}

BOOL UserSession::Update( DWORD dwDeltaTick )
{
	DEBUG_MSG( LVL_TRACE, "[UserSession::Update %d = %d]", dwDeltaTick, m_dwOvertime);

	// Count Down;
	if ( dwDeltaTick > m_dwOvertime ) {
		Disconnect(TRUE);
		return TRUE;
	}

	return FALSE;
}

void UserSession::CloseSession()
{
	DEBUG_MSG( LVL_TRACE, " UserSession::CloseSession ");

	if ( m_pSession != NULL) {
		m_pSession->CloseSocket();
	}
	m_bFirst = TRUE;
}

void UserSession::Release()
{
	DEBUG_MSG( LVL_TRACE, " UserSession::Release " );

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

void UserSession::QuitTable()
{
    m_wBattleKey    = 0;          // 房间号
    m_bReady        = FALSE; 	  // 已经准备好了
	m_bShowcards    = FALSE;      // 显示牌
	m_bPokerSize    = 0;          // 牌数量
    memset( m_bPoker, 0x0, sizeof(m_bPoker) );  // 牌
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

void UserSession::setReady( BOOL _ready ) {
    m_bReady = _ready;
}
void UserSession::setShowcards( BOOL _showcards ) {
    m_bShowcards = _showcards;
}

// 牌数量
BYTE & UserSession::getPokerSize( ) {
    return m_bPokerSize;
}

// 牌
void UserSession::setPokers( char * poker, WORD _size) {
    memset( m_bPoker, 0x0, sizeof(m_bPoker) );
    strcat( m_bPoker, poker);
}

void UserSession::getSeatInfo( char * _seatinfo, int _size )
{
    char _true[10]  = {"true"};
    char _false[10] = {"false"};
    char _buff[256]   = {0};
    char _format[256] = "{\"show\":true,"
                         "\"ready\":%s,"
                         "\"showcards\":%s,"
                         "\"name\":\"%s\","
                         "\"rate\":\"%.0f\","
                         "\"money\":\"%d\"}";

    float _rate = 0;
    float _sum = (m_uiWons + m_uiFaileds + m_uiAways);

    if (_sum>0) {
        _rate = m_uiWons / _sum * 100;
    }

    char * cReady = _true;
    if ( m_bReady == FALSE ) {
        cReady = _false;
    }

    char * cShowcards = _true;
    if ( m_bShowcards == FALSE ) {
        cShowcards = _false;
    }

    snprintf( _buff, sizeof(_buff), _format,
             cReady, cShowcards, m_szName, _rate,  m_iMoneys );

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _seatinfo, _buff );
    }
}

// 牌的信息
void UserSession::getPokerInfo( int userkey, char * _pokerinfo, int _size )
{
    char _true[10]  = {"true"};
    char _false[10] = {"false"};

    char _buff[256]   = {0};
    char _format[256] = "{\"show\":%s,"
                        "\"name\":\"%s\","
                        "\"count\":%d,"
                        "\"poker\":[%s]}";

    char * cShow = _false;
    if ( userkey == m_wUserKey ) {
        cShow = _true;
    }

    snprintf( _buff, sizeof(_buff), _format,
        cShow, m_szName, m_bPokerSize, m_bPoker );

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _pokerinfo, _buff );
    }
}
