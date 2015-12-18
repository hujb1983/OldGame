#include "Handler_Module.h"
#include <dbCommon.h>


#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*  加入游戏    */
class Query_Settlement : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_Settlement );

public:
    WORD wBattleid;
    BYTE bySeatid;

	enum {
		RESULT_COL_NUM = 7,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
        int  m_iSeat0Val;    // 结算结果0
        int  m_iSeat1Val;    // 结算结果1
        int  m_iSeat2Val;    // 结算结果2
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

	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 7, RESULT_COL_NUM)
        _BINDING_COLUMN(0, m_iError)
        _BINDING_COLUMN(1, m_iSeat0Val)
        _BINDING_COLUMN(2, m_iSeat1Val)
        _BINDING_COLUMN(3, m_iSeat2Val)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_Settlement);

#pragma pack(pop)


/*  1. 设置房间号(tb_roominfo)；
    2. 设置桌子号(tb_tableinfo)； */
int User_Query_Settlement ( ServerSession * pServerSession, const char * js_text )
{
    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int _battleid,_seatid;
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "seatid",    _seatid   );      // 赢了的结算坐位

    Query_Settlement * pQuery = Query_Settlement::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    pQuery->wBattleid = (WORD) _battleid;
    pQuery->bySeatid  = (BYTE) _seatid;

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_UserSettlement( %d, %d ); ",
          _battleid, _seatid );

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)Settlement_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;     // 比较忙
}

void MSG_Handler_Settlement ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    User_Query_Settlement( pServerSession, (char*)pMsg );
}

int User_Result_Settlement ( ServerSession * pServerSession, Query_Settlement * pQuery )
{
    int _battleid = 0;
    int _seatid   = 0;

    int iSize = pQuery->vctRes.size();
    if ( iSize>0 )
    {
        _battleid = pQuery->wBattleid;
        _seatid   = pQuery->bySeatid;

        int iError = pQuery->vctRes[0].m_iError;

        if ( iError == 0 ) {
            int _iSeat0Val = pQuery->vctRes[0].m_iSeat0Val;
            int _iSeat1Val = pQuery->vctRes[0].m_iSeat1Val;
            int _iSeat2Val = pQuery->vctRes[0].m_iSeat2Val;

            char format[256] = "{\"protocol\":\"%d\","
                                "\"battleid\":\"%d\","
                                "\"seat0val\":\"%d\","
                                "\"seat1val\":\"%d\","
                                "\"seat2val\":\"%d\" }";

            char szJsonBuff[1024] = {0};
            sprintf( szJsonBuff, format, MAKEDWORD( Games_Protocol, Settlement_BRD ),
                    _battleid, _iSeat0Val, _iSeat1Val, _iSeat2Val );

            int nLength = strlen(szJsonBuff);
            pServerSession->Send( (BYTE*)szJsonBuff, nLength );
        }
    }
}

void MSG_Handler_Settlement_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_Settlement * pQuery = (Query_Settlement*) msg->m_pData;
    if ( pQuery ) {
        User_Result_Settlement( pServerSession, pQuery );
        Query_Settlement::FREE(pQuery);
    }
}

