#ifndef _GamePoker_H_INCLUDED_
#define _GamePoker_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "GameBattle.h"

struct PokerObject
{
    BYTE byType;
    BYTE bySize;
    BYTE byThanCount;
    BYTE byThanValue;
};

class GamePoker
{
public:
    GamePoker();
    virtual ~GamePoker();

public:

    void SetPoker( BYTE * _poker, BYTE _size );

    // Step1, ���������ϵ���;
    void Parser( BYTE * _poker, BYTE _size );

    // Step2, �ж��Ƿ��д��������ϵ���;
    BOOL MoreThan( BYTE _type, BYTE _size, BYTE _value, BYTE _count );

    // Step3, ƥ��һ��������;
    BOOL MarchObject( BYTE & _type, BYTE & _size, BYTE & _than_value, BYTE & _than_count);

    // Step4, �ҳ�һ��������;
    BOOL FindObjectMoreThanToClone( BYTE _type, BYTE _size, BYTE _than_value, BYTE _than_count);

    // ��ȡ��¡�Ķ���;
    BOOL ClonePoker( BYTE * _poker, BYTE & _size );

    // ��ȡ��¡�����в���;
    void ClonePokerType(BYTE & _type, BYTE & _size, BYTE & _value, BYTE & _count);

private:
    void Zero();                         // ȫ������;
    BOOL MainSearch(BYTE _size,
                    BYTE &_value,
                    BYTE _count);        // ����;
    BOOL AttachSearch(BYTE _size,
                      BYTE _value,
                      BYTE _count);      // ����;
    short Transform(BYTE _value);        // ת��;

private:
    BYTE  PokerSize;                     // ������;
    BYTE  PokerTypeTable[MAX_PK_TYPE];   // 0~15 ���±����ͱ�;
    BYTE  PokerIndexArray[MAX_BHOLD];    // 0~54 ���±���б�;
    BYTE  PokerIndexTable[MAX_POKER];    // 0~54 ���±����ͱ�;
    BYTE  PokerCloneSize;                // ��¡����;
    BYTE  PokerCloneArray[MAX_BHOLD];    // ��¡�������±�����;
    BYTE  PokerCloneType;                // ��¡����;
    BYTE  PokerCloneValue;               // ��¡����;
    BYTE  PokerCloneCount;               // ��¡����;

private:

    // Step5, ��ϵĲ�����;
    short Poker_1g_size;
    short Poker_1g_max_value;
    short Poker_1g_min_value;
    short Poker_1g_seq_size;
    short Poker_1g_seq_max_value;
    short Poker_1g_seq_min_value;
    short Poker_2g_size;
    short Poker_2g_max_value;
    short Poker_2g_min_value;
    short Poker_2g_seq_size;
    short Poker_2g_seq_max_value;
    short Poker_2g_seq_min_value;
    short Poker_3g_size;
    short Poker_3g_max_value;
    short Poker_3g_min_value;
    short Poker_3g_seq_size;
    short Poker_3g_seq_max_value;
    short Poker_3g_seq_min_value;
    short Poker_4g_size;
    short Poker_4g_max_value;
    short Poker_4g_min_value;
    short Poker_king_size;
    short Poker_king_max_value;
    short Poker_king_min_value;
};

#endif // ...
