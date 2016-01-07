#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*  1. 桌子信息 */
class Query_RoomInfo : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_RoomInfo );

public:

	enum {
		RESULT_COL_NUM = 4,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        int  m_iError;             // 错误ID
        int  m_iRoomID;            // 房间ID
		int  m_iTableSize;         // 桌子数量
		char m_szRoomName[33];      // 房间名称
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
		_BINDING_COLUMN(2, m_iTableSize)
		_BINDING_COLUMN_PTR(3, m_szRoomName)
	_END_BINDING_DATA()
};

_IMPL_QUERY_POOL(Query_RoomInfo);

#pragma pack(pop)


// 房间号
#define DB_ROOMS_SIZE   (6)

int User_Query_RoomInfo ( ServerSession * pServerSession, const char * js_text )
{
    Query_RoomInfo * pQuery = Query_RoomInfo::ALLOC();
    if ( NULL == pQuery ) {
        return (-1);     // 比较忙
    }

    JsonMap js_map;
    if ( js_map.set_json( js_text ) == -1 ) {
        return -1;
    }

    char szQueryBuff[256] = {0};
    snprintf( szQueryBuff, sizeof( szQueryBuff ), " call p_RoomListInfo(); " );

    pQuery->SetIndex( MAKEDWORD( (WORD)Update_Protocol, (WORD)RoomInfo_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;
}


void MSG_Handler_RoomInfo ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    User_Query_RoomInfo( pServerSession, (char*)pMsg );
}

int User_Result_RoomInfo  ( ServerSession * pServerSession, QueryResult * pResult )
{
    Query_RoomInfo * pQuery = (Query_RoomInfo *) pResult;

    int  iError, iRoomID, iTableSize;
    char szRoomName[33];
    if ( pQuery->vctRes.size() > 0 )
    {
        int iSize = pQuery->vctRes.size();

        JsonBuffer js_send ;
        JsonBuffer js_table;
        JsonBuffer js_array;

        DWORD dw_rm_pid = MAKEDWORD( Update_Protocol, RoomInfo_ANC );
        js_send.clear_buff();
        js_send.WriteInteger("protocol", dw_rm_pid );
        js_send.WriteString ("userkey",  "%d");
        js_send.WriteInteger("count",    iSize );

        for(int i=0; i<iSize; ++i)
        {
            iError = pQuery->vctRes[i].m_iError;
            if ( iError==0 ) {
                iRoomID = pQuery->vctRes[i].m_iRoomID;
                iTableSize  = pQuery->vctRes[i].m_iTableSize;

                memset(szRoomName, 0x0, sizeof(szRoomName) );
                memcpy( szRoomName, pQuery->vctRes[i].m_szRoomName, sizeof(szRoomName) );

                js_table.clear_buff();
                js_table.WriteInteger ("roomid",  iRoomID );
                js_table.WriteInteger ("tbsize",  iTableSize );
                js_table.WriteString  ("name",    szRoomName );
                js_table.package_one();
                js_array.WriteJson(js_table);
            }
        }

        js_array.package_array();
        js_send.WriteJson( "rooms", js_array );
        js_send.package_one();

        const char * sendMsg = js_send.get_json();
        int nLength = strlen(sendMsg);
        pServerSession->Send( (BYTE*)sendMsg, nLength );
    }

    Query_RoomInfo::FREE(pQuery);
}


void MSG_Handler_RoomInfo_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_RoomInfo * pQuery = (Query_RoomInfo*) msg->m_pData;
    if ( pQuery ) {
        User_Result_RoomInfo( pServerSession, pQuery );
    }
}
