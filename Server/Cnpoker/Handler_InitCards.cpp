#include "Handler_Module.h"
#include "CnpokerServer.h"

int SendToPlayer_OwendPoker( BYTE _bySeat, GameBattle * pBattle );
int Battle_Dealer_Shuffle( BYTE * _byAllcards, BYTE _bySize );
int Battle_Check_Shuffle( BYTE * _byAllcards, BYTE _bySize );
int Battle_Poker_Alloc( GameBattle * pBattle, BYTE * _byAllcards, BYTE _bySize );
int Battle_Poker_Random(BYTE i) { return rand() % i; }

/*
    �� [AgentServer] ת������!
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

    // �ж��Ƿ����ڷ��ƽ׶�
    /* if ( !pBattle->canDealing() ) {
        return FALSE;
    } */

    // @{{{ ����
    BYTE _byAllcards[MAX_POKER+1] = {0};
    Battle_Dealer_Shuffle( _byAllcards, sizeof(_byAllcards)-1 );
    Battle_Poker_Alloc( pBattle, _byAllcards, sizeof(_byAllcards)-1 );
    // }}}@ ����

    // @{{{ ����Լ�����
    SendToPlayer_OwendPoker(0, pBattle);
    SendToPlayer_OwendPoker(1, pBattle);
    SendToPlayer_OwendPoker(2, pBattle);
    // }}}@ ������е���

    // �����Ѿ������ˣ�
    pBattle->SetBattleStatus( eGB_CALLING );
}

/*  1. ȡ�� */
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

/*  1. ȡ�� */
int Battle_Poker_Alloc( GameBattle * pBattle, BYTE * _byAllcards, BYTE _bySize )
{
    pBattle->SetUsercards(0, _byAllcards, 17);
    pBattle->SetUsercards(1, _byAllcards + 17, 17);
    pBattle->SetUsercards(2, _byAllcards + 34, 17);
    pBattle->SetBasecards(_byAllcards + 52, 3);
    return TRUE;
}

/*  1. ���ը������ */
int Battle_Check_Shuffle( BYTE * _byAllcards, BYTE _bySize )
{

}

/*  1. ϴ�� */
int Battle_Dealer_Shuffle( BYTE * _byAllcards, BYTE _bySize )
{
    // ���������
	srand( (unsigned)time(NULL) );

	vector<BYTE> vecCards;
	vecCards.reserve( _bySize ); // 54
    for(int i = 0; i< _bySize; ++i) {
		vecCards.push_back(i);
	}

	// ����
	std::random_shuffle( vecCards.begin(), vecCards.end(), Battle_Poker_Random );

	BYTE * pMove = _byAllcards;
	for(int i = 0; i < _bySize; ++i) {
        pMove[i] = vecCards[i];  // �Ѷ����������Ƹ���������
	}

}

