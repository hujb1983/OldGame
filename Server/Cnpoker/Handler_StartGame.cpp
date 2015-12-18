#include "Handler_Module.h"
#include "CnpokerServer.h"

/*
    开始的设置比较自由！
*/
int Processed_StartGame ( ServerSession * pServerSession, const char * pInput )
{
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
    js_map.ReadInteger("userkey",   _nUserkey );
    js_map.ReadInteger("status",    _nStatus );

    // @{{{ 检查参数
    if ( (_nSeatid<0) || (_nSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    // -- 设置为已开始  _nStatus 用于判断是标准开始，还是显示开始
    if ( !pBattle->SetStart( _nSeatid, _nStatus ) ) {
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

        // @{{{ 组合所有的牌
        char format[256] = 	"{\"protocol\":\"%d\","
                            " \"ready\":\" %d \","
                            "%s,"
                            "\"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"status\":\"%d\"}] }";

        sprintf( szMsg, format, MAKEDWORD( Games_Protocol, StartGame_BRD ),
                1,
                szPlayerkey,
                pBattle->getIndex(),
                _nSeatid,
                _nStatus );     // 明牌开始
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );
        return TRUE;
    }

	// }}}@ 发送到其他玩家;
}

void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Processed_StartGame(pServerSession, (char*) pMsg);
}
