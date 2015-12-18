#include "Handler_Module.h"
#define DB_ROOMS_SIZE   (6)

void MSG_Handler_TableInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.TableInfo_REQ( pServerSession, pMsg, wSize);
}

void MSG_Handler_TableInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.TableInfo_ANC ( (char*) pMsg, wSize );
}
