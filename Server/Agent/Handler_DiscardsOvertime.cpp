#include "Handler_Module.h"

void MSG_Handler_DiscardsOvertime_NAK ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    // �Զ���ʼ��Ϸ
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userkey1  = 0;
    int _userkey2  = 0;
    int _userkey3  = 0;

    js_map.ReadInteger( "userkey1",  _userkey1 );
    js_map.ReadInteger( "userkey2",  _userkey2 );
    js_map.ReadInteger( "userkey3",  _userkey3 );

    if (_userkey1 != 0) {
        g_AgentServer->SendToClient( _userkey1, (BYTE*)pMsg, wSize );
    }

    if (_userkey2 != 0) {
        g_AgentServer->SendToClient( _userkey2, (BYTE*)pMsg, wSize );
    }

    if (_userkey3 != 0) {
        g_AgentServer->SendToClient( _userkey3, (BYTE*)pMsg, wSize );
    }

    {
        JsonMap js_data;
        js_map.ReadJson( "data", js_data );

        int iSize = js_data.GetArraySize();
        if ( iSize == 1)
        {
            int _battleid = 0;
            JsonMap js_array;
            js_data.ReadArrayJson( 0, js_array );
            js_array.ReadInteger( "battleid", _battleid );

            char szMsg[1024] = {0};

            // @{{{ ������е���
            char format[256] = 	"{\"protocol\":\"%d\", \"battleid\":\"%d\" }" ;

            sprintf( szMsg, format, MAKEDWORD( Games_Protocol, DiscardsLicense_REQ ), _battleid );
            // }}}@ ������е���

            // @{{{ ���͵�������ң�
            WORD nLen = strlen( szMsg );
            g_AgentServer->SendToGameServer( (BYTE*)szMsg, nLen );
        }
    }
}
