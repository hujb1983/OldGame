#include "Handler_Module.h"
#include "CnpokerServer.h"


void MSG_Handler_QuitGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "QuitGame_REQ to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _seatid(0), _battleid(0);
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid );

    GameBattle * pBattle = NULL;
    pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return;
    }

    /**********************************************************************
        1, 如果桌子未开始游戏，返回FALSE;
        2, 如果桌子已开始游戏，返回TRUE，设置为托管状态;
    ***********************************************************************/
    int _userid = pBattle->getID( _seatid );
    if ( pBattle->getBattleStatus()==eGB_SIT ) {
        char _buff[256]   = {0};
        char _format[256] = "{\"protocol\":\"%d\","
                            "\"battleid\":%d,"
                            "\"seatid\":%d,"
                            "\"userid\":%d}";

        snprintf( _buff, sizeof(_buff), _format,
                  MAKEDWORD( Games_Protocol, QuitGame_REQ ),
                  _battleid, _seatid, _userid );

        WORD nLen = strlen( _buff );
        g_pCnpokerServer->SendToDBServer( (BYTE*)_buff, nLen );

        DEBUG_MSG( LVL_DEBUG, "QuitGame_REQ to db: %s \n", (char*)_buff );
        return;
    }

    // 设置为托管状态
    {
        int _userid    = pBattle->getID ( _seatid );
        int _userkey   = pBattle->getKey( _seatid );
        int _multiple  = pBattle->getMultiple();
        int _mmin      = pBattle->getMinMoney();
        int _brokerage = pBattle->getBrokerage();

        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char _buff[256]   = {0};
        char _format[256] = "{\"protocol\":\"%d\","
                            "%s,"
                            "\"userkey\":%d,"
                            "\"seatid\":%d,"
                            "\"status\":0,"
                            "\"battleid\":%d,"
                            "\"multiple\":%d,"
                            "\"basics\":%d,"
                            "\"brokerage\":%d}";

        snprintf( _buff, sizeof(_buff), _format,
                  MAKEDWORD( Games_Protocol, QuitGame_BRD ),
                  szPlayerkey, _userkey, _seatid, _battleid,
                  _multiple, _mmin, _brokerage);

        WORD nLen = strlen( _buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)_buff, nLen );

        pBattle->QuitBattle( _seatid );
        DEBUG_MSG( LVL_DEBUG, "QuitGame_REQ to agent: %s \n", (char*)_buff );
    }
}

void MSG_Handler_QuitGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "QuitGame_BRD to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _seatid(0), _battleid(0);
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid );

    GameBattle * pBattle = NULL;
    pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return;
    }

    {
        int _userid    = pBattle->getID ( _seatid );
        int _userkey   = pBattle->getKey( _seatid );
        int _multiple  = pBattle->getMultiple();
        int _mmin      = pBattle->getMinMoney();
        int _brokerage = pBattle->getBrokerage();

        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char _buff[256]   = {0};
        char _format[256] = "{\"protocol\":\"%d\","
                            "%s,"
                            "\"userkey\":%d,"
                            "\"seatid\":%d,"
                            "\"status\":0,"
                            "\"battleid\":%d,"
                            "\"multiple\":%d,"
                            "\"basics\":%d,"
                            "\"brokerage\":%d}";

        snprintf( _buff, sizeof(_buff), _format,
                  MAKEDWORD( Games_Protocol, QuitGame_BRD ),
                  szPlayerkey, _userkey, _seatid, _battleid,
                  _multiple, _mmin, _brokerage);

        WORD nLen = strlen( _buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)_buff, nLen );

        pBattle->QuitBattle( _seatid );
        DEBUG_MSG( LVL_DEBUG, "[QuitGame_BRD to agent: %s \n", (char*) _buff );
    }
}

