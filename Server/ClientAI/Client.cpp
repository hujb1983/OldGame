#include "Client.h"

int main(int argc, char ** argv)
{
	acl_msg_printf();
	return -1;
	 
	int len  	= 0;
	int retLen 	= 0;
	char szBuff[1024] = {0};
	Handler_Packet g_Packet;
	
	/*
	printf("%d \n", MAKEDWORD( Games_Protocol, JoinTable_REQ ) );
	return 0;
	*/
	
	// step-1.  预登录连接
	/* if ( acl_prelogin_socket("127.0.0.1", 6010)==-1 ) { 
		return -1;
	} */
	 
	// step-1.  创建连接; 
	int cnSock = acl_create_socket("127.0.0.1", 1234);
	if (cnSock == -1) {
		return -1;
	}
	
	// step-2.  处理所有响应消息
	while(true) 
	{
		memset( szBuff, 0x0, sizeof(szBuff) );
		retLen = recv(cnSock, szBuff, 1024, 0);
		if (retLen > 0) {
			g_Packet.ParsePacket_Agent( cnSock, (szBuff+2), retLen );
			continue;
		}
		break;
	}
	
	return getchar();
}
