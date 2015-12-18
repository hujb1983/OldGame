#include "Handler_Module.h"
#include "CnpokerServer.h"

void MSG_Handler_JoinGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_JoinGame_REQ] pMsg = %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userid(0), _userkey(0), _status(0), _agentkey(0);
    int _battleid(0), _roomid(0), _tableid(0), _seatid(0);

    js_map.ReadInteger( "userid",   _userid   );
    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "status",   _status   );
    js_map.ReadInteger( "agentkey", _agentkey );
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "roomid",   _roomid   );
    js_map.ReadInteger( "tableid",  _tableid  );
    js_map.ReadInteger( "seatid",   _seatid   );

    if ( _status == 0)
    {
        GameBattle * pBattle = g_GameMgr.GetBattle( _battleid );
        if ( pBattle ) {
            pBattle->SetID ( _seatid, _userid  );
            pBattle->SetKey( _seatid, _userkey );
            pBattle->SetBattleStatus( eGB_SIT );    // 设置桌子状态
            pBattle->SetStartSeat( _seatid );       // 设置开始玩家;
            pBattle->SetOnline( _seatid, 0 );       // 开始时都为在线;
        }
    }
    else
    {
        GameBattle * pBattle = g_GameMgr.AllocBattle();
        if ( pBattle ) {
            _seatid = 0;
            _battleid = pBattle->getIndex();
            pBattle->SetID ( _seatid,  _userid  );
            pBattle->SetKey( _seatid,  _userkey );
        }
    }

    char buff[256]   = {0};
    char format[256] = 	   "{\"Protocol\":\"%d\","
                            "\"userid\":\"%d\","
                            "\"userkey\":\"%d\","
                            "\"agentkey\":\"%d\","
                            "\"seatid\":\"%d\","
                            "\"battleid\":\"%d\","
                            "\"roomid\":\"%d\","
                            "\"tableid\":\"%d\" }";

    snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, JoinGame_REQ ),
            _userid, _userkey, _agentkey, _seatid, _battleid, _roomid, _tableid);

    WORD nLen = strlen( buff );
    g_pCnpokerServer->SendToDBServer( (BYTE*)buff, nLen );
}

void MSG_Handler_JoinGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // printf( "[MSG_Handler_JoinGame_BRD] pMsg = %s \n",  (char*)pMsg );
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userid(0) ,_userkey(0), _status(0), _seatid(0), _battleid(0);
    int _mmax(0), _mmin(0), _brokerage(0);

    js_map.ReadInteger( "userid",   _userid   );
    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "status",   _status   );
    js_map.ReadInteger( "seatid",   _seatid   );
    js_map.ReadInteger( "battleid", _battleid );

    if ( _status == 0) {
        js_map.ReadInteger( "mmax",      _mmax );
        js_map.ReadInteger( "mmin",      _mmin );
        js_map.ReadInteger( "brokerage", _brokerage );

        GameBattle * pBattle = g_GameMgr.GetBattle( _battleid );
        if ( pBattle ) {
            pBattle->SetID ( _seatid, _userid  );
            pBattle->SetKey( _seatid, _userkey );

            char szPlayerkey[256] = {0};
            pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

            char buff[256]   = {0};
            char format[256] = 	"{\"protocol\":\"%d\","
                                "%s,"
                                "\"status\":\"%d\","
                                "\"battleid\":\"%d\","
                                "\"basics\":\"%d\","
                                "\"brokerage\":\"%d\"}";

            snprintf( buff, sizeof(buff), format,
                      MAKEDWORD( Games_Protocol, JoinGame_BRD ),
                      szPlayerkey,
                      _status,
                      _battleid,
                      _mmin,
                      _brokerage);

            WORD nLen = strlen( buff );
            g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );
        }
        return;
   }
   else
   {
        // 失败的通知
        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                            "\"userkey\":\"%d\","
                            "\"status\":\"%d\"}";

        snprintf( buff, sizeof(buff), format,
                  MAKEDWORD( Games_Protocol, JoinGame_NAK ),
                  _userkey,
                  _status );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );
   }
}


