#include "Handler_Module.h"
#include "CnpokerServer.h"

/*****************************************************
    MSG_Handler_Discards_REQ
*****************************************************/
void MSG_Handler_Settlement_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize < sizeof(UserPacket) ) {
        return ;
    }

    BYTE _called(0);
    UserPacket user;
    user.SetPacket( (BYTE*)pMsg, wSize );
    int    _iCount = user.GetShowedSize();
    char * _szPokers = user.GetShowedPokers();

    GameTable * table = g_GameMgr.GetTable( user.GetTableId() );
    if (!table) {
        return;
    }

    TablePacket & pack = table->GetTablePacket();

    BYTE _seatid = user.GetSeatId();
    if (_seatid>2) {
        return;
    }

    {   // 核算算法 : 1.sumFee扣钱者用; 2. 得钱者用

        int sumFee = pack.GetBasicMoney() * pack.GetMultiple();
        int deduct = sumFee * pack.GetBrokerage();

        int left  = (_seatid+2)%TEAM_SIZE;
        int rigth = (_seatid+1)%TEAM_SIZE;

        if (pack.GetBankerId()==_seatid) {
            pack.GetGain(_seatid) = deduct;
            pack.GetGain(left) = -(sumFee/2);
            pack.GetGain(rigth) = -(sumFee/2);
        }
        else {
            pack.GetGain(_seatid) = (deduct/2);
            if (pack.GetBankerId()==left) {
                pack.GetGain(left) = -sumFee;
            }
            else{
                pack.GetGain(_seatid) = (deduct/2);
            }
            if (pack.GetBankerId()== rigth) {
                pack.GetGain(rigth) = -sumFee;
            }
            else{
                pack.GetGain(_seatid) = (deduct/2);
            }
        }

        pack.GetProtocol() = MAKEDWORD( Games_Protocol, Settlement_BRD );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
    }

    if ( pack.GetMoney(0) ) {
        pack.GetPosition(0) = 0;
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, QuitTable_BRD );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
    }

}
