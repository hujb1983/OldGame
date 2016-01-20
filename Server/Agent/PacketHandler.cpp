#include "PacketHandler.h"
#include "Handler_Module.h"

/*  1. 游戏的命令转化格式
    2. ...
    */
PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
    m_pFuncMap_Client   = new FunctionMap;
	m_pFuncMap_Lobby    = new FunctionMap;
	m_pFuncMap_Games    = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
    SAFE_DELETE( m_pFuncMap_Client);
	SAFE_DELETE( m_pFuncMap_Lobby );
	SAFE_DELETE( m_pFuncMap_Games );
}

DWORD PacketHandler::GetProtocol( MSG_BASE * szMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char*)szMsg ) ) {
        if ( wSize==sizeof(UserPacket) ) {
            UserPacket pack;
            pack.SetPacket( (BYTE*) szMsg, wSize );
            return pack.GetProtocol();
        }
        return 0;
    }

    int pid = 0;
    js_map.ReadInteger("protocol", pid);
    return (DWORD) pid;
}


DWORD PacketHandler::ClientProtocol( MSG_BASE * szMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char*)szMsg ) ) {
        return 0;
    }

    int pid = 0;
    js_map.ReadInteger("protocol", pid);
    return (DWORD) pid;
}

DWORD PacketHandler::ServerProtocol( MSG_BASE * szMsg, WORD wSize )
{
    if ( wSize>=sizeof(HeadPacket) ) {
        HeadPacket * pack = (HeadPacket*)szMsg;
        return pack->GetProtocol();
    }
    return 0;
}


BOOL PacketHandler::RegisterHandler()
{
    Register_Client();
    Register_Lobby();
    Register_Games();
	return TRUE;
}

BOOL PacketHandler::Register_Client()
{
    AddHandler_Client ( Login_Protocol,  Login_REQ,         MSG_Handler_Login_REQ      );
    AddHandler_Client ( Update_Protocol, RoomInfo_SYN,      MSG_Handler_RoomInfo_REQ   );
    AddHandler_Client ( Update_Protocol, TableInfo_SYN,     MSG_Handler_TableInfo_REQ  );
	AddHandler_Client ( Update_Protocol, OnlineInfo_SYN,    MSG_Handler_Onlines_REQ    );
	AddHandler_Client ( Update_Protocol, WRankInfo_SYN,     MSG_Handler_WRankInfo_REQ  );
	AddHandler_Client ( Update_Protocol, DRankInfo_SYN,     MSG_Handler_DRankInfo_REQ  );
	AddHandler_Client ( Games_Protocol,  FastGame_REQ,      MSG_Handler_FastGame_REQ   );
	AddHandler_Client ( Games_Protocol,  JoinTable_REQ,     MSG_Handler_JoinTable_REQ  );
	AddHandler_Client ( Games_Protocol,  QuitTable_REQ,     MSG_Handler_QuitTable_REQ  );
	AddHandler_Client ( Games_Protocol,  StartGame_REQ,     MSG_Handler_StartGame_REQ  );
	AddHandler_Client ( Games_Protocol,  Called_REQ,        MSG_Handler_CalledBank_REQ );
	AddHandler_Client ( Games_Protocol,  Discards_REQ,      MSG_Handler_Discards_REQ   );
	AddHandler_Client ( Games_Protocol,  Reminder_REQ,      MSG_Handler_Reminder_REQ   );
	AddHandler_Client ( Games_Protocol,  Trusteeship_REQ,   MSG_Handler_Trusteeship_REQ);
}

BOOL PacketHandler::Register_Lobby()
{
    AddHandler_Lobby ( Login_Protocol,  Login_ANC,          MSG_Handler_Login_ANC       );
    AddHandler_Lobby ( Update_Protocol, RoomInfo_ANC,       MSG_Handler_RoomInfo_ANC    );
    AddHandler_Lobby ( Update_Protocol, TableInfo_ANC,      MSG_Handler_TableInfo_ANC   );
	AddHandler_Lobby ( Update_Protocol, OnlineInfo_ANC,     MSG_Handler_Onlines_ANC     );
	AddHandler_Lobby ( Update_Protocol, WRankInfo_ANC,      MSG_Handler_WRankInfo_ANC   );
	AddHandler_Lobby ( Update_Protocol, DRankInfo_ANC,      MSG_Handler_DRankInfo_ANC   );
	AddHandler_Lobby ( Games_Protocol,  JoinTable_ANC,      MSG_Handler_JoinTable_ANC   );
	AddHandler_Client ( Games_Protocol, FastGame_ANC,       MSG_Handler_FastGame_ANC    );
}

BOOL PacketHandler::Register_Games()
{
	AddHandler_Games( Games_Protocol, JoinTable_BRD,         MSG_Handler_JoinTable_BRD       );
	AddHandler_Games( Games_Protocol, QuitTable_BRD,         MSG_Handler_QuitTable_BRD       );
	AddHandler_Games( Games_Protocol, StartGame_BRD,         MSG_Handler_StartGame_BRD       );
	AddHandler_Games( Games_Protocol, InitCards_BRD,         MSG_Handler_InitCards_BRD       );
	AddHandler_Games( Games_Protocol, Called_BRD,            MSG_Handler_CalledBank_BRD      );
	AddHandler_Games( Games_Protocol, CalledLicense_BRD,     MSG_Handler_CalledLicense_BRD   );
	AddHandler_Games( Games_Protocol, CreateBank_BRD,        MSG_Handler_CreateBank_BRD      );
	AddHandler_Games( Games_Protocol, DiscardsLicense_BRD,   MSG_Handler_DiscardsLicense_BRD );
	AddHandler_Games( Games_Protocol, Discards_BRD,          MSG_Handler_Discards_BRD        );
	AddHandler_Games( Games_Protocol, CalledOverTime_NAK,    MSG_Handler_CalledOvertime_NAK  );
	AddHandler_Games( Games_Protocol, DiscardsOverTime_NAK,  MSG_Handler_DiscardsOvertime_NAK);
	AddHandler_Games( Login_Protocol, Offline_BRD,           MSG_Handler_Offline_BRD         );
	AddHandler_Games( Games_Protocol, Reminder_ANC,          MSG_Handler_Reminder_ANC        );
	AddHandler_Games( Games_Protocol, Trusteeship_BRD,       MSG_Handler_Trusteeship_BRD     );
}


BOOL PacketHandler::AddHandler_Client( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Client * pFuncInfo = new FUNC_Client;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Client->Add( pFuncInfo );
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

VOID PacketHandler::ParsePacket_Client( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = ClientProtocol( pMsg, wSize );
    if ( pid != 0 ) {
        FUNC_Client * pFuncInfo = (FUNC_Client *)m_pFuncMap_Client->Find( pid );
        if (pFuncInfo) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

VOID PacketHandler::ParsePacket_Lobby( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert(NULL != pMsg);
    DWORD pid = GetProtocol( pMsg, wSize );
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
	DWORD pid = ServerProtocol( pMsg, wSize );
    if ( pid != 0 ) {
        FUNC_Games * pFuncInfo = (FUNC_Games *)m_pFuncMap_Games->Find( pid );
        if (pFuncInfo) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}
