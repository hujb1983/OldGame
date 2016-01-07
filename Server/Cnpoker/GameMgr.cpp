#include "GameMgr.h"
#include "CnpokerFactory.h"

GameMgr g_GameMgr;

GameMgr::GameMgr() {
}

GameMgr::~GameMgr() {
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
