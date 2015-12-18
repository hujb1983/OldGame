#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_JoinTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_JoinTable_REQ] pMsg = %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    UserSession * pSession = ( UserSession * ) pServerSession;
    int  _userkey   = pSession->GetUserKey();
    int  _userid    = pSession->GetUserid();
    int  _roomid    = 0;
    int  _tableid   = 0;
    js_map.ReadInteger( "userid",  _userid  );
    js_map.ReadInteger( "roomid",  _roomid  );
    js_map.ReadInteger( "tableid", _tableid );

    char buff[256] = {0};
    char format[256] = 	"{\"Protocol\":\"%d\","
                        "\"userid\":\"%d\","
                        "\"userkey\":\"%d\","
                        "\"roomid\":\"%d\","
						"\"tableid\":\"%d\" }";

    snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, JoinTable_REQ ),
        _userid, _userkey, _roomid, _tableid);
    int nLen = strlen(buff);

    g_AgentServer->SendToLobbyServer( (BYTE*) buff, nLen );
}

void MSG_Handler_JoinTable_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_JoinTable_BRD] pMsg = %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int  _userkey(0), _userid(0), _roomid(0), _agentkey(0),
    _tableid(0), _battleid(0), _status(0);

    js_map.ReadInteger( "userid",    _userid   );
    js_map.ReadInteger( "userkey",   _userkey  );
    js_map.ReadInteger( "agentkey",  _agentkey );
    js_map.ReadInteger( "roomid",    _roomid   );
    js_map.ReadInteger( "tableid",   _tableid  );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "status",    _status   );

    {
        char buff[256] = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                            "\"userid\":\"%d\","
                            "\"userkey\":\"%d\","
                            "\"agentkey\":\"%d\","
                            "\"battleid\":\"%d\","
                            "\"roomid\":\"%d\","
                            "\"tableid\":\"%d\","
                            "\"status\":\"%d\"}";

        snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, JoinGame_REQ ),
            _userid, _userkey, _agentkey, _battleid, _roomid, _tableid, _status);

        int nLen = strlen( buff );
        g_AgentServer->SendToGameServer( (BYTE*) buff, nLen );
    }
}

