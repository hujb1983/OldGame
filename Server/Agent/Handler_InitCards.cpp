#include "Handler_Module.h"
#include "AgentServer.h"

void MSG_Handler_InitCards_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[AgentServer] MSG_Handler_InitCards_BRD! \n");
    g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
}
