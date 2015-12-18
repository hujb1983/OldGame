#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*
    加入游戏
*/
class Query_UpdateBattle : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_UpdateBattle );

public:
	enum {
		RESULT_COL_NUM = 1,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
		sRESULT() {
			memset( this, 0, sizeof(sRESULT) );
		}
	};

	ULONG uLength[RESULT_COL_NUM]; // 4

	vector<sRESULT> vctRes;

	void AllocData() {
		sRESULT m_RESULT;
		memset(&m_RESULT, 0, sizeof( m_RESULT) );
		vctRes.push_back( m_RESULT );
	}

	void InitData() { vctRes.clear(); }
	void dealData() { vctRes.pop_back(); }

	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 1, RESULT_COL_NUM)
        _BINDING_COLUMN(0, m_iError)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_UpdateBattle);

#pragma pack(pop)


/*  1. 设置房间号(tb_roominfo)；
    2. 设置桌子号(tb_tableinfo)；
    */
int User_Query_UpdateBattle( ServerSession * pServerSession, const char * js_text )
{
    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int _battleid,_seatid, _banker, _showcard, _called;
    char _usercard[256] = {0};
    char _discards[256] = {0};
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "banker",    _banker   );
    js_map.ReadInteger( "showcard", _showcard );
    js_map.ReadInteger( "called",    _called   );
    js_map.ReadString ( "usercard",  _usercard, sizeof(_usercard) );
    js_map.ReadString ( "discards",  _discards, sizeof(_discards) );

    Query_UpdateBattle * pQuery = Query_UpdateBattle::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_UserUpdateBattle(%d, %d, %d, %d, %d, '%s', '%s'); ",
          _battleid, _seatid, _banker, _showcard, _called, _usercard, _discards );

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)UpdateBattle_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;     // 比较忙
}

void MSG_Handler_UpdateBattle ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    User_Query_UpdateBattle( pServerSession, (char*)pMsg );
}

int User_Result_UpdateBattle ( ServerSession * pServerSession, Query_UpdateBattle * pQuery )
{
    int   iError;
    int   _nLength, _nAutoid;
    int   _nIndex = -1;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        int iError = pQuery->vctRes[0].m_iError;
        return (iError==0)?TRUE:FALSE;
    }
    return FALSE;
}

void MSG_Handler_UpdateBattle_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_UpdateBattle * pQuery = (Query_UpdateBattle*) msg->m_pData;
    if ( pQuery ) {
        User_Result_UpdateBattle( pServerSession, pQuery );
        Query_UpdateBattle::FREE(pQuery);
    }
}

