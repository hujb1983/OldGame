#include "Handler_Module.h"
#include "CnpokerServer.h"

/*****************************************************
    Re_Init_Poker_SetUsercards
*****************************************************/
int Re_Init_Poker_SetUsercards( BYTE * _poker, BYTE _size )
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
    InitCards_Get_Basecards
*****************************************************/
void Banker_Alloc_BasicCards( TablePacket & pack, BYTE byUserType ) {

    BYTE * pMove = pack.GetPokers();
    BYTE bankerId = pack.GetBankerId();
    char * b_poker = pack.GetBasicPokers();
    char * poker = pack.GetDisplayPokers(bankerId);
    pack.GetBasicPokerSize() = 3;

    char szPoker[8] = {0};
    char szPokerList[128] = {0};
    BYTE byPoker(0), byCount(0);
    for (int  i=0; i<POKER_SIZE; i++) {
        byPoker = pMove[i];
        if ( byPoker == PK_BANKER ) {
            pMove[i] = byUserType;
            memset( szPoker, 0x0, sizeof(szPoker) );
            if ( byCount!=0 ) {
                strcat( szPokerList, ",");
            }
            sprintf( szPoker, "%d", i );
            strcat( szPokerList, szPoker);
            byCount++;
        }
    }

    pack.GetDisplayPokerSize(bankerId) = 20;
    *b_poker = '\0';
    strcat( b_poker, szPokerList);
    strcat( poker, ",");
    strcat( poker, szPokerList);
}


/*****************************************************
    MSG_Handler_CalledBank_REQ
*****************************************************/
void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
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

    if ( pack.GetPlaySeatId()!=_seatid ) {
        return;
    }

    BYTE byUserSign = PK_USER_0 + _seatid;

    BYTE _count(0);
    if ( pack.GetCalledStatus(0) == PK_CALLED ) { ++_count;}
    if ( pack.GetCalledStatus(1) == PK_CALLED ) { ++_count;}
    if ( pack.GetCalledStatus(2) == PK_CALLED ) { ++_count;}

    _called = user.GetCalled();
    if ( pack.GetModel()==0 ) {     // 普通叫牌
        if ( _called==0 ) {
            pack.GetCalledType(_seatid) = eGB_Waiver;
        }
        else {
            do {
                     if ( _called==1 )   {   pack.GetCalledType(_seatid) = eGB_Point1; break;  }
                else if ( _called==2 )   {   pack.GetCalledType(_seatid) = eGB_Point2; break;  }
                else if ( _called==3 )   {   pack.GetCalledType(_seatid) = eGB_Point3; break;  }
                return;
            } while(1);
            if ( _called > pack.GetMultiple() ) {
                pack.GetMultiple() = _called;
            }
            pack.GetBankerId() = _seatid;
            if ( pack.GetCalledType(_seatid) == eGB_Point3 ) {
                Banker_Alloc_BasicCards(pack, byUserSign);
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, CreateBank_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );

                pack.GetFirst() = true;
                pack.GetPlaySeatId() = pack.GetBankerId();
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
                return;
            }
        }
    }
    else {  // 翻倍叫牌
        if ( _called==0 ) {
            pack.GetCalledType(_seatid) = eGB_Waiver;
        }
        else {
            pack.GetBankerId() = _seatid;
            pack.GetCalledType(_seatid) = eGB_Apply;
            pack.GetMultiple() *= 2;
        }
    }

    // 判断叫牌
    BYTE l_seatid = (_seatid+2)%3;
    BYTE r_seatid = (_seatid+1)%3;
    if (_count==0 || _count==1) {
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, Called_BRD );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
    }
    else {
        BYTE _hasCalled(0); // 计算叫地主次数
        if ( pack.GetCalledType(0)!=eGB_Waiver )   {   ++_hasCalled;  }
        if ( pack.GetCalledType(1)!=eGB_Waiver )   {   ++_hasCalled;  }
        if ( pack.GetCalledType(3)!=eGB_Waiver )   {   ++_hasCalled;  }

        if (_hasCalled == 0) {
            pack.GetProtocol() = MAKEDWORD( Games_Protocol, InitCards_BRD );
            pack.GetCalledStatus(0) = 0;
            pack.GetCalledStatus(1) = 0;
            pack.GetCalledStatus(2) = 0;
            pack.GetCalledType(0) = eGB_0;
            pack.GetCalledType(1) = eGB_0;
            pack.GetCalledType(2) = eGB_0;
            pack.GetPokerSize(0) = USER_PLAYER;
            pack.GetPokerSize(1) = USER_PLAYER;
            pack.GetPokerSize(2) = USER_PLAYER;
            BYTE * pokers = pack.GetPokers();
            Re_Init_Poker_SetUsercards( pokers, POKER_SIZE );   // 重新发牌
            pack.GetInitcards()  = PK_INITCARDS;
            g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
            return; // 重新洗牌消息
        }

        if (_count==2) {
            if ( pack.GetCalledType(r_seatid)==eGB_Waiver ) {
                Banker_Alloc_BasicCards(pack, byUserSign);
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, CreateBank_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );

                pack.GetFirst() = true;
                pack.GetPlaySeatId() = pack.GetBankerId();
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
                return;
            }
            else {
                // 第一个叫了，那还可以叫一次
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, Called_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
            }
        }

        if (_count==3) {
            if ( pack.GetCalledType(_seatid)==eGB_Waiver ) {
                Banker_Alloc_BasicCards(pack, byUserSign);
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, CreateBank_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );

                pack.GetFirst() = true;
                pack.GetPlaySeatId() = pack.GetBankerId();
                pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
                g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
                return;
            }
        }
    }
}

