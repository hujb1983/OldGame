#include "Handler_Module.h"
#define DB_ROOMS_SIZE   (6)

void MSG_Handler_RoomInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.RoomInfo_REQ( pServerSession, pMsg, wSize);
}

void MSG_Handler_RoomInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.RoomInfo_ANC ( (char*) pMsg, wSize );
}
