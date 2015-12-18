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

    // Step1, 分析出手上的牌;
    void Parser( BYTE * _poker, BYTE _size );

    // Step2, 判断是否有大于他手上的牌;
    BOOL MoreThan( BYTE _type, BYTE _size, BYTE _value, BYTE _count );

    // Step3, 匹配一个最佳组合;
    BOOL MarchObject( BYTE & _type, BYTE & _size, BYTE & _than_value, BYTE & _than_count);

    // Step4, 找出一个类型牌;
    BOOL FindObjectMoreThanToClone( BYTE _type, BYTE _size, BYTE _than_value, BYTE _than_count);

    // 获取克隆的对象;
    BOOL ClonePoker( BYTE * _poker, BYTE & _size );

    // 获取克隆的所有参数;
    void ClonePokerType(BYTE & _type, BYTE & _size, BYTE & _value, BYTE & _count);

private:
    void Zero();                         // 全部清零;
    BOOL MainSearch(BYTE _size,
                    BYTE &_value,
                    BYTE _count);        // 查找;
    BOOL AttachSearch(BYTE _size,
                      BYTE _value,
                      BYTE _count);      // 查找;
    short Transform(BYTE _value);        // 转化;

private:
    BYTE  PokerSize;                     // 牌总数;
    BYTE  PokerTypeTable[MAX_PK_TYPE];   // 0~15 的下标类型表;
    BYTE  PokerIndexArray[MAX_BHOLD];    // 0~54 的下标队列表;
    BYTE  PokerIndexTable[MAX_POKER];    // 0~54 的下标类型表;
    BYTE  PokerCloneSize;                // 克隆总数;
    BYTE  PokerCloneArray[MAX_BHOLD];    // 克隆出来的下标类型;
    BYTE  PokerCloneType;                // 克隆总数;
    BYTE  PokerCloneValue;               // 克隆总数;
    BYTE  PokerCloneCount;               // 克隆总数;

private:

    // Step5, 组合的参数表;
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
