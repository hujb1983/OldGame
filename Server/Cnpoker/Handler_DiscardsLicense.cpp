#include "Handler_Module.h"
#include "GameBattle.h"
#include "GamePoker.h"
#include "CnpokerServer.h"


/********** 提示 **********/
int Battle_Poker_Discards_Reminder ( BYTE seatid, GameBattle * pBattle )
{
    if ( pBattle->getDiscardTimes()!=0 )
    {
        BYTE pbyIndex[MAX_BHOLD] = {0};
        BYTE byIndexSize = MAX_BHOLD;
        BYTE _byCurrSize = 0;
        _byCurrSize = pBattle->getCurrentCards( seatid, pbyIndex, MAX_BHOLD );

        BYTE _byType, _byValue, _byCount, _bySize;
        _byType  = pBattle->getLastType();
        _byValue = pBattle->getLastValue();
        _byCount = pBattle->getLastCount();
        _bySize  = pBattle->getLastSize();

        // step1: 牌的整张数不够多;
        if ( _byCurrSize < _bySize ) {
            return -1;
        }

        // step3: 判断能否打得起;
        GamePoker _gmPoker;
        _gmPoker.Parser( pbyIndex, _byCurrSize );
        if ( _gmPoker.MoreThan( _byType, _bySize, _byValue, _byCount)==FALSE ) {
            return -1;
        }
    }
    return 0;
}

/********** 机器人帮打牌 **********/
int Battle_Poker_Discards_Robot ( BYTE _seatid, GameBattle * pBattle )
{
    BYTE pbyIndex[MAX_BHOLD+1] = {0};
    BYTE byIndexSize = MAX_BHOLD;
    BYTE _byCurrSize = 0;
    _byCurrSize = pBattle->getCurrentCards( _seatid, pbyIndex, MAX_BHOLD );

    BYTE _byType, _byValue, _byCount, _bySize;
    _byType  = pBattle->getLastType();
    _byValue = pBattle->getLastValue();
    _byCount = pBattle->getLastCount();
    _bySize  = pBattle->getLastSize();

    // step1: 牌的整张数不够多;
    int _istatus = 0;
    if ( _byCurrSize < _bySize ) {
        _istatus = -1;
    }

    // step3: 判断能否打得起;
    GamePoker _gmPoker;
    _gmPoker.Parser( pbyIndex, _byCurrSize );
    if ( _gmPoker.MoreThan( _byType, _bySize, _byValue, _byCount)==FALSE ) {
        _istatus = -1;
    }

    // step4. 判断并克隆
    int  _iBattleid = pBattle->getIndex();
    int  _iCount = 0;
    char _szPoker[256] = {0};
    if ( _gmPoker.FindObjectMoreThanToClone( _byType, _bySize, _byValue, _byCount)==FALSE ) {
        _istatus = -1;
        _iCount  = 0;
        strcat(_szPoker, " ");
    }

    // step4. 获得克隆参数
    BYTE byNewIndex[MAX_BHOLD] = {0};
    BYTE byNewIndexSize = MAX_BHOLD;
    BYTE byLastType  = PH_0;
    BYTE byLastSize  = 0;
    BYTE byLastValue = 0;
    BYTE byLastCount = 0;
    if ( _gmPoker.ClonePoker( byNewIndex, byNewIndexSize )==TRUE ) {
        _gmPoker.ClonePokerType(byLastType, byLastSize, byLastValue, byLastCount);
    }

    // 初始化最后出牌者
    if ( byLastType!=PH_0 ){
        pBattle->SetLastType ( byLastType  );
        pBattle->SetLastValue( byLastValue );
        pBattle->SetLastCount( byLastCount );
        pBattle->SetLastSize ( byLastSize );
    }


    /* step6. 打印参数到所有客户端 */
    {
        // @{{{ 组合所有的牌，发送给所有玩家;
        char szMsg[1024] = {0};
        char format[256] = 	"{ \"protocol\":\"%d\","
                                "%s,"
                                "\"data\":[{"
                                "\"errmsg\":\"%d\","
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"pktype\":\"%d\","
                                "\"count\":\"%d\","
                                "\"poker\":\"%s\"}] }";
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        sprintf( szMsg, format, MAKEDWORD(Games_Protocol, Discards_BRD ),
                szPlayerkey,
                0,
                pBattle->getIndex(),
                _seatid,
                byLastType,
                byNewIndexSize,
                byNewIndex);
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );

        BYTE _byNextSeatid;
        _byNextSeatid = pBattle->nextSeat( _seatid );  // 授权-并判断
        pBattle->SetPlaying( _byNextSeatid );
    }

    return 0;
}

/********** 离线不出牌通知 **********/
int Battle_Poker_Discards_Offline ( BYTE seatid, GameBattle * pBattle )
{
    {
        // @{{{ 组合所有的牌，发送给所有玩家;
        char szMsg[1024] = {0};
        char format[256] = 	"{ \"protocol\":\"%d\","
                                "%s,"
                                "\"data\":[{"
                                "\"errmsg\":\"0\","
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"pktype\":\"%d\","
                                "\"count\":\"%d\","
                                "\"poker\":\"%s\"}] }";
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        sprintf( szMsg, format, MAKEDWORD(Games_Protocol, Discards_BRD ),
                szPlayerkey,
                pBattle->getIndex(),
                seatid,
                0,      // 没有类型
                0,      // 没有出牌
                ";");   // 这个不必要取;
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );
    }
}


/*  1. 玩家出牌后，发出这个认证；
    2. Discard只负责验证牌，然后发出通知，牌合不合格;
    3. 认证过程。
        a> 如果是天炸就自己出牌
        b> 发现下一家打不起就再转出;
        c> 发现再一下打不起就再转回;    */

/* ÊÚÈ¨ --- Í¨Öª */
void MSG_Handler_DiscardsLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG,  "DiscardsLicense_REQ to recv: %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey(0), _battleid(0), _seatid(0);
    js_map.ReadInteger( "battleid", _battleid );

    GameBattle *pBattle = g_GameMgr.GetBattle( _battleid );
    if ( !pBattle ) {
        return;
    }

    BYTE _bySeatid = pBattle->getPlaying();  // 授权-并判断


    int _istatus = 0;
    /*
    if ( pBattle->canEnd() ) {
        // pBattle->UpdateToDatabase();
        return;
    }
    */

    /* step1. 托管直接请机器人帮助打牌  */
    if ( pBattle->IsTrusteeship( _bySeatid )==0 ) {
        Battle_Poker_Discards_Robot( _bySeatid, pBattle );
        return;   // 机器人已经出牌}
    }

    /* step2. 提示能否打得起  */
    if ( Battle_Poker_Discards_Reminder( _bySeatid, pBattle)==-1 ) {
        _istatus = -1;
    }

    /* step3. 如果是离线，直接通知打不起  */

    /* if ( pBattle->IsOnline( _bySeatid )==-1 ){
        Battle_Poker_Discards_Offline( _bySeatid, pBattle );
        return;
    } */

    /* step4. 判断授权者是否是最后出牌者  */
    if ( pBattle->getLastSeat()==_bySeatid ) {
        pBattle->getDiscardTimes() = 0;     // 重新开始这轮出牌
    }

    {
        int _basics    = pBattle->getMinMoney();
        int _multiple  = pBattle->getMultiple();
        int _brokerage = pBattle->getBrokerage();

        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"status\":\"%d\","
                            "\"battleid\":\"%d\","
                            "\"seatid\":\"%d\","
                            "\"basics\":\"%d\","
                            "\"multiple\":\"%d\","
                            "\"brokerage\":\"%d\","
                            "\"times\":\"18\" }";   // 暂时设定为18秒;

        snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, DiscardsLicense_BRD ),
                szPlayerkey, _istatus, _battleid, _bySeatid, _basics, _multiple, _brokerage );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );

        /* step5. 送到超时判断队列...  */
        // pBattle->SetDiscardsOvertime();
    }
}

