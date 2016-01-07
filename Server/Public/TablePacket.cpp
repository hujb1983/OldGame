#include "TablePacket.h"

TablePacket::TablePacket()
{
}

TablePacket::~TablePacket()
{
}

void TablePacket::ToInit() {      // ����
    memset( this, 0x0, sizeof(TablePacket) );
}

void TablePacket::SetPacket
(BYTE * byData, WORD wSize) {   memcpy( this, byData, wSize );      }  // ���ð��ṹ

BYTE * TablePacket::GetPacket(BYTE * _data, WORD _size )   {  // ��һ��ָ�벢���ͷָ��;
    memcpy(_data, this, sizeof(TablePacket) );
    return _data;
}
WORD   TablePacket::GetPacketSize() {   // ȡ�������ָ��;
    return sizeof(TablePacket);
}

UINT & TablePacket::GetProtocol()        {  return (m_uiProtocol);   }       // ȡ�ú�����Э���;
UINT & TablePacket::GetTokenKey()        {  return (m_uiTokenKey);   }       // ��ȫ��;
UINT & TablePacket::GetValidTime()       {  return (m_uiValidTime);  }       // ȡ����Чʱ��;
UINT & TablePacket::GetValidCount()      {  return (m_uiValidCount); }       // ��Чʹ�ô���;

BYTE & TablePacket::GetFieldId()         {  return (m_byFieldId);    }       // ���κ�;
BYTE & TablePacket::GetRoomId()          {  return (m_byRoomId);     }       // �����;
UINT & TablePacket::GetTableId()         {  return (m_uiTableId);    }       // ���Ӻ�;

BYTE & TablePacket::GetModel()           {  return (m_byModel);      }       // �����;
UINT & TablePacket::GetMultiple()        {  return (m_uiMultiple);   }       // ���Ӻ�;
UINT & TablePacket::GetBrokerage()       {  return (m_uiBrokerage);  }       // ��λ��;

UINT & TablePacket::GetTimeStep()        {  return (m_uiTimeStep);   }       // ʱ���;
BYTE & TablePacket::GetPlaySeatId()      {  return (m_byPlaySeatId); }       // ��λ��;

UINT & TablePacket::GetUserKey(BYTE seatid)     {  return (m_uiUserKey[seatid]);    }       // �û����ֵ
BYTE & TablePacket::GetPosition(BYTE seatid)    {  return (m_byPosition[seatid]);   }       // ְλ�����������;
BYTE & TablePacket::GetReady(BYTE seatid)       {  return (m_byReady[seatid]);      }       // ׼������;
UINT & TablePacket::GetRate(BYTE seatid)        {  return (m_uiRate[seatid]);       }       // ʤ��;
int  & TablePacket::GetMoney(BYTE seatid)       {  return (m_iMoney[seatid]);       }       // Ǯ;
CHAR * TablePacket::GetName(BYTE seatid)        {  return (m_szName[seatid]);       }       // ����;

void TablePacket::ToPrint()
{
    DEBUG_MSG( LVL_DEBUG, "Begin");

    DEBUG_MSG( LVL_DEBUG, "| Protocol=%d | TokenKey=%d | ValidTime=%d | ValidCount=%d |",
              m_uiProtocol, m_uiTokenKey, m_uiValidTime, m_uiValidCount);

    DEBUG_MSG( LVL_DEBUG, "| FieldId=%d | RoomId=%d | TableId=%d |",
              m_byFieldId, m_byRoomId, m_uiTableId );

    DEBUG_MSG( LVL_DEBUG, "| Model=%d | Multiple=%d | Brokerage=%d | TimeStep=%d | SeatId=%d |",
              m_byModel, m_uiMultiple, m_uiBrokerage, m_uiTimeStep, m_byPlaySeatId );

    DEBUG_MSG( LVL_DEBUG, "| UserKey=%d | Position=%d | Ready=%d | Rate=%d | Money=%d | Name=%s |",
              m_uiUserKey[0],  m_byPosition[0], m_byReady[0], m_uiRate[0], m_iMoney[0], m_szName[0] );

    DEBUG_MSG( LVL_DEBUG, "| UserKey=%d | Position=%d | Ready=%d | Rate=%d | Money=%d | Name=%s |",
              m_uiUserKey[1], m_byPosition[1], m_byReady[1], m_uiRate[1], m_iMoney[1], m_szName[1] );

    DEBUG_MSG( LVL_DEBUG, "| UserKey=%d | Position=%d | Ready=%d | Rate=%d | Money=%d | Name=%s |",
              m_uiUserKey[2], m_byPosition[2], m_byReady[2], m_uiRate[2], m_iMoney[2], m_szName[2] );

    DEBUG_MSG( LVL_DEBUG, "End\n");
}

// �ͻ��˰�
char * TablePacket::ClientData( BYTE bMainSeatid, BYTE bSeatid, char * szBuff, WORD wSize )
{
    memset(szBuff, 0x0, wSize);
    if ( GetPosition(bSeatid)!=0 ) {
        snprintf( szBuff, wSize, "{\"show\":%s,\"ready\":%s,\"name\":\"%s\",\"rate\":%d,\"money\":%d}",
                 "true",
                 (GetReady(bSeatid)==0)? "false":"true",
                 GetName(bSeatid),
                 GetRate(bSeatid),
                 GetMoney(bSeatid) );
    }
    else {
        snprintf( szBuff, wSize, "{\"show\":false, \"ready\":false, \"name\":\"-\", \"rate\":0,\"money\":0}" );
    }
    return (szBuff);
}

UINT TablePacket::JsonData(BYTE bSeatid, char * szBuff, WORD wSize )    // Json���ݴ��
{
    char _protocol[32] = {0};

    szBuff[0] = '\0';
    snprintf( _protocol, sizeof(_protocol), "{\"protocol\":\"%d\",", m_uiProtocol);
    strcat( szBuff, _protocol);

    char _data[256] = {0};
    snprintf( _data, sizeof(_data), "\"data\":[{\"battleid\":%d,\"seatid\":%d,\"basics\":%d,\"multiple\":%d,\"brokerage\":%d,",
             m_uiTableId, bSeatid, m_BasicMoney, m_uiMultiple, m_uiBrokerage );
    strcat( szBuff, _data);

    char _show[256] = {0};
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

