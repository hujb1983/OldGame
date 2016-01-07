#include "LobbyPacket.h"

LobbyPacket::LobbyPacket()
{
}

LobbyPacket::~LobbyPacket()
{
}

void LobbyPacket::ToInit()   {   memset( this, 0x0, sizeof(this) );  }  // 清理
void LobbyPacket::SetPacket (BYTE * byData, WORD wSize){
    memcpy( this, byData, wSize );
}
BYTE * LobbyPacket::GetPacket(BYTE * _data, WORD _size )   {  // 给一个指针并获得头指针;
    memcpy( _data, this, sizeof(this) );
    return _data;
}
WORD   LobbyPacket::GetPacketSize() {   // 取得这个包指针;
    return sizeof(this);
}

UINT & LobbyPacket::GetProtocol()       {  return (m_uiProtocol);   }      // 取得和设置协议号;
UINT & LobbyPacket::GetTokenKey()       {  return (m_uiTokenKey);   }      // 安全码;
UINT & LobbyPacket::GetValidTime()      {  return (m_uiValidTime);  }      // 取得有效时间;
UINT & LobbyPacket::GetValidCount()     {  return (m_uiValidCount); }      // 有效使用次数;
UINT & LobbyPacket::GetPortkey()        {  return (m_uiPortkey);    }      // 端口号
UINT & LobbyPacket::GetAgentkey()       {  return (m_uiAgentkey);   }      // 代理服务器

// 在线人数
UINT & LobbyPacket::GetOnlines()        {  return (m_uiOnlines);    }      // 大厅的在线人数

// 场次的个数
// 场次的偏移
// 场次的指针
UINT & LobbyPacket::GetFieldSize()      {  return (m_sFieldSize);   }
UINT & LobbyPacket::GetFieldOffset()    {  return (m_sFieldOffset); }
BYTE * LobbyPacket::GetFields()         {
    if ( m_pData ) {
        return (m_pData + m_sFieldOffset);
    }
    return (NULL);
}

// 房间的个数
// 房间的偏移
// 房间的指针
UINT & LobbyPacket::GetRoomSize()       {  return (m_sRoomSize);    }
UINT & LobbyPacket::GetRoomOffset()     {  return (m_sRoomsOffset); }
BYTE * LobbyPacket::GetRooms()          {
    if ( m_pData ) {
        return (m_pData + m_sRoomsOffset);
    }
    return (NULL);
}

// 周报的个数
// 周报的偏移
// 周报的指针
UINT & LobbyPacket::GetWeekRankSize()       {  return (m_sWeekRankSize);    }
UINT & LobbyPacket::GetWeekRankOffset()     {  return (m_sWeekRankOffset);  }
BYTE * LobbyPacket::GetWeekRanks()          {
    if ( m_pData ) {
        return (m_pData + m_sWeekRankOffset);
    }
    return (NULL);
}

// 日间的个数
// 日间的偏移
// 日间的指针
UINT & LobbyPacket::GetDayRankSize()       {  return (m_sDayRankSize);      }
UINT & LobbyPacket::GetDayRankOffset()     {  return (m_sDayRankOffset);    }
BYTE * LobbyPacket::GetDayRanks()          {
    if ( m_pData ) {
        return (m_pData + m_sDayRankOffset);
    }
    return (NULL);
}

// 数据包
UINT & LobbyPacket::GetDataSize()           { return (m_pDataSize);         }
BYTE * LobbyPacket::GetData()               { return (m_pData);             }

// 打印所有参数
void LobbyPacket::ToPrint()
{

}



