#include "Handler_Module.h"

int acl_create_jointable( char * pInput, WORD wSize)
{
	MSG_JOINTABLE_REQ msg;

	char buff[1024]  = {0};
	char format[256] = 	"{ \"Protocol\":\"%d\","
						  "\"userid\":\"555\","
						  "\"roomid\":\"%d\","
						  "\"tableid\":\"%d\"}";

	sprintf( buff, format,
			 MAKEDWORD( Games_Protocol, JoinTable_REQ ),		// 131597088
			 2, 1 );

	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(pInput, buff, len);
	}
	
	printf( "[]acl_create_jointable = %d] \n", MAKEDWORD( Games_Protocol, JoinTable_REQ ) ); 
	return len;
}


void MSG_Handler_Onlines_ANC( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_Onlines_ANC) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	char szBuff[1024] = {0};
	int  nLen 	= 0;
	nLen = acl_create_jointable( szBuff, sizeof(szBuff) );
	send( cnSock, szBuff, nLen, 0);
	return;
}