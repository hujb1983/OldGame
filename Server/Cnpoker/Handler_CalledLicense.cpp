#include "Handler_Module.h"
#include "CnpokerServer.h"

/* ÊÚÈ¨ --- Í¨Öª */
void MSG_Handler_CalledLicense_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "[ MSG_Handler_License_REQ %s ] \n",  (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _seatid(0), _battleid(0);
    js_map.ReadInteger( "battleid", _battleid );

    GameBattle * pBattle = NULL;
    pBattle = g_GameMgr.GetBattle( _battleid );
    if ( pBattle==NULL ) {
        return;
    }

    // 设置基本参数;
    int _basics    = pBattle->getMinMoney();
    int _multiple  = pBattle->getMultiple();
    int _brokerage = pBattle->getBrokerage();
    _seatid = pBattle->getCalled();

    {
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        char buff[256]   = {0};
        char format[128] = 	"{\"protocol\":\"%d\","
                            "%s,"
                            "\"battleid\":\"%d\","
                            "\"basics\":\"%d\","
                            "\"multiple\":\"%d\","
                            "\"brokerage\":\"%d\","
                            "\"seatid\":\"%d\","
                            "\"times\":\"%d\" }";   // 暂时设定为18秒;

        snprintf( buff, sizeof(buff), format,
                MAKEDWORD( Games_Protocol, CalledLicense_BRD ),
                szPlayerkey,
                _battleid,
                _basics,
                _multiple,
                _brokerage,
                _seatid,
                16 );

        WORD nLen = strlen( buff );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)buff, nLen );


        /* step5. 送到超时判断队列...  */
        // pBattle->SetCalledOvertime();
    }
}
