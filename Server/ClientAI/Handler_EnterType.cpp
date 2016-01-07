#include "Handler_Module.h"

int acl_create_login( char * pInput, WORD wSize )
{
	char buff[1024]  = {0};
	char format[256] = 	"{\"Protocol\":\"%d\","
                        " \"userid\":%d,"
						" \"sshkey\":\"%s\"}";

	sprintf( buff, format, MAKEDWORD( Login_Protocol, Login_REQ ),		// 
        555, "e85769039ba76677e236164e40279d5f");

	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(pInput, buff, len);
	}
	return len;	
}

void MSG_Handler_EnteryType_ANC( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_EnteryType_ANC) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	// step-3.  开始进入登录;
	
	char szBuff[1024] = {0};
	int len = acl_create_login(szBuff, 1024);
	send(cnSock, szBuff, len, 0);
	
	return;
}