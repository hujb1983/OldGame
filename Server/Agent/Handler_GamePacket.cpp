#include "Handler_Module.h"
#include "UserSession.h"

void MSG_Handler_GamePacket_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize);
}


int User_Login_Query_Room_Info( WORD _userkey )
{
    DEBUG_MSG( LVL_TRACE,  "User_Login_Query_Room_Info \n" );

    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, RoomInfo_SYN ), _userkey );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

void MSG_Handler_GamePacket_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "GamePacket_ANC to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey(0), _agentkey(0), _userid(0);
    int _points(0), _wons(0), _faileds(0), _aways(0);
    char _name[33] = {0};
    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "agentkey", _agentkey );
    js_map.ReadInteger( "userid",   _userid   );
    js_map.ReadInteger( "points",   _points   );
    js_map.ReadInteger( "wons",     _wons     );
    js_map.ReadInteger( "faileds",  _faileds  );
    js_map.ReadInteger( "aways",    _aways    );
    js_map.ReadString ( "name",     _name,  sizeof(_name) );

    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession(_userkey );
    if ( pSession ) {
        pSession->setName( _name, sizeof( _name ) );
        pSession->setGamesinfo(_points, _wons, _faileds, _aways);
    }

    if ( _userkey!=0 ) {
        char _szMsg[1024] = {0};
        char _format[256] = "{\"protocol\":\"%d\",\"userid\":%d,"
                "\"data\":[{\"points\":%d,\"wons\":%d,\"faileds\":%d,\"aways\":%d,\"name\":\"%s\"}]}";
        snprintf( _szMsg, sizeof(_szMsg), _format, MAKEDWORD( Login_Protocol, GamePacket_ANC ),
                _userid, _points, _wons, _faileds, _aways, _name );

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( _szMsg );
        g_AgentServer->SendToClient( _userkey, (BYTE*) _szMsg, nLen );

        DEBUG_MSG( LVL_DEBUG, "GamePacket_ANC to client: %s \n", _szMsg );

        User_Login_Query_Room_Info( _userkey );
    }
}


