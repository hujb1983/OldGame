#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*  1. 桌子信息 */
class Query_TableInfo : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_TableInfo );

public:

	enum {
		RESULT_COL_NUM = 7,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;             // 错误ID
        int  m_iRoomID;            // 房间ID
		int  m_iTableID;           // 桌子ID
		char m_szName[33];         // 房间名称
		int  m_iDoubles;           // 是否加倍
		int  m_iOnlines;           // 在线人数
		int  m_iMoney;             // 带入金额
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
		_BINDING_COLUMN(1, m_iRoomID)
		_BINDING_COLUMN(2, m_iTableID)
		_BINDING_COLUMN_PTR(3, m_szName)
		_BINDING_COLUMN(4, m_iDoubles)
		_BINDING_COLUMN(5, m_iOnlines)
		_BINDING_COLUMN(6, m_iMoney)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_TableInfo);

#pragma pack(pop)


// 房间号
#define DB_ROOMS_SIZE   (6)

int User_Query_TableInfo ( ServerSession * pServerSession, const char * js_text )
{
    Query_TableInfo * pQuery = Query_TableInfo::ALLOC();
    if ( NULL == pQuery ) {
        return (-1);     // 比较忙
    }

    char szQueryBuff[256] = {0};
    snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_TableListInfo(); ");

    pQuery->SetIndex( MAKEDWORD( (WORD)Update_Protocol, (WORD)TableInfo_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;
}


void MSG_Handler_TableInfo ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_TableInfo( pServerSession, (char*)pMsg );
}


int User_Result_TableInfo  ( ServerSession * pServerSession, QueryResult * pResult )
{
    Query_TableInfo * pQuery = (Query_TableInfo *) pResult;
    if ( pQuery->vctRes.size() > 0 )
    {
        int iSize = pQuery->vctRes.size();

        JsonBuffer js_send ;
        JsonBuffer js_table;
        JsonBuffer js_array;

        js_send.clear_buff();
        js_send.WriteInteger("protocol", MAKEDWORD( Update_Protocol, TableInfo_ANC ) );
        js_send.WriteString ("userkey",  "%d");
        js_send.WriteInteger("count",    iSize );

        char szTableName[33] = {0};

        for(int i=0; i<iSize; ++i)
        {
            if ( pQuery->vctRes[i].m_iError==0 ) {
                memset(szTableName, 0x0, sizeof(szTableName) );
                memcpy( szTableName, pQuery->vctRes[i].m_szName, sizeof(szTableName) );

                js_table.clear_buff();
                js_table.WriteInteger ("roomid",    pQuery->vctRes[i].m_iRoomID  );
                js_table.WriteInteger ("tableid",   pQuery->vctRes[i].m_iTableID );
                js_table.WriteString  ("name",      szTableName  );
                js_table.WriteInteger ("doubles",   pQuery->vctRes[i].m_iDoubles  );
                js_table.WriteInteger ("onlines",   pQuery->vctRes[i].m_iOnlines );
                js_table.WriteInteger ("dmoney",    pQuery->vctRes[i].m_iMoney   );
                js_table.package_one();
                js_array.WriteJson(js_table);
            }
        }

        js_array.package_array();
        js_send.WriteJson( "tables", js_array );
        js_send.package_one();

        const char * sendMsg = js_send.get_json();
        int nLength = strlen(sendMsg);
        pServerSession->Send( (BYTE*)sendMsg, nLength );
    }

    Query_TableInfo::FREE(pQuery);
}


void MSG_Handler_TableInfo_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_TableInfo * pQuery = (Query_TableInfo*) msg->m_pData;
    if ( pQuery ) {
        User_Result_TableInfo( pServerSession, pQuery );
    }
}
