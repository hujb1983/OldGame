#ifndef __LobbyMessage_H_INCLUDED__
#define __LobbyMessage_H_INCLUDED__

#include <Common.h>
#include <Network.h>
#include <Utility.h>
#include <Public.h>
#include "ServerSession.h"
#include "AgentSession.h"
#include "LobbyUser.h"

#define DB_QUERY_SIZE       (1024)
#define DB_JSON_SIZE        (8192)

#define DB_QUERY_SUCC       0xFFFF
#define DB_QUERY_FAILED     0x0000

#define DB_QUERY_BUSY       0x0100
#define DB_QUERY_PARAM      0x0200

#define MAX_RM              (0xFF)
#define MAX_TB              (0xFF)
#define MAX_SZ              (0xFFF)

#define MINUTE              (60000)

// ��ѯ��������;
class LobbyUpdate
{
public:
    LobbyUpdate();
    ~LobbyUpdate();

    int Init();
    int Update(DWORD dwTicker);
    int SendAllUser(  BYTE * pMsg, WORD wSize );
    int SendToAgentSession( WORD wport, BYTE * pMsg, WORD wSize );

public:
    WORD AllocSessionKey();
    void FreeSessionKey(WORD _wIndex);

private:
    CObjKeyGenerator m_cObjKey;

public:
    void JoinAgentSession ( AgentSession * pServerSession );
    void QuitAgentSession ( AgentSession * pServerSession );

    void JoinLobby ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void QuitLobby ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

    void JoinTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void QuitTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

    void RoomInfo_REQ  (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize);      // ������Ϣ
    void TableInfo_REQ (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize);      // ������Ϣ

    void Onlines_REQ   (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize);        // ��������

    void WRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

    void RoomInfo_ANC  ( char * pMsg, WORD wSize );
    void TableInfo_ANC ( char * pMsg, WORD wSize );
    void WRankInfo_ANC ( char * pMsg, WORD wSize );
    void DRankInfo_ANC ( char * pMsg, WORD wSize );

    int Print();

private:

    // ��ѯ
    int QueryTextInfo   ();   // ������Ϣ
    int QueryRoomInfo   ();   // ������Ϣ
    int QueryTableInfo  ();   // ������Ϣ
    int QueryOnlineInfo ();   // ������Ϣ
    int QueryWeekRank   ();   // ������
    int QueryDayRank    ();   // ������

    int JsonRoomInfo   ( char * js_table, int js_size );                    // �������з�����Ϣ
    int JsonTableInfo  ( int i, char * js_table, int js_size );             // ����ĳ������������Ϣ
    int JsonOnlineInfo ( char * js_table, int js_size );                    // ������������
    int JsonWeekRank   ( char * js_table, int js_size );                    // ����ĳ��������
    int JsonDayRank    ( char * js_table, int js_size );                    // ����ĳ��������
    int JsonUpdateTableInfo( int r, int t, char * js_table, int js_size );  // ���ظ������ӵ���Ϣ

private:
    // ͬ������;
    int SendOnlinesToAllUser();
    void SendUpdateTableOnlinesToAllUser();

    // ����ʱ��
    DWORD m_dwUpdateTime;       // ����ʱ��
    DWORD m_dwUpdateInterval;   // ���¼��

private:
    AgentSession * mapAgentObject[0xFFFF];
    AgentSessionList m_pAllAgent;

private:
    BYTE byRoomGroupCount;                // ��������
    WORD wRoomGroupSize;                  // �ֽڳ���
    BYTE szRoomGroupInfo[MAX_SZ];         // ������Ϣ

private:
    BYTE byTableOnlines[MAX_RM][MAX_TB];        // ��¼ÿ���ӵĽ�������
    BYTE byTableGroupSunSize;                   // ���ӵ�������
    BYTE byRoomGroupTableSize[MAX_RM];          // ÿ���������������
    int  iTableGroupInfoSize[MAX_RM];           // �������ݳ��ȣ�
    char szTableGroupInfo[MAX_RM][MAX_SZ];      // �������ݣ�

private:
    int  iOnlinesInfo;
    int  iTableOnlinesInfo[6][100];

private:
    int  iWeekRankSize;
    char szWeekRank[4096];
    int  iDayRankSize;
    char szDayRank[4096];

};

extern LobbyUpdate g_LobbyUpdate;


#endif // __PRELOGIN_QUERY_H_INCLUDED__
