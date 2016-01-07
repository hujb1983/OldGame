#include "HeadPacket.h"

HeadPacket::HeadPacket()
{
}

HeadPacket::~HeadPacket()
{
}

void HeadPacket::ToInit()   {   memset( this, 0x0, sizeof(HeadPacket) );  }  // 清理

void HeadPacket::SetPacket
(BYTE * byData, WORD wSize) {   memcpy( this, byData, wSize );      }  // 设置包结构


UINT & HeadPacket::GetProtocol()        {  return (m_uiProtocol);   }       // 取得和设置协议号;
UINT & HeadPacket::GetTokenKey()        {  return (m_uiTokenKey);   }       // 安全码;
UINT & HeadPacket::GetValidTime()       {  return (m_uiValidTime);  }       // 取得有效时间;
UINT & HeadPacket::GetValidCount()      {  return (m_uiValidCount); }       // 有效使用次数;
UINT & HeadPacket::GetPortKey()         {  return (m_uiPortKey);    }       // 端口号
UINT & HeadPacket::GetAgentKey()        {  return (m_uiAgentKey);   }       // 代理服务器

void HeadPacket::ToPrint()
{
    DEBUG_MSG( LVL_DEBUG, "Begin");

    DEBUG_MSG( LVL_DEBUG, "| Protocol=%d | TokenKey=%d | ValidTime=%d | ValidCount=%d | PortKey=%d | AgentKey=%d |",
              m_uiProtocol, m_uiTokenKey, m_uiValidTime, m_uiValidCount, m_uiPortKey, m_uiAgentKey );

    DEBUG_MSG( LVL_DEBUG, "End\n");
}



