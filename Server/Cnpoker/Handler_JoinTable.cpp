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
                    if (_seatid<3)  // ֻ����λ���
                    {
                        pack.GetUserKey(_seatid) = user.GetUserKey() ; // �û����ֵ;
                        pack.GetPosition(_seatid) = 1 ; // ְλ�����������;
                        pack.GetReady(_seatid) = 0 ; // ׼������;
                        pack.GetRate(_seatid) = 10; // ʤ��;
                        pack.GetMoney(_seatid) = user.GetMoney();  // Ǯ;
                        memcpy( pack.GetName(_seatid), user.GetName(), 33); // ����

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
        if (_seatid<3)  // ֻ����λ���
        {
            pack.GetUserKey(_seatid) = user.GetUserKey() ; // �û����ֵ;
            pack.GetPosition(_seatid) = 0x01 ; // ְλ�����������;
            pack.GetReady(_seatid) = 0x00 ; // ׼������;
            pack.GetRate(_seatid) = 10; // ʤ��;
            pack.GetMoney(_seatid) = user.GetMoney();  // Ǯ;
            memcpy( pack.GetName(_seatid), user.GetName(), 33); // ����
        }

        pack.ToPrint();
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
        return;
    }
}
