#include "Handler_Module.h"
#include "CnpokerServer.h"

/*****************************************************
    MSG_Handler_StartGame_REQ
*****************************************************/
void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize>=sizeof(UserPacket) )
    {
        UserPacket user;
        user.SetPacket( (BYTE*)pMsg, wSize );
        user.ToPrint();

        GameTable * table = g_GameMgr.GetTable( user.GetTableId() );
        if (!table) {
            return;
        }

        {
            TablePacket & pack = table->GetTablePacket();
            pack.GetProtocol() = MAKEDWORD( Games_Protocol, StartGame_BRD );

            BYTE _seatid = user.GetSeatId();
            if (_seatid<3) {
                pack.GetReady( _seatid ) = 1;
            }
            pack.ToPrint();
            g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
        }
    }
}
