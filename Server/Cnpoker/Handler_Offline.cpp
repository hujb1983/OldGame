#include "Handler_Module.h"
#include "CnpokerServer.h"

/* 离线通知 */
void MSG_Handler_Offline_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_Offline_NAK] pMsg = %s \n",  (char*)pMsg );

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

    BYTE _bySeatid;
    if ( !pBattle->getSeatid( _userkey, _bySeatid ) ) {
        return;
    }

    // 设置为离线 (1);
    pBattle->SetOnline( _bySeatid, 1 );

    {
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\" }] }";

        snprintf( buff, sizeof(buff), format, MAKEDWORD( Login_Protocol, Offline_BRD ),
                szPlayerkey, _battleid, _bySeatid );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );
    }
}

