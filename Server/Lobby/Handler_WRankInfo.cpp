#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_WRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.WRankInfo_REQ( pServerSession, pMsg, wSize );
}

void MSG_Handler_WRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.WRankInfo_ANC( (char*)pMsg, wSize );
}
