#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


void MSG_Handler_Offline_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "Offline_BRD to recv: %s \n", pMsg );
}
