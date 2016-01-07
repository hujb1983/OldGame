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

    void ToInit();                  // ��ʼ��
    void ToPrint();                 // ��ӡ
    void SetPacket(BYTE *, WORD );  // ���ð��ṹ

public:
    UINT JsonData(BYTE bSeatid, char * szBuff, WORD wSize );    // Json���ݴ��
    char * ClientData(BYTE bMainSeatid, BYTE bSeatid, char * szBuff, WORD wSize );  // Client-Json���ݴ��

    BYTE * GetPacket(BYTE *, WORD);   // ��һ��ָ�벢���ͷָ��;
    WORD   GetPacketSize();           // ȡ���������С;

    UINT & GetProtocol();       // ȡ�ú�����Э���;
    UINT & GetTokenKey();       // ��ȫ��;
    UINT & GetValidTime();      // ȡ����Чʱ��;
    UINT & GetValidCount();     // ��Чʹ�ô���;

    BYTE & GetFieldId();        // ���κ�;
    BYTE & GetRoomId();         // �����;
    UINT & GetTableId();        // �����ֵ;
    WORD & GetBattleId();       // �����;

    BYTE & GetModel();          // ģʽ;
    BYTE & GetBasicMoney();     // ��ͽ��;
    UINT & GetMultiple();       // ����;
    UINT & GetBrokerage();      // Ӷ��;

    UINT & GetTimeStep();       // ʱ���;
    BYTE & GetPlaySeatId();     // ��λ��;

    UINT & GetUserKey(BYTE);    // �û����ֵ
    BYTE & GetPosition(BYTE);   // ְλ�����������;
    BYTE & GetReady(BYTE);      // ׼������;
    UINT & GetRate(BYTE);       // ʤ��;
    int  & GetMoney(BYTE);      // Ǯ;
    CHAR * GetName(BYTE);       // ����

private:

    UINT  m_uiProtocol;         // Э��;
    UINT  m_uiTokenKey;         // ���ɵķ�����;
    UINT  m_uiValidTime;        // ��Чʱ������;
    UINT  m_uiValidCount;       // ��Ч���ʼ���;

    BYTE  m_byFieldId;          // ���κ�;
    BYTE  m_byRoomId;           // �����;
    UINT  m_uiTableId;          // ���Ӻ�;

    BYTE  m_byModel;            // �ӱ�ģʽ;
    UINT  m_BasicMoney;         // �������;
    UINT  m_uiMultiple;         // �ӱ�ֵ;
    UINT  m_uiBrokerage;        // Ӷ��;

    UINT  m_uiTimeStep;         // ʱ���
    BYTE  m_byPlaySeatId;       // ���������;

    UINT m_uiUserKey[TEAM_SIZE];    // �û����ֵ
    BYTE m_byPosition[TEAM_SIZE];   // ְλ���գ����������
    BYTE m_byReady[TEAM_SIZE];      // ׼������
    UINT m_uiRate[TEAM_SIZE];       // ʤ��
    int  m_iMoney[TEAM_SIZE];       // Ǯ
    CHAR m_szName[TEAM_SIZE][33];   // ����

};

#pragma pack(pop)
/*************************************/

#endif // _UserPacket_H_INCLUDED_
