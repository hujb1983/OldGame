#include "Handler_Module.h"



void MSG_Handler_Onlines_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    g_LobbyUpdate.Onlines_REQ( pServerSession, pMsg, wSize );
}

void MSG_Handler_Onlines_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{

}
