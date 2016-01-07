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

private:   // ����
    UINT m_uiMaxTableSize;
    GameTable * m_pAllTable[0xFFFF]; // �������ӵ�����

public:
    /*
    BOOL UpdateTimes( DWORD dwTicket );                   // ����;
    void push_CalledBattles( GameBattle * pBattle );      // ���ý��Ƹ���;
    void push_DiscardsBattles( GameBattle * pBattle );    // ���ó��Ƹ���;
    */
};

extern GameMgr g_GameMgr;

#endif // _AGENTPLAYER_H_INCLUDED_
