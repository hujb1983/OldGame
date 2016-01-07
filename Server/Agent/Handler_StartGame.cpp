#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

// 取得在游戏的用户信息
void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {

    UserSession * pSession = ( UserSession * ) pServerSession;
    {
        UserPacket pack = pSession->GetUserPacket();
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, StartGame_REQ );
        pack.GetReady() = 1;
        pack.ToPrint();
        g_AgentServer->SendToGameServer( (BYTE*)&pack, pack.GetPacketSize() );
    }
}

void MSG_Handler_StartGame_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
   if ( wSize>=sizeof(TablePacket) )
    {
        TablePacket table;
        table.SetPacket( (BYTE*)pMsg, wSize );
        table.GetProtocol() = MAKEDWORD( Games_Protocol, JoinGame_BRD );
        table.ToPrint();

        UINT userkey1 = table.GetUserKey(0);
        UINT userkey2 = table.GetUserKey(1);
        UINT userkey3 = table.GetUserKey(2);

        WORD uiLength = 0;
        if ( userkey1!=0 ){
            char szBuff[1024] = {0};
            uiLength = table.JsonData(0, szBuff, sizeof(szBuff) );
            g_AgentServer->SendToClient( userkey1, (BYTE*)szBuff, uiLength );
        }
        if ( userkey2!=0 ){
            char szBuff[1024] = {0};
            uiLength = table.JsonData(1, szBuff, sizeof(szBuff) );
            g_AgentServer->SendToClient( userkey2, (BYTE*)szBuff, uiLength );
        }
        if ( userkey3!=0 ){
            char szBuff[1024] = {0};
            uiLength = table.JsonData(2, szBuff, sizeof(szBuff) );
            g_AgentServer->SendToClient( userkey3, (BYTE*)szBuff, uiLength );
        }
    }
}

