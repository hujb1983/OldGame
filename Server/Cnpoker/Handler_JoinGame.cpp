#include "Handler_Module.h"
#include "CnpokerServer.h"

void MSG_Handler_JoinGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG , "JoinGame_REQ to recv: %s \n",  (char*)pMsg );

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
        DEBUG_MSG( LVL_DEBUG , "GetBattle: %x, %d \n",  pBattle, _battleid );
    }
    else if( _status == -1 )
    {
        GameBattle * pBattle = g_GameMgr.AllocBattle();
        if ( pBattle ) {
            pBattle->InitBattle();     // 创建房间的时候初始化
            _seatid = 0;
            _battleid = pBattle->getIndex();
            pBattle->SetID ( _seatid,  _userid  );      // 初始化 userid;
            pBattle->SetKey( _seatid,  _userkey );      // 初始化 userkey;
            pBattle->SetTable( _roomid, _tableid );     // 初始化房间号;
        }
        DEBUG_MSG( LVL_DEBUG , "AllocBattle %x, %d \n",  pBattle, _battleid );
    }

    {
        char _buff[256]   = {0};
        char _format[256] = "{\"Protocol\":\"%d\","
                            "\"userid\":\"%d\","
                            "\"userkey\":\"%d\","
                            "\"agentkey\":\"%d\","
                            "\"seatid\":\"%d\","
                            "\"battleid\":\"%d\","
                            "\"roomid\":\"%d\","
                            "\"tableid\":\"%d\" }";

        snprintf( _buff, sizeof(_buff), _format, MAKEDWORD( Games_Protocol, JoinGame_REQ ),
                _userid, _userkey, _agentkey, _seatid, _battleid, _roomid, _tableid);

        DEBUG_MSG( LVL_DEBUG , "JoinGame_REQ to db: %s \n", _buff );

        WORD nLen = strlen( _buff );
        g_pCnpokerServer->SendToDBServer( (BYTE*)_buff, nLen );
    }
}

void MSG_Handler_JoinGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG , "JoinGame_REQ to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userid(0) ,_userkey(0), _status(0), _seatid(0), _battleid(0);
    int _mmax(0), _mmin(0), _brokerage(0), _dmodel(0);

    js_map.ReadInteger( "userid",   _userid   );
    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "status",   _status   );
    js_map.ReadInteger( "seatid",   _seatid   );
    js_map.ReadInteger( "battleid", _battleid );

    if ( _status == 0) {
        js_map.ReadInteger( "dmodel",    _dmodel );
        js_map.ReadInteger( "mmax",      _mmax );
        js_map.ReadInteger( "mmin",      _mmin );
        js_map.ReadInteger( "brokerage", _brokerage );

        GameBattle * pBattle = g_GameMgr.GetBattle( _battleid );
        if ( pBattle ) {
            pBattle->SetID ( _seatid, _userid  );
            pBattle->SetKey( _seatid, _userkey );
            pBattle->getMultiple() = 1;
            pBattle->SetModel( _dmodel );
            pBattle->SetMaxMoney( _mmax );
            pBattle->SetMinMoney( _mmin );
            pBattle->SetBrokerage( _brokerage );

            char szPlayerkey[256] = {0};
            pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

            char buff[256]   = {0};
            char format[256] = 	"{\"protocol\":\"%d\","
                                "%s,"
                                "\"userkey\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"status\":\"%d\","
                                "\"battleid\":\"%d\","
                                "\"multiple\":\"%d\","
                                "\"basics\":\"%d\","
                                "\"brokerage\":\"%d\"}";

            snprintf( buff, sizeof(buff), format,
                      MAKEDWORD( Games_Protocol, JoinGame_BRD ),
                      szPlayerkey, _userkey, _seatid, _status, _battleid,
                      1, _mmin, _brokerage);

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
        return;
   }
}


