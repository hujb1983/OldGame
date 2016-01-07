#ifndef _UserPacket_H_INCLUDED_
#define _LobbyPacket_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "HeadPacket.h"

/*************************************/
#pragma pack(push, 1)

struct SField {
    SField() { memset(this, 0x0, sizeof(this)); }
    BYTE id;        // id号;
    BYTE tables;    // 房间数;
    char name[33];  // 名称;
};

struct SRoom {
    SRoom() { memset(this, 0x0, sizeof(SRoom)); }
    BYTE roomid;        // id号;
    BYTE tableid;       // 房间数;
    char name[33];      // 名称;
    char doub;          // 两倍模式;
    char onlines;       // 在线人数;
    char maxonlines;    // 最高在线人数;
    UINT money;         // 最低限制;
};

class LobbyPacket
{
public:
	LobbyPacket();
	~LobbyPacket();

    void ToInit();                  // 初始化
    void ToPrint();                 // 打印
    void SetPacket(BYTE *, WORD );  // 设置包结构


public:

    BYTE * GetPacket(BYTE *, WORD);   // 给一个指针并获得头指针;
    WORD   GetPacketSize();           // 取得这个包大小;

    UINT & GetProtocol();       // 取得和设置协议号;
    UINT & GetTokenKey();       // 安全码;
    UINT & GetValidTime();      // 取得有效时间;
    UINT & GetValidCount();     // 有效使用次数;
    UINT & GetPortkey();        // 端口号
    UINT & GetAgentkey();       // 代理服务器

    UINT & GetOnlines();        // 大厅的在线人数

    UINT & GetFieldSize();        // 场次个数
    UINT & GetFieldOffset();      // 场次偏移量
    BYTE * GetFields();           // 场次指针

    UINT & GetRoomSize();        // 房间的在线人数
    UINT & GetRoomOffset();      // 房间偏移量
    BYTE * GetRooms();           // 房间指针

    UINT & GetWeekRankSize();    // 周报个数
    UINT & GetWeekRankOffset();  // 周报偏移量
    BYTE * GetWeekRanks();       // 周报指针

    UINT & GetDayRankSize();     // 日报个数
    UINT & GetDayRankOffset();   // 日报偏移量
    BYTE * GetDayRanks();        // 日报指针

    UINT & GetDataSize();        // 数据大小
    BYTE * GetData();            // 获得缓存数据

private:

    UINT  m_uiProtocol;         // 协议;
    UINT  m_uiTokenKey;         // 生成的返回锁;
    UINT  m_uiValidTime;        // 有效时间限制;
    UINT  m_uiValidCount;       // 有效访问记数;
    UINT  m_uiPortkey;          // 入口键值;
    UINT  m_uiAgentkey;         // 代理键值;

    UINT  m_uiOnlines;         // 大厅的在线人数;

    UINT  m_sFieldSize;        // 场次的个数;
    UINT  m_sFieldOffset;      // 最大不能超过255个

    UINT  m_sRoomSize;         // 房间的个数;
    UINT  m_sRoomsOffset;      // 最大不能超过255个

    UINT  m_sWeekRankSize;      // 场次的个数;
    UINT  m_sWeekRankOffset;    // 最大不能超过10个

    UINT  m_sDayRankSize;       // 房间的个数;
    UINT  m_sDayRankOffset;     // 最大不能超过10个

    UINT  m_pDataSize;          // 数据大小
    BYTE * m_pData;             // 指针

};

#pragma pack(pop)
/*************************************/

#endif // _UserPacket_H_INCLUDED_
