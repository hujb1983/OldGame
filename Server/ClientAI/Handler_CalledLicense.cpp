#include "Handler_Module.h"

void MSG_Handler_CalledLicense_BRD( int cnSock, char * pInput, WORD wSize)
{
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_CalledLicense_BRD) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	return;
}