#include "Handler_Module.h"
#include "GamePoker.h"
#include "CnpokerServer.h"


/*******************************************************************
    Step 1: MSG_Handler_DiscardsLicense_REQ (授权通知)
********************************************************************/
void MSG_Handler_DiscardsLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize < sizeof(UserPacket) ) {
        return ;
    }

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

    if ( pack.GetPlaySeatId()!=_seatid ) {
        return;
    }


    BYTE pokerType = 0;
    if ( pokerType==PH_MAX ) {   // 天炸, 本人续继
        pack.GetPlaySeatId() = _seatid;
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
        return;
    }


    {
        pack.GetPlaySeatId() = (++_seatid)%TEAM_SIZE;
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
    }

}

