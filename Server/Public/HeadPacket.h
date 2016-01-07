#ifndef _HeadPacket_H_INCLUDED_
#define _HeadPacket_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>

/*************************************/
#pragma pack(push, 1)

class HeadPacket
{
public:
    static UINT GetTickCount();     // 时间

public:

	HeadPacket();
	~HeadPacket();

    void ToInit();                  // 初始化
    void ToPrint();                 // 打印
    void SetPacket(BYTE *, WORD );  // 设置包结构

public:

    UINT & GetProtocol();       // 取得和设置协议号;
    UINT & GetTokenKey();       // 安全码;
    UINT & GetValidTime();      // 取得有效时间;
    UINT & GetValidCount();     // 有效使用次数;
    UINT & GetPortKey();        // 端口号
    UINT & GetAgentKey();       // 代理服务器

private:

    UINT  m_uiProtocol;         // 协议;
    UINT  m_uiTokenKey;         // 生成的返回锁;
    UINT  m_uiValidTime;        // 有效时间限制;
    UINT  m_uiValidCount;       // 有效访问记数;
    UINT  m_uiPortKey;          // 入口键值;
    UINT  m_uiOldPortKey;       // 旧的入口键值;
    UINT  m_uiAgentKey;         // 代理键值;
};

#pragma pack(pop)
/*************************************/

#endif // _UserPacket_H_INCLUDED_
