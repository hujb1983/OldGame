#include "Handler_Module.h"
#include "UserPacket.h"
#include <dbCommon.h>

#pragma pack(push,1)
/*********************************************************
    Query_Login
*********************************************************/
class Query_Login : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_Login );

public:

    UserPacket m_cPacket;   // 数据包

	enum { RESULT_COL_NUM = 2, PARAM_COL_NUL  = 0,};

	struct sRESULT {
        BYTE m_byError;
        UINT m_uiLoginTime;
		sRESULT() { memset( this, 0, sizeof(sRESULT) ); }
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
        _BINDING_COLUMN(0, m_byError)
        _BINDING_COLUMN(1, m_uiLoginTime)
	_END_BINDING_DATA()
};
_IMPL_QUERY_POOL(Query_Login);


/*********************************************************
    Query_GamePacket
*********************************************************/
class Query_GamePacket : public QueryResult
{
	_DECLARE_QUERY_POOL(Query_GamePacket);

public:

    UserPacket m_cPacket;   // 数据包

	enum { RESULT_COL_NUM = 6, PARAM_COL_NUL = 0, };

	struct sRESULT {
        int  m_byError;
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
        _BINDING_COLUMN(0, m_byError)
        _BINDING_COLUMN_PTR(1, m_szName)
		_BINDING_COLUMN(2, m_iPoints)
		_BINDING_COLUMN(3, m_iWons)
		_BINDING_COLUMN(4, m_iFaileds)
		_BINDING_COLUMN(5, m_iAways)
	_END_BINDING_DATA()
};
_IMPL_QUERY_POOL(Query_GamePacket);


/*********************************************************
    Query_Relogin
*********************************************************/
class Query_Relogin : public QueryResult
{
	_DECLARE_QUERY_POOL( Query_Relogin );

public:

    UserPacket m_cPacket;// 数据包

	enum { RESULT_COL_NUM = 7, PARAM_COL_NUL = 0, };

	struct sRESULT {
		BYTE m_byError;
		UINT m_uiBattleId;
		UINT m_uiFieldId;
		UINT m_uiRoomId;
		UINT m_uiTableId;
		UINT m_uiSeatId;
		UINT m_uiUserKey;
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
		_BINDING_COLUMN(0, m_byError)
		_BINDING_COLUMN(1, m_uiBattleId)
		_BINDING_COLUMN(2, m_uiFieldId)
		_BINDING_COLUMN(3, m_uiRoomId)
		_BINDING_COLUMN(4, m_uiTableId)
		_BINDING_COLUMN(5, m_uiSeatId)
		_BINDING_COLUMN(6, m_uiUserKey)
	_END_BINDING_DATA()
};
_IMPL_QUERY_POOL(Query_Relogin);

#pragma pack(pop)


/*********************************************************
    Step 1: MSG_Handler_Login
*********************************************************/
void MSG_Handler_Login ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "MSG_Handler_Login : %d \n", wSize );

    Query_Login * pQuery = Query_Login::ALLOC();
    if ( NULL == pQuery ) {
        return;
    }

    // 取得数据
    char _sshKey[CODE_KEY_LEN + 1];
    if ( wSize>=sizeof(UserPacket) ) {
        pQuery->m_cPacket.SetPacket( (BYTE*) pMsg, wSize );
        pQuery->m_cPacket.ToPrint();
    }

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_UserLogin(%d, \'%s\'); ",
        pQuery->m_cPacket.GetUserId(), pQuery->m_cPacket.GetSSHKey() );

    pQuery->SetIndex( MAKEDWORD( (WORD)Login_Protocol, (WORD)Login_DBR ) );
    pQuery->SetQuery( szQueryBuff );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery );
}


/*********************************************************
    Step 3: User_Query_GamePacket
*********************************************************/
int User_Query_GamePacket ( ServerSession * pServerSession, UserPacket & pack, UINT _userid, UINT _gametype = 6 )
{
    Query_GamePacket * pQuery = Query_GamePacket::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    // 取得数据
    pQuery->m_cPacket.SetPacket( (BYTE*) &pack, pack.GetPacketSize() );
    pQuery->m_cPacket.ToPrint();

    char szQueryBuff[256] = {0};
	snprintf( szQueryBuff, sizeof(szQueryBuff), " call p_UserGamePacket(%d, %d); ", _userid, _gametype);

    pQuery->SetIndex( MAKEDWORD( (WORD)Login_Protocol, (WORD)GamePacket_DBR ) );
    pQuery->SetQuery( szQueryBuff );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery );
    return 0;
}


/*********************************************************
    Step 2: MSG_Handler_Login_DBR
*********************************************************/
void MSG_Handler_Login_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_Login * pQuery = (Query_Login*) msg->m_pData;
    if ( NULL == pQuery ) {
        return;
    }

    if ( pQuery->vctRes.size() > 0 )
    {
        int iError = pQuery->vctRes[0].m_byError;
        if ( iError == 0 ) {
            pQuery->m_cPacket.GetProtocol()  = MAKEDWORD(Login_Protocol, Login_ANC);
            pQuery->m_cPacket.GetLoginTime() = pQuery->vctRes[0].m_uiLoginTime;
            pQuery->m_cPacket.ToPrint();

            // 获取数据
            User_Query_GamePacket( pServerSession, pQuery->m_cPacket, pQuery->m_cPacket.GetUserId(), 6 );
            Query_Login::FREE(pQuery);
            return ;    // 与用户游戏参数一起发送
        }
    }

    // 错误发送
    pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize() );
    Query_Login::FREE(pQuery);
}


/*********************************************************
    Step 5: User_Query_Relogin
*********************************************************/
int User_Query_Relogin( ServerSession * pServerSession, UserPacket & pack, UINT _userid )
{
    Query_Relogin * pQuery = Query_Relogin::ALLOC();
    if ( NULL == pQuery ) {
        return -1;     // 比较忙
    }

    // 取得数据
    pQuery->m_cPacket.SetPacket( (BYTE*) &pack, pQuery->m_cPacket.GetPacketSize() );
    pQuery->m_cPacket.ToPrint();

    char szQueryBuff[256] = {0};
    snprintf( szQueryBuff, sizeof(szQueryBuff), "call p_UserRelogin( %d )", _userid );
    pQuery->SetIndex( MAKEDWORD( (WORD)Login_Protocol, (WORD)Relogin_DBR ) );
    pQuery->SetQuery( szQueryBuff );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery );
    return 0;
}


/*********************************************************
    Step 4: MSG_Handler_GamePacket_DBR
*********************************************************/
void MSG_Handler_GamePacket_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_GamePacket * pQuery = (Query_GamePacket*) msg->m_pData;
    if ( NULL == pQuery ) {
        return;
    }

    if ( pQuery->vctRes.size() > 0 )
    {
        int iError = pQuery->vctRes[0].m_byError;
        if ( iError == 0 ) {
            pQuery->m_cPacket.GetMoney() = pQuery->vctRes[0].m_iPoints;
            pQuery->m_cPacket.GetWoneds() = pQuery->vctRes[0].m_iWons;
            pQuery->m_cPacket.GetFaileds() = pQuery->vctRes[0].m_iFaileds;
            pQuery->m_cPacket.GetAways() = pQuery->vctRes[0].m_iAways;
            memcpy( pQuery->m_cPacket.GetName(), pQuery->vctRes[0].m_szName, 32 );
            pQuery->m_cPacket.ToPrint();
        }
        User_Query_Relogin( pServerSession, pQuery->m_cPacket, pQuery->m_cPacket.GetUserId() );
        Query_GamePacket::FREE(pQuery);
        return ;    // 与用户游戏参数一起发送
    }

    // 错误发送
    pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize() );
    Query_GamePacket::FREE(pQuery);
}

/*********************************************************
    Step 6: MSG_Handler_Relogin_DBR
*********************************************************/
void MSG_Handler_Relogin_DBR ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    MSG_DBPROXY_RESULT * msg = (MSG_DBPROXY_RESULT*) pMsg;
    Query_Relogin * pQuery = (Query_Relogin*) msg->m_pData;
    if ( NULL == pQuery ) {
        return;
    }

    if ( pQuery->vctRes.size() > 0 )
    {
        int iError = pQuery->vctRes[0].m_byError;
        if ( iError == 0 ) {
            pQuery->m_cPacket.GetBattleId() = pQuery->vctRes[0].m_uiBattleId;
            pQuery->m_cPacket.GetFieldId() = pQuery->vctRes[0].m_uiFieldId;
            pQuery->m_cPacket.GetRoomId() = pQuery->vctRes[0].m_uiRoomId;
            pQuery->m_cPacket.GetTableId() = pQuery->vctRes[0].m_uiTableId;
            pQuery->m_cPacket.GetSeatId() = pQuery->vctRes[0].m_uiSeatId;
            pQuery->m_cPacket.GetOldKey() = pQuery->vctRes[0].m_uiUserKey;
            pQuery->m_cPacket.ToPrint();
        }
        pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize() );
        Query_Relogin::FREE(pQuery);
        return ;
    }

    // 错误发送
    pServerSession->Send( (BYTE*)&pQuery->m_cPacket, pQuery->m_cPacket.GetPacketSize());
    Query_Relogin::FREE(pQuery);
}
