#include "LobbyUpdate.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include <JsonParser.h>

/***********************************************
    Json×ª»¯£»
************************************************/
int LobbyUpdate::JsonRoomInfo( char * js_table, int js_size )
{
    if ( wRoomGroupSize < js_size ) {
        memcpy( js_table, szRoomGroupInfo, wRoomGroupSize );
        return wRoomGroupSize;
    }
    return 0;
}

int LobbyUpdate::JsonTableInfo( int roomid, char * js_table, int js_size )
{
    if ( byRoomGroupCount > roomid ) {
        if ( iTableGroupInfoSize[roomid] < js_size ) {
            memcpy( js_table, szTableGroupInfo[roomid], iTableGroupInfoSize[roomid] );
            return iTableGroupInfoSize[roomid];
        }
    }
    return 0;
}

int LobbyUpdate::JsonUpdateTableInfo( int roomid, int k, char * js_table, int js_size )
{
    if ( byRoomGroupCount > roomid )
    {
        if ( byRoomGroupTableSize[roomid] > k )
        {
            int protocol = 0;
            char szMsg[256] = {0};
            memset( szMsg, 0x0, sizeof(szMsg) );
            snprintf( szMsg, sizeof(szMsg), " {\"protocol\":\"%d\",\"data\":{\"roomid\":%d,\"tableid\":%d,\"update\":%d}} ",
                  protocol, roomid, k, byTableOnlines[roomid][k]);

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
    char szMsg[256] = {0};
    memset( szMsg, 0x0,  sizeof(szMsg)  );
    snprintf( szMsg, sizeof(szMsg), " \"Onlines\":\"%d\" ",
             iOnlinesInfo);
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
        return iWeekRankSize;
    }
    return 0;
}

int LobbyUpdate::JsonDayRank( char * js_table, int js_size )
{
    if ( iDayRankSize < js_size ) {
        memcpy( js_table, szDayRank, iDayRankSize );
        return iDayRankSize;
    }
    return 0;
}

int LobbyUpdate::Print()
{
    //printf( "%s \n", szWeekRank );
    //printf( "%s \n", szDayRank );
}
