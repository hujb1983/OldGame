#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*
    加入游戏
*/
class Query_QuitGame : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_QuitGame );

public:

    int  userid;
    int  battleid;
    int  seatid;

	enum {
		RESULT_COL_NUM = 3,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
        int  m_iBattle;
        int  m_iSeatid;
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
        _BINDING_COLUMN(2, m_iSeatid)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_QuitGame);

#pragma pack(pop)


/*  1. 设置房间号(tb_roominfo)；
    2. 设置桌子号(tb_tableinfo)；    */
int User_Query_QuitGame( ServerSession * pServerSession, const char * js_text )
{
    DEBUG_MSG( LVL_DEBUG, "QuitGame_REQ to recv: %s \n", js_text );

    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int _userid(0), _battleid(0), _seatid(0);
    js_map.ReadInteger( "userid",   _userid );
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid );

    Query_QuitGame * pQuery = Query_QuitGame::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    pQuery->userid    = _userid;
    pQuery->battleid  = _battleid;
    pQuery->seatid    = _seatid;

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_UserQuitGame( %d ); ", _userid );

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)QuitGame_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;     // 比较忙
}

void MSG_Handler_QuitGame ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_QuitGame( pServerSession, (char*)pMsg );
}

int User_Result_QuitGame ( ServerSession * pServerSession, Query_QuitGame * pQuery )
{
    int _iError;
    int _iSize = pQuery->vctRes.size();
    if ( _iSize > 0 )
    {
        _iError = pQuery->vctRes[0].m_iError;
        WORD _battleid(0);
        BYTE _seatid(0);
        int  _nLength = 0;
        char _szJsonBuff[1024] = {0};

        DEBUG_MSG( LVL_DEBUG, "QuitGame_BRD to _iError : %d \n", _iError );
        if ( _iError == 0 )
        {
            _battleid = pQuery->vctRes[0].m_iBattle;
            _seatid   = pQuery->vctRes[0].m_iSeatid;
            DEBUG_MSG( LVL_DEBUG, "QuitGame_BRD to _iError : %d %d \n", _battleid, _seatid );

            snprintf( _szJsonBuff, sizeof(_szJsonBuff),
                     "{\"protocol\":%d,"
                     " \"battleid\":%d,"
                     " \"seatid\":%d }",
                      MAKEDWORD(Games_Protocol, QuitGame_BRD),
                      _battleid,
                      _seatid);
            _nLength = strlen(_szJsonBuff);
        }
        else
        {
            _battleid = pQuery->battleid;
            _seatid   = pQuery->seatid;
            snprintf( _szJsonBuff, sizeof(_szJsonBuff),
                     "{\"protocol\":%d,"
                     " \"battleid\":%d,"
                     " \"seatid\":%d }",
                      MAKEDWORD(Games_Protocol, QuitGame_BRD),
                      _battleid,
                      _seatid);
            _nLength = strlen(_szJsonBuff);
        }

        DEBUG_MSG( LVL_DEBUG, "QuitGame_BRD to send: %s \n", _szJsonBuff );
        pServerSession->Send( (BYTE*)_szJsonBuff, _nLength );
    }
}

void MSG_Handler_QuitGame_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_QuitGame * pQuery = (Query_QuitGame*) msg->m_pData;
    if ( pQuery ) {
        User_Result_QuitGame( pServerSession, pQuery );
        Query_QuitGame::FREE(pQuery);
    }
}

