#include "Handler_Module.h"


void MSG_Handler_CalledOvertime_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG,  "CalledOvertime_NAK to recv: %s \n",  (char*)pMsg );
}
