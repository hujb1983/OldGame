#ifndef _TablePacket_H_INCLUDED_
#define _TablePacket_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "HeadPacket.h"

/*************************************/
#pragma pack(push, 1)

#define TEAM_SIZE (3)

class TablePacket
{
public:
	TablePacket();
	~TablePacket();

    void ToInit();                  // 初始化
    void ToPrint();                 // 打印
    void SetPacket(BYTE *, WORD );  // 设置包结构

public:
    UINT JsonData(BYTE bSeatid, char * szBuff, WORD wSize );    // Json数据打包
    char * ClientData(BYTE bMainSeatid, BYTE bSeatid, char * szBuff, WORD wSize );  // Client-Json数据打包

    BYTE * GetPacket(BYTE *, WORD);   // 给一个指针并获得头指针;
    WORD   GetPacketSize();           // 取得这个包大小;

    UINT & GetProtocol();       // 取得和设置协议号;
    UINT & GetTokenKey();       // 安全码;
    UINT & GetValidTime();      // 取得有效时间;
    UINT & GetValidCount();     // 有效使用次数;

    BYTE & GetFieldId();        // 场次号;
    BYTE & GetRoomId();         // 房间号;
    UINT & GetTableId();        // 房间键值;
    WORD & GetBattleId();       // 房间号;

    BYTE & GetModel();          // 模式;
    BYTE & GetBasicMoney();     // 最低金额;
    UINT & GetMultiple();       // 几倍;
    UINT & GetBrokerage();      // 佣金;

    UINT & GetTimeStep();       // 时间点;
    BYTE & GetPlaySeatId();     // 坐位号;

    UINT & GetUserKey(BYTE);    // 用户入口值
    BYTE & GetPosition(BYTE);   // 职位，地主和玩家;
    BYTE & GetReady(BYTE);      // 准备好了;
    UINT & GetRate(BYTE);       // 胜率;
    int  & GetMoney(BYTE);      // 钱;
    CHAR * GetName(BYTE);       // 名称

private:

    UINT  m_uiProtocol;         // 协议;
    UINT  m_uiTokenKey;         // 生成的返回锁;
    UINT  m_uiValidTime;        // 有效时间限制;
    UINT  m_uiValidCount;       // 有效访问记数;

    BYTE  m_byFieldId;          // 场次号;
    BYTE  m_byRoomId;           // 房间号;
    UINT  m_uiTableId;          // 桌子号;

    BYTE  m_byModel;            // 加倍模式;
    UINT  m_BasicMoney;         // 基本金额;
    UINT  m_uiMultiple;         // 加倍值;
    UINT  m_uiBrokerage;        // 佣金;

    UINT  m_uiTimeStep;         // 时间点
    BYTE  m_byPlaySeatId;       // 正在玩的人;

    UINT m_uiUserKey[TEAM_SIZE];    // 用户入口值
    BYTE m_byPosition[TEAM_SIZE];   // 职位，空，地主和玩家
    BYTE m_byReady[TEAM_SIZE];      // 准备好了
    UINT m_uiRate[TEAM_SIZE];       // 胜率
    int  m_iMoney[TEAM_SIZE];       // 钱
    CHAR m_szName[TEAM_SIZE][33];   // 名称

};

#pragma pack(pop)
/*************************************/

#endif // _UserPacket_H_INCLUDED_
