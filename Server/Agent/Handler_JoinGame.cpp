#include "Handler_Module.h"
#include "AgentServer.h"
#include "UserSession.h"


// 取得所有用户的信息
void get_user_seatinfo(int _userkey, char * _szSeatInfo, int _size)
{
    char _buff[256]   = {0};

    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession(_userkey );
    if ( pSession ) {
        pSession->getSeatInfo( _buff, sizeof( _buff ) );
    }
    else {
        char _format[256] = "{\"show\":\"false\","
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
void user_auto_startGame(ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
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
    char format[256] = "{\"Protocol\":\"%d\","
                       "\"userkey\":\"%d\","
                       "\"seatid\":\"%d\","
                       "\"battleid\":\"%d\"}";

    snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, StartGame_REQ ),
            _userkey, _seatid, _battleid );

    WORD nLen = strlen( buff );
    g_AgentServer->SendToGameServer( (BYTE*)buff, nLen );

    UserSession * pSession = g_AgentServer->GetUserSession( (WORD)_userkey );
    if ( pSession ) {
        pSession->SetBattleKey( (WORD)_battleid );
    }
}


void MSG_Handler_JoinGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "MSG_Handler_JoinGame_BRD \n" );

    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0);
    int _battleid(0), _basics(0),   _brokerage(0), _seatid(0), _userid(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "basics",    _basics   );
    js_map.ReadInteger( "brokerage", _brokerage);
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "userid",    _userid   );

    char _seatinfo[1024] = {0};
    get_user_seatinfo( _userkey1,  _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_seatinfo( _userkey2,  _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_seatinfo( _userkey3,  _seatinfo, sizeof(_seatinfo) );

    {
        char _buff[2048]   = {0};
        char _format[256] = "{\"protocol\":\"%d\","
                              " \"data\":[{"
                                    "\"battleid\":\"%d\","
                                    "\"basics\":\"%d\","
                                    "\"brokerage\":\"%d\","
                                    "\"seatinfo\":[%s]"
                                    "}]}";

        snprintf( _buff, sizeof(_buff), _format,
                 MAKEDWORD( Games_Protocol, JoinGame_BRD ),
                 _battleid, _basics, _brokerage,
                 _seatinfo );

        WORD wLen = strlen( _buff );
        if (_userkey1 != 0) {
            g_AgentServer->SendToClient( _userkey1, (BYTE*)_buff, wLen );
        }

        if (_userkey2 != 0) {
            g_AgentServer->SendToClient( _userkey2, (BYTE*)_buff, wLen );
        }

        if (_userkey3 != 0) {
            g_AgentServer->SendToClient( _userkey3, (BYTE*)_buff, wLen );
        }
    }
}


// -- 登录报错
void MSG_Handler_JoinGame_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_AgentServer->SendToClient( (BYTE*)pMsg, wSize );
}



