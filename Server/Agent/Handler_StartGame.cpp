#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

// 取得在游戏的用户信息
void get_user_startgame(int _userkey, char * _szSeatInfo, int _size)
{
    char _buff[256]   = {0};

    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession(_userkey );
    if ( pSession ) {
        pSession->getSeatInfo( _buff, sizeof( _buff ) );
    }
    else {
        char _format[256] = "{\"show\":false,"
                             "\"ready\":false,"
                             "\"showcards\":false,"
                             "\"name\":\"-\","
                             "\"rate\":\"0\","
                             "\"money\":\"0\"}";
        strcat(_buff, _format);
    }

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _szSeatInfo, _buff);
    }
}

void user_auto_initcards(ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _ready(0), _battleid(0);
    js_map.ReadInteger( "ready",    _ready    );
    js_map.ReadInteger( "battleid", _battleid );

    if ( _ready == 1) {
        char szMsg[1024] = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                             "\"battleid\":\"%d\" }" ;

        sprintf( szMsg, format,
                MAKEDWORD( Games_Protocol, InitCards_REQ ),
                _battleid );
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( szMsg );

        DEBUG_MSG( LVL_DEBUG, "InitCards_REQ to game : %s. \n", szMsg );
        g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );
    }
}

void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {
    DEBUG_MSG( LVL_DEBUG, "StartGame_REQ to recv: %s. \n",  (char*)pMsg );
    g_AgentServer->SendToGameServer( (BYTE*) pMsg, wSize );
}

void set_user_is_startgame( int _userkey ) {
    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession( _userkey );
    if ( pSession ) {
        pSession->setReady( TRUE );
    }
}

void server_to_user_startgame(int _userkey, int _battleid, int _seatid, int _basics, int _brokerage, int _multiple, char * _seatinfo)
{
    char _buff[2048]   = {0};
    char _format[256] = "{\"protocol\":\"%d\","
                          " \"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"basics\":\"%d\","
                                "\"multiple\":\"%d\","
                                "\"brokerage\":\"%d\","
                                "\"seatinfo\":[%s]"
                                "}]}";

    snprintf( _buff, sizeof(_buff), _format,
             MAKEDWORD( Games_Protocol, JoinGame_BRD ),
             _battleid, _seatid, _basics, _multiple, _brokerage,
             _seatinfo );

    WORD wLen = strlen( _buff );
    g_AgentServer->SendToClient( _userkey, (BYTE*)_buff, wLen );

    DEBUG_MSG( LVL_DEBUG, "StartGame_BRD to client: %d %s. \n", wLen, _buff );
}

void MSG_Handler_StartGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "StartGame_BRD to recv : %s. \n",  (char*)pMsg );

    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0), _userkey;
    int _battleid(0), _basics(0),   _brokerage(0), _multiple(0), _seatid(0), _userid(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "userkey",   _userkey );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "basics",    _basics   );
    js_map.ReadInteger( "multiple",  _multiple );
    js_map.ReadInteger( "brokerage", _brokerage);
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "userid",    _userid   );

    char _seatinfo[1024] = {0};
    set_user_is_startgame(_userkey);

    get_user_startgame( _userkey1,  _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_startgame( _userkey2,  _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_startgame( _userkey3,  _seatinfo, sizeof(_seatinfo) );

    if (_userkey1 != 0) {
        server_to_user_startgame( _userkey1, _battleid, 0, _basics, _brokerage, _multiple, _seatinfo);
    }

    if (_userkey2 != 0) {
        server_to_user_startgame( _userkey2, _battleid, 1, _basics, _brokerage, _multiple, _seatinfo);
    }

    if (_userkey3 != 0) {
        server_to_user_startgame( _userkey3, _battleid, 2, _basics, _brokerage, _multiple, _seatinfo);
    }

    // 自动发牌
    user_auto_initcards(pServerSession, pMsg, wSize);
}

