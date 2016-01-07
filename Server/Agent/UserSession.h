#ifndef _UserSession_H_INCLUDED_
#define _UserSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include <Public.h>
#include "ServerSession.h"

class UserSession : public ServerSession
{
public:
	UserSession();
	virtual ~UserSession();

	void Login( ServerSession * pServerSession );

	virtual BOOL Update( DWORD dwDeltaTick );
	virtual BOOL SendPacket(BYTE *pMsg, WORD wSize);

	// UserType
	virtual eUSER_TYPE GetUserType() { return m_eUserType; }
	virtual void SetUserType(eUSER_TYPE eType) { m_eUserType = eType; }

	// UserKey = Accept Port;
	virtual WORD GetUserKey() const;
	void SetUserKey(WORD dwKey);

	virtual BYTE GetSeatKey() const;
	void SetSeatKey(BYTE dwKey);

	virtual WORD GetBattleKey() const;
	void SetBattleKey(WORD dwKey);

	virtual WORD GetUserid() const;
	void SetUserid(UINT dwID);

    void setName( char * _name , int _size );           // 名称
    void setGamesinfo( int _point, UINT _wons, UINT _failed, UINT _aways );  // 胜率
    void setMoney( int _money );  // 钱

    void setReady( BOOL _ready );  // 准备
    void setShowcards( BOOL _showcards );  // 显牌

    BYTE & getPokerSize( );                         // 牌数量
    void setPokers( char * poker, WORD _size);      // 牌

    void getSeatInfo( char * _seatinfo, int _size );    // 坐位信息
    void getPokerInfo( int userkey, char * _pokerinfo,
                      int _size );                      // 牌的信息

    BYTE& getStatus();    // 取得状态;
    void EndGame(int _imoneys, int _iwons, int _ifaileds); // 结束游戏更新数据;
    void JoinGame();    // 加入游戏;
    void LeaveGame();   // 离开游戏;
    void ClearTable();  // 清理数据;

	void CloseSession();    // 关闭桌子;

	void SetUserPacket(BYTE * bPack, WORD wSize);
	UserPacket & GetUserPacket();

private:
    UserPacket  m_cPack;

public:
	virtual void 	Init();
	virtual void 	Release();
	virtual void	OnAccept( DWORD dwNetworkIndex );
	virtual void	OnDisconnect();
	virtual void	OnRecv(BYTE *pMsg, WORD wSize);
	virtual void	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual void	OnLogString( char * pszLog);

private:
	BOOL            m_bFirst;
	eUSER_TYPE 		m_eUserType;

	BYTE 			m_wUserStatus;      // 用户的状态
	WORD 			m_wUserKey;         // 用户健值
	BYTE            m_bySeatKey;        // 坐位ID;
    WORD 			m_wBattleKey;       // 房间号
    UINT            m_dwUserid;         // 用户ID
    char            m_szName[33];       // 名称
    int             m_iMoneys;          // 点数
    UINT            m_uiWons;           // 赢的次数
	UINT            m_uiFaileds; 	    // 失败次数
	UINT            m_uiAways; 		    // 逃跑次数
    BOOL            m_bReady; 	        // 已经准备好了
	BOOL            m_bShowcards;       // 显示牌
	BYTE            m_bPokerSize;       // 牌数量
    char            m_bPoker[128];      // 牌

public:
	// static DWORD    m_byCloseDelay;
	static DWORD    m_dwClientDelay;
	static DWORD    m_dwServerDelay;
	DWORD 			m_dwOvertime;  // 超时时间
};

#endif // _AGENTPLAYER_H_INCLUDED_
