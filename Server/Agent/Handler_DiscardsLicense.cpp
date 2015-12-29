#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

void get_user_discards_license_info( int _seatid, int _main_seat, char * _szCalledInfo, int _size )
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

void server_to_user_discards_license(int _userkey, int _battleid, int _seatid, int _times, char * _szHeadInfo, char * _discardsinfo )
{
    char _buff[1024]   = {0};
    char _format[256] = "{\"protocol\":\"%d\","
                        " \"data\":{"
                            "\"seatid\":%d,"
                            "\"times\":%d,"
                            "%s,"
                            "\"showcard\":[%s]"
                            "}}";

    snprintf( _buff, sizeof(_buff), _format,
             MAKEDWORD( Games_Protocol, DiscardsLicense_BRD ),
             _seatid,
             _times,
             _szHeadInfo,
             _discardsinfo );

    WORD wLen = strlen( _buff );
    g_AgentServer->SendToClient( _userkey, (BYTE*)_buff, wLen );

    DEBUG_MSG( LVL_DEBUG, "DiscardsLicense_BRD to client: %s \n", _buff );
}

void MSG_Handler_DiscardsLicense_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "DiscardsLicense_BRD to recv: %s \n", (char *) pMsg  );

    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0), _seatid(0);
    int _basics(0),  _multiple(0), _brokerage(0);
    int _times(0), _battleid(0), _dmodel(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "times",     _times    );
    js_map.ReadInteger( "basics",    _basics   );
    js_map.ReadInteger( "multiple",  _multiple );
    js_map.ReadInteger( "brokerage", _brokerage);

    char _discardsinfo[256] = {0};
    get_user_discards_license_info( 0,  _seatid, _discardsinfo, sizeof(_discardsinfo) );
    strcat( _discardsinfo, ",");
    get_user_discards_license_info( 1,  _seatid, _discardsinfo, sizeof(_discardsinfo) );
    strcat( _discardsinfo, ",");
    get_user_discards_license_info( 2,  _seatid, _discardsinfo, sizeof(_discardsinfo) );

    char _szHead[80] = {0};
    snprintf( _szHead, sizeof(_szHead), "\"basics\":%d,\"multiple\":%d,\"brokerage\":%d",
             _basics, _multiple, _brokerage );

    if (_userkey1 != 0) {
        server_to_user_discards_license(_userkey1, _battleid, 0, _times, _szHead, _discardsinfo );
    }

    if (_userkey2 != 0) {
        server_to_user_discards_license(_userkey2, _battleid, 1, _times, _szHead, _discardsinfo );
    }

    if (_userkey3 != 0) {
        server_to_user_discards_license(_userkey3, _battleid, 2, _times, _szHead, _discardsinfo );
    }
}
