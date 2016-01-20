#include "UserPacket.h"

UserPacket::UserPacket()
{
}

UserPacket::~UserPacket()
{
}

UINT UserPacket::GetTickCount() {  // 时针
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
}

void UserPacket::ToInit()   {   memset( this, 0x0, sizeof(UserPacket) );  }  // 清理

void UserPacket::SetPacket
(BYTE * byData, WORD wSize) {   memcpy( this, byData, wSize );      }  // 设置包结构

BYTE * UserPacket::GetPacket(BYTE * _data, WORD _size )   {  // 给一个指针并获得头指针;
    memcpy(_data, this, sizeof(UserPacket) );
    return _data;
}
WORD   UserPacket::GetPacketSize() {   // 取得这个包指针;
    return sizeof(UserPacket);
}

UINT & UserPacket::GetProtocol()        {  return (m_uiProtocol);   }       // 取得和设置协议号;
UINT & UserPacket::GetTokenKey()        {  return (m_uiTokenKey);   }       // 安全码;
UINT & UserPacket::GetValidTime()       {  return (m_uiValidTime);  }       // 取得有效时间;
UINT & UserPacket::GetValidCount()      {  return (m_uiValidCount); }       // 有效使用次数;
UINT & UserPacket::GetPortKey()         {  return (m_uiPortKey);    }       // 端口号
UINT & UserPacket::GetAgentKey()        {  return (m_uiAgentKey);   }       // 代理服务器

UINT & UserPacket::GetLoginTime()       {  return (m_uiLogin);      }       // 登录时间;
BYTE * UserPacket::GetSSHKey()          {  return (m_szSSHKey);     }      // 登录Key;
UINT & UserPacket::GetUserId()          {  return (m_uiUserId);     }       // 用户的Id;
UINT & UserPacket::GetUserKey()         {  return (m_uiPortKey);    }       // 用户的Id;
UINT & UserPacket::GetOldKey()          {  return (m_uiOldPortKey); }       // 用户以前用的Key;
CHAR * UserPacket::GetName()            {  return (m_szName);       }       // 取得有效时间;
INT  & UserPacket::GetMoney()           {  return (m_iMoney);       }       // 财富;
UINT & UserPacket::GetWoneds()          {  return (m_uiWoneds);     }       // 赢的次数;
UINT & UserPacket::GetFaileds()         {  return (m_uiFaileds);    }       // 失败的次数;
UINT & UserPacket::GetAways()           {  return (m_uiAways);      }       // 离开的次数;

WORD & UserPacket::GetBattleId()        {  return (m_wBattleId);    }       // 战斗序列;
BYTE & UserPacket::GetFieldId()         {  return (m_byFieldId);    }       // 场次号;
BYTE & UserPacket::GetRoomId()          {  return (m_byRoomId);     }       // 房间号;
UINT & UserPacket::GetTableId()         {  return (m_uiTableId);    }       // 桌子号;
BYTE & UserPacket::GetSeatId()          {  return (m_bySeatId);     }       // 坐位号;

BYTE & UserPacket::GetReady()           {  return (m_byReady);          }       // 是否准备好;
BYTE & UserPacket::GetCalled()          {  return (m_byCalled);         }       // 是否叫地主;
UINT & UserPacket::GetCalledTimes()     {  return (m_uiCallTimes);      }       // 叫地主的授权时间;
UINT & UserPacket::GetCalledTimesLast() {  return (m_uiCallTimesLast);  }       // 叫地主的限制时间;
BYTE & UserPacket::GetBanker()          {  return (m_byBanker);         }       // 是否地主;

BYTE & UserPacket::GetCount()           {  return (m_byCount);      }           // 计牌器;
BYTE * UserPacket::GetPokers()          {  return (m_byPokers);     }           // 手上的牌;

UINT & UserPacket::GetShowTimes()       {  return (m_uiShowTimes);      }       // 出牌时间;
UINT & UserPacket::GetShowTimesLast()   {  return (m_uiShowTimesLast);  }       // 出牌时间;
BYTE & UserPacket::GetShowMan()         {  return (m_byShowMan);        }       // 出牌人;
BYTE * UserPacket::GetShowPokers()      {  return (m_byShowPokers);     }       // 打出的牌;

UINT & UserPacket::GetShowedSize()      {  return (m_uiShowedCount);   }        // 是否出牌;
CHAR * UserPacket::GetShowedPokers()    {  return (m_byShowedPokers);  }        // 已经打出的牌;

CHAR * UserPacket::GetReminderPokers()  {  return (m_szReminderPokers); }        // 出牌提示;
BYTE & UserPacket::GetReminderSize()    {  return (m_byReminderSize);   }        // 出牌大小;

void UserPacket::ToPrint()
{
    DEBUG_MSG( LVL_DEBUG, "Begin");

    DEBUG_MSG( LVL_DEBUG, "| Protocol=%d | TokenKey=%d | ValidTime=%d | ValidCount=%d | PortKey=%d | AgentKey=%d |",
              m_uiProtocol, m_uiTokenKey, m_uiValidTime, m_uiValidCount, m_uiPortKey, m_uiAgentKey );

    DEBUG_MSG( LVL_DEBUG, "| LoginTime=%d |", m_uiLogin);

    DEBUG_MSG( LVL_DEBUG, "| UserId=%d | Name=%s | Money=%d | Woneds=%d | Faileds=%d | Aways=%d |",
              m_uiUserId, m_szName, m_iMoney, m_uiWoneds, m_uiFaileds, m_uiAways );

    DEBUG_MSG( LVL_DEBUG, "| BattleId=%d | FieldId=%d | RoomId=%d | TableId=%d |  SeatId=%d |",
              m_wBattleId, m_byFieldId, m_byRoomId, m_uiTableId, m_bySeatId );

    DEBUG_MSG( LVL_DEBUG, "| Ready=%d | Called=%d | Banker=%d |",
              m_byReady, m_byCalled, m_byBanker );

    DEBUG_MSG( LVL_DEBUG, "| ReminderSize=%d | ReminderPokers=%s |",
               m_byReminderSize, m_szReminderPokers );

    DEBUG_MSG( LVL_DEBUG, "End\n");
}



