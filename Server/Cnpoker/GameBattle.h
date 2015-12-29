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

public:
    int InitBattle();
    int JoinBattle( BYTE _seatid );
    int QuitBattle( BYTE _seatid );

public: // ��ʱ
    int UpdateOvertime( DWORD dwTicket );  // ��ʱ����
    int SendOvertimeMessage();             // ���Ͳ�ʱ��Ϣ
    int IsOvertime();                      // �ж��Ƿ��ʱ
    int LockOvertime();                    // �Ѿ���ʱ
    int UnlockOvertime();                  // ����
    int SetCalledOvertime();               // ���ý��Ʋ�ʱ
    int SetDiscardsOvertime();             // ���ó��Ʋ�ʱ

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
    int SetMaxMoney  ( UINT _money );               // ���ø��޶�;
    int SetMinMoney  ( UINT _money );               // ���õ���;
    int SetBrokerage ( UINT _rate  );               // ����Ӷ��;
    int SetStartSeat( BYTE seatid );                // �������������;
    int SetOnline( BYTE seatid, BYTE status );      // ��������״̬;

	int SetStart( BYTE seatid, BYTE status );       // ��ʼ����
	int SetShow( BYTE seatid );                     // ��������
    int SetCalled( BYTE seatid, BYTE called );      // ���ý���
    int SetBank( BYTE seatid );                     // ����Ϊׯ��
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
    UINT  getMaxMoney ();                   // ���ø��޶�;
    UINT  getMinMoney ();                   // ���õ���;
    UINT  getBrokerage();                   // ����Ӷ��;
    BYTE  getModel();                       // ȡ�����ӵ����б���;
    WORD& getMultiple();                    // ȡ�����ӵ����б���;
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
    BYTE getCalled();                       // �е��������;

public: // �ж�
    int canStart();                         // �ܷ�ʼ����
    int canDealing();                       // �ܷ���
    int canCall();                          // �ܷ������ׯ
    int checkCall();                        // �����ƵĲ���;
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
	UINT getID ( BYTE seatid);
    WORD getKey( BYTE seatid);

private:
    WORD m_wIndex;                              // �����±�;
    UINT m_uCycle;                              // ����;
	BYTE m_byRoomid;                            // ����;
	BYTE m_byTableid;                           // ����;
    BYTE m_byModel;                             // �ӱ�ģʽ;
    WORD m_wMultiple;                           // �ӱ�ģʽ�¶�̬���仯;
    UINT m_uiMaxMoney;                          // �����;
    UINT m_uiMinMoney;                          // ��С���;
    WORD m_wBrokerage;                          // Ӷ��;
	UINT m_uiUpdate;                            // ����ʱ��;
	BYTE m_byPlaying;                           // ���������;
	BYTE m_byStartSeat;                         // ��ʼ������, �е���Ҫ��ƫ��;
	BYTE m_byCallSeat;                          // ��һ��Ϊm_byStartSeat, ��һ���е���;
	BYTE m_byCallTimes;                         // �����Ѿ��е������Ĵ���;
	BYTE m_byBanker;                            // ��¼��������λ��;
	BYTE m_byTableStatus;                       // ��������״̬�������½׶Ρ��е����׶Ρ����ƽ׶Ρ�����׶Σ�;
	BYTE m_byStatus[MAX_USER];                  // �Ƿ����;
	BYTE m_byStart[MAX_USER];                   // �Ƿ��Ѿ����, ��ʼ״̬;
	BYTE m_byCalls[MAX_CALL];                   // ��¼�е���״̬���û�״̬;
    BYTE m_byShow[MAX_USER];                    // ��¼��ʾ�Ƶ��û�״̬;
    UINT m_uiUserid[MAX_USER];                  // �û�ID;
    WORD m_wUserkey[MAX_USER];                  // �û��˿�;
    BYTE m_byOnline[MAX_USER];                  // �û�����״̬;
    BYTE m_byTrusteeship[MAX_USER];             // �û��й�״̬;
    char m_szUsercards[MAX_USER][CHAR_POKER+1]; // �û��������;
    BYTE m_byUsercards[MAX_USER][MAX_POKER+1];  // �û����ϵ���;
    int  m_nDiscardsSize[MAX_USER];             // �û����ϵ���Size;
    char m_szDiscards[MAX_USER][CHAR_POKER+1];  // �û��������;
    BYTE m_byBasecards[BASE_POKER+1];           // ������;
	BYTE m_byPlayTimes;                         // ������Ĵ�����
    BYTE m_byLastActive;                        // �����Ƶ���λ;
    BYTE m_byLastType;                          // �����Ƶ�����;
	BYTE m_byLastValue;                         // �����Ƶ�ֵ;
	BYTE m_byLastCount;                         // �����ƵıȽϸ���;
	BYTE m_byLastSize;                          // ������������;
    DWORD m_dwBeginTicket;                      // ��ʼ�δ�;
    DWORD m_dwOverTicket;                       // ��ʱ�δ�;
    BYTE  m_byOverTime;                         // �Ƿ��Ѿ���ʱ;
    char  m_szOverTime[OT_SIZE];                // ��ʱʱ���ú�;

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
