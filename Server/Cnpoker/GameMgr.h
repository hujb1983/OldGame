#ifndef _GameMgr_H_INCLUDED_
#define _GameMgr_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "GameTable.h"

// The biggest count;
#define MC_BATTLES             (0xFFFF / 3)
#define MC_USERS               (0xFFFF)

class GameMgr
{
public:
	GameMgr();
	virtual ~GameMgr();

public:
    int  InitTable();
    int  UpdateTable();

    GameTable * AllocTable( UINT uiIndex );
    GameTable * GetTable( UINT uiIndex );
    void  FreeBattles( UINT uiIndex );

private:   // 桌子
    UINT m_uiMaxTableSize;
    GameTable * m_pAllTable[0xFFFF]; // 所有桌子的容器

public:
    /*
    BOOL UpdateTimes( DWORD dwTicket );                   // 更新;
    void push_CalledBattles( GameBattle * pBattle );      // 设置叫牌复本;
    void push_DiscardsBattles( GameBattle * pBattle );    // 设置出牌复本;
    */
};

extern GameMgr g_GameMgr;

#endif // _AGENTPLAYER_H_INCLUDED_
