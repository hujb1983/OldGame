#ifndef _Handler_Packet_H_INCLUDED_
#define _Handler_Packet_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"

typedef void (*fnHandler)( int cnSock, char * pInput, WORD wSize );

class Handler_Packet
{
public:
	Handler_Packet(void);
	
	~Handler_Packet(void);
	
    DWORD GetProtocol( char * szMsg );

public:
	BOOL RegisterHandler();
	
	BOOL Register_Agent ();
	
	BOOL AddHandler_Agent ( WORD category, WORD protocol, fnHandler fnHandler);
	
	void ParsePacket_Agent  ( int cnSock, char * pMsg, WORD wSize );

private:
	struct FUNC_Agent  : public BASE_FUNC {
		fnHandler	m_fnHandler;
	};

	FunctionMap	*	m_pFuncMap_Agent;
};

#endif // _AGENTPLAYER_H_INCLUDED_
