#include "Handler_Module.h"
#include "CnpokerServer.h"
#include "GamePoker.h"

/*****************************************************
    Battle_Get_Usercards
*****************************************************/
BYTE Reminder_Get_Usercards( TablePacket & pack, BYTE byVal, BYTE seatId, BYTE * pIndex, BYTE maxSize );

/*****************************************************
    MSG_Handler_Reminder_REQ
*****************************************************/
int Reminder_User_Discards_Reminder ( TablePacket & pack, char * reminderPokers, BYTE & reminderSize );


/*****************************************************
    MSG_Handler_Reminder_REQ
*****************************************************/
void MSG_Handler_Reminder_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize < sizeof(UserPacket) ) {
        return ;
    }

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

    char * reminderPokers = user.GetReminderPokers();
    BYTE   reminderSize(0);
    Reminder_User_Discards_Reminder( pack, reminderPokers, reminderSize );
    user.GetReminderSize() = reminderSize;

    user.GetProtocol() = MAKEDWORD( Games_Protocol, Reminder_ANC );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)&user, user.GetPacketSize() );
}


/*****************************************************
    Reminder_Get_Cards
*****************************************************/
BYTE Reminder_Get_Byte_Usercards( TablePacket & pack, BYTE byVal, BYTE seatId, BYTE * pIndex, BYTE maxSize ) {

    char * poker = pack.GetDisplayPokers(seatId);
    BYTE * pMove = pack.GetPokers();
    char szPoker[8] = {0};
    char szPokerList[128] = {0};
    BYTE byPoker(0), byCount(0);
    for (int  i=0; i<POKER_SIZE; i++) {
        byPoker = pMove[i];
        if ( byPoker == byVal ) {
            pIndex[byCount] = i;
            byCount++;
            if(byCount>=maxSize){
                break;
            }
        }
    }
    return byCount;
}

/*****************************************************
    Reminder_Get_Usercards
*****************************************************/
BYTE Reminder_Get_Char_Cards( BYTE * byIndex, BYTE bySize, char * szList) {
    BYTE pk(0);
    char szPoker[8] = {0};
    for (int  i=0; i<bySize; i++) {
        pk = byIndex[i];
        memset( szPoker, 0x0, sizeof(szPoker) );
        if ( i!=0 ) {
            strcat( szList, ",");
        }
        sprintf( szPoker, "%d", pk );
        strcat( szList, szPoker);
    }
    return bySize;
}


/*****************************************************
    MSG_Handler_Reminder_REQ
*****************************************************/
int Reminder_User_Discards_Reminder ( TablePacket & pack, char * reminderPokers, BYTE & reminderSize )
{
    BYTE currSize(0);
    BYTE seatid = pack.GetPlaySeatId();
    BYTE userSign = PK_USER_0 + seatid;
    BYTE userPoker[USER_BANKER] = {0};
    currSize = Reminder_Get_Byte_Usercards( pack, userSign, seatid, userPoker, USER_BANKER );

    if ( currSize==0 ) {
        return false;    // NO.1 : 牌不够情况
    }

    if ( pack.GetFirst()==true ) {  // 查找第一张牌

    }
    else {

        // 比较对方的牌
        BYTE _byType(0), _byValue(0), _byCount(0), _bySize(0);

        _byType  = pack.GetThanType();
        _byValue = pack.GetThanValue();
        _byCount = pack.GetThanCount();
        _bySize  = pack.GetIndexSize();

        if ( currSize < _bySize ) {
            return false;   // NO.1 : 牌不够情况
        }

        // step3: 判断能否打得起;
        GamePoker _gmPoker;
        _gmPoker.Parser( userPoker, currSize );
        if ( !_gmPoker.MoreThan( _byType, _bySize, _byValue, _byCount) ) {
            return false;   // 查找对象;
        }

        if ( !_gmPoker.FindObjectMoreThanToClone( _byType, _bySize, _byValue, _byCount ) ) {
            return false;   // ---
        }

        if ( !_gmPoker.ClonePoker( userPoker, reminderSize ) ) {
            return false;   // 克隆对象
        }

        Reminder_Get_Char_Cards(userPoker, reminderSize, reminderPokers);
        return true;
    }
    return false;
}
