#include "PacketHandler.h"
#include "Handler_Module.h"

PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
	m_pFuncMap_Client   = new FunctionMap;
	m_pFuncMap_Database = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_Client);
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
    if (pid==0) {
        DEBUG_MSG( LVL_ERROR, "LoginServer PacketHandler::GetProtocol %s ", szMsg );
    }
    return (DWORD) pid;
}

BOOL PacketHandler::RegisterHandler()
{
	Register_Client();
	Register_Database();
	return TRUE;
}

void PacketHandler::Register_Client()
{
	AddHandler_Client( Login_Protocol,   PreLogin_REQ,  MSG_Handler_PreLogin_REQ ); // 预登陆
}

void PacketHandler::Register_Database()
{
	AddHandler_Database( Login_Protocol, PreLogin_ANC,  MSG_Handler_PreLogin_ANC );
	AddHandler_Database( Login_Protocol, PreLogin_NAK,  MSG_Handler_PreLogin_NAK );
}

BOOL PacketHandler::AddHandler_Client( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Client * pFuncInfo	= new FUNC_Client;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Client->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Database * pFuncInfo	= new FUNC_Database;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Database->Add( pFuncInfo );
}

VOID PacketHandler::ParsePacket_Client( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
	assert( NULL != pMsg );
	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Client * pFuncInfo = (FUNC_Client *)m_pFuncMap_Client->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

VOID PacketHandler::ParsePacket_Database( ServerSession * pSession, MSG_BASE * pMsg, WORD wSize )
{
    assert( NULL != pMsg );
	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Database * pFuncInfo = (FUNC_Database *)m_pFuncMap_Database->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}
