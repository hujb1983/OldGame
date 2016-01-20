#include "Handler_Module.h"
#include "UserPacket.h"
#include "DBServer.h"
#include <dbCommon.h>

#pragma pack(push,1)
/*********************************************************
    Query_JoinTable
*********************************************************/
class Query_JoinTable : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_JoinTable );

public:

    UserPacket m_cPacket;// 数据包

	enum {
		RESULT_COL_NUM = 4,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        BYTE m_byError;
        UINT m_uiBattleId;
        UINT m_uiTableId;
        UINT m_uiSeatId;
		sRESULT() { memset( this, 0, sizeof(sRESULT) ); }
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
        _BINDING_COLUMN(0, m_byError)
        _BINDING_COLUMN(1, m_uiBattleId)
        _BINDING_COLUMN(2, m_uiTableId)
        _BINDING_COLUMN(3, m_uiSeatId)
	_END_BINDING_DATA()
};
_IMPL_QUERY_POOL(Query_JoinTable);


/*********************************************************
    Query_CreateTable
*********************************************************/
class Query_CreateTable : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_CreateTable );

public:

    UserPacket m_cPacket;// 数据包

	enum {
		RESULT_COL_NUM = 4,
		PARAM_COL_NUL  = 0,
	};

	struct sRESULT {
        BYTE m_byError;
        UINT m_uiBattleId;
        UINT m_uiTableId;
        UINT m_uiSeatId;
		sRESULT() { memset( this, 0, sizeof(sRESULT) ); }
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
        _BINDING_COLUMN(0, m_byError)
        _BINDING_COLUMN(1, m_uiBattleId)
        _BINDING_COLUMN(2, m_uiTableId)
        _BINDING_COLUMN(3, m_uiSeatId)
	_END_BINDING_DATA()
};
_IMPL_QUERY_POOL(Query_CreateTable);

#pragma pack(pop)


/*********************************************************
    Step 1: MSG_Handler_JoinTable
*********************************************************/
void MSG_Handler_JoinTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Query_JoinTable * pQuery = Query_JoinTable::ALLOC();
    if ( NULL == pQuery ) {
        return;     // 比较忙
    }

    // 取得数据
    if ( wSize>=sizeof(UserPacket) ) {
        pQuery->m_cPacket.SetPacket( (BYTE*) pMsg, wSize );
        pQuery->m_cPacket.ToPrint();
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_UserJoinTable(%d, %d, %d, %d, %d); ",
          pQuery->m_cPacket.GetUserId(),  pQuery->m_cPacket.GetUserKey(), pQuery->m_cPacket.GetAgentKey(),
          pQuery->m_cPacket.GetFieldId(), pQuery->m_cPacket.GetRoomId() );

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)JoinTable_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return;
}

/*********************************************************
    Step 3: User_Query_CreateTable
*********************************************************/
int User_Query_CreateTable ( ServerSession * pServerSession, UserPacket & pack )
{
    Query_CreateTable * pQuery = Query_CreateTable::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    // 取得数据
    pQuery->m_cPacket.SetPacket( (BYTE*)&pack, pack.GetPacketSize() );
    pQuery->m_cPacket.ToPrint();

    UINT wTable = g_DBServer->GetTableKey();
    if (wTable==0 || wTable>0xFF) { // 先保证65535个桌子数运行正常
        return -1;
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_UserCreateTable(%d, %d, %d, %d, %d, %d); ",
          pQuery->m_cPacket.GetUserId(),  pQuery->m_cPacket.GetUserKey(), pQuery->m_cPacket.GetAgentKey(),
          pQuery->m_cPacket.GetFieldId(), pQuery->m_cPacket.GetRoomId(), wTable );

    pQuery->SetIndex( MAKEDWORD( (WORD)Games_Protocol, (WORD)CreateTable_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pQuery->SetQuery( szQueryBuff );
    pServerSession->DBQuery( pQuery );
    return 0;
}

/*********************************************************
    Step 2: MSG_Handler_Login_DBR
*********************************************************/
void MSG_Handler_JoinTable_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_JoinTable * pQuery = (Query_JoinTable*) msg->m_pData;
    if ( NULL == pQuery ) {
        return;
    }

    if ( pQuery->vctRes.size() > 0 )
    {
        int iError = pQuery->vctRes[0].m_byError;
        if ( iError == 0 ) {
            pQuery->m_cPacket.GetProtocol()  = MAKEDWORD(Games_Protocol, JoinTable_ANC);
            pQuery->m_cPacket.GetBattleId() = pQuery->vctRes[0].m_uiBattleId;
            pQuery->m_cPacket.GetTableId() = pQuery->vctRes[0].m_uiTableId;
            pQuery->m_cPacket.GetSeatId() = pQuery->vctRes[0].m_uiSeatId;
            pQuery->m_cPacket.ToPrint();

            pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize() );
            Query_JoinTable::FREE(pQuery);
            return ;    // 与用户游戏参数一起发送
        }
    }

    // 创建一个桌子
    User_Query_CreateTable( pServerSession, pQuery->m_cPacket );
}


/*********************************************************
    Step 4: MSG_Handler_CreateTable_DBR
*********************************************************/
void MSG_Handler_CreateTable_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_CreateTable * pQuery = (Query_CreateTable*) msg->m_pData;
    if ( NULL == pQuery ) {
        return;
    }

    if ( pQuery->vctRes.size() > 0 )
    {
        int iError = pQuery->vctRes[0].m_byError;
        if ( iError == 0 ) {
            pQuery->m_cPacket.GetProtocol() = MAKEDWORD(Games_Protocol, JoinTable_ANC);
            pQuery->m_cPacket.GetBattleId() = pQuery->vctRes[0].m_uiBattleId;
            pQuery->m_cPacket.GetTableId() = pQuery->vctRes[0].m_uiTableId;
            pQuery->m_cPacket.GetSeatId() = pQuery->vctRes[0].m_uiSeatId;
            pQuery->m_cPacket.ToPrint();

            pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize() );
            Query_CreateTable::FREE(pQuery);
            return ;    // 与用户游戏参数一起发送
        }
    }

    // 创建一个
    pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize() );
    Query_CreateTable::FREE(pQuery);
}
