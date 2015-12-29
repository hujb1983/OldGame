#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void get_user_called_license_info( int _seatid, int _main_seat, char * _szCalledInfo, int _size )
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

void server_to_user_calledlicense(int _userkey, int _battleid, int _seatid, int _multiple, int _dmodel, int _times,  char * _calledinfo )
{
    char _buff[1024]   = {0};
    char _format[256] = "{\"protocol\":\"%d\","
                        " \"data\":{"
                            "\"battleid\":%d,"
                            "\"seatid\":%d,"
                            "\"multiple\":%d,"
                            "\"dmodel\":%s,"
                            "\"times\":%d,"
                            "\"callinfo\":[%s]"
                            "}}";

    snprintf( _buff, sizeof(_buff), _format,
             MAKEDWORD( Games_Protocol, CalledLicense_BRD ),
             _battleid,
             _seatid,
             _multiple,
             (_dmodel==0)?"false":"true",
             _times,
             _calledinfo );

    WORD wLen = strlen( _buff );
    g_AgentServer->SendToClient( _userkey, (BYTE*)_buff, wLen );

    DEBUG_MSG( LVL_DEBUG, "CalledLicense_BRD to client: %s \n", _buff );
}

void MSG_Handler_CalledLicense_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "CalledLicense_BRD to recv: %s \n", (char *) pMsg  );

    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0);
    int _seatid(0), _multiple(0), _times(0), _battleid(0), _dmodel(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "dmodel",    _dmodel   );
    js_map.ReadInteger( "multiple",  _multiple );
    js_map.ReadInteger( "times",     _times    );

    char _calledinfo[256] = {0};
    get_user_called_license_info( 0,  _seatid, _calledinfo, sizeof(_calledinfo) );
    strcat( _calledinfo, ",");
    get_user_called_license_info( 1,  _seatid, _calledinfo, sizeof(_calledinfo) );
    strcat( _calledinfo, ",");
    get_user_called_license_info( 2,  _seatid, _calledinfo, sizeof(_calledinfo) );

    if (_userkey1 != 0) {
        server_to_user_calledlicense(_userkey1, _battleid, 0, _multiple, _dmodel, _times, _calledinfo );
    }

    if (_userkey2 != 0) {
        server_to_user_calledlicense(_userkey2, _battleid, 1, _multiple, _dmodel, _times, _calledinfo );
    }

    if (_userkey3 != 0) {
        server_to_user_calledlicense(_userkey3, _battleid, 2, _multiple, _dmodel, _times, _calledinfo );
    }
}

