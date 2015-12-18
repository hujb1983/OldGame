#include "Handler_Module.h"
#include "CnpokerServer.h"

/*
    {protocol : 1, userkey : 2, status : 3}
*/
int Processed_Showcards ( ServerSession * pServerSession, const char * pInput )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return -1;
    }

    int _nBattleid = 0;
    int _nSeatid   = 0;
    int _nUserkey  = 0;

    js_map.ReadInteger("battleid",  _nBattleid);
    js_map.ReadInteger("seatid",    _nSeatid);
    js_map.ReadInteger("userkey" ,  _nUserkey);

    // @{{{ 检查参数
    if ( (_nSeatid<0) || (_nSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    if ( !pBattle->SetShow(_nSeatid) ) {
        return FALSE;
    }
    // }}}@ 检查参数

    // if ( pBattle->getBattleStatus() == eGB_DEALING )
    {

        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        // @{{{ 组合所有的牌
        char szMsg[1024] = {0};
        char poker[256]  = {0};
        char format[256] = 	"{\"protocol\":\"%d\", "
                            " \"showcard\":\"%d\", "
                            "%s,"
                            " \"data\":[ {"
                                "\"battleid\":\"%d\", "
                                "\"seatid\":\"%d\", "
                                "\"poker\":\"%s\"} ] }";

        pBattle->getUsercards( _nSeatid, poker, sizeof(poker) );
        sprintf( szMsg, format, MAKEDWORD(Games_Protocol, ShowCards_BRD ),
              _nUserkey, szPlayerkey, pBattle->getIndex(), _nSeatid, poker );
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        int nLen = strlen(szMsg);
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );
        return TRUE;
    }
}


void MSG_Handler_ShowCards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Processed_Showcards(pServerSession, (char*)pMsg);
}

