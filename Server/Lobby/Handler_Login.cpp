#include "Handler_Module.h"
#include <dbCommon.h>

void MSG_Handler_Login_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize==sizeof(UserPacket) ) {
        UserPacket pack;
        AgentSession * pSession = (AgentSession*) pServerSession;
        pack.SetPacket( (BYTE*) pMsg, wSize );
        pack.GetAgentKey() = pSession->GetIndexKey();
        g_LobbyServer->SendToDBServer( (BYTE*)&pack, sizeof(pack) );
    }
}

void MSG_Handler_Login_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize>=sizeof(UserPacket) ) {
        UserPacket pack;
        pack.SetPacket( (BYTE*) pMsg, wSize );
        g_LobbyUpdate.SendToAgentSession( (WORD)pack.GetAgentKey(), (BYTE*)&pack, sizeof(pack) );
    }
}

