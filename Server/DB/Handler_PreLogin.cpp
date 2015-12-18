#include "Handler_Module.h"
#include "Yond_drng.h"
#include "Yond_md5.h"
#include <dbCommon.h>
#pragma pack(push,1)

class Query_PreLogin : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_PreLogin );

public:

    WORD wUserKey; // 用户端口

	enum
	{
		RESULT_COL_NUM = 4,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
		int 		    m_iError;
		unsigned int    m_uiUserID;
		unsigned int    m_uiPort;
		char            m_szIP[16];
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
		_BINDING_COLUMN(1, m_uiUserID)
		_BINDING_COLUMN(2, m_uiPort)
		_BINDING_COLUMN(3, m_szIP)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_PreLogin);
#pragma pack(pop)


/*  1. 用户查询；
    2. 返回参数后验证； */
int User_Query_PreLogin( ServerSession * pServerSession, const char * js_text )
{

    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    int   _iUserDB   = 0;
    int   _iUserKey  = 0;
    int   _iAccessID = 0;
    char  _cUsername[33] = {0};
    char  _cPassword[33] = {0};
    char  _cSshKey[33]   = {0};
    int   _iGameID = 0;
    int   _iLoginType = 0;

    js_map.ReadInteger( "userkey",   _iUserKey  );
    js_map.ReadInteger( "aid",       _iAccessID );
    js_map.ReadString ( "username",  _cUsername, sizeof(_cUsername) );
    js_map.ReadString ( "password",  _cPassword, sizeof(_cPassword) );
    js_map.ReadString ( "sshkey",    _cSshKey  , sizeof(_cSshKey)   );
    js_map.ReadInteger( "gameid",    _iGameID    );
    js_map.ReadInteger( "logintype", _iLoginType );

    Query_PreLogin * pQuery = Query_PreLogin::ALLOC();
    if ( NULL == pQuery ) {
        return -1;
    }

    pQuery->wUserKey  = _iUserKey;

    {
        // 生成 用户 MD5s( 随机数 + lastime + rid )
        char szUserSshKey[ CODE_KEY_LEN + 1 ] = { 0 }; // 33位
        char szRandom[11] = {0};    // 生成一个5位的随机值
        if ( is_support_drng() ) {  // 支持随机数
            GetRandom(szRandom);
        }
        else {  // 不支持随机数
            GetRandom_C(szRandom);
        }
        printf("Random = %s\n", szRandom);

        // MD5加密
        string tmpStr(szRandom);
        string outMD5 = MD5(tmpStr).toString() ;
        sprintf( (char *)szUserSshKey, "%s", outMD5.c_str() );
        szUserSshKey[ CODE_KEY_LEN ] = '\0';
        printf("User SSH Key = %s\n", szUserSshKey );

        char szQueryBuff[256] = {0};
        snprintf( szQueryBuff, sizeof(szQueryBuff), "call p_PreLogin(\'%s\',\'%s\',\'%s\',\'%s\', %d, %d)",
            _cUsername, _cPassword, _cSshKey, szUserSshKey, _iGameID, _iLoginType );

        pQuery->SetIndex( MAKEDWORD( (WORD)Login_Protocol, (WORD)PreLogin_DBR ) );
        pQuery->SetQuery( szQueryBuff );
        pQuery->SetVoidObject( pServerSession );
        pServerSession->DBQuery( pQuery );
    }

    return 0;
}

void MSG_Handler_PreLogin ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_PreLogin( pServerSession, (char*)pMsg );
}

int User_Result_PreLogin ( ServerSession * pServerSession, Query_PreLogin * pQuery )
{
    int     _iError = 0;
    char*   _szName;

    WORD _userkey = pQuery->wUserKey;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        _iError = pQuery->vctRes[0].m_iError;
        if ( _iError==0 )
        {
            int  _userid  = pQuery->vctRes[0].m_uiUserID;
            int  _sport   = pQuery->vctRes[0].m_uiPort;
            char _sip[16] = {0};
            strcat(_sip , pQuery->vctRes[0].m_szIP );

            char szJsonBuff[1024]  = {0};
            char format[256] = 	"{\"protocol\":\"%d\","
                                " \"userkey\":\"%d\","
                                " \"data\":{"
                                " \"errmsg\":\"0\","
                                " \"userid\":\"%d\","
                                " \"sip\":\"%s\","
                                " \"port\":\"%d\"} }";

            sprintf( szJsonBuff, format, MAKEDWORD( (WORD)Login_Protocol,(WORD)PreLogin_ANC ),
                    _userkey, _userid, _sip, _sport );

            int nLength = strlen(szJsonBuff);
            pServerSession->Send( (BYTE*)szJsonBuff, nLength );
            return TRUE;
        }
    }

    {
        char szJsonBuff[1024]  = {0};
        char format[256] = 	"{\"protocol\":\"%d\", \"userkey\":\"%d\" }";
        sprintf( szJsonBuff, format, MAKEDWORD( (WORD)Login_Protocol, (WORD)PreLogin_NAK), _userkey );
        int nLength = strlen(szJsonBuff);
        pServerSession->Send( (BYTE*)szJsonBuff, nLength );
    }
    return FALSE;
}

void MSG_Handler_PreLogin_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // Handler_PrintMsg( (MSG_BASE_FORWARD*)pMsg, "Login_DBR" );
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_PreLogin * pQuery = (Query_PreLogin*) msg->m_pData;
    if ( pQuery ) {
        User_Result_PreLogin( pServerSession, pQuery );
        Query_PreLogin::FREE(pQuery);
    }

}
