#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_Uplines_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    //g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_Upline_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    //g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}
