#include "Handler_Module.h"

void MSG_Handler_TableInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    //g_AgentServer->SendToLobbyServer( (BYTE*) pMsg, wSize);
}

int User_Login_Query_Week_Rank( WORD _userkey )
{
    printf("User_Login_Query_Week_Rank\n");
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, WRankInfo_SYN), _userkey );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

void MSG_Handler_TableInfo_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    printf("[AgentServer::MSG_Handler_TableInfo_ANC] %s \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey = 0;
    js_map.ReadInteger( "userkey", _userkey );
    if ( _userkey!=0 ) {
        g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
        User_Login_Query_Week_Rank( _userkey );
    }
}
