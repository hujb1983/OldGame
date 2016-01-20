#include "Handler_Module.h"
#include "AgentServer.h"
#include "UserSession.h"

void MSG_Handler_FastGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    UserSession * pSession = ( UserSession * ) pServerSession;
    {
        UserPacket pack = pSession->GetUserPacket();
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, JoinTable_REQ );
        pack.GetFieldId() = 1;
        pack.GetRoomId() = 2;
        pack.ToPrint();
        g_AgentServer->SendToLobbyServer( (BYTE*)&pack, pack.GetPacketSize() );
    }
}

void MSG_Handler_FastGame_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{

}




