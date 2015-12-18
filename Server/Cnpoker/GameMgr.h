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
    /*  1. �ӹ������ʼ��һ�����������ݵ��յ��������棻
        2. Ȼ��ŵ��ն������� */
    int  InitBattles(int _resize = 1);                       // ��ʼ����������;

    /*  1. ����һ���µ� GameTable��*/
    GameBattle * AllocBattle();

    /*  1. ͨ���ҵ�����ID ����һ�� GameTable�� */
    GameBattle * GetBattle( int t_idx );

    /*  1. �����������Ӳ����� */
    void  FreeBattles(GameBattle * pBattle);

private:   // ����
    WORD             m_wBattleSize;                  // ���ӵĴ�С;
    GameBattle *     m_pAllBattles[MC_BATTLES];      // �������ӵ�����;
    GameBattleList   m_pEmptyBattles;                // �յ����Ӷ���;

public:
    BOOL UpdateTimes( DWORD dwTicket );                           // ����;
    void push_CalledBattles( GameBattle * pBattle );      // ���ý��Ƹ���;
    void push_DiscardsBattles( GameBattle * pBattle );    // ���ó��Ƹ���;

private:   // ����
    GameBattleList   m_pCalledBattle;                // ���Ƹ���;
    GameBattleList   m_pDiscardsBattle;              // ���Ƹ���;
};

extern GameMgr g_GameMgr;

#endif // _AGENTPLAYER_H_INCLUDED_
