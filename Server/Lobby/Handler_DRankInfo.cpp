#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.DRankInfo_REQ( pServerSession, pMsg, wSize );
}

void MSG_Handler_DRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.DRankInfo_ANC( (char*)pMsg, wSize );
}

