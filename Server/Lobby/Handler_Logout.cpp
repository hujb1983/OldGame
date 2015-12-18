#include "Handler_Module.h"
#include <dbCommon.h>

void MSG_Handler_Logout_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.QuitLobby( pServerSession, pMsg, wSize );
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_Logout_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _agentkey  = 0;
    js_map.ReadInteger( "agentkey", _agentkey );
    g_LobbyUpdate.SendToAgentSession( (WORD)_agentkey, (BYTE*) pMsg, wSize );
}
