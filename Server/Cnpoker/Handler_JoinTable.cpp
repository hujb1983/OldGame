#include "Handler_Module.h"
#include "CnpokerServer.h"
#include "UserPacket.h"
#include "GameMgr.h"

void MSG_Handler_JoinTable_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize>=sizeof(UserPacket) )
    {
        UserPacket user;
        user.SetPacket( (BYTE*)pMsg, wSize );
        user.ToPrint();

        if ( user.GetTableId()==0 ){
            return;
        }

        GameTable * table = g_GameMgr.GetTable( user.GetTableId() );
        if (!table) {
            table = g_GameMgr.AllocTable( user.GetTableId() );
            if (table) {
                table->GetIndex() = user.GetTableId();
                {
                    TablePacket & pack = table->GetTablePacket();
                    pack.ToInit();
                    pack.GetProtocol() = MAKEDWORD( Games_Protocol, JoinTable_BRD );
                    pack.GetFieldId() = user.GetFieldId();
                    pack.GetRoomId() = user.GetRoomId();
                    pack.GetTableId() = user.GetTableId();
                    pack.GetMultiple() = 1;
                    strcat( pack.GetBasicPokers(), "-1" );
                    strcat( pack.GetDisplayPokers(0), "-1" );
                    strcat( pack.GetDisplayPokers(1), "-1" );
                    strcat( pack.GetDisplayPokers(2), "-1" );

                    BYTE _seatid = user.GetSeatId();
                    if (_seatid<3)  // 只有三位玩家
                    {
                        pack.GetUserKey(_seatid) = user.GetUserKey() ; // 用户入口值;
                        pack.GetPosition(_seatid) = 1 ; // 职位，地主和玩家;
                        pack.GetReady(_seatid) = 0 ; // 准备好了;
                        pack.GetRate(_seatid) = 10; // 胜率;
                        pack.GetMoney(_seatid) = user.GetMoney();  // 钱;
                        memcpy( pack.GetName(_seatid), user.GetName(), 33); // 名称

                        pack.GetOnline(_seatid) = PK_ONLINE;
                        pack.GetTrusteeship(_seatid) = 0;
                    }
                    pack.ToPrint();
                    g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
                }
            }
            return;
        }

        TablePacket & pack = table->GetTablePacket();
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, JoinTable_BRD );
        BYTE _seatid = user.GetSeatId();
        if (_seatid<3)  // 只有三位玩家
        {
            pack.GetUserKey(_seatid) = user.GetUserKey() ; // 用户入口值;
            pack.GetPosition(_seatid) = 0x01 ; // 职位，地主和玩家;
            pack.GetReady(_seatid) = 0x00 ; // 准备好了;
            pack.GetRate(_seatid) = 10; // 胜率;
            pack.GetMoney(_seatid) = user.GetMoney();  // 钱;
            memcpy( pack.GetName(_seatid), user.GetName(), 33); // 名称
        }

        pack.ToPrint();
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
        return;
    }
}
