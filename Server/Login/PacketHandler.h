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

	DWORD GetProtocol( char * szMsg );

public:
	BOOL RegisterHandler();

	void Register_Client();
	void Register_Database();

	BOOL AddHandler_Client( WORD category, WORD protocol, fnHandler fnHandler);
	BOOL AddHandler_Database( WORD category, WORD protocol, fnHandler fnHandler);

	void ParsePacket_Client( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
	void ParsePacket_Database( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

private:
	struct FUNC_Client  : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	struct FUNC_Database : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_Client;
	FunctionMap	*	m_pFuncMap_Database;
};

extern PacketHandler g_PacketHandler;

#endif
