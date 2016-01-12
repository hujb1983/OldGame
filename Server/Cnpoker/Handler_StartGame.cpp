#include "Handler_Module.h"
#include "CnpokerServer.h"

/*****************************************************
    MSG_Handler_StartGame_REQ
*****************************************************/
int Init_Poker_SetUsercards( BYTE * _poker, BYTE _size )
{
    BYTE newPoker[POKER_SIZE + 1];
    g_GameMgr.UpdateShuffle( newPoker, _size );

    int i(0), sIndex(0);
    BYTE index = 0;
    int play_size = (_size - 3) / 3;
    sIndex = i + play_size;
    for (; i<(sIndex); i++) {
        index = newPoker[i];
        if ( index<MAX_POKER ) {
            _poker[index] = PK_USER_0;
        }
    }
    sIndex = i + play_size;
    for (; i<(sIndex); i++) {
        index = newPoker[i];
        if ( index<MAX_POKER ) {
            _poker[index] = PK_USER_1;
        }
    }
    sIndex = i + play_size;
    for (; i<(sIndex); i++) {
        index = newPoker[i];
        if ( index<MAX_POKER ) {
            _poker[index] = PK_USER_2;
        }
    }
    for (; i < _size; i++) {
        index = newPoker[i];
        if ( index<MAX_POKER ) {
            _poker[index] = PK_BANKER;
        }
    }
}


/*****************************************************
    InitCards_Get_Usercards
*****************************************************/
BYTE InitCards_Get_Usercards( TablePacket & pack, BYTE byVal, BYTE seatId ) {
    pack.GetDiscardPokerSize(seatId) = 17;
    char * poker = pack.GetDiscardPokers(seatId);
    BYTE * pMove = pack.GetPokers();
    char szPoker[8] = {0};
    char szPokerList[128] = {0};
    BYTE byPoker(0), byCount(0);
    for (int  i=0; i<POKER_SIZE; i++) {
        byPoker = pMove[i];
        if ( byPoker == byVal ) {
            memset( szPoker, 0x0, sizeof(szPoker) );
            if ( byCount!=0 ) {
                strcat( szPokerList, ",");
            }
            sprintf( szPoker, "%d", i );
            strcat( szPokerList, szPoker);
            byCount++;
        }
    }
    DEBUG_MSG( LVL_DEBUG, "byCount = %d ",  byCount);
    *poker = '\0';
    strcat( poker, szPokerList);
}

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
                user.GetReady() = PK_READY;            // 用户参数包,准备标识
                pack.GetReady( _seatid ) = PK_READY;   // 桌子参数包,准备标识
            }

            if ( pack.GetReady(0)==PK_READY &&
                 pack.GetReady(1)==PK_READY &&
                 pack.GetReady(2)==PK_READY ) {

                {
                    pack.GetProtocol() = MAKEDWORD( Games_Protocol, InitCards_BRD );
                    pack.GetPokerSize(0) = USER_PLAYER;
                    pack.GetPokerSize(1) = USER_PLAYER;
                    pack.GetPokerSize(2) = USER_PLAYER;
                    BYTE * pokers = pack.GetPokers();
                    Init_Poker_SetUsercards( pokers, POKER_SIZE );
                    pack.GetInitcards()  = PK_INITCARDS;
                    InitCards_Get_Usercards(pack,PK_USER_0,0);
                    InitCards_Get_Usercards(pack,PK_USER_1,1);
                    InitCards_Get_Usercards(pack,PK_USER_2,2);
                    g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
                }

                {
                    pack.GetProtocol() = MAKEDWORD( Games_Protocol, CalledLicense_BRD );
                    pack.GetPlaySeatId() = pack.GetBankerId();
                    pack.GetBankerId() = 0;
                    pack.GetLimitedTimes() = 16;
                    g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
                }
                return;
            }

            pack.ToPrint();
            g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
        }
    }
}
