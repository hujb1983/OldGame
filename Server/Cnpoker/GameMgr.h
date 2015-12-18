#ifndef _GameMgr_H_INCLUDED_
#define _GameMgr_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "GameBattle.h"

// The biggest count;
#define MC_BATTLES              (0xFFFF / 3)
#define MC_USERS               (0xFFFF)

class GameMgr
{
public:
	GameMgr();
	virtual ~GameMgr();

public:
    /*  1. 从工厂里初始化一定数量的数据到空的容器里面；
        2. 然后放到空队列里面 */
    int  InitBattles(int _resize = 1);                       // 初始化所有桌子;

    /*  1. 分配一个新的 GameTable；*/
    GameBattle * AllocBattle();

    /*  1. 通过找到桌子ID 查找一个 GameTable； */
    GameBattle * GetBattle( int t_idx );

    /*  1. 用作回收桌子操作； */
    void  FreeBattles(GameBattle * pBattle);

private:   // 桌子
    WORD             m_wBattleSize;                  // 桌子的大小;
    GameBattle *     m_pAllBattles[MC_BATTLES];      // 所有桌子的容器;
    GameBattleList   m_pEmptyBattles;                // 空的桌子队列;

public:
    BOOL UpdateTimes( DWORD dwTicket );                           // 更新;
    void push_CalledBattles( GameBattle * pBattle );      // 设置叫牌复本;
    void push_DiscardsBattles( GameBattle * pBattle );    // 设置出牌复本;

private:   // 桌子
    GameBattleList   m_pCalledBattle;                // 叫牌复本;
    GameBattleList   m_pDiscardsBattle;              // 出牌复本;
};

extern GameMgr g_GameMgr;

#endif // _AGENTPLAYER_H_INCLUDED_
