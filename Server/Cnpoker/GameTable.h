#ifndef _GamePlyer_H_INCLUDED_
#define _GamePlyer_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "UserPacket.h"
#include "TablePacket.h"


class GameTable
{
public:
	GameTable();
	~GameTable();

private:
    WORD m_wIndex;                              // 复本下标;

public:
    GameTable * set_prev( GameTable * _prev );
    GameTable * set_next( GameTable * _next );

    bool is_null_prev();
    bool is_null_next();

    GameTable * get_prev();
    GameTable * get_next();

public:
    GameTable * m_prev;
    GameTable * m_next;

    UINT & GetIndex();
    TablePacket & GetTablePacket();

private:
    UINT m_uiIndex; // 标识号
    TablePacket m_cPack; // 数据包

};


/**********************************************
    游戏桌子队列对象
**********************************************/
class GameTableList
{
public:
	GameTableList();
	~GameTableList();

public:

    void clear();
    void push_back(GameTable * pUser);
    void remove(GameTable * pUser);
    WORD size() { return m_size; }

    GameTable * get_frist();
    GameTable * pop_frist();

private:
    WORD m_size;
    GameTable * m_head;
    GameTable * m_last;
};


#endif // _AGENTPLAYER_H_INCLUDED_
