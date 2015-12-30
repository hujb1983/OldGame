#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_Onlines_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}


/*******************************************************
        从大厅获取在线人数   ( Onlines_ANC )
******************************************************/
void MSG_Handler_Onlines_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
}
