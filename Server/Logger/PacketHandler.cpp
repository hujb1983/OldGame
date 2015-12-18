#include "PacketHandler.h"
#include "Handler_Module.h"

PacketHandler g_PacketHandler;

PacketHandler::PacketHandler(void)
{
	m_pFuncMap_Games    = new FunctionMap;
	m_pFuncMap_Agent    = new FunctionMap;
	m_pFuncMap_Lobby    = new FunctionMap;
	m_pFuncMap_Login    = new FunctionMap;
	m_pFuncMap_Database = new FunctionMap;
	RegisterHandler();
}

PacketHandler::~PacketHandler(void)
{
	SAFE_DELETE(m_pFuncMap_Games);
	SAFE_DELETE(m_pFuncMap_Agent);
	SAFE_DELETE(m_pFuncMap_Lobby);
	SAFE_DELETE(m_pFuncMap_Login);
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
	Register_Games();
	Register_Agent();
	Register_Lobby();
	Register_Login();
	Register_Database();
	return TRUE;
}

void PacketHandler::Register_Games()
{

}

void PacketHandler::Register_Agent()
{

}

void PacketHandler::Register_Lobby()
{

}

void PacketHandler::Register_Login()
{

}

void PacketHandler::Register_Database()
{

}

BOOL PacketHandler::AddHandler_Games   ( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Games * pFuncInfo	= new FUNC_Games;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Games->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Agent   ( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Agent * pFuncInfo	= new FUNC_Agent;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Agent->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Lobby   ( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Login * pFuncInfo	= new FUNC_Login;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Login->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Login   ( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Login * pFuncInfo	= new FUNC_Login;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Login->Add( pFuncInfo );
}

BOOL PacketHandler::AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler)
{
	FUNC_Database * pFuncInfo	= new FUNC_Database;
	pFuncInfo->m_dwFunctionKey	= MAKELONG( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_Database->Add( pFuncInfo );
}

// ParsePacket
void PacketHandler::ParsePacket_Games   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
	assert( NULL != pMsg );
	printf("[Login] PacketHandler::ParsePacket_Client \n");

	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Client * pFuncInfo = (FUNC_Client *)m_pFuncMap_Client->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

void PacketHandler::ParsePacket_Agent   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    assert( NULL != pMsg );
	printf("[Login] PacketHandler::ParsePacket_Client \n");

	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Agent * pFuncInfo = (FUNC_Agent *)m_pFuncMap_Agent->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

void PacketHandler::ParsePacket_Lobby   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    assert( NULL != pMsg );
	printf("[Login] PacketHandler::ParsePacket_Client \n");

	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Lobby * pFuncInfo = (FUNC_Lobby *)m_pFuncMap_Client->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

void PacketHandler::ParsePacket_Login   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    assert( NULL != pMsg );
	printf("[Login] PacketHandler::ParsePacket_Client \n");

	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Client * pFuncInfo = (FUNC_Client *)m_pFuncMap_Client->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}

void PacketHandler::ParsePacket_Database( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    assert( NULL != pMsg );
	printf("[Login] PacketHandler::ParsePacket_Client \n");

	DWORD pid = GetProtocol( (char*)pMsg );
    if ( pid != 0 ) {
        FUNC_Client * pFuncInfo = (FUNC_Client *)m_pFuncMap_Client->Find( pid );
        if ( pFuncInfo ) {
            pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
        }
    }
}
