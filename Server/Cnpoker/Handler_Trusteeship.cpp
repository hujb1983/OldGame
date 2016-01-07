#include "Handler_Module.h"
#include "CnpokerServer.h"

/* 托管设置 */
void MSG_Handler_Trusteeship_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
   printf( "[MSG_Handler_License_REQ] pMsg = %s \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey, _battleid, _seatid, _status;
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid );
    js_map.ReadInteger( "status",   _status );

    if ( !(seatid>=0 && _seatid<3) ) {
        return ;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return ;
    }

    // 设置时返回 -1， 证明已经操作过
    if ( _status==1 ) {
        if ( pBattle->SetTrusteeship( _seatid )==-1 ) {
            return ;
        }
    }
    else {
        if ( pBattle->CancelTrusteeship( _seatid )==-1 ) {
            return ;
        }
    }

    {
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char buff[256]   = {0};
        char format[256] = 	"{\"protocol\":\"%d\", %s,\"battleid\":%d,\"seatid\":%d,\"status\":%d}";   // 暂时设定为18秒;

        snprintf( buff, sizeof(buff), format, MAKEDWORD( Games_Protocol, Trusteeship_BRD ),
                szPlayerkey,
                _seatid,
                _status );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );
    }
}
