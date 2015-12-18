#include "Handler_Module.h"
#include <dbCommon.h>

void MSG_Handler_GamePacket_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    AgentSession * pSession = (AgentSession*) pServerSession;
    WORD _agentkey  = pSession->GetIndexKey();
    int  _userkey  = 0;
    int  _userid   = 0;
    js_map.ReadInteger( "userid",  _userid );
    js_map.ReadInteger( "userkey", _userkey );

    char buff[256] = {0};
    char format[256] = 	"{\"Protocol\":\"%d\","
                        "\"userid\":\"%d\","
                        "\"userkey\":\"%d\","
                        "\"agentkey\":\"%d\" }";

	snprintf( buff, sizeof(buff), format, MAKEDWORD( Login_Protocol, GamePacket_SYN ),
         _userid, _userkey, _agentkey);

    printf("[LobbyServer MSG_Handler_GamePacket_REQ] %s.\n", buff);
    g_LobbyServer->SendToDBServer( (BYTE*) buff, strlen(buff) );

    // º”»Î¥ÛÃ¸
    g_LobbyUpdate.JoinLobby( pServerSession, pMsg, wSize );
}

void MSG_Handler_GamePacket_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _agentkey  = 0;
    js_map.ReadInteger( "agentkey", _agentkey );
    g_LobbyUpdate.SendToAgentSession( (WORD)_agentkey, (BYTE*) pMsg, wSize );
}

