#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


/*  1. 日排名，从数据库中查找用户并清理掉 */
class Query_QuitTable : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_QuitTable );

public:

	enum
	{
		RESULT_COL_NUM = 3,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;
		int  m_iUserid;
		int  m_iStatus;     // 是否都开始
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
		_BINDING_COLUMN(1, m_iUserid)
		_BINDING_COLUMN(2, m_iStatus)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_QuitTable);

#pragma pack(pop)

void MSG_Handler_QuitTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Query_QuitTable * pQuery = Query_QuitTable::ALLOC();
    if ( NULL == pQuery ) {
        return;     // 比较忙
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_UserQuitTable( %d ); ");

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)QuitTable_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
}

void MSG_Handler_QuitTable_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_QuitTable * pQuery = (Query_QuitTable *) msg->m_pData;
    if ( pQuery ) {
        int iError;
        int iSize = pQuery->vctRes.size();

        if ( iSize > 0 )
        {
            int iError = pQuery->vctRes[0].m_iError;
            printf( " m_iError = %d \n", iError );

            // 打印日志消息即可
        }

        Query_QuitTable::FREE(pQuery);
    }
}

