#ifndef _GamePlyer_H_INCLUDED_
#define _GamePlyer_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>

class GameBattle
{
public:
	GameBattle();
	virtual ~GameBattle();

public: // ��ʱ
    BOOL UpdateOvertime( DWORD dwTicket );  // ��ʱ����
    BOOL SendOvertimeMessage();             // ���Ͳ�ʱ��Ϣ
    BOOL IsOvertime();                      // �ж��Ƿ��ʱ
    BOOL LockOvertime();                    // �Ѿ���ʱ
    BOOL UnlockOvertime();                  // ����
    BOOL SetCalledOvertime();               // ���ý��Ʋ�ʱ
    BOOL SetDiscardsOvertime();             // ���ó��Ʋ�ʱ

public: // ��ȡ�û�key
    int GetAllPlayerKey(char * json_text, WORD wSize);  // ȡ�������û�Hash���

public: // ����֪ͨ
    int UpdateToDatabase();
    int UpdateBattleInfo(BYTE _seatid);   // �������ݵ����ݿ�;
	int SendToSettleAccounts();           // ���ͽ���֪ͨ;

public: // ��������
    int SetIndex( WORD index );
	int SetTable( BYTE room, BYTE table );
	int SetID ( BYTE seatid, int id  );
    int SetKey( BYTE seatid, int key );
    int SetStartSeat( BYTE seatid );                // �������������;
    int SetOnline( BYTE seatid, BYTE status );      // ��������״̬;

	int SetStart( BYTE seatid, BYTE status );       // ��ʼ����
	int SetShow( BYTE seatid );                     // ��������
    int SetCalled( BYTE seatid, BYTE called );      // ���ý���
    int SetBank( BYTE seatid );                     // ����Ϊ����
    int SetPlaying( BYTE seatid );                  // ����������Ա
    int SetTrusteeship( BYTE seatid );              // �Ƿ��й�
    int CancelTrusteeship( BYTE seatid );           // ȡ���й�

    int SetBasecards( BYTE * poker, BYTE bySize );              // ���ö�������
    int SetUsercards( BYTE seatid, BYTE * poker, BYTE bySize ); // �û������������
    int SetDiscardString( BYTE seatid, char * poker );          // ���ô������Ϊ�ַ���
    int SetDiscards( BYTE seatid, BYTE * poker, BYTE bySize );  // �޸Ĵ�����Ʊ�ʶ
    int SetModel( BYTE _byDoub );                               // ����Ϊ�ӱ�ģʽ;
    int SetBattleStatus( BYTE _status );                        // ���ø���״̬;

    int SetLastSeat (  BYTE _seatid );       // �����Ƶ���λ;
    int SetLastType (  BYTE _type   );       // �����Ƶ����ͣ�
    int SetLastValue( BYTE _value  );        // �����Ƶ�ֵ;
    int SetLastCount( BYTE _count  );        // �����ƱȽ�����
    int SetLastSize ( BYTE _count  );        // �������ܸ�����

public: // ������
    BYTE getUsercards( BYTE seatid, char * poker, WORD bySize );
    BYTE getBasecards( BYTE seatid, char * poker, WORD bySize );
    BYTE getCurrentCards( BYTE seatid, BYTE * poker, WORD bySize );

public: // ��ȡ����
    BYTE  getSeatid(int key, BYTE & seatid);// ����Key�����λID;
    BYTE  getBanker();                      // ȡ��ׯ;
    BYTE  getBattleStatus();                // ����һ��ս��״̬;
    BYTE  getStartSeat();                   // ����ϴε�Ӯ��;
    BYTE& getDiscardTimes();                // ȡ�ó��ƴ���;
    BYTE& getLastSeat();                    // �����Ƶ���λ;
    BYTE& getLastType();                    // �����Ƶ����ͣ�
    BYTE& getLastValue();                   // �����Ƶ�ֵ;
    BYTE& getLastCount();                   // �����ƱȽϸ�����
    BYTE& getLastSize();                    // ��������������
    BYTE prevSeat( BYTE seatid );           // ǰһ��
    BYTE nextSeat( BYTE seatid );           // ��һ��
    BYTE getPlaying();                      // ȡ��������� Seat

public: // �ж�
    int canStart();                         // �ܷ�ʼ����
    int canDealing();                       // �ܷ���
    int canCall();                          // �ܷ������ׯ
    int canGame();                          // �����Ѿ����壬���Կ�ʼ
    int canEnd();                           // �ܷ����

public:
    int hadDiscards( BYTE seatid, BYTE * poker, BYTE bySize );      // �ж��Ƿ�����Щ��
    int IsOnline( BYTE seatid );            // �Ƿ�����
    int IsTrusteeship( BYTE seatid );       // �Ƿ��й�

public:
    WORD getIndex();
	WORD prevKey(BYTE seatid);
	WORD nextKey(BYTE seatid);
    WORD getKey( BYTE seatid);

private:
    WORD m_wIndex;                              // �����±�
    UINT m_uCycle;                              // ����
	BYTE m_byRoomid;                            // ����
	BYTE m_byTableid;                           // ����
    BYTE m_byModel;                             // �ӱ�ģʽ
	UINT m_uiUpdate;                            // ����ʱ��
	BYTE m_byPlaying;                           // ���������
	BYTE m_byStartSeat;                         // ��ʼ������, ��Ҫ�ǽе���;
	BYTE m_byCallSeat;                          // ��һ��Ϊm_byStartSeat, ��һ���е���;
	BYTE m_byCallTimes;                         // �е�������;
	BYTE m_byBanker;                            // ����;
	BYTE m_byTableStatus;                       // ��������״̬;
	BYTE m_byStatus[MAX_USER];                  // �Ƿ����;
	BYTE m_byStart[MAX_USER];                   // ��ʼ״̬
	BYTE m_byCalls[MAX_CALL];                   // �е���״̬
    BYTE m_byShow[MAX_USER];                    // ��ʾ״̬
    UINT m_uiUserid[MAX_USER];                  // �û�ID
    WORD m_wUserkey[MAX_USER];                  // �û��˿�
    BYTE m_byOnline[MAX_USER];                  // �û�����״̬
    BYTE m_byTrusteeship[MAX_USER];             // �û��й�״̬
    char m_szUsercards[MAX_USER][CHAR_POKER+1]; // �û��������
    BYTE m_byUsercards[MAX_USER][MAX_POKER+1];  // �û����ϵ���
    int  m_nDiscardsSize[MAX_USER];             // �û����ϵ���Size
    char m_szDiscards[MAX_USER][CHAR_POKER+1];  // �û��������
    BYTE m_byBasecards[BASE_POKER+1];           // ������
	BYTE m_byPlayTimes;                         // ������Ĵ�����
    BYTE m_byLastActive;                        // �����Ƶ���λ
    BYTE m_byLastType;                          // �����Ƶ�����
	BYTE m_byLastValue;                         // �����Ƶ�ֵ
	BYTE m_byLastCount;                         // �����ƵıȽϸ���
	BYTE m_byLastSize;                          // ������������
    DWORD m_dwBeginTicket;                      // ��ʼ�δ�
    DWORD m_dwOverTicket;                       // ��ʱ�δ�
    BYTE  m_byOverTime;                         // �Ƿ��Ѿ���ʱ
    char  m_szOverTime[OT_SIZE];                // ��ʱʱ���ú�

public:
    GameBattle * set_prev( GameBattle * _prev );
    GameBattle * set_next( GameBattle * _next );

    bool is_null_prev();
    bool is_null_next();

    GameBattle * get_prev();
    GameBattle * get_next();

private:
    GameBattle * m_prev;
    GameBattle * m_next;
};


/**********************************************
    ��Ϸ���Ӷ��ж���
**********************************************/
class GameBattleList
{
public:
	GameBattleList();
	~GameBattleList();

public:

    void clear();
    void push_back(GameBattle * pUser);
    void remove(GameBattle * pUser);
    WORD size() { return m_size; }

    GameBattle * get_frist();
    GameBattle * pop_frist();

private:
    WORD m_size;
    GameBattle * m_head;
    GameBattle * m_last;
};


#endif // _AGENTPLAYER_H_INCLUDED_
