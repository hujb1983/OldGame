#include "Handler_Packet.h"
#include "Handler_Module.h"

int acl_msg_printf()
{
	printf( "%10d : >>请求登录游戏 \n",       		MAKEDWORD(Login_Protocol,  Login_REQ) ); 
	printf( "%10d : <<请求登录游戏应答 \n",   		MAKEDWORD(Login_Protocol,  Login_ANC) );  
    printf( "%10d : <<请求登录游戏失败 \n",   		MAKEDWORD(Login_Protocol,  Login_ANC) );   	
	printf( "%10d : >>预登陆连接 \n",       		MAKEDWORD(Login_Protocol,  PreLogin_REQ) );   
	printf( "%10d : <<预登陆连接应答 \n",    		MAKEDWORD(Login_Protocol,  PreLogin_ANC) );
	printf( "%10d : <<预登陆连接游戏 \n",    		MAKEDWORD(Login_Protocol,  PreLogin_NAK) );	
	printf( "%10d : <<个人游戏数据 \n",  			MAKEDWORD(Login_Protocol,  GamePacket_ANC) );
	printf( "%10d : <<房间分类列表信息 \n",    		MAKEDWORD(Update_Protocol, RoomInfo_ANC) );   
	printf( "%10d : <<房间列表信息 \n",   			MAKEDWORD(Update_Protocol, TableInfo_ANC) ); 
	printf( "%10d : <<在线人数信息 \n",  			MAKEDWORD(Update_Protocol, OnlineInfo_ANC) ); 
	printf( "%10d : <<周排行列表信息 \n",   		MAKEDWORD(Update_Protocol, WRankInfo_ANC) ); 
	printf( "%10d : <<日排行列表信息 \n",   		MAKEDWORD(Update_Protocol, DRankInfo_ANC) ); 
	printf( "%10d : >>加入房间信息 \n",   			MAKEDWORD(Games_Protocol,  JoinTable_REQ) );  
	printf( "%10d : >>退出房间信息 \n",   			MAKEDWORD(Games_Protocol,  QuitTable_REQ) ); 
	printf( "%10d : >>游戏准备请求 \n",   			MAKEDWORD(Games_Protocol,  StartGame_REQ) ); 
	printf( "%10d : >>显示游戏请求 \n",   			MAKEDWORD(Games_Protocol,  ShowCards_REQ) ); 
	printf( "%10d : >>叫地主请求 \n",      			MAKEDWORD(Games_Protocol,  Called_REQ) );    
	printf( "%10d : >>出牌请求 \n",    			    MAKEDWORD(Games_Protocol,  Discards_REQ) ); 
	printf( "%10d : >>托管游戏请求 \n", 			MAKEDWORD(Games_Protocol,  Trusteeship_REQ) );
	printf( "%10d : <<加入游戏广播 \n",      		MAKEDWORD(Games_Protocol, JoinGame_BRD) );       
	printf( "%10d : <<加入游戏失败 \n",      		MAKEDWORD(Games_Protocol, JoinGame_NAK) );         
	printf( "%10d : <<退出桌子广播 \n",        		MAKEDWORD(Games_Protocol, QuitTable_BRD) );         
	printf( "%10d : <<开始游戏广播 \n",        		MAKEDWORD(Games_Protocol, StartGame_BRD) );         
	printf( "%10d : <<分配牌广播 \n",        		MAKEDWORD(Games_Protocol, InitCards_BRD) );         
	printf( "%10d : <<显示牌广播 \n",        		MAKEDWORD(Games_Protocol, ShowCards_BRD) );        
	printf( "%10d : <<叫地主广播 \n",           	MAKEDWORD(Games_Protocol, Called_BRD) );           
	printf( "%10d : <<叫地主授权广播 \n",    		MAKEDWORD(Games_Protocol, CalledLicense_BRD) );     
	printf( "%10d : <<创建地主广播 \n",       		MAKEDWORD(Games_Protocol, CreateBank_BRD) );        
	printf( "%10d : <<出版授权广播 \n",  		    MAKEDWORD(Games_Protocol, DiscardsLicense_BRD) );   
	printf( "%10d : <<出版广播 \n",         		MAKEDWORD(Games_Protocol, Discards_BRD) );          
	printf( "%10d : <<叫地主超时通知 \n",   		MAKEDWORD(Games_Protocol, CalledOverTime_NAK) );    
	printf( "%10d : <<出牌超时通知 \n", 		    MAKEDWORD(Games_Protocol, DiscardsOverTime_NAK) );  
	printf( "%10d : <<玩家重新登入游戏广播 \n",     MAKEDWORD(Login_Protocol, Relogin_BRD) );           
	printf( "%10d : <<玩家离开游戏广播 \n",         MAKEDWORD(Login_Protocol, Offline_BRD) );          
	printf( "%10d : <<游戏托管广播 \n",      		MAKEDWORD(Games_Protocol, Trusteeship_BRD) );    
}


int acl_create_socket(const char * szip, unsigned short usport)
{
	int nRet  = 0;
	int nZero = 0;
	
	SOCKET newSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( newSocket == INVALID_SOCKET ) {
		return -1;
	}
	
	SOCKADDR_IN addr;
	addr.sin_family 		= AF_INET;
	addr.sin_addr.s_addr 	= inet_addr(szip);
	addr.sin_port			= htons(usport);
	
	int err = connect(newSocket,(SOCKADDR*)(&addr),sizeof(SOCKADDR_IN));
	if (err) {
		return -1;
	}
	
	return newSocket;
}

int acl_prelogin_socket(const char * szip, unsigned short usport)
{
	int len  	= 0;
	int retLen 	= 0;
	char szBuff[1024] = {0};
	Handler_Packet g_Packet;
	 
	// step-1.  预登录连接
	int PreloginSock = acl_create_socket("127.0.0.1", 6010);
	if (PreloginSock == -1) {
		return -1;
	}
	
	Sleep(10);
	retLen = recv(PreloginSock, szBuff, 1024, 0);
	if (retLen > 0) {
		int pid = g_Packet.GetProtocol( szBuff);
		if ( pid != MAKEDWORD( (WORD)Connect_Protocol,  (WORD)(ClientType_ANC+1) ) ) {
			return -1;
		}
	}
	
	int   _iUserDB   = 0;
    int   _iAccessID = 100;
    char  _cUsername[33] = { "kim"    };
    char  _cPassword[33] = { "123qwe" };
    char  _cSshKey[33]   = { "123" };
    int   _iGameID    = 2;
    int   _iLoginType = 6;
	
	char szPrelogin[1024]  = {0};
    char format[256] = 	   "{\"Protocol\":\"%d\","
							"\"aid\":\"%d\","
                            "\"username\":\"%s\","
                            "\"password\":\"%s\","
							"\"sshkey\":\"%s\","
							"\"gameid\":\"%d\","
                            "\"logintype\":\"%d\" }";
	
	sprintf( szPrelogin, format, MAKEDWORD( (WORD)Login_Protocol, (WORD)PreLogin_REQ ), 
		_iAccessID, _cUsername, _cPassword, _cSshKey, _iGameID, _iLoginType );
	
	Sleep(10);
	len = strlen(szPrelogin);
	send(PreloginSock, szPrelogin, len, 0);
	
	Sleep(10);
	retLen = recv( PreloginSock, szBuff, 1024, 0 );
	if (retLen > 0) {
		int pid = g_Packet.GetProtocol( szBuff);
		if ( pid != MAKEDWORD( (WORD)Login_Protocol, (WORD)PreLogin_ANC ) ) {
			return -1;
		}
	}
	
	Sleep(10);	
	close(PreloginSock);
	return 0;
}
