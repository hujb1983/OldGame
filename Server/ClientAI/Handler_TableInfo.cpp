#include "Handler_Module.h"


void MSG_Handler_TableInfo_ANC( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_TableInfo_ANC) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	return;
}