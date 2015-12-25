#include "PacketHandler.h"
#include "Handler_Module.h"

/*  1. 游戏的命令转化格式
    2. ...
    */
PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
    m_pFuncMap_Agent  = new FunctionMap;
	m_pFuncMap_Database = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
    SAFE_DELETE(m_pFuncMap_Agent);
	SAFE_DELETE(m_pFuncMap_Database);
}

DWORD PacketHandler::GetProtocol( char * szMsg )
{
    JsonMap js_map;
    if ( js_map.set_json( szMsg) ) {
        return 0;
    }

    int pid = 0;
    js_map.ReadInteger("protocol", pid);
    return (DWORD) pid;
}

BOOL PacketHandler::RegisterHandler()
{
    Register_Agent();
    Register_Database();
	return TRUE;
}

BOOL PacketHandler::Register_Agent()
{
    AddHandler_Agent( Login_Protocol,  Login_REQ,      MSG_Handler_Login_REQ );
    AddHandler_Agent( Login_Protocol,  Relogin_REQ,    MSG_Handler_Relogin_REQ );
    AddHandler_Agent( Login_Protocol,  GamePacket_SYN, MSG_Handler_GamePacket_REQ  );
    AddHandler_Agent( Games_Protocol,  JoinTable_REQ,  MSG_Handler_JoinTable_REQ );
    AddHandler_Agent( Games_Protocol,  QuitTable_REQ,  MSG_Handler_QuitTable_REQ );
    AddHandler_Agent( Update_Protocol, RoomInfo_SYN,   MSG_Handler_RoomInfo_REQ );
    AddHandler_Agent( Update_Protocol, TableInfo_SYN,  MSG_Handler_TableInfo_REQ );
	AddHandler_Agent( Update_Protocol, OnlineInfo_SYN, MSG_Handler_Onlines_REQ   );
	AddHandler_Agent( Update_Protocol, WRankInfo_SYN,  MSG_Handler_WRankInfo_REQ );
	AddHandler_Agent( Update_Protocol, DRankInfo_SYN,  MSG_Handler_DRankInfo_REQ );
}

BOOL PacketHandler::Register_Database()
{
    AddHandler_Database( Login_Protocol,  Login_ANC,       MSG_Handler_Login_ANC       );
    AddHandler_Database( Login_Protocol,  Relogin_ANC,     MSG_Handler_Relogin_ANC     );
    AddHandler_Database( Login_Protocol,  GamePacket_ANC,  MSG_Handler_GamePacket_ANC  );
    AddHandler_Database( Update_Protocol, RoomInfo_ANC,    MSG_Handler_RoomInfo_ANC    );
    AddHandler_Database( Update_Protocol, TableInfo_ANC,   MSG_Handler_TableInfo_ANC   );
	AddHandler_Database( Update_Protocol, OnlineInfo_ANC,  MSG_Handler_Onlines_ANC     );
	AddHandler_Database( Update_Protocol, WRankInfo_ANC,   MSG_Handler_WRankInfo_ANC   );
	AddHandler_Database( Update_Protocol, DRankInfo_ANC,   MSG_Handler_DRankInfo_ANC   );
	AddHandler_Database( Games_Protocol,  JoinTable_BRD,   MSG_Handler_JoinTable_BRD   );
}


BOOL PacketHandler::AddHandler_Agent( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Agent * pFuncInfo = new FUNC_Agent;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Agent->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Database * pFuncInfo = new FUNC_Database;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Database->Add( pFuncInfo );
}


VOID PacketHandler::ParsePacket_Agent( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Agent * pFuncInfo = (FUNC_Agent *)m_pFuncMap_Agent->Find( pid );
        if ( pFuncInfo ) {
                pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

VOID PacketHandler::ParsePacket_Database( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Database * pFuncInfo = (FUNC_Database *)m_pFuncMap_Database->Find( pid );
        if ( pFuncInfo ) {
                pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

