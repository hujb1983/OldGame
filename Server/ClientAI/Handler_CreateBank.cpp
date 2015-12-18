#include "Handler_Module.h"
#include "GameUser.h"


void MSG_Handler_CreateBank_BRD( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_CreateBank_BRD) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
		
	return;
}