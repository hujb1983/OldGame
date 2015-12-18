#include "Handler_Module.h"
#include "LobbyUpdate.h"

/* 重新上线通知 */
void MSG_Handler_Relogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // printf( "[MSG_Handler_Relogin_REQ] pMsg = %s \n",  (char*)pMsg );
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, wSize );
}


void MSG_Handler_Relogin_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _agentkey  = 0;
    js_map.ReadInteger( "agentkey", _agentkey );
    g_LobbyUpdate.SendToAgentSession( (WORD)_agentkey, (BYTE*) pMsg, wSize );
}
