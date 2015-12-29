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

public: // 超时
    int UpdateOvertime( DWORD dwTicket );  // 操时更新
    int SendOvertimeMessage();             // 发送操时信息
    int IsOvertime();                      // 判断是否操时
    int LockOvertime();                    // 已经操时
    int UnlockOvertime();                  // 解锁
    int SetCalledOvertime();               // 设置叫牌操时
    int SetDiscardsOvertime();             // 设置出牌操时

public: // 获取用户key
    int GetAllPlayerKey(char * json_text, WORD wSize);  // 取得所有用户Hash入口

public: // 结算通知
    int UpdateToDatabase();
    int UpdateBattleInfo(BYTE _seatid);   // 更新数据到数据库;
	int SendToSettleAccounts();           // 发送结算通知;

public: // 基本设置
    int SetIndex( WORD index );
	int SetTable( BYTE room, BYTE table );
	int SetID ( BYTE seatid, int id  );
    int SetKey( BYTE seatid, int key );
    int SetMaxMoney  ( UINT _money );               // 设置高限额;
    int SetMinMoney  ( UINT _money );               // 设置底柱;
    int SetBrokerage ( UINT _rate  );               // 设置佣金;
    int SetStartSeat( BYTE seatid );                // 定义申请地主人;
    int SetOnline( BYTE seatid, BYTE status );      // 设置在线状态;

	int SetStart( BYTE seatid, BYTE status );       // 开始设置
	int SetShow( BYTE seatid );                     // 显牌设置
    int SetCalled( BYTE seatid, BYTE called );      // 设置叫牌
    int SetBank( BYTE seatid );                     // 设置为庄家
    int SetPlaying( BYTE seatid );                  // 设置玩牌人员
    int SetTrusteeship( BYTE seatid );              // 是否托管
    int CancelTrusteeship( BYTE seatid );           // 取消托管

    int SetBasecards( BYTE * poker, BYTE bySize );              // 设置斗地主牌
    int SetUsercards( BYTE seatid, BYTE * poker, BYTE bySize ); // 用户分配的牌设置
    int SetDiscardString( BYTE seatid, char * poker );          // 设置打掉的牌为字符串
    int SetDiscards( BYTE seatid, BYTE * poker, BYTE bySize );  // 修改打掉的牌标识
    int SetModel( BYTE _byDoub );                               // 设置为加倍模式;
    int SetBattleStatus( BYTE _status );                        // 设置复本状态;

    int SetLastSeat (  BYTE _seatid );       // 最后出牌的坐位;
    int SetLastType (  BYTE _type   );       // 最后出牌的类型；
    int SetLastValue( BYTE _value  );        // 最后出牌的值;
    int SetLastCount( BYTE _count  );        // 最后出牌比较数；
    int SetLastSize ( BYTE _count  );        // 最后出牌总个数；

public: // 牌设置
    BYTE getUsercards( BYTE seatid, char * poker, WORD bySize );
    BYTE getBasecards( BYTE seatid, char * poker, WORD bySize );
    BYTE getCurrentCards( BYTE seatid, BYTE * poker, WORD bySize );

public: // 获取数据
    UINT  getMaxMoney ();                   // 设置高限额;
    UINT  getMinMoney ();                   // 设置底柱;
    UINT  getBrokerage();                   // 设置佣金;
    BYTE  getModel();                       // 取得桌子的现有倍数;
    WORD& getMultiple();                    // 取得桌子的现有倍数;
    BYTE  getSeatid(int key, BYTE & seatid);// 根据Key获得座位ID;
    BYTE  getBanker();                      // 取得庄;
    BYTE  getBattleStatus();                // 制造一个战斗状态;
    BYTE  getStartSeat();                   // 获得上次的赢家;
    BYTE& getDiscardTimes();                // 取得出牌次数;
    BYTE& getLastSeat();                    // 最后出牌的坐位;
    BYTE& getLastType();                    // 最后出牌的类型；
    BYTE& getLastValue();                   // 最后出牌的值;
    BYTE& getLastCount();                   // 最后出牌比较个数；
    BYTE& getLastSize();                    // 最后出牌总张数；
    BYTE prevSeat( BYTE seatid );           // 前一个
    BYTE nextSeat( BYTE seatid );           // 后一个
    BYTE getPlaying();                      // 取得正在玩的 Seat
    BYTE getCalled();                       // 叫地主的玩家;

public: // 判断
    int canStart();                         // 能否开始发牌
    int canDealing();                       // 能否发牌
    int canCall();                          // 能否继续叫庄
    int checkCall();                        // 检查叫牌的参数;
    int canGame();                          // 地主已经分清，可以开始
    int canEnd();                           // 能否结束
public:
    int hadDiscards( BYTE seatid, BYTE * poker, BYTE bySize );      // 判断是否有这些牌
    int IsOnline( BYTE seatid );            // 是否在线
    int IsTrusteeship( BYTE seatid );       // 是否托管

public:
    WORD getIndex();
	WORD prevKey(BYTE seatid);
	WORD nextKey(BYTE seatid);
	UINT getID ( BYTE seatid);
    WORD getKey( BYTE seatid);

private:
    WORD m_wIndex;                              // 复本下标;
    UINT m_uCycle;                              // 周期;
	BYTE m_byRoomid;                            // 房间;
	BYTE m_byTableid;                           // 桌子;
    BYTE m_byModel;                             // 加倍模式;
    WORD m_wMultiple;                           // 加倍模式下动态数变化;
    UINT m_uiMaxMoney;                          // 最大金额;
    UINT m_uiMinMoney;                          // 最小金额;
    WORD m_wBrokerage;                          // 佣金;
	UINT m_uiUpdate;                            // 更新时间;
	BYTE m_byPlaying;                           // 正在玩的人;
	BYTE m_byStartSeat;                         // 开始的桌子, 叫地主要做偏移;
	BYTE m_byCallSeat;                          // 第一个为m_byStartSeat, 下一个叫地主;
	BYTE m_byCallTimes;                         // 桌子已经叫地主整的次数;
	BYTE m_byBanker;                            // 记录地主的坐位号;
	BYTE m_byTableStatus;                       // 桌子现有状态：（坐下阶段、叫地主阶段、打牌阶段、结算阶段）;
	BYTE m_byStatus[MAX_USER];                  // 是否地主;
	BYTE m_byStart[MAX_USER];                   // 是否已经点击, 开始状态;
	BYTE m_byCalls[MAX_CALL];                   // 记录叫地主状态的用户状态;
    BYTE m_byShow[MAX_USER];                    // 记录显示牌的用户状态;
    UINT m_uiUserid[MAX_USER];                  // 用户ID;
    WORD m_wUserkey[MAX_USER];                  // 用户端口;
    BYTE m_byOnline[MAX_USER];                  // 用户在线状态;
    BYTE m_byTrusteeship[MAX_USER];             // 用户托管状态;
    char m_szUsercards[MAX_USER][CHAR_POKER+1]; // 用户打掉的牌;
    BYTE m_byUsercards[MAX_USER][MAX_POKER+1];  // 用户手上的牌;
    int  m_nDiscardsSize[MAX_USER];             // 用户手上的牌Size;
    char m_szDiscards[MAX_USER][CHAR_POKER+1];  // 用户打掉的牌;
    BYTE m_byBasecards[BASE_POKER+1];           // 地主牌;
	BYTE m_byPlayTimes;                         // 设置玩的次数；
    BYTE m_byLastActive;                        // 最后打牌的坐位;
    BYTE m_byLastType;                          // 最后打牌的类型;
	BYTE m_byLastValue;                         // 最后打牌的值;
	BYTE m_byLastCount;                         // 最后打牌的比较个数;
	BYTE m_byLastSize;                          // 最后打牌总张数;
    DWORD m_dwBeginTicket;                      // 开始滴达;
    DWORD m_dwOverTicket;                       // 操时滴达;
    BYTE  m_byOverTime;                         // 是否已经操时;
    char  m_szOverTime[OT_SIZE];                // 操时时配置好;

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
    游戏桌子队列对象
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
