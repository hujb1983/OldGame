#include "Handler_Module.h"
#include "CnpokerServer.h"

/*
    ��ʼ�����ñȽ����ɣ�
*/
int Processed_StartGame ( ServerSession * pServerSession, const char * pInput )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return -1;
    }

    int _nBattleid = 0;
    int _nSeatid   = 0;
    int _nUserkey  = 0;
    int _nStatus   = 0;

    js_map.ReadInteger("battleid",  _nBattleid);
    js_map.ReadInteger("seatid",    _nSeatid  );
    js_map.ReadInteger("userkey",   _nUserkey );
    js_map.ReadInteger("status",    _nStatus );

    // @{{{ ������
    if ( (_nSeatid<0) || (_nSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    // -- ����Ϊ�ѿ�ʼ  _nStatus �����ж��Ǳ�׼��ʼ��������ʾ��ʼ
    if ( !pBattle->SetStart( _nSeatid, _nStatus ) ) {
        return FALSE;
    }

    int _nReady = 0;
    if ( pBattle->canStart() ) {
        _nReady = 1;
        pBattle->SetBattleStatus( eGB_DEALING );
    }
    // }}}@ ������

    // ��������ʼ
    {
        char szMsg[1024] = {0};
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        // @{{{ ������е���
        char format[256] = 	"{\"protocol\":\"%d\","
                            " \"ready\":\" %d \","
                            "%s,"
                            "\"data\":[{"
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"status\":\"%d\"}] }";

        sprintf( szMsg, format, MAKEDWORD( Games_Protocol, StartGame_BRD ),
                1,
                szPlayerkey,
                pBattle->getIndex(),
                _nSeatid,
                _nStatus );     // ���ƿ�ʼ
        // }}}@ ������е���

        // @{{{ ���͵�������ң�
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );
        return TRUE;
    }

	// }}}@ ���͵��������;
}

void MSG_Handler_StartGame_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    Processed_StartGame(pServerSession, (char*) pMsg);
}
