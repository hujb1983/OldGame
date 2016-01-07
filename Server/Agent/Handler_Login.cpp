#include "Handler_Module.h"
#include <dbCommon.h>

/*********************************************************
    Step 1: MSG_Handler_Login_REQ
*********************************************************/
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

    {
        UserPacket pack;
        pack.ToInit();
        pack.GetProtocol() = MAKEDWORD( Login_Protocol, Login_REQ );
        pack.GetUserKey()  = _userkey;
        pack.GetUserId()   = _userid;
        memcpy ( pack.GetSSHKey(), _szSSHKEY, sizeof(_szSSHKEY) );
        pack.ToPrint();
        g_AgentServer->SendToLobbyServer( (BYTE*)&pack, sizeof(pack) );
    }
}

/*********************************************************
    Step 4: User_GamePacket
*********************************************************/
int User_RoomsInfo( WORD _userkey )
{
    DEBUG_MSG( LVL_TRACE,  "User_Login_Query_Room_Info \n" );

    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Update_Protocol, RoomInfo_SYN ), _userkey );
    int nLen = strlen(szMsg);
    g_AgentServer->SendToLobbyServer( (BYTE*) szMsg, nLen );
}

/*********************************************************
    Step 3: User_GamePacket
*********************************************************/
int User_GamePacket( UserPacket & pack )
{
    char _szMsg[256] = {0};
    char _format[128] = "{\"protocol\":\"%d\",\"userid\":%d,"
        "\"data\":[{\"points\":%d,\"wons\":%d,\"faileds\":%d,\"aways\":%d,\"name\":\"%s\"}]}";

    snprintf( _szMsg, sizeof(_szMsg), _format,
              MAKEDWORD( Login_Protocol, GamePacket_ANC ),  pack.GetUserId(),
             pack.GetMoney(), pack.GetWoneds(), pack.GetFaileds(), pack.GetAways(), pack.GetName() );

    WORD nLen = strlen( _szMsg );
    g_AgentServer->SendToBuffer( pack.GetUserKey(), (BYTE*) _szMsg, nLen );

    User_RoomsInfo( pack.GetUserKey() );
}

/*********************************************************
    Step 2: MSG_Handler_Login_ANC
*********************************************************/
void MSG_Handler_Login_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    UINT _logintime(0);
    if ( wSize>=sizeof(UserPacket) )
    {
        UserPacket pack;
        pack.SetPacket( (BYTE*) pMsg, wSize );
        _logintime = pack.GetLoginTime();
        pack.ToPrint();

        char szJsonBuff[1024] = {0};
        if ( _logintime != 0 ) {
            snprintf( szJsonBuff, sizeof(szJsonBuff), " {\"protocol\":%d,\"userid\":%d,\"userkey\":%d,\"errmsg\":%d} ",
                    pack.GetProtocol(), pack.GetUserId(),  pack.GetUserKey(),  MAKEDWORD( Errors_Protocol, Correct_NAK ) );
        }
        else {
            snprintf( szJsonBuff, sizeof(szJsonBuff), " {\"protocol\":%d,\"userid\":%d,\"userkey\":%d,\"errmsg\":%d} ",
                    pack.GetProtocol(), pack.GetUserId(),  pack.GetUserKey(),  MAKEDWORD( Errors_Protocol, ClientLogin_NAK )  );
        }

        int iLength = strlen(szJsonBuff);
        g_AgentServer->SendToClient( pack.GetUserKey(),  (BYTE*) szJsonBuff, iLength );

        DEBUG_MSG( LVL_DEBUG, "Login_ANC: %s \n", szJsonBuff);

        if ( _logintime!=0 ) {
            UserSession * pSession = g_AgentServer->GetUserSession( pack.GetUserKey() );
            if ( pSession) {
                pSession->SetUserPacket( (BYTE*)pMsg, wSize );
            }
            User_GamePacket( pack );
        }
    }
}

