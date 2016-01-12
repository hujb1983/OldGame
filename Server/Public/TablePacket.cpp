#include "TablePacket.h"

TablePacket::TablePacket()
{
}

TablePacket::~TablePacket()
{
}

void TablePacket::ToInit() {      // 清理
    memset( this, 0x0, sizeof(TablePacket) );
}

void TablePacket::SetPacket
(BYTE * byData, WORD wSize) {   memcpy( this, byData, wSize );      }  // 设置包结构

BYTE * TablePacket::GetPacket(BYTE * _data, WORD _size )   {  // 给一个指针并获得头指针;
    memcpy(_data, this, sizeof(TablePacket) );
    return _data;
}
WORD   TablePacket::GetPacketSize() {   // 取得这个包指针;
    return sizeof(TablePacket);
}

UINT & TablePacket::GetProtocol()        {  return (m_uiProtocol);   }       // 取得和设置协议号;
UINT & TablePacket::GetTokenKey()        {  return (m_uiTokenKey);   }       // 安全码;
UINT & TablePacket::GetValidTime()       {  return (m_uiValidTime);  }       // 取得有效时间;
UINT & TablePacket::GetValidCount()      {  return (m_uiValidCount); }       // 有效使用次数;

BYTE & TablePacket::GetFieldId()         {  return (m_byFieldId);    }       // 场次号;
BYTE & TablePacket::GetRoomId()          {  return (m_byRoomId);     }       // 房间号;
UINT & TablePacket::GetTableId()         {  return (m_uiTableId);    }       // 桌子号;

BYTE & TablePacket::GetModel()           {  return (m_byModel);      }       // 房间号;
UINT & TablePacket::GetMultiple()        {  return (m_uiMultiple);   }       // 桌子号;
UINT & TablePacket::GetBrokerage()       {  return (m_uiBrokerage);  }       // 坐位号;

UINT & TablePacket::GetTimeStep()        {  return (m_uiTimeStep);   }       // 时间点;
BYTE & TablePacket::GetPlaySeatId()      {  return (m_byPlaySeatId); }       // 坐位号;

BYTE & TablePacket::GetBankerId()        {  return (m_byBankerId);      }   // 地主ID;
BYTE & TablePacket::GetBasicPokerSize()  {  return (m_byBasicPokerSize);}   // 地主牌大小;
CHAR * TablePacket::GetBasicPokers()    {  return (m_byBasicPokers);   }   // 地主牌值;

UINT & TablePacket::GetUserKey(BYTE seatid)     {  return (m_uiUserKey[seatid]);    }       // 用户入口值
BYTE & TablePacket::GetPosition(BYTE seatid)    {  return (m_byPosition[seatid]);   }       // 职位，地主和玩家;
BYTE & TablePacket::GetReady(BYTE seatid)       {  return (m_byReady[seatid]);      }       // 准备好了;
UINT & TablePacket::GetRate(BYTE seatid)        {  return (m_uiRate[seatid]);       }       // 胜率;
int  & TablePacket::GetMoney(BYTE seatid)       {  return (m_iMoney[seatid]);       }       // 钱;
CHAR * TablePacket::GetName(BYTE seatid)        {  return (m_szName[seatid]);       }       // 名称;

BYTE & TablePacket::GetOnline(BYTE seatid)         {  return (m_byOnline[seatid]);        }   // 在线
BYTE & TablePacket::GetTrusteeship(BYTE seatid)    {  return (m_byTrusteeship[seatid]);   }   // 托管

BYTE & TablePacket::GetPokerSize(BYTE seatid)   {  return (m_byPokerSize[seatid]);    }     // 牌数
BYTE * TablePacket::GetPokers()                 {  return (m_szPokers);               }     // 牌

BYTE & TablePacket::GetLimitedTimes()               {  return (m_byLimitedTimes);             }     // 桌子限时;
BYTE & TablePacket::GetCalledStatus(BYTE seatid)    {  return (m_byCalledStatus[seatid]);     }     // 叫牌情况;
BYTE & TablePacket::GetCalledType(BYTE seatid)      {  return (m_byCalledType[seatid]);       }     // 叫牌情况;
BYTE & TablePacket::GetDiscardStatus(BYTE seatid)   {  return (m_byDiscardStatus[seatid]);    }     // 出牌情况;

BYTE & TablePacket::GetFirst()                      {  return (m_byFirst);                    }     // 第一次出牌;
BYTE & TablePacket::GetInitcards()                  {  return (m_byInitCards);                }     // 初始化出牌;

BYTE & TablePacket::GetDiscardSeatId()              {  return (m_byDiscardSeatId);            }     // 打出牌的玩家;
BYTE & TablePacket::GetDiscardPokerSize(BYTE seatid){  return (m_byDiscardPokerSize[seatid]); }     // 打出牌的玩家;
CHAR * TablePacket::GetDiscardPokers(BYTE seatid)   {  return (m_szDiscardPokers[seatid]);    }     // 玩家打的牌;

void TablePacket::ToPrint()
{
    DEBUG_MSG( LVL_DEBUG, "Begin");

    DEBUG_MSG( LVL_DEBUG, "| Protocol=%d | TokenKey=%d | ValidTime=%d | ValidCount=%d |",
                m_uiProtocol, m_uiTokenKey, m_uiValidTime, m_uiValidCount);

    DEBUG_MSG( LVL_DEBUG, "| FieldId=%d | RoomId=%d | TableId=%d |",
                m_byFieldId, m_byRoomId, m_uiTableId );

    DEBUG_MSG( LVL_DEBUG, "| Model=%d | Multiple=%d | Brokerage=%d | TimeStep=%d | SeatId=%d |",
                m_byModel, m_uiMultiple, m_uiBrokerage, m_uiTimeStep, m_byPlaySeatId );

    DEBUG_MSG( LVL_DEBUG, "| UserKey=%d | Position=%d | Ready=%d | Rate=%d | Money=%d | Name=%s | ",
                m_uiUserKey[0],  m_byPosition[0], m_byReady[0], m_uiRate[0], m_iMoney[0], m_szName[0] );

    DEBUG_MSG( LVL_DEBUG, "| UserKey=%d | Position=%d | Ready=%d | Rate=%d | Money=%d | Name=%s |",
                m_uiUserKey[1], m_byPosition[1], m_byReady[1], m_uiRate[1], m_iMoney[1], m_szName[1] );

    DEBUG_MSG( LVL_DEBUG, "| UserKey=%d | Position=%d | Ready=%d | Rate=%d | Money=%d | Name=%s |",
                m_uiUserKey[2], m_byPosition[2], m_byReady[2], m_uiRate[2], m_iMoney[2], m_szName[2] );

    DEBUG_MSG( LVL_DEBUG, "| CalledStatus=%d | CalledType=%d | DiscardPokers=%s |",
                m_byCalledStatus[0], m_byCalledType[0], m_szDiscardPokers[0] );

    DEBUG_MSG( LVL_DEBUG, "| CalledStatus=%d | CalledType=%d | DiscardPokers=%s |",
                m_byCalledStatus[1], m_byCalledType[1], m_szDiscardPokers[1] );

    DEBUG_MSG( LVL_DEBUG, "| CalledStatus=%d | CalledType=%d | DiscardPokers=%s |",
                m_byCalledStatus[2], m_byCalledType[2], m_szDiscardPokers[2] );

    DEBUG_MSG( LVL_DEBUG, "End\n");
}

// 客户端包
char * TablePacket::ClientData( BYTE bMainSeatid, BYTE bSeatid, char * szBuff, WORD wSize )
{
    memset(szBuff, 0x0, wSize);
    if ( GetPosition(bSeatid)!=0 ) {

        BYTE byCount = GetDiscardPokerSize(bSeatid);
        char * poker = NULL;
        if (bMainSeatid==bSeatid) {
            poker = GetDiscardPokers(bSeatid);
        }

        snprintf( szBuff, wSize, "{ \"show\":%s,"        // 自己
                                   "\"ready\":%s,"       // 准备
                                   "\"name\":\"%s\","    // 姓名
                                   "\"rate\":%d,"        // 胜率
                                   "\"money\":%d,"       // 钱
                                   "\"online\":%s,"      // 状态:
                                   "\"trusteeship\":%s," // 托管:
                                   "\"called\":%s,"      // 叫地主
                                   "\"calltype\":%d,"    // 叫地主:分数（0|1）
                                   "\"discard\":%s,"     // 出牌：
                                   "\"count\":%d,"
                                   "\"pokers\":[%s]"
                                 "}",
                 "true",
                 (GetReady(bSeatid)!=PK_READY)? "false":"true",
                 GetName(bSeatid),
                 GetRate(bSeatid),
                 GetMoney(bSeatid),
                 (GetOnline(bSeatid)!=PK_ONLINE)? "false":"true",
                 (GetTrusteeship(bSeatid)!=PK_TRUSTEESHIP)? "false":"true",
                 (GetPlaySeatId()!=bSeatid)?"false":"true",   // 叫的地主
                 GetCalledStatus(bSeatid),
                 (GetPlaySeatId()!=bSeatid)?"false":"true",   // 出牌
                 byCount,   // 个人手牌长度
                 (poker)?(poker):("-1") );
    }
    else
    {
        snprintf( szBuff, wSize, "{\"show\":false,"     // 其他玩家参数
                                  "\"ready\":false,"    // 已经准备
                                  "\"name\":\"-\","     // 名称
                                  "\"rate\":0,"         // 胜率
                                  "\"money\":0,"        // 钱
                                  "\"online\":false,"       // 状态
                                  "\"trusteeship\":false,"  // 托管
                                  "\"called\":false,"       // 叫地主
                                  "\"calltype\":0,"
                                  "\"discard\":false,"      // 出牌
                                  "\"count\":0,"            // 长度
                                  "\"pokers\":[-1]"         // 出牌字符串
                                  "}" );
    }
    return (szBuff);
}

UINT TablePacket::JsonData(BYTE bSeatid, char * szBuff, WORD wSize )    // Json数据打包
{
    char _protocol[32] = {0};

    szBuff[0] = '\0';
    snprintf( _protocol, sizeof(_protocol), "{\"protocol\":\"%d\",", m_uiProtocol);
    strcat( szBuff, _protocol);

    char _data[256] = {0};
    snprintf( _data, sizeof(_data), "\"data\":[{"
                                    "\"battleid\":%d,"
                                    "\"dmodel\":%s,"
                                    "\"seatid\":%d,"
                                    "\"basics\":%d,"
                                    "\"multiple\":%d,"
                                    "\"brokerage\":%d,"
                                    "\"first\":%s,"
                                    "\"banker\":%d,"
                                    "\"times\":%d,"            // 倒计时间
                                    "\"count\":%d,"         // 地主牌数
                                    "\"pokers\":[%s],",      // 地主牌
             m_uiTableId, (m_byModel==0)?"false":"true", bSeatid, m_BasicMoney, m_uiMultiple, m_uiBrokerage,
             (GetFirst()==0)?"true":"false", GetBankerId(), GetLimitedTimes(), GetBasicPokerSize(), GetBasicPokers() );

    strcat( szBuff, _data);

    char _show[1024] = {0};
    strcat( _show, "\"seatinfo\":[" );
    {
        char * _ptr = NULL;
        char _buff[256] = {0};
        _ptr = ClientData( bSeatid, 0, _buff, sizeof(_buff) );
        strcat( _show, _ptr );
        strcat( _show, "," );
        _ptr = ClientData( bSeatid, 1, _buff, sizeof(_buff) );
        strcat( _show, _ptr );
        strcat( _show, "," );
        _ptr = ClientData( bSeatid, 2, _buff, sizeof(_buff) );
        strcat( _show, _ptr );
    }
    strcat( _show, "]" );
    strcat( szBuff, _show);
    strcat( szBuff, "}]}");

    return strlen( szBuff );
}

