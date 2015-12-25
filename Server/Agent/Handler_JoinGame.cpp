#include "Handler_Module.h"
#include "AgentServer.h"
#include "UserSession.h"


// 取得所有用户的信息
void get_user_seatinfo_joingame(int _userkey, int _seatid, char * _szSeatInfo, int _size)
{
    char _buff[256]   = {0};

    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession(_userkey );
    if ( pSession ) {
        pSession->SetSeatKey( _seatid );
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


// 自动开始游戏
void user_auto_startgame(ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey  = 0;
    int _battleid = 0;
    int _seatid   = 0;

    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid   );

    char buff[256]   = {0};
    char format[256] = "{\"protocol\":\"%d\","
                       "\"userkey\":\"%d\","
                       "\"seatid\":\"%d\","
                       "\"battleid\":\"%d\"}";

    snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, StartGame_REQ ),
            _userkey, _seatid, _battleid );

    WORD nLen = strlen( buff );
    g_AgentServer->SendToGameServer( (BYTE*)buff, nLen );

    UserSession * pSession = g_AgentServer->GetUserSession( (WORD)_userkey );
    if ( pSession ) {
        DEBUG_MSG( LVL_DEBUG, "JoinGame_REQ to SetBattleKey: %d \n", _battleid);
        pSession->SetBattleKey( (WORD)_battleid );
    }
}

void server_to_user_joingame(int _userkey, int _battleid, int _seatid, int _basics, int _brokerage, int _multiple, char * _seatinfo)
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

    DEBUG_MSG( LVL_DEBUG, "JoinGame_BRD to send: %s \n",  _buff );
}

void MSG_Handler_JoinGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "JoinGame_BRD to recv: %s \n",  (char*)pMsg );

    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0);
    int _battleid(0), _basics(0),   _brokerage(0), _multiple(0), _seatid(0), _userid(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "basics",    _basics   );
    js_map.ReadInteger( "multiple",  _multiple );
    js_map.ReadInteger( "brokerage", _brokerage);
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "userid",    _userid   );

    char _seatinfo[1024] = {0};
    get_user_seatinfo_joingame( _userkey1, 0, _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_seatinfo_joingame( _userkey2, 1, _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_seatinfo_joingame( _userkey3, 3, _seatinfo, sizeof(_seatinfo) );

    if (_userkey1 != 0) {
        server_to_user_joingame( _userkey1, _battleid, 0, _basics, _brokerage, _multiple, _seatinfo);
    }

    if (_userkey2 != 0) {
        server_to_user_joingame( _userkey2, _battleid, 1, _basics, _brokerage, _multiple, _seatinfo);
    }

    if (_userkey3 != 0) {
        server_to_user_joingame( _userkey3, _battleid, 2, _basics, _brokerage, _multiple, _seatinfo);
    }

}

// -- 登录报错
void MSG_Handler_JoinGame_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToClient( (BYTE*)pMsg, wSize );
}



