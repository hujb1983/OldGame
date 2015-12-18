#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_QuitTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_QuitTable_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
    g_AgentServer->SendToGameServer( (BYTE*) pMsg, wSize );
}

