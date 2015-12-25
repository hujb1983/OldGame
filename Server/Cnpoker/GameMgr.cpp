#include "GameMgr.h"
#include "CnpokerFactory.h"

GameMgr g_GameMgr;

GameMgr::GameMgr()
{

}

GameMgr::~GameMgr()
{

}

/***********************************************
服务器桌子管理模块
***********************************************/
/*  1. 从工厂里初始化一定数量的数据到空的容器里面；
    2. 然后放到空队列里面 */
int GameMgr::InitBattles(int _resize)
{
    GameBattle       * pBattle = NULL;
    CnpokerFactory  * pFactory = NULL;
    pFactory = CnpokerFactory::Instance();

    m_wBattleSize = 0;
    m_pEmptyBattles.clear();
    if ( pFactory )
    {
        // index 是不能分配的;
        for (int i=1; i<_resize; i++)
        {
            pBattle = pFactory->AllocGameBattle();
            if ( pBattle ) {
               if ( i < MC_BATTLES )
                {
                    pBattle->SetIndex(i);
                    m_pAllBattles[i] = pBattle;
                    m_pEmptyBattles.push_back(pBattle);
                    ++m_wBattleSize;
                }
            }
        }
    }
}

/*  1. 分配一个新的 GameBattle；
    2. 先从 m_pEmptyBattles 里面取；
    3. 不够就从GameFactory去取;
    4. 然后存入桌子窗口*/
GameBattle * GameMgr::AllocBattle()
{
    GameBattle * pBattle = NULL;
    if ( m_pEmptyBattles.size() > 0 ) {
        pBattle = m_pEmptyBattles.pop_frist();
        if ( pBattle ) {
            return pBattle;
        }
    }
    else {

        CnpokerFactory * pFactory = CnpokerFactory::Instance();
        if ( pFactory )
        {
            pBattle = pFactory->AllocGameBattle();
            if ( pBattle ) {

                // pBattle->FreeBattle();
                if ( m_wBattleSize < MC_BATTLES )
                {
                    pBattle->SetIndex(m_wBattleSize);
                    m_pAllBattles[m_wBattleSize] = pBattle;
                    ++m_wBattleSize;
                    return pBattle;
                }
                pFactory->FreeGameBattle(pBattle);
            }
        }
    }
    return NULL;
}

/*  1. 通过找到桌子ID 查找一个 GameBattle； */
GameBattle * GameMgr::GetBattle( WORD t_idx ) {
    if ( t_idx<MC_BATTLES ) {
        return m_pAllBattles[t_idx];
    }
    return NULL;
}

/*  1. 用作回收桌子操作；
    2. 判断不为空就收回； */
void GameMgr::FreeBattles( GameBattle * pBattle ) {
    m_pEmptyBattles.push_back( pBattle );
}


BOOL GameMgr::UpdateTimes( DWORD dwTicket )
{
    DWORD dwTick = Session::GetTickCount();
    GameBattle * pBattle = NULL;

    // 叫地主队列超时处理
    pBattle = m_pCalledBattle.get_frist();
    for(;pBattle;)
    {
        if ( !pBattle->UpdateOvertime( dwTick ) ) {
            break;
        }
        m_pCalledBattle.pop_frist();
        pBattle = m_pCalledBattle.get_frist();
    }

    // 出牌队列超时处理
    pBattle = m_pDiscardsBattle.get_frist();
    for(;pBattle;)
    {
        if ( !pBattle->UpdateOvertime( dwTick ) ) {
            break;
        }
        m_pDiscardsBattle.pop_frist();              // 先推出第一个;
        pBattle = m_pDiscardsBattle.get_frist();    // 再压入第一个;
    }
    return 0;
}

void GameMgr::push_CalledBattles( GameBattle * pBattle ) {
    m_pCalledBattle.push_back( pBattle );
}

void GameMgr::push_DiscardsBattles( GameBattle * pBattle ) {
    m_pDiscardsBattle.push_back( pBattle );
}
