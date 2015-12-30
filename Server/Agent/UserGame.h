#ifndef _UserGame_H_INCLUDED_
#define _UserGame_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "UserSession.h"

class UserGame
{
public:
	UserGame();
	virtual ~UserGame();

    void Init();
    int  Update( DWORD dwDeltaTick );

    // 登录 - ！
	void Login( UserSession * pSession );

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

    void QuitTable();   // 退出桌子;

private:
    UserSession *   m_pSession;         // 用户的网络会话层

private:
	eUSER_TYPE 		m_eUserType;
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

#endif // _UserGame_H_INCLUDED_
