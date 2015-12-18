#include "Handler_Module.h"
#include "GameUser.h"

void MSG_Handler_JoinGame_BRD( int cnSock, char * pInput, WORD wSize)
{
	// 自动开始游戏
    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return;
    }

    int _userkey  = 0;
    int _battleid = 0;
    int _seatid   = 0;

    js_map.ReadInteger( "userkey",  _userkey  );
    js_map.ReadInteger( "battleid", _battleid );
    js_map.ReadInteger( "seatid",   _seatid   );
	
	g_GameUser.m_nBattleid = _battleid;
	g_GameUser.m_nSeatid   = _seatid;
	g_GameUser.m_nSeatKey  = _userkey;
	
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_JoinGame_BRD) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	return;
}