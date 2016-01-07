#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*******************************************************
        从大厅获取在线人数   ( Onlines_ANC )
*******************************************************/
int User_Login_Query_Relogin( WORD _userkey, int _userid )
{
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " {\"protocol\":\"%d\",\"userkey\":%d,\"userid\":%d} ",
            MAKEDWORD( Login_Protocol, Relogin_REQ ), _userkey, _userid );

    int nLen = strlen( szMsg );
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

void MSG_Handler_Onlines_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}


/*******************************************************
        从大厅获取在线人数   ( Onlines_ANC )
******************************************************/
void MSG_Handler_Onlines_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {

    DEBUG_MSG( LVL_DEBUG, "Onlines_ANC to client: %s \n",(char *) pMsg );


    JsonMap js_map;
    if ( js_map.set_json( (char*) pMsg ) == -1 ) {
        return;
    }

    int _userkey(0), _userid(0);
    js_map.ReadInteger( "userkey", _userkey );

    if ( _userkey!=0 )
    {
        UserSession * pSession = g_AgentServer->GetUserSession( _userkey );
        if ( pSession ) {
            _userid = pSession->GetUserid();
            User_Login_Query_Relogin( _userkey , _userid );
        }

        g_AgentServer->SendToClient( _userkey, (BYTE*) pMsg, wSize );
    }

}
