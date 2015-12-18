#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/* 接收来自Lobby的查询 */
void MSG_Handler_Relogin_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _battleid, _status, _userid, _userkey;
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "status",    _status   );
    js_map.ReadInteger( "userid",    _userid  );
    js_map.ReadInteger( "userkey",   _userkey  );

    if ( _status==1 )
   {
        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                             "\"battleid\":\"%d\","
                             "\"status\":\"%d\","
                             "\"userid\":\"%d\","
                             "\"userkey\":\"%d\" }";   // 暂时设定为18秒;

        snprintf( buff, sizeof(buff), format,
                MAKEDWORD( Login_Protocol, Relogin_REQ ),
                _battleid, _status, _userid, _userkey);

        WORD nLen = strlen( buff );
        g_AgentServer->SendToGameServer( (BYTE*)buff, nLen );
    }
}

void MSG_Handler_Relogin_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1  = 0;
    int _userkey2  = 0;
    int _userkey3  = 0;

    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );

    if (_userkey1 != 0) {
        g_AgentServer->SendToClient( _userkey1, (BYTE*)pMsg, wSize );
    }

    if (_userkey2 != 0) {
        g_AgentServer->SendToClient( _userkey2, (BYTE*)pMsg, wSize );
    }

    if (_userkey3 != 0) {
        g_AgentServer->SendToClient( _userkey3, (BYTE*)pMsg, wSize );
    }
}
