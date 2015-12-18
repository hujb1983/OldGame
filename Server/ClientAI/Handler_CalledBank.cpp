#include "Handler_Module.h"
#include "GameUser.h"

int acl_create_discard( char * pInput, WORD wSize )
{
	MSG_LOGIN_REQ msg;

	char buff[1024]  = {0};
	char format[256] = 	"{ \"Protocol\":\"%d\" }";

	sprintf( buff, format, MAKEDWORD( Login_Protocol, Login_REQ ) );

	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(pInput, buff, len);
	}
	
	printf("%s \n", buff);
}

void MSG_Handler_CalledBank_BRD( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_CalledBank_BRD) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	/*
	char szBuff[1024] = {0};
	int len = acl_create_called(szBuff, 1024);
	send(cnSock, szBuff, len, 0);
	*/
	
	return;
}