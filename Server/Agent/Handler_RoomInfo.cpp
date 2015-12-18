#include "Handler_Module.h"

void MSG_Handler_RoomInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize);
}


int User_Login_Query_Table_Info( WORD _userkey, BYTE _roomid)
{
    printf("User_Login_Query_Table_Info\n");
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg),
            " { \"protocol\":\"%d\", \"userkey\":\"%d\", \"roomid\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, TableInfo_SYN ), _userkey, _roomid );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}


void MSG_Handler_RoomInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf("[AgentServer::MSG_Handler_RoomInfo_ANC] %s \n", (char*)pMsg );
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey = 0;
    int _count = 0;
    js_map.ReadInteger( "userkey", _userkey );
    js_map.ReadInteger( "count",   _count );

    if ( _count >= 0 ) {
        JsonMap js_rooms, js_array;
        int _roomid = 0;

        js_map.ReadJson( "rooms",       js_rooms );
        if ( js_rooms.GetArraySize() > 0 ) {
            js_rooms.ReadArrayJson(0, js_array);
            js_array.ReadInteger("roomid",  _roomid);

            if ( _userkey!=0 ) {
                g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
                User_Login_Query_Table_Info( _userkey, _roomid);
            }
        }
    }
}
