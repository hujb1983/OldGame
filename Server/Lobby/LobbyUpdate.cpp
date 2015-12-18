#include "LobbyUpdate.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include <JsonParser.h>

#define MINUTE  (60000)

LobbyUpdate g_LobbyUpdate;

LobbyUpdate::LobbyUpdate() {
    memset( mapUserObject, 0x0, sizeof(mapUserObject) );
}

LobbyUpdate::~LobbyUpdate() {
}

/* 从数据库查询到所有信息 */
int LobbyUpdate::Init()
{
    printf("LobbyUpdate::Init\n");

    memset( this, 0x0, sizeof(this) );
    this->QueryRoomInfo();
    this->QueryTableInfo();
    this->QueryOnlineInfo();
    this->QueryWeekRank();
    this->QueryDayRank();
}

int LobbyUpdate::Update(DWORD dwDeltaTick)
{
    if ( m_dwUpdateTime < dwDeltaTick ) {
        printf("%d, %d \n", dwDeltaTick, m_dwUpdateTime);
        m_dwUpdateTime = dwDeltaTick + 50000;
        SendOnlinesToAllUser();
    }
    return TRUE;
}

int LobbyUpdate::SendAllUser( BYTE * pMsg, WORD wSize )
{
    LobbyUser * pUser = m_pAllUser.get_frist();
    for ( ; pUser ; pUser = pUser->get_next() ) {
        pUser->SendToClient(pMsg, wSize);
    }
}

/* 发送在线人数到所有用户列表 */
int LobbyUpdate::SendOnlinesToAllUser()
{
    char szInfo[1024] = {0};
    int nLen = JsonOnlineInfo( szInfo, sizeof(szInfo) );
    SendAllUser( (BYTE*) szInfo, nLen);
}

void LobbyUpdate::SendUpdateTableOnlinesToAllUser ( )
{

    /*for (int i=0; i<byRoomGroupSize; ++i)
    char szInfo[1024] = {0};
    int nLen = JsonOnlineInfo( szInfo, sizeof(szInfo) );
    SendAllUser( (BYTE*) szInfo, nLen);*/

}

/***********************************************
    消息处理
************************************************/
void LobbyUpdate::JoinLobby ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    iOnlinesInfo++;
    LobbyUser * pUser = LobbyFactory::Instance()->AllocUserSession();
    if (pUser) {
        pUser->SetHashKey(_userport);
    }

    mapUserObject[_userport] = pUser;
    m_pAllUser.push_back( pUser );
}

void LobbyUpdate::QuitLobby ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );
    if ( mapUserObject[_userport] )
    {
        LobbyFactory::Instance()->FreeUserSession( mapUserObject[_userport] );
        m_pAllUser.remove( mapUserObject[_userport] );
        mapUserObject[_userport] = NULL;
        iOnlinesInfo--;
    }
}

void LobbyUpdate::JoinTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    printf("LobbyUpdate::JoinTable\n");
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Games_Protocol, JoinTable_REQ), _userport );
    int nLen = strlen(szMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) szMsg, nLen );
}

void LobbyUpdate::QuitTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    printf("LobbyUpdate::QuitTable\n");
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Games_Protocol, QuitTable_REQ), _userport );
    int nLen = strlen(szMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) szMsg, nLen );
}

void LobbyUpdate::RoomInfo_REQ  (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    int _userroom;
    js_map.ReadInteger( "userkey", _userport );

    if ( mapUserObject[(WORD)_userport] ) {
        char szInfo[1024] = {0};
        int nLen = JsonRoomInfo( szInfo, sizeof(szInfo) );
        mapUserObject[_userport]->SendToClient( (BYTE*)szInfo, wSize);
    }
}

void LobbyUpdate::TableInfo_REQ (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    int _userroom;
    js_map.ReadInteger( "userkey", _userport );
    js_map.ReadInteger( "roomid",  _userroom );

    if ( mapUserObject[(WORD)_userport] ) {
        char szInfo[1024] = {0};
        int nLen = JsonTableInfo( _userroom, szInfo, sizeof(szInfo) );
        mapUserObject[_userport]->SendToClient( (BYTE*)szInfo, wSize);
    }
}


void LobbyUpdate::Onlines_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    if ( mapUserObject[(WORD)_userport] )
    {
        char szInfo[1024] = {0};
        int nLen = JsonOnlineInfo( szInfo, sizeof(szInfo) );
        mapUserObject[_userport]->SendToClient( (BYTE*)szInfo, wSize);
    }
}

void LobbyUpdate::WRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    if ( mapUserObject[(WORD)_userport] )
    {
        char szInfo[8192] = {0};
        int nLen = JsonWeekRank( szInfo, sizeof(szInfo) );
        mapUserObject[_userport]->SendToClient( (BYTE*)szInfo, wSize);
    }
}

void LobbyUpdate::DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    if ( mapUserObject[(WORD)_userport] )
    {
        char szInfo[8192] = {0};
        int nLen = JsonDayRank( szInfo, sizeof(szInfo) );
        mapUserObject[_userport]->SendToClient( (BYTE*)szInfo, wSize);
    }
}

void LobbyUpdate::RoomInfo_ANC  ( char * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _count;
    js_map.ReadInteger( "count", _count );
    byRoomGroupCount = (BYTE) _count;

    if (  MAX_SZ > wSize ) {
        byRoomGroupSize = wSize;
        memcpy( szRoomGroupInfo, pMsg, wSize );
        printf("[LobbyUpdate::RoomInfo_ANC] %d, %s \n", byRoomGroupCount, pMsg);
    }
}

void LobbyUpdate::TableInfo_ANC ( char * pMsg ,  WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _roomid;
    js_map.ReadInteger( "roomid", _roomid );

    if ( MAX_RM > _roomid ) {
        if (  MAX_SZ > wSize ) {
            iTableGroupInfoSize[_roomid] = wSize;
            memcpy( szTableGroupInfo[_roomid], pMsg, wSize );
            printf("[LobbyUpdate::TableInfo_ANC] %s \n", pMsg);
        }
    }
}

void LobbyUpdate::WRankInfo_ANC ( char * pMsg,  WORD wSize)
{
    if ( wSize < sizeof( szWeekRank ) ) {
        memcpy( szWeekRank, pMsg, wSize );
        printf("[LobbyUpdate::WRankInfo_ANC] %s \n", szWeekRank);
    }
}

void LobbyUpdate::DRankInfo_ANC ( char * pMsg, WORD wSize)
{
    if ( wSize < sizeof( szDayRank ) ) {
        memcpy( szDayRank, pMsg, wSize );
        printf("[LobbyUpdate::DRankInfo_ANC] %s \n", szDayRank);
    }
}

/***********************************************
    数据库查询
************************************************/
int LobbyUpdate::QueryRoomInfo()
{
    printf("LobbyUpdate::QueryTableInfo\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg), " { \"protocol\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, RoomInfo_SYN ) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen );
}

int LobbyUpdate::QueryTableInfo()
{
    printf("LobbyUpdate::QueryTableInfo\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg), " { \"protocol\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, TableInfo_SYN ) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen );
}

int LobbyUpdate::QueryOnlineInfo()
{
    // 不需要查了, 在线人生记录到大厅当中；
}

int LobbyUpdate::QueryWeekRank()
{
    printf("LobbyUpdate::QueryWeekRank\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg),
             " { \"protocol\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, WRankInfo_SYN) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen );
}

int LobbyUpdate::QueryDayRank()
{
    printf("LobbyUpdate::QueryDayRank\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg),
             " { \"protocol\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, DRankInfo_SYN) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen );
}

/***********************************************
    Json转化；
************************************************/
int LobbyUpdate::JsonRoomInfo( char * js_table, int js_size )
{
    if ( byRoomGroupSize < js_size ) {
        memcpy( js_table, szRoomGroupInfo, byRoomGroupSize );
    }
}

int LobbyUpdate::JsonTableInfo( int i, char * js_table, int js_size )
{
    if ( byRoomGroupSize > i) {
        if ( iTableGroupInfoSize[i] < js_size ) {
            memcpy( js_table, szTableGroupInfo[i], iTableGroupInfoSize[i] );
        }
    }
}

int LobbyUpdate::JsonUpdateTableInfo( int i, int k, char * js_table, int js_size )
{
    if ( byRoomGroupSize > i )
    {
        if ( byTableGroupSize[i] > k )
        {
            MSG_TABLEINFO_ANC send;
            char szMsg[256] = {0};
            memset( szMsg, 0x0, sizeof(szMsg) );
            snprintf( szMsg, sizeof(szMsg),
                     " {\"protocol\":\"%d\", \"data\":{\"roomid\":\" %d \", \"tableid\":\" %d \", \"update\":\" %d \"}} ",
                     send.m_dwProtocol, i, k, byTableOnlines[i][k]);

            int nLen = strlen( szMsg );
            if ( nLen < js_size ) {
                memcpy( js_table, szMsg, nLen );
                return nLen;
            }
        }
        return 0;
    }
    return 0;
}

int LobbyUpdate::JsonOnlineInfo( char * js_table, int js_size )
{
    MSG_ONLINEINFO_ANC send;
    char szMsg[256] = {0};
    memset( szMsg, 0x0,  sizeof(szMsg)  );
    snprintf( szMsg, sizeof(szMsg), " {\"protocol\":\"%d\", \"data\":{\"Onlines\":\" %d \"}} ",
             send.m_dwProtocol, iOnlinesInfo);
    int nLen = strlen( szMsg );

    if ( nLen < js_size ) {
        memcpy( js_table, szMsg, nLen );
        return nLen;
    }
    return 0;
}

int LobbyUpdate::JsonWeekRank( char * js_table, int js_size )
{
    if ( iWeekRankSize < js_size ) {
        memcpy( js_table, szWeekRank, iWeekRankSize );
    }
}

int LobbyUpdate::JsonDayRank( char * js_table, int js_size )
{
    if ( iWeekRankSize < js_size ) {
        memcpy( js_table, szDayRank, iWeekRankSize );
    }
}

int LobbyUpdate::Pinrt()
{
    //printf( "%s \n", szWeekRank );
    //printf( "%s \n", szDayRank );
}
