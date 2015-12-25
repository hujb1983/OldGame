#include "Handler_Module.h"
#include "CnpokerServer.h"

/*
    开始的设置比较自由！
*/
int Processed_StartGame ( ServerSession * pServerSession, const char * pInput )
{
    DEBUG_MSG( LVL_DEBUG, "StartGame_REQ to recv: %s. \n",  pInput );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return -1;
    }

    int _nBattleid = 0;
    int _nSeatid   = 0;
    int _nUserkey  = 0;
    int _nStatus   = 0;

    js_map.ReadInteger("battleid",  _nBattleid);
    js_map.ReadInteger("seatid",    _nSeatid  );
    js_map.ReadInteger("status",    _nStatus  );

    // @{{{ 检查参数
    if ( (_nSeatid<0) || (_nSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    _nUserkey = pBattle->getKey(_nSeatid);

    // 如果为 FALSE, 表示已经开始游戏了;
    if ( pBattle->SetStart( _nSeatid, eGB_Ready )==FALSE ) {
        return FALSE;
    }

    int _nReady = 0;
    if ( pBattle->canStart() ) {
        _nReady = 1;
        pBattle->SetBattleStatus( eGB_DEALING );
    }
    // }}}@ 检查参数

    // 先声明开始
    {
        char szMsg[1024] = {0};
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        UINT _mult(0), _mmax(0), _mmin(0);
        WORD _brokerage(0);
        _mult = pBattle->getMultiple();
        _mmax = pBattle->getMaxMoney();
        _mmin = pBattle->getMinMoney();
        _brokerage = pBattle->getBrokerage();

        // @{{{ 组合所有的牌
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"userkey\":\"%d\","
                            "\"ready\":\"%d\","
                            "\"seatid\":\"%d\","
                            "\"status\":\"%d\","
                            "\"battleid\":\"%d\","
                            "\"multiple\":\"%d\","
                            "\"basics\":\"%d\","
                            "\"brokerage\":\"%d\"}";

        sprintf( szMsg, format, MAKEDWORD( Games_Protocol, StartGame_BRD ),
                szPlayerkey, _nUserkey, _nReady, _nSeatid, _nStatus,
                pBattle->getIndex(),
                _mult, _mmin, _brokerage );
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );

        DEBUG_MSG( LVL_DEBUG, "StartGame_REQ to agent: %s. \n",  szMsg );
        return TRUE;
    }
}

void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Processed_StartGame(pServerSession, (char*) pMsg);
}
