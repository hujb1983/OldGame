#include "Handler_Module.h"
#include "AgentServer.h"


void save_to_user_session( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize  )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey0(0), _userkey1(0), _userkey2(0);
    int _count0(0), _count1(0), _count2(0);
    char _poker0[256]  = {0};
    char _poker1[256]  = {0};
    char _poker2[256]  = {0};
    char format[256] = 	"{\"protocol\":\"%d\","
                        " %s,"
                        " \"battleid\":\"%d\","
                        " \"count\":\"%d\","
                        " \"poker\":\"%s\","
                        " \"count0\":\"%d\","
                        " \"poker0\":\"%s\","
                        " \"count1\":\"%d\","
                        " \"poker1\":\"%s\","
                        " \"count2\":\"%d\","
                        " \"poker2\":\"%s\" }";

    js_map.ReadInteger( "userkey1",  _userkey0 );
    js_map.ReadInteger( "userkey2",  _userkey1 );
    js_map.ReadInteger( "userkey3",  _userkey2 );
    js_map.ReadInteger( "count0",    _count0 );
    js_map.ReadString ( "poker0",    _poker0, sizeof(_poker0) );
    js_map.ReadInteger( "count1",    _count1 );
    js_map.ReadString ( "poker1",    _poker1, sizeof(_poker1) );
    js_map.ReadInteger( "count2",    _count2 );
    js_map.ReadString ( "poker2",    _poker2, sizeof(_poker2) );

    UserSession * pSession = NULL;
    pSession = g_AgentServer->GetUserSession( _userkey0 );
    if ( pSession ) {
        pSession->getStatus() = eGB_DEALING;
        pSession->getPokerSize() = _count0;
        pSession->setPokers( _poker0, sizeof( _poker0 ) );
    }

    pSession = g_AgentServer->GetUserSession( _userkey1 );
    if ( pSession ) {
        pSession->getStatus() = eGB_DEALING;
        pSession->getPokerSize() = _count1;
        pSession->setPokers( _poker1, sizeof( _poker1 ) );
    }

    pSession = g_AgentServer->GetUserSession( _userkey2 );
    if ( pSession ) {
        pSession->getStatus() = eGB_DEALING;
        pSession->getPokerSize() = _count2;
        pSession->setPokers( _poker2, sizeof( _poker2 ) );
    }
}

void get_user_pokerinfo( int _userkey, int _sendkey, char * _szPokerInfo, int _size )
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

void send_to_user_pokerinfo( int _seatid, int _userkey, ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0);
    int _battleid(0);
    int _count(0);
    char _poker[256]   = {0};
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );
    js_map.ReadInteger( "battleid",  _battleid );
    js_map.ReadInteger( "count",    _count );
    js_map.ReadString ( "poker",    _poker, sizeof(_poker) );

    char _pokerinfo[1024] = {0};
    get_user_pokerinfo( _userkey1, _userkey, _pokerinfo, sizeof(_pokerinfo) );
    strcat( _pokerinfo, ",");
    get_user_pokerinfo( _userkey2, _userkey, _pokerinfo, sizeof(_pokerinfo) );
    strcat( _pokerinfo, ",");
    get_user_pokerinfo( _userkey3, _userkey, _pokerinfo, sizeof(_pokerinfo) );

    {
        char _buff[2048]   = {0};
        char _format[256] = "{\"protocol\":\"%d\","
             " \"data\":{ \"battleid\":\"%d\", \"count\":\"%d\", \"poker\":[%s], \"seatid\":\"%d\","
             "\"pokerinfo\":[%s] }}";
        snprintf( _buff, sizeof(_buff), _format,
                 MAKEDWORD( Games_Protocol, InitCards_BRD ),
                 _battleid, _count, _poker, _seatid, _pokerinfo );

        WORD wLen = strlen( _buff );
        g_AgentServer->SendToClient( _userkey, (BYTE*)_buff, wLen );

        DEBUG_MSG( LVL_DEBUG,  "InitCards_BRD to client: %s \n",  (char*)_buff );
    }
}

void initcards_send_to_client( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize  )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1(0), _userkey2(0), _userkey3(0);
    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );

    if ( _userkey1!=0 ) {
        send_to_user_pokerinfo( 0, _userkey1, pServerSession, pMsg, wSize );
    }
    if ( _userkey2!=0 ) {
        send_to_user_pokerinfo( 1, _userkey2, pServerSession, pMsg, wSize );
    }
    if ( _userkey3!=0 ) {
        send_to_user_pokerinfo( 2, _userkey3, pServerSession, pMsg, wSize );
    }
}

void game_send_called_license(ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    // 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _battleid(0);
    js_map.ReadInteger( "battleid", _battleid );

    char szMsg[1024] = {0};
    char format[256] = 	"{\"protocol\":\"%d\", \"battleid\":\"%d\" }" ;
    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, CalledLicense_REQ ), _battleid );

    // }}}@ 组合所有的牌
    WORD nLen = strlen( szMsg );
    g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );

    DEBUG_MSG( LVL_DEBUG,  "InitCards_BRD to game: %s \n",  (char*)szMsg );
}

void MSG_Handler_InitCards_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG,  "InitCards_BRD to recv: %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    save_to_user_session( pServerSession, pMsg, wSize );
    initcards_send_to_client( pServerSession, pMsg, wSize );
    game_send_called_license( pServerSession, pMsg, wSize );
}
