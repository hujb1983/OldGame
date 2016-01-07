#include "Handler_Module.h"
#include "CnpokerServer.h"


/*  */
void MSG_Handler_Settlement_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    g_pCnpokerServer->SendToAgentServer( (BYTE*)pMsg, wSize );
}
