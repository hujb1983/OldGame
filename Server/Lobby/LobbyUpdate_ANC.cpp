#include "LobbyUpdate.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include <JsonParser.h>

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
        wRoomGroupSize = wSize;
        memcpy( szRoomGroupInfo, pMsg, wSize );
        DEBUG_MSG( LVL_DEBUG, "TableInfo_ANC: %d, %s \n", byRoomGroupCount, pMsg);
    }
}

void LobbyUpdate::TableInfo_ANC ( char * pMsg ,  WORD wSize)
{
    JsonMap js_map;
    JsonMap js_array;
    JsonMap js_table;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _roomid, _count;
    js_map.ReadInteger( "count", _count );
    byTableGroupSunSize = _count;

    js_map.ReadJson("tables", js_array);
    int js_size = js_array.GetArraySize();
    for ( int i=0; i<js_size; i++) {
        js_array.ReadArrayJson( i, js_table );
        js_table.ReadInteger( "roomid", _roomid );
        if ( MAX_RM > _roomid ) {
            byRoomGroupTableSize[_roomid] = 1;
            if (  MAX_SZ > wSize ) {
                iTableGroupInfoSize[_roomid] = wSize;
                memcpy( szTableGroupInfo[_roomid], pMsg, wSize );
            }
        }
    }
    DEBUG_MSG( LVL_DEBUG, "TableInfo_ANC: %s \n", pMsg);
}

void LobbyUpdate::WRankInfo_ANC ( char * pMsg,  WORD wSize)
{
    if ( wSize < sizeof( szWeekRank ) ) {
        iWeekRankSize = wSize;
        memcpy( szWeekRank, pMsg, wSize );
        DEBUG_MSG( LVL_DEBUG, "WRankInfo_ANC: %s \n", pMsg);
    }
}

void LobbyUpdate::DRankInfo_ANC ( char * pMsg, WORD wSize)
{
    if ( wSize < sizeof( szDayRank ) ) {
        iDayRankSize = wSize;
        memcpy( szDayRank, pMsg, wSize );
        DEBUG_MSG( LVL_DEBUG, "DRankInfo_ANC: %s \n", pMsg);
    }
}

