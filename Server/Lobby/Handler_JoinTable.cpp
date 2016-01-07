#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_JoinTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    g_LobbyServer->SendToDBServer( (BYTE*) pMsg, wSize );
}

void MSG_Handler_JoinTable_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {

    if ( wSize>=sizeof(UserPacket) ) {
        UserPacket pack;
        pack.SetPacket( (BYTE*) pMsg, wSize );
        g_LobbyUpdate.SendToAgentSession( (WORD)pack.GetAgentKey(), (BYTE*)pMsg, wSize );
    }
}

