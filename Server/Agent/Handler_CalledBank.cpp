#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "Called_REQ to recv: %s \n", (char*) pMsg );

    // 叫地主接收
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _calls(0);
    js_map.ReadInteger( "points",  _calls );

    UserSession * pSession = (UserSession*) pServerSession;
    assert( pSession );

    int _battleid = pSession->GetBattleKey();
    int _seatid   = pSession->GetSeatKey();

    char _buff[1024] = {0};
    char _format[256] = "{\"protocol\":\"%d\",\"calltype\":\"%d\",\"battleid\":\"%d\",\"seatid\":\"%d\" }" ;
    sprintf( _buff, _format, MAKEDWORD( Games_Protocol, Called_REQ ), _calls, _battleid, _seatid );

    WORD _wLen = strlen( _buff );
    g_AgentServer->SendToGameServer( (BYTE*) _buff, _wLen );

    DEBUG_MSG( LVL_DEBUG, "Called_REQ to game: %s \n", _buff );
}

void get_user_called_info( int _seatid, int _main_seat, char * _szCalledInfo, int _size )
{
    char _buff[256]   = {0};

    if ( _seatid == _main_seat ) {
        strcat(_buff, "{\"show\":true}");
    }
    else {
        strcat(_buff, "{\"show\":false}");
    }

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _szCalledInfo, _buff);
    }
}

void server_to_user_called(int _userkey, int _battleid, int _seatid,
                           int _multiple, int _dmodel, int _times,
                           int _calltype, char * _calledinfo )
{
    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession( _userkey );
    if ( pSession ) {
        pSession->getStatus() = eGB_CALLING;
    }

    char _buff[1024]   = {0};
    char _format[256] = "{\"protocol\":\"%d\","
                        " \"data\":{"
                            "\"battleid\":%d,\"seatid\":%d,\"multiple\":%d,\"dmodel\":%s,"
                            "\"times\":%d,\"calltype\":%d,\"callinfo\":[%s]}}";

    snprintf( _buff, sizeof(_buff), _format,
             MAKEDWORD( Games_Protocol, Called_BRD ),
             _battleid, _seatid, _multiple,
             (_dmodel==0)?"false":"true",
             _times, _calltype, _calledinfo );

    WORD wLen = strlen( _buff );
    g_AgentServer->SendToClient( _userkey, (BYTE*)_buff, wLen );

    DEBUG_MSG( LVL_DEBUG, "Called_BRD to client: %s \n", _buff );
}

// 叫地主授权
void game_second_send_called_license( int _battleid )
{
    char szMsg[1024] = {0};
    char format[256] = 	"{\"protocol\":\"%d\", \"battleid\":\"%d\" }" ;
    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, CalledLicense_REQ ), _battleid );

    WORD nLen = strlen( szMsg );
    g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );

    DEBUG_MSG( LVL_DEBUG,  "CalledLicense_REQ to game: %s \n",  (char*)szMsg );
}

// 创建地主
void game_send_create_banker( int _battleid )
{
    char szMsg[1024] = {0};
    char format[256] = 	"{\"protocol\":\"%d\",\"battleid\":\"%d\" }" ;
    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, CreateBank_REQ ), _battleid );

    WORD nLen = strlen( szMsg );
    g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );

    DEBUG_MSG( LVL_DEBUG,  "CreateBanker to game: %s \n",  (char*)szMsg );
}

// 没有人叫地主
void did_hava_called_initcards( int _battleid )
{
    char szMsg[1024] = {0};
    char format[256] = 	"{\"protocol\":\"%d\", \"battleid\":\"%d\" }" ;
    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, InitCards_REQ ),  _battleid );

    WORD nLen = strlen( szMsg );
    g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );

    DEBUG_MSG( LVL_DEBUG, "InitCards_REQ to game : %s. \n", szMsg );
}


void MSG_Handler_CalledBank_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "Called_BRD to recv: %s \n", (char *) pMsg  );

    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0), _seatid(0);
    int _initcard(0), _create(0), _battleid(0);
    int _multiple(0), _dmodel(0), _calltype(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "initcard",  _initcard );
    js_map.ReadInteger( "create",    _create   );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "multiple",  _multiple );
    js_map.ReadInteger( "dmodel",    _dmodel   );
    js_map.ReadInteger( "calltype",  _calltype );
    js_map.ReadInteger( "seatid",    _seatid   );

    char _calledinfo[256] = {0};
    get_user_called_info( 0,  _seatid, _calledinfo, sizeof(_calledinfo) );
    strcat( _calledinfo, ",");
    get_user_called_info( 1,  _seatid, _calledinfo, sizeof(_calledinfo) );
    strcat( _calledinfo, ",");
    get_user_called_info( 2,  _seatid, _calledinfo, sizeof(_calledinfo) );

    if (_userkey1 != 0) {
        server_to_user_called( _userkey1, _battleid, 0, _multiple, _dmodel, -1, _calltype, _calledinfo );
    }

    if (_userkey2 != 0) {
        server_to_user_called( _userkey2, _battleid, 1, _multiple, _dmodel, -1, _calltype, _calledinfo );
    }

    if (_userkey3 != 0) {
        server_to_user_called( _userkey3, _battleid, 2, _multiple, _dmodel, -1, _calltype, _calledinfo );
    }

    // 下一次活动, 是否重新发牌，是否创建地主
    if ( _initcard==1 )
    {
        did_hava_called_initcards( _battleid );
    }
    else
    {
        if ( _create == 1 )
        {
            game_send_create_banker( _battleid );
        }
        else
        {
            game_second_send_called_license( _battleid );
        }
    }
}

