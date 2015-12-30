#include "Handler_Module.h"

void MSG_Handler_DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    //g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize );
}

int User_Login_Query_Onlines_Info( WORD _userkey)
{
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, OnlineInfo_SYN ), _userkey );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

void MSG_Handler_DRankInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "DRankInfo_ANC to recv: %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey = 0;
    js_map.ReadInteger( "userkey", _userkey );
    if ( _userkey!=0 ) {
        g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
        User_Login_Query_Onlines_Info( _userkey );
    }
}

