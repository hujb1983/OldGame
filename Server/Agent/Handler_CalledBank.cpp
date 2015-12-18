#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToGameServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_CalledBank_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1  = 0;
    int _userkey2  = 0;
    int _userkey3  = 0;
    int _create    = 0;
    int _initcard  = 0;

    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "initcard",  _initcard );
    js_map.ReadInteger( "create",    _create   );

    // 没有人选择地主, 重新分配牌;
    if (_initcard == 1) {

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
        return ;
    }

    if (_userkey1 != 0) {
        g_AgentServer->SendToClient( _userkey1, (BYTE*)pMsg, wSize );
    }

    if (_userkey2 != 0) {
        g_AgentServer->SendToClient( _userkey2, (BYTE*)pMsg, wSize );
    }

    if (_userkey3 != 0) {
        g_AgentServer->SendToClient( _userkey3, (BYTE*)pMsg, wSize );
    }

    // 构建地主结果给他们;
    if ( _create == 1 ) {

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

            sprintf( szMsg, format, MAKEDWORD( Games_Protocol, CreateBank_REQ ), _battleid );
            // }}}@ 组合所有的牌

            // @{{{ 发送到其他玩家；
            WORD nLen = strlen( szMsg );
            g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );
        }
    }
    else {

        // 如果不到创建的时候，授权下一位叫牌者
        JsonMap js_data;
        js_map.ReadJson( "data", js_data );

        int iSize = js_data.GetArraySize();
        if ( iSize == 1)
        {
            int _battleid, _seatid;
            JsonMap js_array;
            js_data.ReadArrayJson( 0, js_array );
            js_array.ReadInteger( "battleid", _battleid );
            js_array.ReadInteger( "seatid",   _seatid );

            // @{{{ 组合所有的牌
            char szMsg[1024] = {0};
            char format[256] = 	"{\"protocol\":\"%d\", \"seatid\":\"%d\", \"battleid\":\"%d\" }" ;
            sprintf( szMsg, format, MAKEDWORD( Games_Protocol, CalledLicense_REQ ), _seatid, _battleid );
            // }}}@ 组合所有的牌

            // @{{{ 发送到其他玩家；
            WORD nLen = strlen( szMsg );
            g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );
        }
    }
}

