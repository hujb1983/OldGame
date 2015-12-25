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
���������ӹ���ģ��
***********************************************/
/*  1. �ӹ������ʼ��һ�����������ݵ��յ��������棻
    2. Ȼ��ŵ��ն������� */
int GameMgr::InitBattles(int _resize)
{
    GameBattle       * pBattle = NULL;
    CnpokerFactory  * pFactory = NULL;
    pFactory = CnpokerFactory::Instance();

    m_wBattleSize = 0;
    m_pEmptyBattles.clear();
    if ( pFactory )
    {
        // index �ǲ��ܷ����;
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

/*  1. ����һ���µ� GameBattle��
    2. �ȴ� m_pEmptyBattles ����ȡ��
    3. �����ʹ�GameFactoryȥȡ;
    4. Ȼ��������Ӵ���*/
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

/*  1. ͨ���ҵ�����ID ����һ�� GameBattle�� */
GameBattle * GameMgr::GetBattle( WORD t_idx ) {
    if ( t_idx<MC_BATTLES ) {
        return m_pAllBattles[t_idx];
    }
    return NULL;
}

/*  1. �����������Ӳ�����
    2. �жϲ�Ϊ�վ��ջأ� */
void GameMgr::FreeBattles( GameBattle * pBattle ) {
    m_pEmptyBattles.push_back( pBattle );
}


BOOL GameMgr::UpdateTimes( DWORD dwTicket )
{
    DWORD dwTick = Session::GetTickCount();
    GameBattle * pBattle = NULL;

    // �е������г�ʱ����
    pBattle = m_pCalledBattle.get_frist();
    for(;pBattle;)
    {
        if ( !pBattle->UpdateOvertime( dwTick ) ) {
            break;
        }
        m_pCalledBattle.pop_frist();
        pBattle = m_pCalledBattle.get_frist();
    }

    // ���ƶ��г�ʱ����
    pBattle = m_pDiscardsBattle.get_frist();
    for(;pBattle;)
    {
        if ( !pBattle->UpdateOvertime( dwTick ) ) {
            break;
        }
        m_pDiscardsBattle.pop_frist();              // ���Ƴ���һ��;
        pBattle = m_pDiscardsBattle.get_frist();    // ��ѹ���һ��;
    }
    return 0;
}

void GameMgr::push_CalledBattles( GameBattle * pBattle ) {
    m_pCalledBattle.push_back( pBattle );
}

void GameMgr::push_DiscardsBattles( GameBattle * pBattle ) {
    m_pDiscardsBattle.push_back( pBattle );
}
