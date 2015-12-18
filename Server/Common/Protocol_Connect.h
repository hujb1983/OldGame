#ifndef _PROTOCOLCA_CONNECT_H_INCLUDED_
#define _PROTOCOLCA_CONNECT_H_INCLUDED_

#include "Protocol.h"

/***** Client -> Login *****/
enum eCONNECT_PROTOCOL
{
	ClientType_SYN		= 100,
	ClientType_ANC		= 101,

	ServerType_SYN 		= 200,
	ServerType_ANC		= 201 ,

	Heartbeat_SYN		= 300,
	Heartbeat_ANC		= 301,
};

#endif // _PROTOCOLCA_H_INCLUDED_
