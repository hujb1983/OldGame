#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*  1. 日排名，从数据库中查找 */
class Query_DayRank : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_DayRank );

public:
	enum
	{
		RESULT_COL_NUM = 4,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
		int  m_iIndex;
		char m_cName[32];
		int  m_iRate;

		sRESULT()
		{
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
		_BINDING_COLUMN(1, m_iIndex)
		_BINDING_COLUMN_PTR(2, m_cName)
		_BINDING_COLUMN(3, m_iRate)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_DayRank);

#pragma pack(pop)

int User_Query_DayRank( ServerSession * pServerSession, const char * js_text )
{
    Query_DayRank * pQuery = Query_DayRank::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_DayRank(); ");

    pQuery->SetIndex( MAKEDWORD( (WORD)Update_Protocol, (WORD)DRankInfo_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;
}

void MSG_Handler_DRankInfo ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_DayRank( pServerSession, (char*)pMsg );
}

int User_Result_DayRankInfo ( ServerSession * pServerSession, Query_DayRank * pQuery )
{
    int   iError, iIndex, iRate;
    char* szName;

    int iSize = pQuery->vctRes.size();
    if ( iSize > 0 )
    {
        JsonBuffer js_send ;
        JsonBuffer js_rank ;
        JsonBuffer js_array;

        js_send.clear_buff();
        js_send.WriteInteger("protocol",  MAKEDWORD( (WORD)Update_Protocol, (WORD)DRankInfo_ANC) );
        js_send.WriteString ("userkey",  "%d");

        for(int i=0; i<iSize; ++i)
        {
            iError = pQuery->vctRes[i].m_iError;
            if ( iError==0 ) {
                iIndex = pQuery->vctRes[i].m_iIndex;
                memset( szName, 0x0, sizeof(szName) );
                strcat( szName, pQuery->vctRes[i].m_cName );
                iRate  = pQuery->vctRes[i].m_iRate ;

                /*  printf( "iIndex  = %d; \n", iIndex );
                    printf( "szName  = %s; \n", szName );
                    printf( "iRate   = %d; \n", iRate );    */

                js_rank.clear_buff();
                js_rank.WriteInteger("rank", iIndex );
                js_rank.WriteString ("name", szName );
                js_rank.WriteInteger("rate", iRate  );
                js_rank.package_one();
                js_array.WriteJson(js_rank);
            }
        }

        js_array.package_array();
        js_send.WriteJson( "ranking", js_array );
        js_send.package_one();

        const char * sendMsg = js_send.get_json();
        int nLength = strlen(sendMsg);
        pServerSession->Send( (BYTE*)sendMsg, nLength );
    }

    Query_DayRank::FREE(pQuery);
}

void MSG_Handler_DRankInfo_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_DayRank * pQuery = (Query_DayRank*) msg->m_pData;
    if ( pQuery ) {
        User_Result_DayRankInfo( pServerSession, pQuery );
    }
}

