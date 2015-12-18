#include "Handler_Module.h"
#include "CnpokerServer.h"


/* 结算数据返回 */
void MSG_Handler_Settlement_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    g_pCnpokerServer->SendToAgentServer( (BYTE*)pMsg, wSize );
}
