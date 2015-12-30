#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_Discards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "Discards_REQ to recv: %s \n", (char *) pMsg  );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _battleid(0), _seatid(0), _count(0);
    char _poker[256] = {0};

    UserSession * pSession = (UserSession*) pServerSession;
    assert( pSession );
    _battleid = pSession->GetBattleKey();
    _seatid   = pSession->GetSeatKey();

    js_map.ReadInteger( "count",  _count );
    js_map.ReadString ( "poker",  _poker, sizeof(_poker) );

    char _buff[1024] = {0};
    char _format[256] = "{\"protocol\":\"%d\",\"battleid\":%d,\"seatid\":%d,\"count\":%d,\"poker\":\"%s\" }" ;
    sprintf( _buff, _format, MAKEDWORD( Games_Protocol, Discards_REQ ), _battleid, _seatid, _count, _poker );

    WORD _wLen = strlen( _buff );
    g_AgentServer->SendToGameServer( (BYTE*) _buff, _wLen );
}

// 打牌授权
void game_second_send_discards_license( int _battleid )
{
    char szMsg[1024] = {0};
    char format[256] = 	"{\"protocol\":\"%d\", \"battleid\":\"%d\" }" ;
    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, DiscardsLicense_REQ ), _battleid );

    WORD nLen = strlen( szMsg );
    g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );

    DEBUG_MSG( LVL_DEBUG,  "DiscardsLicense_REQ to game: %s \n",  (char*)szMsg );
}

// 叫地主授权
void get_user_discards_info( int _userkey, int _sendkey, char * _szPokerInfo, int _size )
{
    char _buff[256]   = {0};

    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession( _userkey );
    if ( pSession ) {
        pSession->getPokerInfo( _sendkey, _buff, sizeof( _buff ) );
    }
    else {
        char _format[256] = "{\"show\":false, \"name\":\"-\", \"count\":17, \"poker\":[-1]}";
        strcat(_buff, _format);
    }

    int szLen = strlen( _buff );
    if ( szLen < _size ) {
        strcat( _szPokerInfo, _buff);
    }
}

void server_to_user_discards(int _userkey, int _seatid, char * _szHead, ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );

    // 打印需要创建地主的数据
    char _szCreatebank[256] = {0};
    get_user_discards_info( _userkey1, _userkey, _szCreatebank, sizeof(_szCreatebank) );
    strcat( _szCreatebank, ",");
    get_user_discards_info( _userkey2, _userkey, _szCreatebank, sizeof(_szCreatebank) );
    strcat( _szCreatebank, ",");
    get_user_discards_info( _userkey3, _userkey, _szCreatebank, sizeof(_szCreatebank) );

    char _buff[1024]   = {0};
    char _format[256] = "{\"protocol\":\"%d\", \"data\":{ \"seatid\":%d, %s, \"bankerinfo\":[%s] }}";
    snprintf( _buff, sizeof(_buff), _format, MAKEDWORD( Games_Protocol, CreateBank_BRD ),
              _seatid, _szHead, _szCreatebank );

    WORD wLen = strlen( _buff );
    g_AgentServer->SendToClient( _userkey, (BYTE*)_buff, wLen );

    DEBUG_MSG( LVL_DEBUG, "CreateBank_BRD to client: %s \n", _buff );
}


void MSG_Handler_Discards_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "Called_BRD to recv: %s \n", (char *) pMsg  );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0), _seatid(0);
    int _battleid(0), _basics(0), _multiple(0), _dmodel(0);
    int _count(0), _count0(0), _status(0);
    char _poker[128] = {0};
    char _poker0[128] = {0};
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "seatid",    _seatid   );
    js_map.ReadInteger( "status",    _status   );
    js_map.ReadInteger( "basics",    _basics   );
    js_map.ReadInteger( "multiple",  _multiple );
    js_map.ReadInteger( "dmodel",    _dmodel   );
    js_map.ReadInteger( "count",     _count    );
    js_map.ReadString ( "poker",     _poker,  sizeof(_poker) );
    js_map.ReadInteger( "count0",    _count0    );
    js_map.ReadString ( "poker0",    _poker0, sizeof(_poker0) );

    // 获得头数据
    char _szBuff[256]   = {0};
    char _szHead [128] = " \"battleid\":%d, \"multiple\":%d, \"dmodel\":%s, \"count\":%d, \"poker\":[%s]";
    snprintf( _szBuff, sizeof(_szBuff), _szHead,  _battleid,  _multiple, (_dmodel==0)?"false":"true", _count, _poker );

    if (_userkey1 != 0) {
        server_to_user_discards( _userkey1, 0, _szBuff, pServerSession, pMsg, wSize );
    }

    if (_userkey2 != 0) {
        server_to_user_discards( _userkey2, 1, _szBuff, pServerSession, pMsg, wSize );
    }

    if (_userkey3 != 0) {
        server_to_user_discards( _userkey3, 2, _szBuff, pServerSession, pMsg, wSize );
    }

    if ( _status==1 ) {
        game_second_send_discards_license( _battleid );
    }
}


