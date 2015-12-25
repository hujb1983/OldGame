#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*
    加入游戏
*/
class Query_JoinGame : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_JoinGame );

public:

    int  userid;
    WORD userport;
    WORD agentport;
    BYTE roomid;
    BYTE tableid;
    BYTE seatid;
    WORD battleid;

	enum {
		RESULT_COL_NUM = 5,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
        int  m_iAutoid;
        int  m_iMaxMoney;
        int  m_iMinMoney;
        int  m_iBrokerage;
		sRESULT() {
			memset( this, 0, sizeof(sRESULT) );
		}
	};

	ULONG uLength[RESULT_COL_NUM]; // 4

	vector<sRESULT> vctRes;

	void AllocData()
	{
		sRESULT m_RESULT;
		memset(&m_RESULT, 0, sizeof( m_RESULT) );
		vctRes.push_back( m_RESULT );
	}

	void InitData() { vctRes.clear(); }
	void dealData() { vctRes.pop_back(); }

	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 5, RESULT_COL_NUM)
        _BINDING_COLUMN(0, m_iError)
        _BINDING_COLUMN(1, m_iAutoid)
        _BINDING_COLUMN(2, m_iMaxMoney)
        _BINDING_COLUMN(3, m_iMinMoney)
        _BINDING_COLUMN(4, m_iBrokerage)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_JoinGame);

#pragma pack(pop)


/*  1. 设置房间号(tb_roominfo)；
    2. 设置桌子号(tb_tableinfo)；
    */
int User_Query_JoinGame( ServerSession * pServerSession, const char * js_text )
{
    DEBUG_MSG(LVL_DEBUG, "JoinGame_REQ to recv: %s.",  js_text );

    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int _dwUserDB,_userkey, _agentkey, _roomid, _tableid, _battleid, _seatid;

    js_map.ReadInteger( "userid",    _dwUserDB );
    js_map.ReadInteger( "userkey",   _userkey  );
    js_map.ReadInteger( "agentkey",  _agentkey );
    js_map.ReadInteger( "roomid",    _roomid   );
    js_map.ReadInteger( "tableid",   _tableid  );
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "battleid",  _battleid );

    Query_JoinGame * pQuery = Query_JoinGame::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    pQuery->userport  = _userkey;
    pQuery->agentport = _agentkey;
    pQuery->userid    = _dwUserDB;
    pQuery->roomid    = _roomid;
    pQuery->tableid   = _tableid;
    pQuery->seatid    = _seatid;
    pQuery->battleid  = _battleid;

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof( szQueryBuff ),
          " call p_UserJoinGame(%d, %d, %d, %d, %d, %d, %d); ",
          _dwUserDB, _userkey, _agentkey, _roomid, _tableid, _seatid, _battleid);

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)JoinGame_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );

    return 0;     // 比较忙
}

void MSG_Handler_JoinGame ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_JoinGame( pServerSession, (char*)pMsg );
}

int User_Result_JoinGame ( ServerSession * pServerSession, Query_JoinGame * pQuery )
{
    int   iError;
    int   _nLength, _nAutoid, _mmax, _mmin, _brokerage;
    int   _nIndex = -1;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        int iError = pQuery->vctRes[0].m_iError;
        WORD _userid   = pQuery->userid;
        WORD _userkey  = pQuery->userport;
        WORD _agentkey = pQuery->agentport;
        BYTE _roomid   = pQuery->roomid;
        BYTE _tableid  = pQuery->tableid;
        BYTE _seatid   = pQuery->seatid;
        WORD _battleid = pQuery->battleid;

        char szJsonBuff[1024] = {0};
        if ( iError == 0 ) {

            _nAutoid   = pQuery->vctRes[0].m_iAutoid;
            _mmax      = pQuery->vctRes[0].m_iMaxMoney;
            _mmin      = pQuery->vctRes[0].m_iMinMoney;
            _brokerage = pQuery->vctRes[0].m_iBrokerage;


            snprintf( szJsonBuff, sizeof(szJsonBuff),
                     " { \"protocol\":%d,\"battleid\":%d,\"status\":%d,"
                     "\"mmax\":%d,\"mmin\":%d,\"brokerage\":%d,"
                     "\"userid\":%d,\"userkey\":%d,\"seatid\":%d,\"autoid\":%d} ",
                     MAKEDWORD( Games_Protocol,JoinGame_BRD),
                     _battleid,
                     0,
                     _mmax,
                     _mmin,
                     _brokerage,
                     _userid,
                     _userkey,
                     _seatid,
                     _nAutoid );
        }
        else {
            snprintf( szJsonBuff, sizeof(szJsonBuff),
                     " { \"protocol\":%d,\"battleid\":%d,\"status\":-1,\"userid\":%d,\"userkey\":%d,\"seatid\":%d,\"autoid\":-1} ",
                     MAKEDWORD( Games_Protocol, JoinGame_BRD),
                     _battleid,
                     _userid,
                     _userkey,
                     _seatid);
        }

        DEBUG_MSG( LVL_DEBUG, "JoinGame_BRD to send: %s.", szJsonBuff);

        int nLength = strlen(szJsonBuff);
        pServerSession->Send( (BYTE*)szJsonBuff, nLength );
    }
}

void MSG_Handler_JoinGame_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_JoinGame * pQuery = (Query_JoinGame*) msg->m_pData;
    if ( pQuery ) {
        User_Result_JoinGame( pServerSession, pQuery );
        Query_JoinGame::FREE(pQuery);
    }
}

