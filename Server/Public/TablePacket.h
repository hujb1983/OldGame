#ifndef _TablePacket_H_INCLUDED_
#define _TablePacket_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "HeadPacket.h"

/*************************************/
#pragma pack(push, 1)

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
    UINT & GetBasicMoney();     // 最低金额;
    UINT & GetMultiple();       // 几倍;
    UINT & GetBrokerage();      // 佣金;

    UINT & GetTimeStep();       // 时间点;
    BYTE & GetPlaySeatId();     // 坐位号;

    BYTE & GetBankerId();           // 地主ID;
    BYTE & GetBasicPokerSize();     // 地主牌大小;
    CHAR * GetBasicPokers();       // 地主牌值;

    UINT & GetUserKey(BYTE);    // 用户入口值
    BYTE & GetPosition(BYTE);   // 职位，地主和玩家;
    BYTE & GetReady(BYTE);      // 准备好了;
    UINT & GetRate(BYTE);       // 胜率;
    int  & GetMoney(BYTE);      // 钱;
    int  & GetGain(BYTE);       // 获利;
    CHAR * GetName(BYTE);       // 名称

    BYTE & GetOnline(BYTE);         // 在线
    BYTE & GetTrusteeship(BYTE);    // 托管

    BYTE & GetPokerSize(BYTE);  // 牌数
    BYTE * GetPokers();         // 牌

    BYTE & GetLimitedTimes();           // 桌子限时;
    BYTE & GetCalledStatus(BYTE);       // 叫牌情况;
    BYTE & GetCalledType(BYTE seatid);  // 叫牌类型;

    BYTE & GetFirst();                  // 第一次出牌;
    BYTE & GetInitcards();              // 初始化出牌;

    BYTE & GetDiscardStatus(BYTE);      // 出牌情况;
    BYTE & GetDisplayPokerSize(BYTE);   // 出牌张数;
    CHAR * GetDisplayPokers(BYTE);      // 玩家打的牌;

    BYTE & GetThanSeatId();      // 出牌坐位;
    BYTE & GetThanType();        // 比较类型
    BYTE & GetThanValue();       // 比较值
    BYTE & GetThanCount();       // 比较个数
    BYTE & GetIndexSize();       // 出牌个数
    BYTE & GetThanPokerSize();   // 比较个数
    CHAR * GetThanPokers();      // 出牌个数

    BYTE & GetReminderPokerSize(BYTE);   // 出牌张数;
    CHAR * GetReminderPokers(BYTE);      // 玩家打的牌;

    BYTE & GetSettlement();      // 结算;

private:

    UINT  m_uiProtocol;         // 协议;
    UINT  m_uiTokenKey;         // 生成的返回锁;
    UINT  m_uiValidTime;        // 有效时间限制;
    UINT  m_uiValidCount;       // 有效访问记数;

    BYTE  m_byFieldId;          // 场次号;
    BYTE  m_byRoomId;           // 房间号;
    UINT  m_uiTableId;          // 桌子号;

    BYTE  m_byModel;            // 加倍模式;
    UINT  m_uiBasicMoney;       // 基本金额;
    UINT  m_uiMultiple;         // 加倍值;
    UINT  m_uiBrokerage;        // 佣金;

    UINT  m_uiTimeStep;         // 时间点
    BYTE  m_byPlaySeatId;       // 正在玩的人;

    BYTE  m_byBankerId;         // 地主的坐位号;
    BYTE  m_byBasicPokerSize;   // 底牌数
    CHAR  m_byBasicPokers[16];  // 底牌

    UINT m_uiUserKey[TEAM_SIZE];    // 用户入口值
    BYTE m_byPosition[TEAM_SIZE];   // 职位，空，地主和玩家
    BYTE m_byReady[TEAM_SIZE];      // 准备好了
    UINT m_uiRate[TEAM_SIZE];       // 胜率
    int  m_iMoney[TEAM_SIZE];       // 钱
    int  m_iGain[TEAM_SIZE];        // 获利
    CHAR m_szName[TEAM_SIZE][33];   // 名称

    BYTE m_byOnline[TEAM_SIZE];         // 在线
    BYTE m_byTrusteeship[TEAM_SIZE];    // 托管

    BYTE m_byPokerSize[TEAM_SIZE];    // 牌数
    BYTE m_szPokers[POKER_SIZE+1];    // 牌

    BYTE m_byLimitedTimes;              // 桌子限时状态
    BYTE m_byCalledStatus[TEAM_SIZE];   // 叫牌状态
    BYTE m_byCalledType[TEAM_SIZE];     // 出牌类型

    BYTE m_byInitCards;                 // 初始化牌
    BYTE m_byFirst;                     // 第一次出牌吗

    BYTE m_byDiscardStatus[TEAM_SIZE];  // 出牌状态
    BYTE m_byDisplayPokerSize[TEAM_SIZE];    // 打出的座位
    CHAR m_szDisplayPokers[TEAM_SIZE][80+1]; // 打出的牌

    BYTE m_byThanSeatId;      // 比较的坐位
    BYTE m_byThanType;
    BYTE m_byThanValue;
    BYTE m_byThanCount;
    BYTE m_byIndexSize;

    BYTE m_byThanPokerSize;    // 打出的座位
    CHAR m_szThanPokers[80+1]; // 打出的牌

    BYTE m_byReminderPokerSize[TEAM_SIZE];    // 打出的座位
    CHAR m_szReminderPokers[TEAM_SIZE][60+1]; // 打出的牌

    BYTE m_bySettlement;
};

#pragma pack(pop)
/*************************************/

#endif // _UserPacket_H_INCLUDED_
