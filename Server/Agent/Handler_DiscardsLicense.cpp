#include "Handler_Module.h"
#include <dbCommon.h>
#pragma pack(push,1)

/*****************************************************
    MSG_Handler_DiscardsLicense_BRD
*****************************************************/
void MSG_Handler_DiscardsLicense_BRD ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize>=sizeof(TablePacket) )
    {
        TablePacket table;
        table.SetPacket( (BYTE*)pMsg, wSize );
        table.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
        table.ToPrint();

        UINT userkey1 = table.GetUserKey(0);
        UINT userkey2 = table.GetUserKey(1);
        UINT userkey3 = table.GetUserKey(2);

        char szBuff[4069] = {0};
        WORD uiLength = 0;
        BYTE * pokers = table.GetPokers();
        if ( userkey1!=0 ){
            memset( szBuff, 0x0, sizeof(szBuff) );
            uiLength = table.JsonData(0, szBuff, sizeof(szBuff) );
            g_AgentServer->SendToClient( userkey1, (BYTE*)szBuff, uiLength );
            DEBUG_MSG( LVL_DEBUG, "%s", szBuff);
        }
        if ( userkey2!=0 ){
            memset( szBuff, 0x0, sizeof(szBuff) );
            uiLength = table.JsonData(1, szBuff, sizeof(szBuff) );
            g_AgentServer->SendToClient( userkey2, (BYTE*)szBuff, uiLength );
            DEBUG_MSG( LVL_DEBUG, "%s", szBuff);
        }
        if ( userkey3!=0 ){
            memset( szBuff, 0x0, sizeof(szBuff) );
            uiLength = table.JsonData(2, szBuff, sizeof(szBuff) );
            g_AgentServer->SendToClient( userkey3, (BYTE*)szBuff, uiLength );
            DEBUG_MSG( LVL_DEBUG, "%s", szBuff);
        }
    }
}
