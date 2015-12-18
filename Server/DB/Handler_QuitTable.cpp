#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


/*  1. �������������ݿ��в��� */
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
		int  m_iStatus;     // �Ƿ񶼿�ʼ
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

int User_Query_QuitTable( ServerSession * pServerSession, const char * js_text )
{
    Query_QuitTable * pQuery = Query_QuitTable::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // �Ƚ�æ
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_UserQuitTable( %d ); ");

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)StartGame_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;     // �Ƚ�æ
}

void MSG_Handler_QuitTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_QuitTable( pServerSession, (char*)pMsg );
}

int User_Result_QuitTable ( ServerSession * pServerSession, Query_QuitTable * pQuery )
{
    int iError;
    int iSize = pQuery->vctRes.size();

    if ( iSize > 0 )
    {
        int iError = pQuery->vctRes[0].m_iError;
        printf( " m_iError = %d \n", iError );

        // ��ӡ��־��Ϣ����
    }
}

void MSG_Handler_QuitTable_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_QuitTable * pQuery = (Query_QuitTable*) msg->m_pData;
    if ( pQuery ) {
        User_Result_QuitTable( pServerSession, pQuery );
        Query_QuitTable::FREE(pQuery);
    }
}

