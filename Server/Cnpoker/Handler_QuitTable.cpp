#include "Handler_Module.h"
#include "CnpokerServer.h"

void MSG_Handler_QuitTable_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
   if ( wSize < sizeof(UserPacket) ) {
        return ;
    }

    BYTE _called(0);
    UserPacket user;
    user.SetPacket( (BYTE*)pMsg, wSize );

    GameTable * table = g_GameMgr.GetTable( user.GetTableId() );
    if (!table) {
        return;
    }

    TablePacket & pack = table->GetTablePacket();

    BYTE _seatid = user.GetSeatId();
    if (_seatid>2) {
        return;
    }

    if ( pack.GetTimeStep()==PK_DEALING ||
         pack.GetTimeStep()==PK_CALLING ||
         pack.GetTimeStep()==PK_PLAYING ) {
        pack.GetOnline(_seatid)=false;
    }
    else {
        pack.GetPosition(_seatid) = 0;
    }

    pack.GetProtocol() = MAKEDWORD( Games_Protocol, QuitTable_BRD );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
    return;
}

