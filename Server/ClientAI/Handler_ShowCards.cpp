#include "Handler_Module.h"
#include "GameUser.h"

int acl_create_called( char * pInput, WORD wSize )
{
	int _battleid = g_GameUser.m_nBattleid;
    int _seatid   = g_GameUser.m_nSeatid;
	int _userkey  = g_GameUser.m_nSeatKey;
	
	char buff[1024]  = {0};
	char format[256] = 	"{ \"Protocol\":\"%d\", "
						"  \"battleid\":\"%d\", "
						"  \"called\":\"%d\", "
                        "  \"seatid\":\"%d\"} ";
						
	sprintf( buff, format, MAKEDWORD( Games_Protocol, Called_REQ  ) ,
				_battleid, 1, _seatid);

	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(pInput, buff, len);
	}
	
	// printf("%s \n", buff);
}

void MSG_Handler_ShowCards_BRD( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_ShowCards_ANC) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	char szBuff[1024] = {0};
	int len = acl_create_called(szBuff, 1024);
	send(cnSock, szBuff, len, 0);
	
	return;
}