#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_JoinTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    AgentSession * pSession = (AgentSession*) pServerSession;
    WORD _agentkey  = pSession->GetIndexKey();
    int  _userkey   = 0;
    int  _userdb    = 0;
    int  _roomid    = 0;
    int  _tableid   = 0;

    js_map.ReadInteger( "userid",   _userdb  );
    js_map.ReadInteger( "userkey",  _userkey );
    js_map.ReadInteger( "roomid",   _roomid  );
    js_map.ReadInteger( "tableid",  _tableid );

    char buff[256] = {0};
    char format[256] = 	"{\"Protocol\":\"%d\","
                        "\"userid\":\"%d\","
                        "\"userkey\":\"%d\","
                        "\"roomid\":\"%d\","
                        "\"tableid\":\"%d\","
                        "\"agentkey\":\"%d\" }";

    snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, JoinTable_REQ ),
         _userdb, _userkey, _roomid, _tableid, _agentkey);
    int nLen = strlen(buff);

    g_LobbyServer->SendToDBServer( (BYTE*) buff, nLen );
}

void MSG_Handler_JoinTable_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_JoinTable_BRD = %s]\n", (char*)pMsg );
    g_LobbyUpdate.JoinTable( pServerSession, pMsg, wSize );
}

