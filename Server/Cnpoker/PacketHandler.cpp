#include "PacketHandler.h"
#include "Handler_Module.h"

/*  1. 游戏的命令转化格式
    2. ...
    */
PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
    m_pFuncMap_Agent    = new FunctionMap;
	m_pFuncMap_Database = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
    SAFE_DELETE(m_pFuncMap_Agent);
	SAFE_DELETE(m_pFuncMap_Database);
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

BOOL PacketHandler::RegisterHandler()
{
    Register_Agent();
    Register_Database();
	return TRUE;
}

BOOL PacketHandler::Register_Agent()
{
	AddHandler_Agent( Games_Protocol,  JoinTable_ANC,       MSG_Handler_JoinTable_ANC   );
	AddHandler_Agent( Games_Protocol,  StartGame_REQ,       MSG_Handler_StartGame_REQ   );
	AddHandler_Agent( Games_Protocol,  Called_REQ,          MSG_Handler_CalledBank_REQ  );
	AddHandler_Agent( Games_Protocol,  Discards_REQ,        MSG_Handler_Discards_REQ    );
	AddHandler_Agent( Games_Protocol,  Trusteeship_REQ,     MSG_Handler_Trusteeship_REQ );
	AddHandler_Agent( Games_Protocol,  QuitTable_REQ,       MSG_Handler_QuitTable_REQ   );
    AddHandler_Agent( Games_Protocol,  Settlement_REQ,      MSG_Handler_Settlement_REQ  );
	AddHandler_Agent( Games_Protocol,  Reminder_REQ,        MSG_Handler_Reminder_REQ    );
	// AddHandler_Agent( Login_Protocol,  Offline_NAK,         MSG_Handler_Offline_NAK    );
	// AddHandler_Agent( Games_Protocol,  DiscardsLicense_REQ, MSG_Handler_DiscardsLicense_REQ );
}

BOOL PacketHandler::Register_Database()
{

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
	DWORD pid = GetProtocol( pMsg, wSize );
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
	DWORD pid = GetProtocol( pMsg, wSize );
    if ( pid != 0 ) {
        FUNC_Database * pFuncInfo = (FUNC_Database *)m_pFuncMap_Database->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

