#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)


/*****************************************************
    MSG_Handler_Discards_REQ
*****************************************************/
void MSG_Handler_Discards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    UserSession * pSession = ( UserSession * ) pServerSession;
    {
        UserPacket & user = pSession->GetUserPacket();
        user.GetProtocol() = MAKEDWORD( Games_Protocol, Discards_REQ );

        int _count(0);
        char * _poker = user.GetShowedPokers();

        js_map.ReadInteger( "count",  _count );
        js_map.ReadString ( "poker",  _poker,  sizeof(_poker) );

        user.GetShowedSize() = _count;

        user.ToPrint();
        g_AgentServer->SendToGameServer( (BYTE*)&user, user.GetPacketSize() );
    }
}

/*****************************************************
    MSG_Handler_Discards_REQ
*****************************************************/
void MSG_Handler_Discards_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize>=sizeof(TablePacket) )
    {
        TablePacket table;
        table.SetPacket( (BYTE*)pMsg, wSize );
        table.GetProtocol() = MAKEDWORD( Games_Protocol, Discards_BRD );

        table.ToPrint();
        g_AgentServer->BroadcastToClient( table, (BYTE*)pMsg, wSize );
    }
}


