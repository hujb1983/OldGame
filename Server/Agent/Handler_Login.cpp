#include "Handler_Module.h"
#include <dbCommon.h>

void MSG_Handler_Login_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    UserSession * pSession = ( UserSession * ) pServerSession;
    int  _userkey  = pSession->GetUserKey();
    int  _userid   = 0;
    char _szSSHKEY[128] = {0};
    js_map.ReadInteger( "userid", _userid );
    js_map.ReadString ( "sshkey", _szSSHKEY, sizeof(_szSSHKEY) );
    pSession->SetUserid(_userid);

    char buff[256] = {0};
    char format[256] = 	"{\"Protocol\":\"%d\","
                        "\"userid\":%d,"
                        "\"userkey\":%d,"
						"\"sshkey\":\"%s\"}";

	snprintf( buff, sizeof(buff), format, MAKEDWORD( Login_Protocol, Login_REQ ),
         _userid, _userkey, _szSSHKEY);

    g_AgentServer->SendToLobbyServer( (BYTE*) buff, sizeof(buff) );
}

int User_Login_Query_GamePacket( WORD _userkey, int _userid)
{
    printf( "User_Login_Query_GamePacket \n" );
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\", \"userid\":\"%d\" } ",
            MAKEDWORD( Login_Protocol, GamePacket_SYN ), _userkey, _userid );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

void MSG_Handler_Login_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userid  = 0;
    int _userkey = 0;
    int _errmsg  = 0;
    js_map.ReadInteger( "errmsg",  _errmsg  );
    js_map.ReadInteger( "userid",  _userid  );
    js_map.ReadInteger( "userkey", _userkey );

    g_AgentServer->SendToClient( (BYTE*) pMsg, wSize );
    if ( _errmsg==MAKEDWORD( Errors_Protocol, Correct_NAK ) ) {
        User_Login_Query_GamePacket( _userkey, _userid);
    }
}

