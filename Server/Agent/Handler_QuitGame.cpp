#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


// 取得所有用户的信息
void get_user_seatinfo_quitgame( BOOL show, int _userkey, char * _szSeatInfo, int _size)
{
    char _buff[256]   = {0};
    char _format[256] = "{\"show\":false,"
                         "\"ready\":false,"
                         "\"showcards\":false,"
                         "\"name\":\"-\","
                         "\"rate\":\"0\","
                         "\"money\":\"0\"}";
    strcat(_buff, _format);

    /**********************************************
        show 为 False, 取值;
        show 为 True,  不取值;
    **********************************************/
    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession(_userkey );
    if ( pSession ) {
        if ( show==TRUE ) {
            pSession->LeaveGame();
        }
        else if ( show==FALSE ) {
            memset( _buff, 0x0, sizeof(_buff) );
            pSession->getSeatInfo( _buff, sizeof( _buff ) );
        }
    }

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _szSeatInfo, _buff);
    }
}


void server_to_user_quitgame(int _userkey, int _battleid, int _seatid, int _basics, int _brokerage, int _multiple, char * _seatinfo)
{
    char _buff[2048]  = {0};
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

    DEBUG_MSG( LVL_DEBUG, "QuitGame_BRD to send: %s \n", _buff );
}

void MSG_Handler_QuitGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "QuitGame_BRD to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _status(0);
    js_map.ReadInteger( "status",    _status   );   // 状态分强正常请求退出和离线退出;

    /**********************************************************************
        1, 如果玩家正在游戏，那么 Status为不正常，不要通过Lobby清理，
    因为这房间还在继续游戏;
        2, 如果是未开始游戏，那么 Status为1，表是正常退出房间，
    要经过 LobbyServer清理房间号后，通过QuitTable退出;
    ***********************************************************************/
    /* if (_status == 1) {
        g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
        return;
    } */

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
    get_user_seatinfo_quitgame( (_seatid==0), _userkey1,  _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_seatinfo_quitgame( (_seatid==1), _userkey2,  _seatinfo, sizeof(_seatinfo) );
    strcat( _seatinfo, ",");
    get_user_seatinfo_quitgame( (_seatid==2), _userkey3,  _seatinfo, sizeof(_seatinfo) );

    if (_userkey1!=0 && (_seatid!=0) )  {
        server_to_user_quitgame( _userkey1, _battleid, 0, _basics, _brokerage, _multiple, _seatinfo);
    }

    if (_userkey2!=0 && (_seatid!=1)) {
        server_to_user_quitgame( _userkey2, _battleid, 1, _basics, _brokerage, _multiple, _seatinfo);
    }

    if (_userkey3!=0 && (_seatid!=2)) {
        server_to_user_quitgame( _userkey3, _battleid, 2, _basics, _brokerage, _multiple, _seatinfo);
    }
}

