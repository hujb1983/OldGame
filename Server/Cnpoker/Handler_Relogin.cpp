#include "Handler_Module.h"
#include "CnpokerServer.h"

/* 重新上线通知 */
void MSG_Handler_Relogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_Offline_NAK] pMsg = %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey, _battleid, _seatid;
    js_map.ReadInteger( "userkey",  _userkey );
    js_map.ReadInteger( "battleid", _battleid );

    GameBattle *pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return;
    }

    BYTE _bySeatid;
    if ( !pBattle->getSeatid( _userkey, _bySeatid ) ) {
        return;
    }

    // 设置为离线 (1);
    pBattle->SetOnline( _seatid, 0 );
    {
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\" }] }";

        snprintf( buff, sizeof(buff), format, MAKEDWORD( Login_Protocol, Relogin_BRD ),
                szPlayerkey, _battleid, _seatid );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );
    }
}
