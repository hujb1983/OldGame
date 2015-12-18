#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*
    加入游戏
*/
class Query_JoinTable : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_JoinTable );

public:

    int  userid;
    WORD userport;
    WORD agentport;
    BYTE roomid;
    BYTE tableid;

	enum {
		RESULT_COL_NUM = 2,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
        int  m_iBattle;
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

	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 1, RESULT_COL_NUM)
        _BINDING_COLUMN(0, m_iError)
        _BINDING_COLUMN(1, m_iBattle)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_JoinTable);

#pragma pack(pop)


/*  1. 设置房间号(tb_roominfo)；
    2. 设置桌子号(tb_tableinfo)；
    */
int User_Query_JoinTable( ServerSession * pServerSession, const char * js_text )
{
    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int _dwUserDB = 0;
    int _roomid = 0;
    int _tableid = 0;
    int _userkey = 0;
    int _agentkey = 0;

    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "agentkey", _agentkey );
    js_map.ReadInteger( "userid",   _dwUserDB );
    js_map.ReadInteger( "roomid",   _roomid   );
    js_map.ReadInteger( "tableid",  _tableid  );

    Query_JoinTable * pQuery = Query_JoinTable::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    pQuery->userport  = _userkey;
    pQuery->agentport = _agentkey;
    pQuery->userid    = _dwUserDB;
    pQuery->roomid    = _roomid;
    pQuery->tableid   = _tableid;

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_UserJoinTable(%d, %d, %d, %d, %d); ", _dwUserDB, _userkey, _agentkey, _roomid, _tableid );

    printf ( "[User_Query_JoinTable = %s] \n", szQueryBuff );
    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)JoinTable_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;     // 比较忙
}

void MSG_Handler_JoinTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_JoinTable( pServerSession, (char*)pMsg );
}

int User_Result_JoinTable ( ServerSession * pServerSession, Query_JoinTable * pQuery )
{
    int   iError;
    int   nLength;
    int   _iBattle = -1;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        int iError = pQuery->vctRes[0].m_iError;
        printf( " m_iError = %d \n", pQuery->vctRes[0].m_iError );

        WORD _userid   = pQuery->userid;
        WORD _userkey  = pQuery->userport;
        WORD _agentkey = pQuery->agentport;
        BYTE _roomid   = pQuery->roomid;
        BYTE _tableid  = pQuery->tableid;

        int  nLength = 0;
        char szJsonBuff[1024] = {0};
        if ( iError == 0 )
        {
            _iBattle = pQuery->vctRes[0].m_iBattle;
            snprintf( szJsonBuff, sizeof(szJsonBuff),
                     "{\"protocol\":%d,"
                     "\"userid\":%d,"
                     "\"agentkey\":%d,"
                     "\"roomid\":%d,"
                     "\"tableid\":%d,"
                     "\"battleid\":%d,"
                     "\"status\": 0,"
                     "\"userkey\":%d}",
                      MAKEDWORD(Games_Protocol, JoinTable_BRD),
                      _userid,
                      _agentkey,
                      _roomid,
                      _tableid,
                      _iBattle,
                      _userkey);
            nLength = strlen(szJsonBuff);
        }
        else
        {
            snprintf( szJsonBuff, sizeof(szJsonBuff),
                     "{\"protocol\":%d,"
                     "\"userid\":%d,"
                     "\"agentkey\":%d,"
                     "\"roomid\":%d,"
                     "\"tableid\":%d,"
                     "\"battleid\":%d,"
                     "\"status\": -1,"
                     "\"userkey\":%d}",
                      MAKEDWORD(Games_Protocol, JoinTable_BRD),
                      _userid,
                      _agentkey,
                      _roomid,
                      _tableid,
                      -1,
                      _userkey);
            nLength = strlen(szJsonBuff);
        }

        pServerSession->Send( (BYTE*)szJsonBuff, nLength );
    }
}

void MSG_Handler_JoinTable_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_JoinTable * pQuery = (Query_JoinTable*) msg->m_pData;
    if ( pQuery ) {
        User_Result_JoinTable( pServerSession, pQuery );
        Query_JoinTable::FREE(pQuery);
    }
}
