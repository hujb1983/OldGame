#include "Handler_Module.h"
#include "AgentServer.h"

/*****************************************************
    MSG_Handler_CalledBank_REQ
*****************************************************/
void MSG_Handler_Reminder_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    UserSession * pSession = ( UserSession * ) pServerSession;
    {
        UserPacket & user = pSession->GetUserPacket();
        user.GetProtocol() = MAKEDWORD( Games_Protocol, Reminder_REQ );
        user.ToPrint();
        g_AgentServer->SendToGameServer( (BYTE*)&user, user.GetPacketSize() );
    }
}

/*****************************************************
    MSG_Handler_Reminder_REQ
*****************************************************/
void MSG_Handler_Reminder_ANC ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize>=sizeof(UserPacket) )
    {
        UserPacket user;
        user.SetPacket( (BYTE*)pMsg, wSize );
        user.GetProtocol() = MAKEDWORD( Games_Protocol, Reminder_ANC );
        user.ToPrint();

        UINT userkey = user.GetUserKey();

        char szBuff[4069] = {0};
        int  uiLength = user.GetReminderSize();

        snprintf( szBuff, sizeof(szBuff), "{\"protocol\":%d,\"count\":%d,\"pokers\":[%s]}",
            user.GetProtocol(), uiLength, (uiLength>0)?(user.GetReminderPokers()):("-1") );

        int szLen = strlen(szBuff);
        g_AgentServer->SendToClient( userkey, (BYTE*)szBuff, szLen );
        DEBUG_MSG( LVL_DEBUG, "%s", szBuff);
    }
}

