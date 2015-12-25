#include "Handler_Module.h"
#include <dbCommon.h>

#pragma pack(push,1)
class Query_Login : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_Login );

public:
    int  userid;
    WORD userport;
    WORD agentport;

	enum {
		RESULT_COL_NUM = 1,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int m_iError;
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

	void InitData() { vctRes.clear();       }
	void dealData() { vctRes.pop_back();    }

	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 1, RESULT_COL_NUM)
        _BINDING_COLUMN(0, m_iError)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_Login);

#pragma pack(pop)


/*  1. 用户查询；
    2. 返回参数后验证； */
int User_Query_Login( ServerSession * pServerSession, const char * js_text )
{
    DEBUG_MSG( LVL_DEBUG, "Login_REQ to recv: %s \n", (char*)js_text );

    int  _userport;
    int  _agentport;
    int  _dwUserDB;
    char _bySshKey[CODE_KEY_LEN + 1];

    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    js_map.ReadInteger( "userid",   _dwUserDB );
    js_map.ReadString ( "sshkey",   _bySshKey,  sizeof(_bySshKey) );
    js_map.ReadInteger( "userkey",  _userport );
    js_map.ReadInteger( "agentkey", _agentport );

    Query_Login * pQuery = Query_Login::ALLOC();
    if ( NULL == pQuery ) {
        return -1;
    }

    pQuery->userport  = _userport;
    pQuery->agentport = _agentport;
    pQuery->userid    = _dwUserDB;

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_UserLogin(%d, \'%s\'); ", _dwUserDB, _bySshKey );

    pQuery->SetIndex( MAKEDWORD( (WORD)Login_Protocol, (WORD)Login_DBR ) );
    pQuery->SetQuery( szQueryBuff );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery );
    return 0;
}

void MSG_Handler_Login ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_Login(pServerSession, (char*)pMsg);
}

int User_Result_Login ( ServerSession * pServerSession, Query_Login * pQuery )
{
    WORD _userport;
    WORD _agentport;
    int  _userid = 0;
    int   iError, iIndex, iRate;
    char* szName;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        _userport  = pQuery->userport;
        _agentport = pQuery->agentport;
        _userid    = pQuery->userid;

        int iError = pQuery->vctRes[0].m_iError;

        char szJsonBuff[1024] = {0};
        if ( iError == 0 ) {
            snprintf( szJsonBuff, sizeof(szJsonBuff),
                     " { \"userid\" : %d, \"userkey\" : %d, \"agentkey\" : %d, \"protocol\": %d, \"errmsg\": %d } ",
                    _userid, _userport, _agentport, MAKEDWORD(Login_Protocol, Login_ANC), MAKEDWORD(Errors_Protocol, Correct_NAK) );
        }
        else {
            snprintf( szJsonBuff, sizeof(szJsonBuff),
                     " { \"userid\" : %d, \"userkey\" : %d, \"agentkey\" : %d, \"protocol\": %d, \"errmsg\": %d } ",
                    _userid,  _userport, _agentport, MAKEDWORD(Login_Protocol, Login_ANC), MAKEDWORD(Errors_Protocol, ClientLogin_NAK) );
        }

        DEBUG_MSG( LVL_DEBUG, "Login_ANC to send: %s \n", (char*)szJsonBuff );

        int nLength = strlen(szJsonBuff);
        pServerSession->Send( (BYTE*)szJsonBuff, nLength );
    }
}

void MSG_Handler_Login_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // Handler_PrintMsg( (MSG_BASE_FORWARD*)pMsg, "Login_DBR" );
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_Login * pQuery = (Query_Login*) msg->m_pData;
    if ( pQuery ) {
        User_Result_Login( pServerSession, pQuery );
        Query_Login::FREE(pQuery);
    }

}
