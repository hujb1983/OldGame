#ifndef _PACKET_HANDLER_H_INCLUDED_
#define _PACKET_HANDLER_H_INCLUDED_

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

	DWORD GetProtocol( MSG_BASE * szMsg, WORD wSize );

public:
	BOOL RegisterHandler();

	BOOL Register_Login ();
	BOOL Register_Lobby ();
	BOOL Register_Games ();
	BOOL Register_Database();

	BOOL AddHandler_Login   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Lobby   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Games   ( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler);

	void ParsePacket_Login   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void ParsePacket_Lobby   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void ParsePacket_Games   ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void ParsePacket_Database( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

private:
	struct FUNC_Login  : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	struct FUNC_Lobby : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	struct FUNC_Games : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	struct FUNC_Database : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_Login;
	FunctionMap	*	m_pFuncMap_Lobby;
	FunctionMap	*	m_pFuncMap_Games;
	FunctionMap	*	m_pFuncMap_Database;
};

extern PacketHandler g_PacketHandler;

#endif
