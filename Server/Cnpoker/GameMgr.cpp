#include "GameMgr.h"
#include "CnpokerFactory.h"

int Init_Poker_Random( BYTE i )
{
    return rand() % i;
}

GameMgr g_GameMgr;

GameMgr::GameMgr() {
}

GameMgr::~GameMgr() {
}


/***********************************************
    洗一个牌
***********************************************/
int GameMgr::InitShuffle( BYTE _Team, BYTE _Size )
{
	m_vecCards.reserve( _Team * _Size );    // 54
    for(int i=0; i<_Team; ++i) {
		for(int j=0; j<_Size; ++j) {
            m_vecCards.push_back(j);
        }
	}
}

int GameMgr::UpdateShuffle( BYTE * _Pokers, BYTE _Size ) // 乱序
{
	srand( (unsigned)time(NULL) );
	std::random_shuffle( m_vecCards.begin(), m_vecCards.end(), Init_Poker_Random );
	if ( m_vecCards.size()>=_Size) {
        for(int i = 0; i < _Size; ++i) {
            _Pokers[i] = m_vecCards[i];
        }
	}
}

/***********************************************
    服务器桌子管理模块
***********************************************/
int GameMgr::InitTable()
{
    m_uiMaxTableSize = 0;
    memset( m_pAllTable, 0x0, sizeof(m_pAllTable) );
}

int GameMgr::UpdateTable()
{
    return 0;
}

GameTable * GameMgr::AllocTable( UINT uiIndex ) {
    GameTable * table = NULL;
    CnpokerFactory * factory = CnpokerFactory::Instance();
    if ( factory ) {
        table = factory->AllocGameTable();
        m_pAllTable[uiIndex] = table;
        m_uiMaxTableSize++;
    }
    return table;
}
GameTable * GameMgr::GetTable( UINT uiIndex ) {
    if ( uiIndex < 0xFFFF ) {
        return m_pAllTable[uiIndex];
    }
    return NULL;
}

void GameMgr::FreeBattles( UINT uiIndex ) {
    if ( uiIndex < 0xFFFF ) {
        if (m_pAllTable[uiIndex]!=NULL) {
            CnpokerFactory * factory = CnpokerFactory::Instance();
            if ( factory ) {
                factory->FreeGameTable(m_pAllTable[uiIndex]);
                m_uiMaxTableSize--;
            }
        }
    }
}

/*
void GameMgr::push_CalledBattles( GameBattle * pBattle ) {
    // m_pCalledBattle.push_back( pBattle );
}

void GameMgr::push_DiscardsBattles( GameBattle * pBattle ) {
    // m_pDiscardsBattle.push_back( pBattle );
}
*/
