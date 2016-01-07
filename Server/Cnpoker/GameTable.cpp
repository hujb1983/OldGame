#include "GameTable.h"

/**********************************************
    游戏桌子
**********************************************/
GameTable::GameTable()
{
    m_prev = NULL;
    m_next = NULL;
}

GameTable::~GameTable()
{

}

GameTable * GameTable::set_prev( GameTable * _prev ) {
    GameTable * t = m_prev;
    m_prev = _prev;
    return t;
}

GameTable * GameTable::set_next( GameTable * _next ) {
    GameTable * t = m_next;
    m_next = _next;
    return t;
}

bool GameTable::is_null_prev() {
    return (m_prev != NULL);
}

bool GameTable::is_null_next() {
    return (m_next != NULL);
}

GameTable * GameTable::get_prev() {
    return (m_prev);
}

GameTable * GameTable::get_next() {
    return (m_next);
}

UINT & GameTable::GetIndex()        {
    return (m_uiIndex);
}
TablePacket & GameTable::GetTablePacket()  {
    return (m_cPack);
}


/**********************************************
    桌子队列
**********************************************/
GameTableList::GameTableList() {
}

GameTableList::~GameTableList() {
}

void GameTableList::clear()
{
    m_size = 0;
    m_head = NULL;
    m_last = NULL;
}

void GameTableList::push_back( GameTable * pPoker )
{
    if ( !m_head ) {
        m_head = pPoker;
        m_head->set_prev(NULL);
        m_head->set_next(NULL);
        m_last = NULL;
        ++m_size;
    }
    else if ( !m_last ){
        m_head->set_next( pPoker );
        pPoker->set_prev( m_head );
        pPoker->set_next( NULL );
        m_last = pPoker;
    }
    else if ( m_last ){
        m_last->set_next( pPoker );
        pPoker->set_prev( m_last );
        pPoker->set_next( NULL );
        m_last = pPoker;
    }
}

void GameTableList::remove(GameTable * pPoker)
{
    GameTable * _prev = pPoker->get_prev();
    GameTable * _next = pPoker->get_next();
    if ( _prev ) {
        _prev->set_next( _next );
    }
    if ( _next ) {
        _next->set_prev( _prev );
    }
    --m_size;
}

GameTable * GameTableList::get_frist() {
    return m_head;
}

GameTable * GameTableList::pop_frist() {
    GameTable * _prev = m_head;
    GameTable * _next = NULL;
    if ( m_head ) {
        _next = m_head->get_next();
        if ( _next ) {
            _next->set_prev(NULL);
        }
        m_head = _next;
        _prev->set_prev( NULL );
        _prev->set_next( NULL );
    }
    return _prev;
}
