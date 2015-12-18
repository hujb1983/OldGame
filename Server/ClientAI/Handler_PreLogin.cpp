#include "Handler_Module.h"
#include "GameUser.h"


void MSG_Handler_PreLogin_ANC( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_EnteryType_ANC) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	// step-3.  开始进入登录;
	
	/*
	char szBuff[1024] = {0};
	int len = acl_create_login(szBuff, 1024);
	send(cnSock, szBuff, len, 0);
	*/
	
	return;
}