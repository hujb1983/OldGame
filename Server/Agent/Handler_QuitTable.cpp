#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_QuitTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "QuitTable_REQ to recv: %s \n", (char*) pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _seatid(0), _battleid(0);
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid );

    {
        char _buff[256]   = {0};
        char _format[128] = "{\"protocol\":\"%d\","
                            "\"battleid\":%d,"
                            "\"seatid\":%d}";

        snprintf( _buff, sizeof(_buff), _format,
                  MAKEDWORD( Games_Protocol, QuitGame_REQ ),
                  _battleid,
                  _seatid);

        WORD nLen = strlen( _buff );
        g_AgentServer->SendToGameServer( (BYTE*) _buff, nLen );

        DEBUG_MSG( LVL_DEBUG, "QuitTable_REQ to send: %s \n", (char*)_buff );
    }
}

void MSG_Handler_QuitTable_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    //g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}

