#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"
#include "ServerSession.h"
#include "LoginServer.h"


/*  Ô¤µÇÂ½  */
void MSG_Handler_PreLogin_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_PreLogin_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
void MSG_Handler_PreLogin_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );


#endif //
