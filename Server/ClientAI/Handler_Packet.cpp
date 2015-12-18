#include "Handler_Module.h"
#include "Handler_Packet.h"

Handler_Packet::Handler_Packet(void)
{
	m_pFuncMap_Agent   = new FunctionMap;
	RegisterHandler();
}

Handler_Packet::~Handler_Packet(void)
{
	SAFE_DELETE( m_pFuncMap_Agent);
}

DWORD Handler_Packet::GetProtocol( char * szMsg )
{
	JsonMap js_map;
	if ( js_map.set_json( szMsg) ) {
		return 0;
	}
	
	int pid = 0;
	js_map.ReadInteger("protocol", pid);
	return (DWORD) pid;
}

BOOL Handler_Packet::RegisterHandler()
{
	Register_Agent();
}

BOOL Handler_Packet::Register_Agent ()
{
	AddHandler_Agent ( Connect_Protocol,  ClientType_ANC+1, MSG_Handler_EnteryType_ANC );
	AddHandler_Agent ( Login_Protocol,    Login_ANC,      	MSG_Handler_Login_ANC      );
	AddHandler_Agent ( Update_Protocol,   GamePacket_ANC, 	MSG_Handler_GamePacket_ANC );
	AddHandler_Agent ( Update_Protocol,   RoomInfo_ANC,   	MSG_Handler_RoomInfo_ANC   );
	AddHandler_Agent ( Update_Protocol,   TableInfo_ANC,  	MSG_Handler_TableInfo_ANC  );
	AddHandler_Agent ( Update_Protocol,   OnlineInfo_ANC, 	MSG_Handler_Onlines_ANC    );
	AddHandler_Agent ( Update_Protocol,   WRankInfo_ANC,  	MSG_Handler_WRankInfo_ANC  );
	AddHandler_Agent ( Update_Protocol,   DRankInfo_ANC,  	MSG_Handler_DRankInfo_ANC  );
	AddHandler_Agent ( Games_Protocol,    JoinGame_BRD,  	MSG_Handler_JoinGame_BRD   );
	AddHandler_Agent ( Games_Protocol,    StartGame_BRD,  	MSG_Handler_StartGame_BRD  );
	AddHandler_Agent ( Games_Protocol,    InitCards_BRD,  	MSG_Handler_InitCards_BRD  );
	AddHandler_Agent ( Games_Protocol,    ShowCards_BRD,  	MSG_Handler_ShowCards_BRD  );
	AddHandler_Agent ( Games_Protocol,    Called_BRD,  		MSG_Handler_CalledBank_BRD );
	AddHandler_Agent ( Games_Protocol,    CreateBank_BRD,  	MSG_Handler_CreateBank_BRD );
	AddHandler_Agent ( Games_Protocol,    Discards_BRD,  	MSG_Handler_Discard_BRD    );
	
	AddHandler_Agent ( Games_Protocol, CalledLicense_BRD,  	MSG_Handler_CalledLicense_BRD   );
	AddHandler_Agent ( Games_Protocol, DiscardsLicense_BRD, MSG_Handler_DiscardsLicense_BRD );
}

BOOL Handler_Packet::AddHandler_Agent ( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Agent * pFuncInfo = new FUNC_Agent;
	pFuncInfo->m_dwFunctionKey	= MAKEDWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Agent->Add( pFuncInfo );
}

void Handler_Packet::ParsePacket_Agent  ( int cnSock, char * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
	
	DWORD pid = GetProtocol( pMsg );
	if ( pid != 0 ) {
		FUNC_Agent * pFuncInfo = (FUNC_Agent *)m_pFuncMap_Agent->Find( pid );
		if (pFuncInfo) {
			pFuncInfo->m_fnHandler( cnSock, pMsg, wSize );
			return;
		}
	}
	
	printf("\n");
	printf( "[Client] MSG Begin >> (Handler_Packet::ParsePacket_Agent) \n" );
	printf( "[Client] %s \n",  (char*)pMsg );
	printf( "[Client] MSG End \n");
}
