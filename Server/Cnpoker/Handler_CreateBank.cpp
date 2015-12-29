#include "Handler_Module.h"
#include "CnpokerServer.h"


void MSG_Handler_CreateBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "CreateBank_REQ to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _nBattleid(0);
    js_map.ReadInteger("battleid",  _nBattleid);
    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return;
    }

    int  _dmodel    = pBattle->getModel();
    int  _multiple  = pBattle->getMultiple();
    BYTE _bySeatid  = pBattle->getBanker();
    int  _bankerkey = pBattle->getKey( _bySeatid );

    char szMsg[1024] = {0};
    char bankerpoker[128]  = {0};
    char poker[60]  = {0};
    char szPlayerkey[256] = {0};
    pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

    char format[256] = 	"{\"protocol\":\"%d\","
                        "%s,"
                        "\"multiple\":\"%d\","
                        "\"dmodel\":\"%d\","
                        "\"battleid\":\"%d\","
                        "\"banker\":%d,"
                        "\"bankerkey\":%d,"
                        "\"bankercount\":%d,"
                        "\"bankerpoker\":\"%s\","
                        "\"count\":3,"
                        "\"poker\":\"%s\"}";

    pBattle->getBasecards( _bySeatid, poker, sizeof(poker)  );
    int retCount = pBattle->getUsercards( _bySeatid, bankerpoker, sizeof(bankerpoker) );
    sprintf( szMsg, format,  MAKEDWORD( Games_Protocol, CreateBank_BRD ), szPlayerkey,
            _multiple, _dmodel, _nBattleid, _bySeatid,
            _bankerkey, retCount, bankerpoker, poker );
    // }}}@ 组合所有的牌

    // @{{{ 发送到其他玩家；
    WORD nLen = strlen( szMsg );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );

    // 设置玩家
    pBattle->SetPlaying( _bySeatid );
    pBattle->SetBattleStatus( eGB_PLAYING );
    pBattle->getDiscardTimes() = 0;

    DEBUG_MSG( LVL_DEBUG, "CreateBank_REQ to agent: %s \n", (char*)szMsg );
}

