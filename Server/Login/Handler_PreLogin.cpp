#include "Handler_Module.h"
#include "LoginServer.h"

void MSG_Handler_PreLogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "PreLogin_REQ to recv: %s. \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    UserSession * pUser = (UserSession*) pServerSession;
    int   _iUserKey  = pUser->GetUserKey();
    int   _iAccessID = 100;
    char  _cUsername[33] = {0};
    char  _cPassword[33] = {0};
    char  _cSshKey[33]   = {0};
    int   _iGameID = 2;
    int   _iLoginType = 6;

    js_map.ReadInteger( "userkey",   _iUserKey  );
    js_map.ReadInteger( "aid",       _iAccessID );
    js_map.ReadString ( "username",  _cUsername, sizeof(_cUsername) );
    js_map.ReadString ( "password",  _cPassword, sizeof(_cPassword) );
    js_map.ReadString ( "sshkey",    _cSshKey  , sizeof(_cSshKey)   );
    js_map.ReadInteger( "gameid",    _iGameID    );
    js_map.ReadInteger( "logintype", _iLoginType );

    char buff[1024]  = {0};
    char format[256] = 	   "{\"Protocol\":\"%d\","
                            "\"userkey\":\"%d\","
							"\"aid\":\"%d\","
                            "\"username\":\"%s\","
                            "\"password\":\"%s\","
							"\"sshkey\":\"%s\","
							"\"gameid\":\"%d\","
                            "\"logintype\":\"%d\" }";

	sprintf( buff, format, MAKEDWORD( (WORD)Login_Protocol, (WORD)PreLogin_REQ ),
		_iUserKey, _iAccessID, _cUsername, _cPassword, _cSshKey, _iGameID, _iLoginType );

	int len = strlen( buff );
    g_pLoginServer->SendToDBServer( (BYTE*) buff, len );
}

void MSG_Handler_PreLogin_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "PreLogin_ANC to recv: %s. \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey = 0;
    js_map.ReadInteger( "userkey", _userkey );

    if ( _userkey!=0 ) {
        g_pLoginServer->SendToClient( (BYTE*) pMsg, wSize );
    }
}

void MSG_Handler_PreLogin_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "PreLogin_NAK to recv: %s. \n", (char*)pMsg );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey = 0;
    js_map.ReadInteger( "userkey", _userkey );

    if ( _userkey!=0 ) {
        g_pLoginServer->SendToClient( (BYTE*) pMsg, wSize );
    }
}
