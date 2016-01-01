#include "GameBattle.h"
#include "CnpokerServer.h"
#include "GameMgr.h"

GameBattle::GameBattle()
{
    m_prev = NULL;
    m_next = NULL;

    m_dwBeginTicket = 0;
    m_dwOverTicket  = 0;
    m_byOverTime    = eGB_HaveNot;      // û�ж�ʱ��
    memset(m_szOverTime, 0x0, OT_SIZE);
}

GameBattle::~GameBattle()
{

}

GameBattle * GameBattle::set_prev( GameBattle * _prev ) {
    GameBattle * t = m_prev;
    m_prev = _prev;
    return t;
}

GameBattle * GameBattle::set_next( GameBattle * _next ) {
    GameBattle * t = m_next;
    m_next = _next;
    return t;
}

bool GameBattle::is_null_prev() {
    return (m_prev != NULL);
}

bool GameBattle::is_null_next() {
    return (m_next != NULL);
}

GameBattle * GameBattle::get_prev() {
    return (m_prev);
}

GameBattle * GameBattle::get_next() {
    return (m_next);
}

/**********************************************
    ��ʱ����
**********************************************/
int GameBattle::InitBattle()
{
    m_byTableStatus = eGB_EMPTY;
}

int GameBattle::JoinBattle( BYTE _seatid )
{

}



int GameBattle::QuitBattle( BYTE _seatid )
{
    if (m_byTableStatus==eGB_SIT)
    {
        if (_seatid<MAX_USER) {
            m_uiUserid[_seatid] = 0;    // �û���id;
            m_wUserkey[_seatid] = 0;    // �û���key;
            m_byStatus[_seatid] = 0;    // �Ƿ����;
            m_byStart[_seatid]  = 0;    // �Ƿ��Ѿ����, ��ʼ״̬;
            m_byCalls[_seatid]  = 0;    //��¼�е���״̬���û�״̬;
            m_byShow[_seatid]   = 0;    // ��¼��ʾ�Ƶ��û�״̬;
            return FALSE;
        }
    }
    else
    {
        if (_seatid<MAX_USER) {
            m_byOnline[_seatid] = eGB_Leave;
        }
    }
    return TRUE;
}

/**********************************************
    ��ʱ����
**********************************************/
int GameBattle::UpdateOvertime( DWORD dwTicket )
{
    if ( IsOvertime()==eGB_TimerOpen ) {
        if ( dwTicket>=m_dwOverTicket ) {
            this->SendOvertimeMessage();    // ���Ͳ�ʱ��Ϣ
            this->LockOvertime();           // ������ʱ
        }
    }
}

int GameBattle::SendOvertimeMessage()
{
    if ( m_byOverTime != eGB_TimerLock ) {
        WORD nLen = strlen( m_szOverTime );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)m_szOverTime, nLen );
    }
}

int GameBattle::IsOvertime() {
    return (m_byOverTime==eGB_TimerLock);      // �������ס����ʱ
}

int GameBattle::LockOvertime() {
    return (m_byOverTime=eGB_TimerLock);
}

int GameBattle::UnlockOvertime() {
    return (m_byOverTime=eGB_TimerOpen);       // ��һ����Ȩ֪ͨ������
}

int GameBattle::SetCalledOvertime()
{
    {
        char szPlayerkey[256] = {0};
        GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        memset(m_szOverTime, 0x0, OT_SIZE);
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"overtime\":\"%d\" }] }";   // ��ʱ�趨Ϊ16��;

        snprintf( m_szOverTime, sizeof(m_szOverTime), format,
                MAKEDWORD( (WORD)Games_Protocol, (WORD)CalledOverTime_NAK ),
                m_wIndex,
                szPlayerkey,
                m_byPlaying,
                -1);
    }

    this->UnlockOvertime();   // ������ʱ

    m_dwBeginTicket = Session::GetTickCount();
    m_dwOverTicket  = m_dwBeginTicket + 12000; // ���Ʋ�ʱ��
    g_GameMgr.push_CalledBattles( this );           // ����ѭ��������
}

int GameBattle::SetDiscardsOvertime()
{
    {
        char szPlayerkey[256] = {0};
        GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        memset(m_szOverTime, 0x0, OT_SIZE);
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"overtime\":\"%d\" }] }";   // ��ʱ�趨Ϊ18��;

        snprintf( m_szOverTime, sizeof(m_szOverTime), format,
                MAKEDWORD( (WORD)Games_Protocol, (WORD)DiscardsOverTime_NAK ),
                m_wIndex,
                szPlayerkey,
                m_byPlaying,
                -1);
    }

    this->UnlockOvertime();   // ������ʱ

    m_dwBeginTicket = Session::GetTickCount();
    m_dwOverTicket  = m_dwBeginTicket + 18000; // ���Ʋ�ʱ��
    g_GameMgr.push_DiscardsBattles( this );         // ����ѭ��������
}

/**********************************************
    ��Ϸ����
**********************************************/
int GameBattle::GetAllPlayerKey(char * json_text, WORD wSize) {

    // @{{{ ������е���
    char buff[1024] = {0};
    char format[256] = 	" \"userkey1\":\"%d\","
                        " \"userkey2\":\"%d\","
                        " \"userkey3\":\"%d\" ";

    sprintf( buff, format,
            m_wUserkey[0],
            m_wUserkey[1],
            m_wUserkey[2]);

    int nLen = strlen(buff);
    if (nLen < wSize) {
        memcpy( json_text, buff, nLen);
        *(json_text + nLen) = '\0';
    }
}

int GameBattle::SendToSettleAccounts() {
    // Ԥ����
}

int GameBattle::SetIndex( WORD index ) {
    m_wIndex = index;
}
int GameBattle::SetTable( BYTE room, BYTE table ) {
    m_byRoomid  = room;
	m_byTableid = table;
}
int GameBattle::SetID( BYTE seatid,  int id) {
    m_uiUserid[seatid] = id;
}
int GameBattle::SetKey( BYTE seatid, int key) {
    m_wUserkey[seatid] = key;
}
int GameBattle::SetMaxMoney  ( UINT _money ) {
    m_uiMaxMoney = _money;
}
int GameBattle::SetMinMoney  ( UINT _money ) {
    m_uiMinMoney = _money;
}
int GameBattle::SetBrokerage ( UINT _rate  ) {
    m_wBrokerage = _rate;
}
int GameBattle::SetStartSeat( BYTE seatid ) {
    m_byStartSeat = seatid;
    m_byCallSeat  = seatid;
    m_byCallTimes = 0;
}
int GameBattle::SetOnline( BYTE seatid, BYTE status )
{
    if ( status == 0 ) {
        m_byOnline[seatid] = eGB_Online;
    }
    else if ( status == 1 ) {
        m_byOnline[seatid] = eGB_Offline;
    }
    else if ( status == -1 ) {
        m_byOnline[seatid] = eGB_Leave;
    }

    if ( m_byTableStatus != eGB_PLAYING ) {
        if ( m_byOnline[0] == eGB_Offline &&
             m_byOnline[1] == eGB_Offline &&
             m_byOnline[2] == eGB_Offline ) {
            return FALSE;
        }
    }
    return TRUE;
}

/******************************************************
    ����ÿһ���׶ε�״̬
******************************************************/
int GameBattle::SetStart( BYTE seatid, BYTE status ) {
    if (status==eGB_Ready) {
        m_byStart[seatid] = eGB_Ready;
        if (m_byTableStatus== eGB_DEALING) {
            return FALSE;   // ��ʾ�Ѿ���ʼ��Ϸ
        }
        return TRUE;    // ��ʾ�Ѿ���ʼ��Ϸ
    }
    m_byStart[seatid] = status; // ��ʾ����״̬
}
int GameBattle::SetShow( BYTE seatid ) {
    m_byShow[seatid] = eGB_Show;
    return TRUE;
}
int GameBattle::SetCalled( BYTE seatid, BYTE called ) {
    if ( m_byCallSeat==seatid ) {
        m_byCallTimes++;
        if ( getModel()==0 ) {
            if ( called==0 ) {
                m_byCalls[seatid] = eGB_Waiver;
                return TRUE;    // ���е���
            }

            do {
                if ( called==1 ) {
                    m_byCalls[seatid] = eGB_Point1;
                    break;
                }
                else if ( called==2 ) {
                    m_byCalls[seatid] = eGB_Point2;
                    break;
                }
                else if ( called==3 ) {
                    m_byCalls[seatid] = eGB_Point3;
                    break;
                }
                return FALSE;
            } while(1);

            if ( m_byCallTimes==1 ) {
                m_wMultiple = called;
                return TRUE;    // ��һ���е�����
            }
            else if ( called>m_wMultiple){
                m_wMultiple = called;
                return TRUE;   // �ڶ����Ͻе�����Ҫ��ǰһ����
            }
            return FALSE;
        }
        else   // ��
        {
            if ( called==0 ) {
                m_byCalls[seatid] = eGB_Waiver;
                return TRUE;
            }
            else {
                m_byCalls[seatid] = eGB_Apply;
                m_wMultiple *= 2;
                return TRUE;
            }
            return FALSE;
        }
    }
    return FALSE;
}


int GameBattle::SetBank( BYTE seatid ) {
    m_byStatus[(seatid-1)%MAX_USER] = eGB_Player;
    m_byStatus[seatid] = eGB_Banker;
    m_byStatus[(seatid+1)%MAX_USER] = eGB_Player;
    m_byBanker = seatid;
}
int GameBattle::SetPlaying( BYTE seatid ) {
    m_byPlaying = seatid;
}
int GameBattle::SetModel( BYTE _byDouble ) {
    m_wMultiple = 1;
    m_byModel = _byDouble;
}
int GameBattle::SetBasecards( BYTE * poker, BYTE bySize ) {
    if ( bySize <= BASE_POKER ) {
        memcpy( m_byBasecards, poker, bySize);
        return TRUE;
    }
    return FALSE;
}
int GameBattle::SetTrusteeship( BYTE seatid ){
    if ( m_byTrusteeship[seatid] == eGB_Trusteeship_Yes ) {
        return -1;
    }
    m_byTrusteeship[seatid] = eGB_Trusteeship_Yes;
    return 0;
}
int GameBattle::CancelTrusteeship( BYTE seatid ){
    if ( m_byTrusteeship[seatid] == eGB_Trusteeship_No ) {
        return -1;
    }
    m_byTrusteeship[seatid] = eGB_Trusteeship_No;
    return 0;
}

// ������û���;
int GameBattle::SetUsercards( BYTE seatid, BYTE * poker, BYTE bySize ) {
    BYTE *pMove  = m_byUsercards[seatid];
    BYTE byPoker = 0;
    for (int i=0; i<bySize; i++) {
        byPoker = poker[i];
        if ( byPoker<MAX_POKER ) {
            pMove[byPoker] = eGB_Have;
        }
    }
    m_nDiscardsSize[seatid] = bySize;
}

int GameBattle::SetDiscardString( BYTE seatid, char * poker ) {
    char *pMove  = m_szDiscards [seatid];
    strcat( pMove, poker );
    strcat( pMove, ";"   );
}

int GameBattle::SetDiscards( BYTE seatid, BYTE * poker, BYTE bySize )
{
    BYTE *pMove = m_byUsercards[seatid];
    BYTE  byPoker  = 0;

    for (int i=0; i<bySize; i++) {
        byPoker = poker[i];
        if ( byPoker<MAX_POKER ) {
            pMove[byPoker] = eGB_Disc;
        }
    }

    m_nDiscardsSize[seatid] -= bySize;
    return TRUE;
}

/******************************************************
    ȡ��
******************************************************/
BYTE GameBattle::getUsercards( BYTE seatid, char * poker, WORD wSize ) {

    char szPoker[16], szPokerList[256] = {0};

    BYTE *pMove = m_byUsercards[seatid];
    int disSize = sizeof( m_byUsercards[seatid] );

     // step1 : ���û����Ƹ��û���������ҿ���;
    BYTE retSize = 0;
    BYTE byPoker  = 0;
    for (int i=0; i<disSize; i++) {
        byPoker = pMove[i];
        if ( byPoker == 1 ) {
            memset( szPoker, 0x0, sizeof(szPoker) );
            if (retSize!=0) {
                strcat( szPokerList, ",");
            }
            sprintf( szPoker, "%d", i );
            strcat( szPokerList, szPoker);
            retSize++;
        }
    }

    int iLen = strlen( szPokerList );
    if ( iLen < wSize ) {
        memcpy( poker, szPokerList, wSize);
    }
    return retSize;
}

BYTE GameBattle::getBasecards( BYTE seatid, char * poker, WORD wSize ) {

    // step1 : �ѵ��Ƹ�������λ;
    BYTE retSize = 0;
    BYTE byPoker  = 0;
    for (int i=0; i<BASE_POKER; i++) {
        byPoker = m_byBasecards[i];
        if ( byPoker < MAX_POKER ) {
             m_byUsercards[seatid][byPoker] = 1;
             retSize = i;
        }
    }
    m_nDiscardsSize[seatid] += BASE_POKER;

    // step2 : ���ظ���ҿ�����;
    if ( poker ) {
        char szPokerList[60] = {0};
        sprintf( szPokerList, "%d,%d,%d",
            m_byBasecards[0], m_byBasecards[1], m_byBasecards[2]);

        int iLen = strlen( szPokerList );
        if ( iLen < wSize ) {
            memcpy( poker, szPokerList, wSize);
        }
    }
    return retSize;
}

// ȡ���������ϵ���
BYTE GameBattle::getCurrentCards( BYTE seatid, BYTE * poker, WORD bySize )
{
    BYTE xlen=0;

    // step1 : �ѵ��Ƹ�������λ;
    BYTE byPoker  = 0;
    for (int i=0; i<MAX_POKER; i++) {
        byPoker = m_byUsercards[seatid][i];
        if ( byPoker < MAX_POKER ) {
            if ( byPoker==1 ) {
                poker[xlen] = i;
                xlen++;
                if ( xlen==bySize ) {
                    break;
                }
            }
        }
    }
    return (xlen);
}

int GameBattle::SetLastSeat(  BYTE _seatid ) { m_byLastActive = _seatid; }
int GameBattle::SetLastType(  BYTE _type   ) { m_byLastType   = _type;   }
int GameBattle::SetLastValue( BYTE _value  ) { m_byLastValue  = _value;  }
int GameBattle::SetLastCount( BYTE _count  ) { m_byLastCount  = _count;  }
int GameBattle::SetLastSize ( BYTE _size  )  { m_byLastSize   = _size;  }

/******************************************************
    ȡ�ñ�Ҫ�Ĳ���
******************************************************/
BYTE GameBattle::getSeatid(int key, BYTE & seatid) {
    if ( m_wUserkey[0] == key ) {
        seatid = 0;
        return TRUE;
    }
    else if ( m_wUserkey[1] == key ) {
        seatid = 1;
        return TRUE;
    }
    else if ( m_wUserkey[2] == key ) {
        seatid = 2;
        return TRUE;
    }
    return FALSE;
}

UINT GameBattle::getMaxMoney ()     {  return m_uiMaxMoney;     }    // ���ø��޶�;
UINT GameBattle::getMinMoney ()     {  return m_uiMinMoney;     }    // ���õ���;
UINT GameBattle::getBrokerage()     {  return m_wBrokerage;     }    // ����Ӷ��;
BYTE GameBattle::getModel()         {  return m_byModel;        }
WORD& GameBattle::getMultiple()     {  return m_wMultiple;      }
BYTE GameBattle::getBanker()        {  return m_byBanker;       }
BYTE GameBattle::getStartSeat()     {  return m_byStartSeat;    }
BYTE& GameBattle::getLastSeat()     {  return m_byLastActive;   }
BYTE& GameBattle::getLastType()     {  return m_byLastType;     }
BYTE& GameBattle::getLastValue()    { return m_byLastValue;     }
BYTE& GameBattle::getLastCount()    { return m_byLastCount;     }
BYTE& GameBattle::getLastSize()     { return m_byLastSize;      }
BYTE& GameBattle::getDiscardTimes() { return m_byPlayTimes;     }

/******************************************************
    ������һ���׶ε�״̬
******************************************************/
int GameBattle::SetBattleStatus( BYTE _status ) {
    m_byTableStatus = _status;
}
BYTE GameBattle::getBattleStatus( ) {
    return m_byTableStatus;
}

/******************************************************
    �ж���һ���׶ε�״̬
******************************************************/
int GameBattle::canStart() {
    if ( m_byTableStatus == eGB_SIT ) {
        if ( m_byStart[0] == eGB_Ready &&
             m_byStart[1] == eGB_Ready &&
             m_byStart[2] == eGB_Ready ) {
            return TRUE;
        }
    }
    return FALSE;
}
int GameBattle::canCall() {
    if ( m_byTableStatus == eGB_CALLING ) {
        return TRUE;
    }
    return FALSE;
}
int GameBattle::checkCall() {
    if (m_byCalls[m_byCallSeat]==eGB_Point3) {
        m_byBanker =  m_byCallSeat;
        return TRUE;  // �Ѿ��ҳ��˵�����;
    }
    if (m_byCallTimes==3) {
        if ( m_byCalls[0] == eGB_Waiver &&
             m_byCalls[1] == eGB_Waiver &&
             m_byCalls[2] == eGB_Waiver ) {
            return FALSE;   // 3����û��, �����·���
        }
        BYTE _byTemp = (m_byCallSeat+1)%MAX_USER;
        if ( m_byCalls[_byTemp] == eGB_Waiver ) {
            _byTemp = (m_byCallSeat)%MAX_USER;
            if ( m_byCalls[ _byTemp ] == eGB_Apply ) {
                SetBank( _byTemp );                     // ��һ�����Ϊ����
                return TRUE;
            }
            _byTemp = (m_byCallSeat-1)%MAX_USER;
            if ( m_byCalls[ _byTemp ] == eGB_Apply ) {
                SetBank( _byTemp );                     // ���¸���ҵ���
                return TRUE;
            }
        }
    }
    if (m_byCallTimes==4) {
        BYTE _byTemp = m_byCallSeat;
        if ( m_byCalls[_byTemp] == eGB_Waiver ) {
            _byTemp = (m_byCallSeat-1)%MAX_USER;
            if ( m_byCalls[ _byTemp ] == eGB_Apply ) {
                SetBank( _byTemp );                     // ��һ�����Ϊ����
                return TRUE;
            }
            _byTemp = (m_byCallSeat-2)%MAX_USER;
            if ( m_byCalls[ _byTemp ] == eGB_Apply ) {
                SetBank( _byTemp );                     // ���¸���ҵ���
                return TRUE;
            }
            return FALSE;
        }
        SetBank( m_byCallSeat );                     // ��һ�����Ϊ����
        return TRUE;
    }
    m_byCallSeat = (m_byCallSeat+1) % MAX_USER;
    return TRUE;
}
int GameBattle::canGame() {
    if ( m_byBanker != eGB_NoBanker ) {
        return TRUE;
    }
    return FALSE;
}
int GameBattle::canEnd() {
    if ( m_nDiscardsSize[ m_byPlaying ] > 0 ) {
        return FALSE;   // δ����
    }
    return TRUE;  // �Ѿ�����
}

int GameBattle::hadDiscards( BYTE seatid, BYTE * poker, BYTE bySize ) {

    BYTE *pMove = m_byUsercards[seatid];
    BYTE  byPoker  = 0;

    for (int i=0; i<bySize; i++) {
        byPoker = poker[i];
        if ( byPoker<MAX_POKER ) {
            if ( pMove[byPoker] == eGB_Not ) {
                return FALSE;
            }
            if ( pMove[byPoker] == eGB_Disc ) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

int GameBattle::IsOnline( BYTE seatid ) {
    return (m_byOnline[seatid] == eGB_Online)? 0 : -1;
}
int GameBattle::IsTrusteeship( BYTE seatid ) {
    return (m_byTrusteeship[seatid] == eGB_Trusteeship_Yes)? 0 : -1;
}

BYTE GameBattle::prevSeat( BYTE seatid ) {
    return (seatid - 1) % MAX_USER;
}
BYTE GameBattle::nextSeat( BYTE seatid ) {
    return (seatid + 1) % MAX_USER;
}

WORD GameBattle::getIndex() {
    return m_wIndex;
}
WORD GameBattle::prevKey(BYTE seatid){
    return m_wUserkey[(seatid - 1) % MAX_USER];
}
WORD GameBattle::nextKey(BYTE seatid){
    return m_wUserkey[(seatid + 1) % MAX_USER];
}
UINT GameBattle::getID(BYTE seatid) {
    return m_uiUserid[seatid];
}
WORD GameBattle::getKey( BYTE seatid ){
    return m_wUserkey[seatid];
}
BYTE GameBattle::getPlaying() {
    return m_byPlaying;
}
BYTE GameBattle::getCalled() {
    return m_byCallSeat;
}

int GameBattle::UpdateToDatabase() {
    this->UpdateBattleInfo(0);
    this->UpdateBattleInfo(1);
    this->UpdateBattleInfo(2);
    this->SendToSettleAccounts();
}

int GameBattle::UpdateBattleInfo( BYTE _seatid )
{
    {
        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                             "\"battleid\":\"%d\","
                             "\"seatid\":\"%d\","
                             "\"banker\":\"%d\","
                             "\"called\":\"%d\","
                             "\"showcards\":\"%d\","
                             "\"usercards\":\"%s\","
                             "\"discards\":\"%s\", }";   // ��ʱ�趨Ϊ18��;

        snprintf( buff, sizeof(buff), format,
                MAKEDWORD( Games_Protocol, UpdateBattle_REQ ),
                m_wIndex,
                _seatid,
                m_byBanker,
                m_byCalls[_seatid],
                m_byShow[_seatid],
                m_szUsercards[_seatid],
                m_szDiscards[_seatid] );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToDBServer( (BYTE*)buff, nLen );
    }
}


/**********************************************
    ��Ϸ�û����ж���
**********************************************/
GameBattleList::GameBattleList() {
}

GameBattleList::~GameBattleList() {
}

void GameBattleList::clear()
{
    m_size = 0;
    m_head = NULL;
    m_last = NULL;
}

void GameBattleList::push_back( GameBattle * pPoker )
{
    if ( !m_head ) {
        m_head = pPoker;
        m_head->set_prev(NULL);
        m_head->set_next(NULL);
        m_last = NULL;
        ++m_size;
    }
    else if ( !m_last ){
        m_head->set_next( pPoker );
        pPoker->set_prev( m_head );
        pPoker->set_next( NULL );
        m_last = pPoker;
    }
    else if ( m_last ){
        m_last->set_next( pPoker );
        pPoker->set_prev( m_last );
        pPoker->set_next( NULL );
        m_last = pPoker;
    }
}

void GameBattleList::remove(GameBattle * pPoker)
{
    GameBattle * _prev = pPoker->get_prev();
    GameBattle * _next = pPoker->get_next();
    if ( _prev ) {
        _prev->set_next( _next );
    }
    if ( _next ) {
        _next->set_prev( _prev );
    }
    --m_size;
}

GameBattle * GameBattleList::get_frist() {
    return m_head;
}

GameBattle * GameBattleList::pop_frist() {
    GameBattle * _prev = m_head;
    GameBattle * _next = NULL;
    if ( m_head ) {
        _next = m_head->get_next();
        if ( _next ) {
            _next->set_prev(NULL);
        }
        m_head = _next;
        _prev->set_prev( NULL );
        _prev->set_next( NULL );
    }
    return _prev;
}
