#include "Handler_Module.h"
#include "CnpokerServer.h"

/* 托管设置 */
void MSG_Handler_Trusteeship_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize < sizeof(UserPacket) ) {
        return ;
    }

    BYTE _called(0);
    UserPacket user;
    user.SetPacket( (BYTE*)pMsg, wSize );
    user.ToPrint();

    GameTable * table = g_GameMgr.GetTable( user.GetTableId() );
    if (!table) {
        return;
    }

    TablePacket & pack = table->GetTablePacket();

    BYTE _seatid = user.GetSeatId();
    if (_seatid>2) {
        return;
    }

    BYTE trusteeship = pack.GetTrusteeship(_seatid);
    pack.GetTrusteeship(_seatid) = (!trusteeship); // 反向处理

    pack.GetProtocol() = MAKEDWORD( Games_Protocol, Trusteeship_BRD );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
}
