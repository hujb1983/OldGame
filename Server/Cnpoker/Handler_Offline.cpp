#include "Handler_Module.h"
#include "CnpokerServer.h"

/* 离线通知 */
void MSG_Handler_Offline_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, " MSG_Handler_Offline_NAK %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey , _battleid;
    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "battleid", _battleid );

    GameBattle *pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return;
    }

    BYTE _seatid;
    if ( !pBattle->getSeatid( _userkey, _seatid ) ) {
        return;
    }

    // 设置为离线, 发离线 (1);
    if ( pBattle->SetOnline(_seatid, 1)==FALSE ) {
        int _userid = pBattle->getID( _seatid );
        pBattle->QuitBattle( _seatid );

        char _buff[256]   = {0};
        char _format[256] = "{\"protocol\":\"%d\",\"battleid\":%d,\"seatid\":%d,\"userid\":%d}";
        snprintf( _buff, sizeof(_buff), _format, MAKEDWORD( Games_Protocol, QuitGame_REQ ),
                  _battleid, _seatid, _userid );

        WORD nLen = strlen( _buff );
        g_pCnpokerServer->SendToDBServer( (BYTE*)_buff, nLen );

        DEBUG_MSG( LVL_DEBUG, "QuitGame_REQ to db: %s \n", (char*)_buff );
    }
    else
    {
        int _userkey = pBattle->getKey( _seatid );

        char _buff[256]   = {0};
        char _format[256] = "{\"protocol\":\"%d\",\"userkey\":%d,\"battleid\":%d,\"seatid\":%d}";
        snprintf( _buff, sizeof(_buff), _format, MAKEDWORD( Login_Protocol, Offline_BRD ),
                _userkey, _battleid, _seatid );

        WORD nLen = strlen( _buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)_buff, nLen );

        DEBUG_MSG( LVL_DEBUG, "Offline_BRD to db: %s \n", (char*)_buff );
    }
}

