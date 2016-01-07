#include "Handler_Module.h"
#include "CnpokerServer.h"

int Initcards_SendToAgentServer( WORD _wBattleid, GameBattle * pBattle );
int Battle_Dealer_Shuffle( BYTE * _byAllcards, BYTE _bySize );
int Battle_Check_Shuffle( BYTE * _byAllcards, BYTE _bySize );
int Battle_Poker_Alloc( GameBattle * pBattle, BYTE * _byAllcards, BYTE _bySize );
int Battle_Poker_Random(BYTE i) { return rand() % i; }

/*  �� [AgentServer] ת������! */
void MSG_Handler_InitCards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_InitCards_REQ %s] \n",  (char *) pMsg );

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
    Initcards_SendToAgentServer( _nBattleid, pBattle );
    // }}}@ ������е���

    // �����Ѿ������ˣ�
    pBattle->SetStartSeat( 0 );
    pBattle->SetBattleStatus( eGB_CALLING );
    pBattle->SetBank( eGB_NoBanker );
}

/* ȡ�� */
int Initcards_SendToAgentServer( WORD _wBattleid, GameBattle * pBattle )
{
	char szMsg[1024]  = {0};
    char format[256] = 	"{\"protocol\":\"%d\","
                        " %s,"
                        " \"battleid\":\"%d\","
                        " \"count\":\"%d\","
                        " \"poker\":\"-1\","
                        " \"count0\":\"%d\","
                        " \"poker0\":\"%s\","
                        " \"count1\":\"%d\","
                        " \"poker1\":\"%s\","
                        " \"count2\":\"%d\","
                        " \"poker2\":\"%s\" }";

    char poker1[256]  = {0};
    char poker2[256]  = {0};
    char poker3[256]  = {0};

    BYTE pkSize1(0), pkSize2(0), pkSize3(0);
    pkSize1   = pBattle->getUsercards( 0, poker1, sizeof(poker1) );
    pkSize2   = pBattle->getUsercards( 1, poker2, sizeof(poker2) );
    pkSize3   = pBattle->getUsercards( 2, poker3, sizeof(poker3) );

    char szPlayerkey[256] = {0};
    pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

    sprintf( szMsg, format, MAKEDWORD(Games_Protocol, InitCards_BRD ),
         szPlayerkey,
         _wBattleid,        // ����ID;
         3,                 // ����
         pkSize1, poker1,   // �û�-1
         pkSize2, poker2,   // �û�-2
         pkSize3, poker3);  // �û�-3

    int len = strlen(szMsg);
    g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, len );
	return len;
}

/* ȡ�� */
int Battle_Poker_Alloc( GameBattle * pBattle, BYTE * _byAllcards, BYTE _bySize )
{
    pBattle->SetUsercards(0, _byAllcards, 17);
    pBattle->SetUsercards(1, _byAllcards + 17, 17);
    pBattle->SetUsercards(2, _byAllcards + 34, 17);
    pBattle->SetBasecards( _byAllcards + 51, 3 );
    return TRUE;
}

/* ���ը������ */
int Battle_Check_Shuffle( BYTE * _byAllcards, BYTE _bySize )
{

}

/* ϴ�� */
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

