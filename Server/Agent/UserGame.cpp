#include "UserGame.h"
#include "AgentServer.h"
#include "AgentFactory.h"

DWORD UserGame::m_dwClientDelay = 0xFFFFFFFF;
DWORD UserGame::m_dwServerDelay = 0xFFFFFFFF;

UserGame::UserGame()
{
    m_pSession = NULL;
}

UserGame::~UserGame()
{

}

WORD UserGame::GetUserKey() const        { return m_wUserKey;    }
void UserGame::SetUserKey(WORD dwKey)    { m_wUserKey = dwKey;   }
BYTE UserGame::GetSeatKey() const        { return m_bySeatKey;   }
void UserGame::SetSeatKey(BYTE dwKey)    { m_bySeatKey = dwKey;  }
WORD UserGame::GetBattleKey() const      { return m_wBattleKey;  }
void UserGame::SetBattleKey(WORD dwKey)  { m_wBattleKey = dwKey; }
WORD UserGame::GetUserid() const         { return m_dwUserid;    }
void UserGame::SetUserid(UINT dwID)      { m_dwUserid = dwID;    }

/************ protected *************/
void UserGame::Init()
{
	m_wUserKey 		= 0;

	DWORD dwCurrent = Session::GetTickCount();
	m_dwOvertime    = dwCurrent + UserSession::m_dwClientDelay;

	DEBUG_MSG( LVL_TRACE, "UserGame::Init: %d", m_dwOvertime);

	m_wUserKey   = 0;       // �û���ֵ
    m_wBattleKey = 0;       // �����
    m_dwUserid   = 0;       // �û�ID
    memset( m_szName, 0x0, sizeof(m_szName) ); // ����
    m_iMoneys    = 0;       // ����
    m_uiWons     = 0;       // Ӯ�Ĵ���
	m_uiFaileds  = 0; 	    // ʧ�ܴ���
	m_uiAways    = 0; 		// ���ܴ���

	m_bReady     = FALSE;
    m_bShowcards = FALSE;
}

BOOL UserGame::Update( DWORD dwDeltaTick )
{
	DEBUG_MSG( LVL_TRACE, "UserSession::Update: %d %d", dwDeltaTick, m_dwOvertime);

	// Count Down;
	if ( dwDeltaTick > m_dwOvertime ) {
		// Disconnect(TRUE);
		return TRUE;
	}

	return FALSE;
}

void UserGame::QuitTable()
{
    m_wBattleKey    = 0;          // �����
    m_bReady        = FALSE; 	  // �Ѿ�׼������
	m_bShowcards    = FALSE;      // ��ʾ��
	m_bPokerSize    = 0;          // ������
    memset( m_bPoker, 0x0, sizeof(m_bPoker) );  // ��
}

void UserGame::setName( char * _name, int _size ) {
    memset( m_szName, 0x0, sizeof(m_szName) );
    memcpy( m_szName, _name, _size );
}

void UserGame::setGamesinfo( int _point, UINT _wons, UINT _failed, UINT _aways ) {
    m_iMoneys   = _point;           // ����
    m_uiWons    = _wons;            // Ӯ�Ĵ���
	m_uiFaileds = _failed; 	        // ʧ�ܴ���
	m_uiAways   = _aways; 		    // ���ܴ���
}

void UserGame::setMoney( int _money ) {
    m_iMoneys = _money;
}

void UserGame::setReady( BOOL _ready ) {
    m_bReady = _ready;
}
void UserGame::setShowcards( BOOL _showcards ) {
    m_bShowcards = _showcards;
}

// ������
BYTE & UserGame::getPokerSize( ) {
    return m_bPokerSize;
}

// ��
void UserGame::setPokers( char * poker, WORD _size) {
    memset( m_bPoker, 0x0, sizeof(m_bPoker) );
    strcat( m_bPoker, poker);
}

void UserGame::getSeatInfo( char * _seatinfo, int _size )
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

// �Ƶ���Ϣ
void UserGame::getPokerInfo( int userkey, char * _pokerinfo, int _size )
{
    char _true[10]  = {"true"};
    char _false[10] = {"false"};
    char _poker[128] ={ 0 };

    char _buff[256]   = {0};
    char _format[256] = "{\"show\":%s,"
                        "\"name\":\"%s\","
                        "\"count\":%d,"
                        "\"poker\":[%s]}";

    char * cShow = _false;
    if ( userkey == m_wUserKey ) {
        cShow = _true;
        strcat( _poker, m_bPoker);
    }
    else {
        strcat( _poker, "-1");
    }

    snprintf( _buff, sizeof(_buff), _format,
        cShow, m_szName, m_bPokerSize, _poker );

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _pokerinfo, _buff );
    }
}
