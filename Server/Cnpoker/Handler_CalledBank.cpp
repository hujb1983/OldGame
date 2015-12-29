#include "Handler_Module.h"
#include "CnpokerServer.h"


int Processed_Called ( int _nBattleid, BYTE _nSeatid, int _nUserkey, int _nCalltype )
{
    // -- ������
    if ( (_nSeatid<0) || (_nSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _nBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    // -- �ж��Ƿ����ڽе����׶�;
    if ( !pBattle->canCall() ) {
        return FALSE;
    }

    // -- �ж��Ƿ���һ���ӽе���;
    if ( pBattle->getCalled()!=_nSeatid ) {
        return FALSE;
    }

    // -- ����Ϊ��ʼ;
    if ( !pBattle->SetCalled( _nSeatid, _nCalltype ) ) {
        return FALSE;
    }

    // -- �жϽе����Ƿ���Ч;
    int _initcard = 0;
    if ( pBattle->checkCall()==FALSE ) {
        _initcard = 1;
    }

    // --���ɵ�����;
    int _createbarker = 0;
    if ( pBattle->canGame() ) {
        _createbarker = 1;
        pBattle->SetBattleStatus( eGB_PLAYING );
    }

    int _dmodel   = pBattle->getModel();
    int _multiple = pBattle->getMultiple();

    // @{{{ ������е���
    char szPlayerkey[256] = {0};
    pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

    char szMsg[1024] = {0};
    char format[256] = 	"{\"protocol\":\"%d\","
                        " \"initcard\": \"%d\", "
                        " \"create\": \"%d\", "
                        "%s,"
                        "\"multiple\":\"%d\","
                        "\"dmodel\":\"%d\","
                        "\"battleid\":\"%d\","
                        "\"calltype\":\"%d\","
                        "\"seatid\":\"%d\" }";

    sprintf( szMsg, format, MAKEDWORD( Games_Protocol, Called_BRD ),
            _initcard,         // ����Ҫ���·���;
            _createbarker,     // ��������;
            szPlayerkey,
            _multiple,         // ����;
            _dmodel,           // ģʽ;
            _nBattleid,        // �����!
            _nCalltype,        // Ҫ����?
            _nSeatid );        // ���ڽе�������;
    // }}}@ ������е���

    // @{{{ ���͵�������ң�
    WORD nLen = strlen( szMsg );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );
    // }}}@

    DEBUG_MSG( LVL_DEBUG, "CalledBank_REQ to agent: %s \n", (char *) szMsg  );
}

void MSG_Handler_CalledBank_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    DEBUG_MSG( LVL_DEBUG, "CalledBank_REQ to recv: %s \n", (char *) pMsg  );

    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _nBattleid(0), _nSeatid(0), _nUserkey(0), _nCalltype(0);
    js_map.ReadInteger("battleid",  _nBattleid);
    js_map.ReadInteger("seatid",    _nSeatid  );
    js_map.ReadInteger("userkey",   _nUserkey );
    js_map.ReadInteger("calltype",  _nCalltype );

    Processed_Called( _nBattleid, _nSeatid, _nUserkey, _nCalltype );
}

