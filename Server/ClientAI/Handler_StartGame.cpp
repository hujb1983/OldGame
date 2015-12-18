#include "Handler_Module.h"
#include "GameUser.h"

int acl_create_showcards_0( char * pInput, WORD wSize )
{
	int _battleid = g_GameUser.m_nBattleid;
    int _seatid   = g_GameUser.m_nSeatid;
	int _userkey  = g_GameUser.m_nSeatKey;
	
	char buff[1024]  = {0};
	char format[256] = 	"{ \"Protocol\":\"%d\", "
						"  \"called\":\"%d\", "
						"  \"battleid\":\"%d\", "
						"  \"status\":\"1\", "
                        "  \"seatid\":\"%d\"} ";
						
	sprintf( buff, format, MAKEDWORD( Games_Protocol,  ShowCards_REQ ), 
				1, _battleid, _seatid);

	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(pInput, buff, len);
	}
	
	// printf("%s \n", buff);	
}

void MSG_Handler_StartGame_BRD( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_StartGame_BRD) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	/*
	char szBuff[1024] = {0};
	int len = acl_create_showcards_0(szBuff, 1024);
	send(cnSock, szBuff, len, 0);
	*/
	
	return;
}