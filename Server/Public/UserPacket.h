#ifndef _UserPacket_H_INCLUDED_
#define _UserPacket_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "HeadPacket.h"

/*************************************/
#pragma pack(push, 1)

class UserPacket
{
public:
    static UINT GetTickCount();     // 时间

public:

	UserPacket();
	~UserPacket();

    void ToInit();                  // 初始化
    void ToPrint();                 // 打印
    void SetPacket(BYTE *, WORD );  // 设置包结构

public:

    BYTE * GetPacket(BYTE *, WORD);   // 给一个指针并获得头指针;
    WORD   GetPacketSize();               // 取得这个包大小;

    UINT & GetProtocol();       // 取得和设置协议号;
    UINT & GetTokenKey();       // 安全码;
    UINT & GetValidTime();      // 取得有效时间;
    UINT & GetValidCount();     // 有效使用次数;
    UINT & GetPortKey();        // 端口号
    UINT & GetAgentKey();       // 代理服务器

    UINT & GetLoginTime();      // 登录时间;
    BYTE * GetSSHKey();         // 登录Key;
    UINT & GetUserId();         // 用户的ID;
    UINT & GetUserKey();        // 用户的Key;
    UINT & GetOldKey();         // 用户以前用的Key;
    CHAR * GetName();           // 取得有效时间;
    INT  & GetMoney();          // 财富;
    UINT & GetWoneds();         // 赢的次数;
    UINT & GetFaileds();        // 失败的次数;
    UINT & GetAways();          // 离开的次数;

    BYTE & GetFieldId();        // 场次号;
    BYTE & GetRoomId();         // 房间号;
    UINT & GetTableId();        // 房间键值;
    WORD & GetBattleId();       // 房间号;
    BYTE & GetSeatId();         // 坐位号;

    BYTE & GetReady();          // 是否准备好;
    BYTE & GetCalled();         // 是否叫地主;
    BYTE & GetBanker();         // 是否地主;
    BYTE & GetEnd();            // 是否结束;

    UINT & GetCalledTimes();        // 叫地主的授权时间;
    UINT & GetCalledTimesLast();    // 叫地主的限制时间;

    BYTE & GetCount();             // 计牌器;
    BYTE * GetPokers();            // 手上的牌;

    UINT & GetShowTimes();          // 出牌授权时间;
    UINT & GetShowTimesLast();      // 最后出牌限制时间;
    BYTE & GetShowMan();            // 是否出牌;
    BYTE * GetShowPokers();         // 打出的牌;

    UINT & GetShowedSize();        // 出牌大小;
    CHAR * GetShowedPokers();      // 已经打出的牌;

    CHAR * GetReminderPokers();    // 出牌提示;
    BYTE & GetReminderSize();      // 出牌大小;

private:

    UINT  m_uiProtocol;         // 协议;
    UINT  m_uiTokenKey;         // 生成的返回锁;
    UINT  m_uiValidTime;        // 有效时间限制;
    UINT  m_uiValidCount;       // 有效访问记数;
    UINT  m_uiPortKey;          // 入口键值;
    UINT  m_uiOldPortKey;       // 旧的入口键值;
    UINT  m_uiAgentKey;         // 代理键值;

    UINT  m_uiLogin;            // 登录时间;
    BYTE  m_szSSHKey[128];      // 登陆Key;
    UINT  m_uiUserId;           // 数据库编号;
    CHAR  m_szName[33];         // 名称
    INT   m_iMoney;             // 点数
    UINT  m_uiWoneds;           // 赢的次数
	UINT  m_uiFaileds; 	        // 失败次数
	UINT  m_uiAways; 		    // 逃跑次数

    WORD  m_wBattleId;          // 战斗序列;
    BYTE  m_byFieldId;          // 场次号;
    BYTE  m_byRoomId;           // 房间号;
    UINT  m_uiTableId;          // 桌子号;
    BYTE  m_bySeatId;           // 坐位号;

    BYTE  m_byReady;            // 是否准备好;
    BYTE  m_byCalled;           // 是否叫地主;
    UINT  m_uiCallTimes;        // 叫地主授权时间;
    UINT  m_uiCallTimesLast;    // 叫地主限制时间;
    BYTE  m_byBanker;           // 是否地主;
    BYTE  m_byFirst;            // 是否出牌;

    BYTE  m_byCount;            // 计牌器;
    BYTE  m_byPokers[20];       // 手上的牌;

    UINT  m_uiShowTimes;         // 出牌授权时间;
    UINT  m_uiShowTimesLast;     // 出牌限制时间;
    BYTE  m_byShowMan;           // 是否出牌;
    BYTE  m_byShowPokers[20];    // 打出的牌;

    UINT  m_uiShowedCount;          // 已经打出计牌;
    CHAR  m_byShowedPokers[80];     // 已经打出的牌;

    CHAR m_szReminderPokers[60];
    BYTE m_byReminderSize;
};

#pragma pack(pop)
/*************************************/

#endif // _UserPacket_H_INCLUDED_
