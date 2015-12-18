#include "Handler_Module.h"
#include "CnpokerServer.h"


void MSG_Handler_CreateBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _nBattleid = 0;
    js_map.ReadInteger("battleid",  _nBattleid);

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return;
    }

    // -- �ж��Ƿ��Ѿ������ˣ�׼�����˷��ƣ�
    /* if ( !pBattle->canGame() ) {
        return;
    } */

    BYTE _bySeatid = pBattle->getBanker();

    char szMsg[1024] = {0};
    char poker[60]  = {0};
    char szPlayerkey[256] = {0};
    pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

    char format[256] = 	"{\"protocol\":\"%d\","
                        "%s,"
                        "\"data\":[{"
                            "\"battleid\":\"%d\","
                            "\"banker\":\"%d\","
                            "\"poker\":\"%s\" }] }";

    pBattle->getBasecards( _bySeatid, poker, sizeof(poker) );
    sprintf( szMsg, format,
            MAKEDWORD( Games_Protocol, CreateBank_BRD ),
            szPlayerkey,
            pBattle->getIndex(),
            _bySeatid,
            poker );
    // }}}@ ������е���

    // @{{{ ���͵�������ң�
    WORD nLen = strlen( szMsg );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );

    // �������
    pBattle->SetPlaying( _bySeatid );
    pBattle->SetBattleStatus( eGB_PLAYING );
}

