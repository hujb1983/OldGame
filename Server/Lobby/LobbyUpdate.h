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

// 查询服务类型;
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

    void RoomInfo_REQ  (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize);      // 桌子信息
    void TableInfo_REQ (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize);      // 桌子信息

    void Onlines_REQ   (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize);        // 在线人数

    void WRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
    void DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );

    void RoomInfo_ANC  ( char * pMsg, WORD wSize );
    void TableInfo_ANC ( char * pMsg, WORD wSize );
    void WRankInfo_ANC ( char * pMsg, WORD wSize );
    void DRankInfo_ANC ( char * pMsg, WORD wSize );

    int Print();

private:

    // 查询
    int QueryTextInfo   ();   // 测试信息
    int QueryRoomInfo   ();   // 房间信息
    int QueryTableInfo  ();   // 桌面信息
    int QueryOnlineInfo ();   // 在线信息
    int QueryWeekRank   ();   // 周排行
    int QueryDayRank    ();   // 日排行

    int JsonRoomInfo   ( char * js_table, int js_size );                    // 返回所有房间信息
    int JsonTableInfo  ( int i, char * js_table, int js_size );             // 返回某场所有桌子信息
    int JsonOnlineInfo ( char * js_table, int js_size );                    // 返回在线人数
    int JsonWeekRank   ( char * js_table, int js_size );                    // 返回某场周排行
    int JsonDayRank    ( char * js_table, int js_size );                    // 返回某场日排行
    int JsonUpdateTableInfo( int r, int t, char * js_table, int js_size );  // 返回更新桌子的信息

private:
    // 同步数据;
    int SendOnlinesToAllUser();
    void SendUpdateTableOnlinesToAllUser();

    // 更新时间
    DWORD m_dwUpdateTime;       // 更新时间
    DWORD m_dwUpdateInterval;   // 更新间格

private:
    AgentSession * mapAgentObject[0xFFFF];
    AgentSessionList m_pAllAgent;

private:
    BYTE byRoomGroupCount;                // 大厅个数
    WORD wRoomGroupSize;                  // 字节长度
    BYTE szRoomGroupInfo[MAX_SZ];         // 房间信息

private:
    BYTE byTableOnlines[MAX_RM][MAX_TB];        // 记录每桌子的进出人数
    BYTE byTableGroupSunSize;                   // 桌子的总数量
    BYTE byRoomGroupTableSize[MAX_RM];          // 每个房间的桌子总数
    int  iTableGroupInfoSize[MAX_RM];           // 房间数据长度；
    char szTableGroupInfo[MAX_RM][MAX_SZ];      // 房间数据；

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
