#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToGameServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_StartGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1  = 0;
    int _userkey2  = 0;
    int _userkey3  = 0;
    int _ready  = 0;

    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "ready",     _ready );

    if (_userkey1 != 0) {
        g_AgentServer->SendToClient( _userkey1, (BYTE*)pMsg, wSize );
    }

    if (_userkey2 != 0) {
        g_AgentServer->SendToClient( _userkey2, (BYTE*)pMsg, wSize );
    }

    if (_userkey3 != 0) {
        g_AgentServer->SendToClient( _userkey3, (BYTE*)pMsg, wSize );
    }

    if ( _ready == 1) {

        JsonMap js_data;
        js_map.ReadJson( "data", js_data );

        int iSize = js_data.GetArraySize();
        if ( iSize == 1)
        {
            int _battleid = 0;
            JsonMap js_array;
            js_data.ReadArrayJson( 0, js_array );
            js_array.ReadInteger( "battleid", _battleid );

            char szMsg[1024] = {0};

            // @{{{ 组合所有的牌
            char format[256] = 	"{\"protocol\":\"%d\","
                                 "\"battleid\":\"%d\" }" ;

            sprintf( szMsg, format, MAKEDWORD( Games_Protocol, InitCards_REQ ), _battleid );
            // }}}@ 组合所有的牌

            // @{{{ 发送到其他玩家；
            WORD nLen = strlen( szMsg );
            g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );
        }
    }
}

