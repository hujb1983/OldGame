#include "LobbyUpdate.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include <JsonParser.h>

/***********************************************
    ���ݿ��ѯ
************************************************/
int LobbyUpdate::QueryTextInfo()
{
    /* printf("LobbyUpdate::QueryTextInfo\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg),
             " { \"userid\":\"555\", \"protocol\":\"%d\" } ",
            MAKEDWORD( Login_Protocol, Login_REQ ) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen ); */
}


int LobbyUpdate::QueryRoomInfo()
{
    printf("LobbyUpdate::QueryRoomInfo\n");
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
    // ����Ҫ����, ����������¼���������У�
}

int LobbyUpdate::QueryWeekRank()
{
    printf("LobbyUpdate::QueryWeekRank\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg), " { \"protocol\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, WRankInfo_SYN) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen );
}

int LobbyUpdate::QueryDayRank()
{
    printf("LobbyUpdate::QueryDayRank\n");
    char pMsg[1024] = {0};
    snprintf( pMsg, sizeof(pMsg), " { \"protocol\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, DRankInfo_SYN) );
    int nLen = strlen(pMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, nLen );
}

