#ifndef _PacketHandler_INCLUDED_
#define _PacketHandler_INCLUDED_

#include <Common.h>
#include <Network.h>
#include <Utility.h>
using namespace A;

#include "ServerSession.h"
typedef VOID (*fnHandler)( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

class PacketHandler
{
public:
	PacketHandler(void);
	~PacketHandler(void);

	DWORD GetProtocol( char * szMsg );

public:
	BOOL RegisterHandler();

	// Register
	void Register_Games();
	void Register_Agent();
	void Register_Lobby();
	void Register_Login();
	void Register_Database();

	// AddHandler
	BOOL AddHandler_Games   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Agent   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Lobby   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Login   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler);

	// ParsePacket
	void ParsePacket_Games   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_Agent   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_Lobby   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_Login   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_Database( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

private:
	struct FUNC_Games : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};
	struct FUNC_Agent : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};
	struct FUNC_Lobby : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};
	struct FUNC_Login : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};
	struct FUNC_Database : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_Games;
	FunctionMap	*	m_pFuncMap_Agent;
	FunctionMap	*	m_pFuncMap_Lobby;
	FunctionMap	*	m_pFuncMap_Login;
	FunctionMap	*	m_pFuncMap_Database;
};

extern PacketHandler g_PacketHandler;
#endif
