#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*  1. 在线人数，从数据库中查找 */
class Query_OnlineInfo : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_OnlineInfo );

public:

	enum {
		RESULT_COL_NUM = 2,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int m_iError;
        int m_iCount;            // 房间ID
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
		_BINDING_COLUMN(1, m_iCount)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_OnlineInfo);

#pragma pack(pop)

int User_Query_OnlineInfo( ServerSession * pServerSession, const char * js_text )
{
    Query_OnlineInfo * pQuery = Query_OnlineInfo::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff),
          " call p_OnlinesInfo(); ");

    pQuery->SetIndex( MAKEDWORD( (WORD)Update_Protocol, (WORD)OnlineInfo_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;
}

void MSG_Handler_Onlines ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_OnlineInfo( pServerSession, (char*)pMsg );
}

int User_Result_OnlineInfo( ServerSession * pServerSession, Query_OnlineInfo * pQuery )
{
    char _szOnlineInfo[256] = {0};
    if ( pQuery->vctRes.size() > 0 ) {

        int iError = pQuery->vctRes[0].m_iError;
        int iCount = pQuery->vctRes[0].m_iCount;

        if ( iError==0 ) {

            memset( _szOnlineInfo, 0x0, sizeof(_szOnlineInfo) );
            snprintf( _szOnlineInfo, sizeof(_szOnlineInfo),
                     " {\"protocol\":\"%d\","
                     " \"data\":{\"Onlines\":\" %d \"} } ",
                     MAKEDWORD(Update_Protocol, OnlineInfo_ANC), iCount);
        }

        /*  printf( "iError         = %d; \n", iError );
            printf( "iCount         = %d; \n", iCount );
            printf( "szOnlineInfo   = %s; \n", szOnlineInfo );  */

        printf( "m_szOnlineInfo   = %s; \n", _szOnlineInfo );

        Query_OnlineInfo::FREE(pQuery);

        int nLength = strlen(_szOnlineInfo);
        pServerSession->Send( (BYTE*)_szOnlineInfo, nLength );
    }
}

void MSG_Handler_Onlines_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // Handler_FromGameServer_PrintMsg( (MSG_BASE_FORWARD*)pMsg, "OnlineInfo_DBR" );

    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_OnlineInfo * pQuery = (Query_OnlineInfo*) msg->m_pData;
    if ( pQuery ) {
        User_Result_OnlineInfo( pServerSession, pQuery );
    }
}
