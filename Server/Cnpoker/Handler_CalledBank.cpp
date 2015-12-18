#include "Handler_Module.h"
#include "CnpokerServer.h"


/*
    {protocol : 1, userkey : 2, status : 3}
*/
int Processed_Called ( ServerSession * pServerSession, const char * pInput )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return -1;
    }

    int _nBattleid = 0;
    int _nSeatid   = 0;
    int _nUserkey  = 0;
    int _nCalled   = 0;

    js_map.ReadInteger("battleid",  _nBattleid);
    js_map.ReadInteger("seatid",    _nSeatid  );
    js_map.ReadInteger("userkey",   _nUserkey );
    js_map.ReadInteger("called",    _nCalled );

     // @{{{ 检查参数
    if ( (_nSeatid<0) || (_nSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    // 判断是否是在叫地主阶段
    /* if ( !pBattle->canCall() ) {
        return FALSE;
    } */


    // -- 设置为开始
    int _initcard = 0;
    if ( _nCalled != 0 ) {
        if ( !pBattle->SetCalled( _nSeatid, _nCalled ) ) {
            _initcard = 1;
        }
    }
     // }}}@ 检查参数

    // 生成地主了;
    int _nLastCalled = 0;
    if ( pBattle->canGame() ) {
        _nLastCalled = 1;
        pBattle->SetBattleStatus( eGB_PLAYING );
    }

    // @{{{ 组合所有的牌
    char szMsg[1024] = {0};

    char szPlayerkey[256] = {0};
    pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

    char format[256] = 	"{\"protocol\":\"%d\","
                        " \"initcard\": \"%d\", "
                        " \"create\": \"%d\", "
                        "%s,"
                        "\"data\":[{"
                            "\"battleid\":\"%d\","
                            "\"called\":\"%d\","
                            "\"seatid\":\"%d\" }] }";

    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, Called_BRD ),
            _initcard,                    // 都不要重新分牌;
            1,                            // 替换成 _nLastCalled;
            szPlayerkey,
            _nBattleid,                   // 房间号!
            _nCalled,                     // 要地吗?
            _nSeatid                      // 现在叫地主的人;
            );
    // }}}@ 组合所有的牌

    // @{{{ 发送到其他玩家；
    WORD nLen = strlen( szMsg );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );
    // }}}@

}

void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Processed_Called( pServerSession, (char *)pMsg );
}

