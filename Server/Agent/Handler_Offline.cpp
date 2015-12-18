#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_Offline_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
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
