#include "Handler_Module.h"

void MSG_Handler_DiscardsOvertime_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG,  "DiscardsOvertime_NAK to recv: %s \n",  (char*)pMsg );
}
