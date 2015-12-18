#include "Handler_Module.h"
#include <dbCommon.h>

void MSG_Handler_Logout_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_Logout_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
}
