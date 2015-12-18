#include "Handler_Module.h"
#include <dbCommon.h>

void MSG_Handler_Login_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    AgentSession * pSession = (AgentSession*) pServerSession;
    WORD _agentkey = pSession->GetIndexKey();
    int  _userkey  = 0;
    int  _userid   = 0;
    char _szSSHKEY[128] = {0};
    js_map.ReadInteger( "userid",  _userid );
    js_map.ReadInteger( "userkey", _userkey );
    js_map.ReadString ( "sshkey",  _szSSHKEY, sizeof(_szSSHKEY) );

    char buff[256] = {0};
    char format[256] = 	"{\"Protocol\":\"%d\","
                        "\"userid\":%d,"
                        "\"userkey\":%d,"
                        "\"agentkey\":%d,"
						"\"sshkey\":\"%s\"}";

	snprintf( buff, sizeof(buff), format, MAKEDWORD( Login_Protocol, Login_REQ ),
         _userid, _userkey, _agentkey, _szSSHKEY);

    g_LobbyServer->SendToDBServer( (BYTE*) buff, strlen(buff) );
}

void MSG_Handler_Login_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _agentkey  = 0;
    js_map.ReadInteger( "agentkey", _agentkey );
    g_LobbyUpdate.SendToAgentSession( (WORD)_agentkey, (BYTE*) pMsg, wSize );
}

