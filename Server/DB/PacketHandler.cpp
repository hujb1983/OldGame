#include "PacketHandler.h"
#include "Handler_Module.h"

/*  1. 游戏的命令转化格式
    2. ...
    */
PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
    m_pFuncMap_Login    = new FunctionMap;
	m_pFuncMap_Lobby    = new FunctionMap;
	m_pFuncMap_Games    = new FunctionMap;
	m_pFuncMap_Database = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
    SAFE_DELETE(m_pFuncMap_Login);
	SAFE_DELETE(m_pFuncMap_Lobby);
	SAFE_DELETE(m_pFuncMap_Games);
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
    Register_Login();
    Register_Lobby();
    Register_Games();
    Register_Database();
	return TRUE;
}

BOOL PacketHandler::Register_Login()
{
    AddHandler_Login( Login_Protocol,  PreLogin_REQ,    MSG_Handler_PreLogin );
}

BOOL PacketHandler::Register_Lobby()
{
    AddHandler_Lobby( Login_Protocol,  Login_REQ,      MSG_Handler_Login        );
    AddHandler_Lobby( Login_Protocol,  Relogin_REQ,    MSG_Handler_Relogin      );
    AddHandler_Lobby( Login_Protocol,  GamePacket_SYN, MSG_Handler_GamePacket   );
    AddHandler_Lobby( Update_Protocol, RoomInfo_SYN,   MSG_Handler_RoomInfo     );
    AddHandler_Lobby( Update_Protocol, TableInfo_SYN,  MSG_Handler_TableInfo    );
	AddHandler_Lobby( Update_Protocol, OnlineInfo_SYN, MSG_Handler_Onlines      );
	AddHandler_Lobby( Update_Protocol, WRankInfo_SYN,  MSG_Handler_WRankInfo    );
	AddHandler_Lobby( Update_Protocol, DRankInfo_SYN,  MSG_Handler_DRankInfo    );
	AddHandler_Lobby( Games_Protocol,  JoinTable_REQ,  MSG_Handler_JoinTable    );
}

BOOL PacketHandler::Register_Games()
{
    AddHandler_Games( Games_Protocol, JoinGame_REQ,     MSG_Handler_JoinGame     );
    AddHandler_Games( Games_Protocol, QuitGame_REQ,     MSG_Handler_QuitGame     );
    AddHandler_Games( Games_Protocol, Settlement_REQ,   MSG_Handler_Settlement   );
    AddHandler_Games( Games_Protocol, UpdateBattle_REQ, MSG_Handler_UpdateBattle );
}

BOOL PacketHandler::Register_Database()
{
    AddHandler_Database( Login_Protocol,  PreLogin_DBR,     MSG_Handler_PreLogin_DBR    );
    AddHandler_Database( Login_Protocol,  Login_DBR,        MSG_Handler_Login_DBR       );
    AddHandler_Database( Login_Protocol,  Relogin_DBR,      MSG_Handler_Relogin_DBR     );
    AddHandler_Database( Update_Protocol, GamePacket_DBR,   MSG_Handler_GamePacket_DBR  );
    AddHandler_Database( Update_Protocol, RoomInfo_DBR,     MSG_Handler_RoomInfo_DBR    );
    AddHandler_Database( Update_Protocol, TableInfo_DBR,    MSG_Handler_TableInfo_DBR   );
	AddHandler_Database( Update_Protocol, OnlineInfo_DBR,   MSG_Handler_Onlines_DBR     );
	AddHandler_Database( Update_Protocol, WRankInfo_DBR,    MSG_Handler_WRankInfo_DBR   );
	AddHandler_Database( Update_Protocol, DRankInfo_DBR,    MSG_Handler_DRankInfo_DBR   );
	AddHandler_Database( Games_Protocol,  JoinTable_DBR,    MSG_Handler_JoinTable_DBR   );
	AddHandler_Database( Games_Protocol,  JoinGame_DBR,     MSG_Handler_JoinGame_DBR    );
	AddHandler_Database( Games_Protocol,  QuitGame_DBR,     MSG_Handler_QuitGame_DBR    );
	AddHandler_Database( Games_Protocol,  UpdateBattle_DBR, MSG_Handler_UpdateBattle_DBR);
	AddHandler_Database( Games_Protocol,  Settlement_DBR,   MSG_Handler_Settlement_DBR  );
}

BOOL PacketHandler::AddHandler_Login( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Login * pFuncInfo = new FUNC_Login;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Login->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Lobby( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Lobby * pFuncInfo = new FUNC_Lobby;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Lobby->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Games( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Games * pFuncInfo = new FUNC_Games;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Games->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Database * pFuncInfo = new FUNC_Database;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Database->Add( pFuncInfo );
}


VOID PacketHandler::ParsePacket_Login( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Login * pFuncInfo = (FUNC_Login *)m_pFuncMap_Login->Find( pid );
        if (pFuncInfo) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

VOID PacketHandler::ParsePacket_Lobby( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Lobby * pFuncInfo = (FUNC_Lobby *)m_pFuncMap_Lobby->Find( pid );
        if (pFuncInfo) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

VOID PacketHandler::ParsePacket_Games( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Games * pFuncInfo = (FUNC_Games *)m_pFuncMap_Games->Find( pid );
        if (pFuncInfo) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

VOID PacketHandler::ParsePacket_Database( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    FUNC_Database * pFuncInfo = (FUNC_Database *)m_pFuncMap_Database->Find( MAKELONG( pMsg->m_byCategory, pMsg->m_byProtocol ) );
    if (pFuncInfo) {
        pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
    }
}

