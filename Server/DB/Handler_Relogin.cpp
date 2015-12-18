#include "Handler_Module.h"
#include "Yond_drng.h"
#include "Yond_md5.h"
#include <dbCommon.h>
#pragma pack(push,1)

class Query_Relogin : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_Relogin );

public:
    int  iUserid;     // 用户端口
    WORD wUserkey;    // 用户端口

	enum {
		RESULT_COL_NUM = 3,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
		int m_iError;
		int m_iBattleid;
		int m_iSeatid;
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
		_BINDING_COLUMN(1, m_iBattleid)
		_BINDING_COLUMN(2, m_iSeatid)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_Relogin);
#pragma pack(pop)


/*  1. 用户查询；
    2. 返回参数后验证； */
int User_Query_Relogin( ServerSession * pServerSession, const char * js_text )
{
    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int _userid, _userkey;
    js_map.ReadInteger( "userid",  _userid );
    js_map.ReadInteger( "userkey", _userkey );

    Query_Relogin * pQuery = Query_Relogin::ALLOC();
    if ( NULL == pQuery ) {
        return -1;
    }

    pQuery->iUserid  = _userid;
    pQuery->wUserkey = _userkey;

    {
        char szQueryBuff[256] = {0};
        snprintf( szQueryBuff,
                  sizeof(szQueryBuff),
                  "call p_UserRelogin(%d)",
                  _userid );

        pQuery->SetIndex( MAKEDWORD( (WORD)Login_Protocol, (WORD)Relogin_DBR ) );
        pQuery->SetQuery( szQueryBuff );
        pQuery->SetVoidObject( pServerSession );
        pServerSession->DBQuery( pQuery );
    }

    return 0;
}

void MSG_Handler_Relogin ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    User_Query_Relogin( pServerSession, (char*)pMsg );
}

int User_Result_Relogin ( ServerSession * pServerSession, Query_Relogin * pQuery )
{
    int     _iError = -1;
    char*   _szName;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        WORD _userid  = pQuery->iUserid;
        WORD _userkey = pQuery->wUserkey;
        _iError = pQuery->vctRes[0].m_iError;
        if ( _iError==0 ) {

            int  _battleid = pQuery->vctRes[0].m_iBattleid;
            int  _seatid   = pQuery->vctRes[0].m_iSeatid;

            char szJsonBuff[1024]  = {0};
            char format[256] = 	"{\"protocol\":\"%d\","
                                " \"errmsg\":\"%d\","
                                " \"battleid\":\"%d\","
                                " \"seatid\":\"%d\","
                                " \"userid\":\"%d\","
                                " \"userkey\":\"%d\" }";

            sprintf( szJsonBuff, format, MAKEDWORD( (WORD)Login_Protocol,(WORD)Relogin_ANC ),
                    _iError, _battleid, _seatid, _userid, _userkey );

            int nLength = strlen(szJsonBuff);
            pServerSession->Send( (BYTE*)szJsonBuff, nLength );
            return TRUE;
        }
    }

    {
        char szJsonBuff[1024]  = {0};
        char format[256] = 	"{\"protocol\":\"%d\", \"errmsg\":\"%d\" }";
        sprintf( szJsonBuff, format, MAKEDWORD( (WORD)Login_Protocol, (WORD)Relogin_ANC), _iError );
        int nLength = strlen(szJsonBuff);
        pServerSession->Send( (BYTE*)szJsonBuff, nLength );
    }
    return FALSE;
}

void MSG_Handler_Relogin_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_Relogin * pQuery = (Query_Relogin*) msg->m_pData;
    if ( pQuery ) {
        User_Result_Relogin( pServerSession, pQuery );
        Query_Relogin::FREE(pQuery);
    }

}
