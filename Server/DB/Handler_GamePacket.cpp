#include "Handler_Module.h"
#include <dbCommon.h>

#pragma pack(push,1)
class Query_GamePacket : public QueryResult
{
	_DECLARE_QUERY_POOL(Query_GamePacket);

public:

    int  userid;
    WORD userport;
    WORD agentport;

	enum {
		RESULT_COL_NUM = 6,
		PARAM_COL_NUL = 0,
	};

	struct sRESULT {
        int  m_iError;
        char m_szName[33];      // 逃跑次数
		int  m_iPoints; 	    // 分数
		int  m_iWons; 		    // 失败次数
		int  m_iFaileds; 	    // 赢的次数
		int  m_iAways; 		    // 逃跑次数
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

	_BEGIN_BINDING_DATA( sRESULT, vctRes, uLength, 1, RESULT_COL_NUM )
        _BINDING_COLUMN(0, m_iError)
        _BINDING_COLUMN_PTR(1, m_szName)
		_BINDING_COLUMN(2, m_iPoints)
		_BINDING_COLUMN(3, m_iWons)
		_BINDING_COLUMN(4, m_iFaileds)
		_BINDING_COLUMN(5, m_iAways)
	_END_BINDING_DATA()
};
_IMPL_QUERY_POOL(Query_GamePacket);

#pragma pack(pop)

int User_Query_GamePacket ( ServerSession * pServerSession, const char * js_text )
{
    int  _userport;
    int  _agentport;
    int _dwUserDB;
    int _dwGameID;

    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    js_map.ReadInteger( "userkey",  _userport );
    js_map.ReadInteger( "agentkey", _agentport );
    js_map.ReadInteger( "userid", _dwUserDB );
    js_map.ReadInteger( "gameid", _dwGameID );

    Query_GamePacket * pQuery = Query_GamePacket::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    pQuery->userport  = _userport;
    pQuery->agentport = _agentport;
    pQuery->userid    = _dwUserDB;

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_UserGamePacket(%d, %d); ", _dwUserDB, 6);

    pQuery->SetIndex( MAKEDWORD( (WORD)Update_Protocol, (WORD)GamePacket_DBR ) );
    pQuery->SetQuery( szQueryBuff );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery );
    return 0;
}


void MSG_Handler_GamePacket ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_GamePacket(pServerSession, (char*)pMsg );
}

int User_Result_GamePacket ( ServerSession * pServerSession, Query_GamePacket * pQuery )
{
    WORD _userport;
    WORD _agentport;
    int  _userid = 0;

	int   _iPoints;         // 总点数
    int   _iWons; 		    // 失败次数
    int   _iFaileds; 	    // 赢的次数
    int   _iAways; 		    // 逃跑次数
    char  _szName[33];      // 名称

    if ( pQuery->vctRes.size() > 0) {

        _userport  = pQuery->userport;
        _agentport = pQuery->agentport;
        _userid    = pQuery->userid;

        int   iError = pQuery->vctRes[0].m_iError;
        if ( iError == 0 ) {
            _iPoints  = pQuery->vctRes[0].m_iPoints;
            _iWons    = pQuery->vctRes[0].m_iWons;
            _iFaileds = pQuery->vctRes[0].m_iFaileds;
            _iAways   = pQuery->vctRes[0].m_iAways;
            memcpy( _szName, pQuery->vctRes[0].m_szName, sizeof(_szName) );
        }

        JsonBuffer js_send;
        JsonBuffer js_array;

        js_send.clear_buff();
        js_send.WriteInteger("protocol", MAKEDWORD( (WORD)Login_Protocol, (WORD)GamePacket_ANC ) );
        js_send.WriteInteger("agentkey", _agentport );
        js_send.WriteInteger("userkey" , _userport  );
        js_send.WriteInteger("userid"  , _userid    );

        if ( iError==0 ) {
            js_array.clear_buff();
            js_array.WriteInteger ( "points",     _iPoints  );
            js_array.WriteInteger ( "wons",       _iWons    );
            js_array.WriteInteger ( "faileds",    _iFaileds );
            js_array.WriteInteger ( "aways",      _iAways   );
            js_array.WriteString  ( "name",       _szName   );
            js_array.package_one();
        }

        js_array.package_array();
        js_send.WriteJson( "data", js_array );
        js_send.package_one();

        Query_GamePacket::FREE(pQuery);

        const char * sendMsg = js_send.get_json();
        int nLength = strlen(sendMsg);
        printf("[User_Result_GamePacket] %s \n", sendMsg);
        pServerSession->Send( (BYTE*)sendMsg, nLength );
    }

    return 0;
}

void MSG_Handler_GamePacket_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // Handler_FromGameServer_PrintMsg( (MSG_BASE_FORWARD*)pMsg, "GamePacket_DBR" );

    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_GamePacket * pQuery = (Query_GamePacket*) msg->m_pData;
    if ( pQuery ) {
        User_Result_GamePacket( pServerSession, pQuery );
    }
}


