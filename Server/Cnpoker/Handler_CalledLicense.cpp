#include "Handler_Module.h"
#include "CnpokerServer.h"

/* ÊÚÈ¨ --- Í¨Öª */
void MSG_Handler_CalledLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf( "[MSG_Handler_License_REQ] pMsg = %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey, _battleid, _seatid;
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid );

    GameBattle *pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return ;
    }

    BYTE _bySeatid = pBattle->nextSeat( (WORD) _seatid );

    {
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"data\":[{"
                                "\"seatid\":\"%d\","
                                "\"times\":\"%d\" }] }";   // 暂时设定为18秒;

        snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, CalledLicense_BRD ),
                szPlayerkey, _bySeatid, 6 );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );

        /* step5. 送到超时判断队列...  */
        pBattle->SetCalledOvertime();
    }
}
