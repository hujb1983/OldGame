#include "Handler_Module.h"
#include "UserSession.h"

void MSG_Handler_GamePacket_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize);
}


int User_Login_Query_Room_Info( WORD _userkey )
{
    printf( "User_Login_Query_Room_Info \n" );
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, RoomInfo_SYN ), _userkey );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

void MSG_Handler_GamePacket_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf("[AgentServer::MSG_Handler_GamePacket_ANC] %s \n", (char*)pMsg );
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey = 0;
    js_map.ReadInteger( "userkey", _userkey );

    JsonMap js_data;
    js_map.ReadJson( "data", js_data );

    int iSize = js_data.GetArraySize();
    if ( iSize == 1)
    {
        int _points(0), _wons(0), _faileds(0), _aways(0);
        char _name[33] = {0};
        JsonMap js_array;
        js_data.ReadArrayJson( 0, js_array );
        js_array.ReadInteger( "points",  _points );
        js_array.ReadInteger( "wons",    _wons   );
        js_array.ReadInteger( "faileds", _faileds);
        js_array.ReadInteger( "aways",   _aways  );
        js_array.ReadString ( "name",    _name, sizeof(_name) );

        UserSession * pSession = NULL;
        pSession = g_AgentServer->GetUserSession(_userkey );
        if ( pSession ) {
            pSession->setName( _name, sizeof( _name ) );
            pSession->setGamesinfo(_points, _wons, _faileds, _aways);

            /* char _buff[256]   = {0};
            pSession->getSeatInfo( _buff, sizeof( _buff ) );
            printf( "[MSG_Handler_GamePacket_ANC %s] \n", _buff ); */
        }
    }

    if ( _userkey!=0 ) {
        g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
        User_Login_Query_Room_Info( _userkey );
    }
}


