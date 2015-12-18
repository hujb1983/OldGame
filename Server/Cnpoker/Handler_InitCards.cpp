#include "Handler_Module.h"
#include "CnpokerServer.h"

int SendToPlayer_OwendPoker( BYTE _bySeat, GameBattle * pBattle );
int Battle_Dealer_Shuffle( BYTE * _byAllcards, BYTE _bySize );
int Battle_Check_Shuffle( BYTE * _byAllcards, BYTE _bySize );
int Battle_Poker_Alloc( GameBattle * pBattle, BYTE * _byAllcards, BYTE _bySize );
int Battle_Poker_Random(BYTE i) { return rand() % i; }

/*
    由 [AgentServer] 转发过来!
*/
void MSG_Handler_InitCards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _nBattleid = 0;
    js_map.ReadInteger("battleid",  _nBattleid);

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return;
    }

    // 判断是否是在发牌阶段
    /* if ( !pBattle->canDealing() ) {
        return FALSE;
    } */

    // @{{{ 发牌
    BYTE _byAllcards[MAX_POKER+1] = {0};
    Battle_Dealer_Shuffle( _byAllcards, sizeof(_byAllcards)-1 );
    Battle_Poker_Alloc( pBattle, _byAllcards, sizeof(_byAllcards)-1 );
    // }}}@ 发牌

    // @{{{ 组合自己的牌
    SendToPlayer_OwendPoker(0, pBattle);
    SendToPlayer_OwendPoker(1, pBattle);
    SendToPlayer_OwendPoker(2, pBattle);
    // }}}@ 组合所有的牌

    // 设置已经发牌了；
    pBattle->SetBattleStatus( eGB_CALLING );
}

/*  1. 取牌 */
int SendToPlayer_OwendPoker( BYTE _bySeat, GameBattle * pBattle )
{
	char szMsg[1024]  = {0};
	char poker[256]  = {0};
    char format[256] = 	"{\"protocol\":\"%d\", "
                        " \"userkey\":\"%d\", "
                        " \"data\":[{"
                            "\"battleid\":\"%d\", "
                            "\"seatid\":\"%d\", "
                            "\"status\":\"%d\", "
                            "\"poker\":\"%s\"}] }";

    pBattle->getUsercards( _bySeat, poker, sizeof(poker) );
    sprintf( szMsg, format, MAKEDWORD(Games_Protocol, InitCards_BRD ),
         pBattle->getKey(_bySeat),
         pBattle->getIndex(),
         pBattle->getStartSeat(),
         1,
         poker );

    int len = strlen(szMsg);
    g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, len );
	return len;
}

/*  1. 取牌 */
int Battle_Poker_Alloc( GameBattle * pBattle, BYTE * _byAllcards, BYTE _bySize )
{
    pBattle->SetUsercards(0, _byAllcards, 17);
    pBattle->SetUsercards(1, _byAllcards + 17, 17);
    pBattle->SetUsercards(2, _byAllcards + 34, 17);
    pBattle->SetBasecards(_byAllcards + 52, 3);
    return TRUE;
}

/*  1. 检查炸弹数量 */
int Battle_Check_Shuffle( BYTE * _byAllcards, BYTE _bySize )
{

}

/*  1. 洗牌 */
int Battle_Dealer_Shuffle( BYTE * _byAllcards, BYTE _bySize )
{
    // 随机打乱牌
	srand( (unsigned)time(NULL) );

	vector<BYTE> vecCards;
	vecCards.reserve( _bySize ); // 54
    for(int i = 0; i< _bySize; ++i) {
		vecCards.push_back(i);
	}

	// 乱序
	std::random_shuffle( vecCards.begin(), vecCards.end(), Battle_Poker_Random );

	BYTE * pMove = _byAllcards;
	for(int i = 0; i < _bySize; ++i) {
        pMove[i] = vecCards[i];  // 把额外那三张牌给保存下来
	}

}

