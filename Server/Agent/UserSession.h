#ifndef _UserSession_H_INCLUDED_
#define _UserSession_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
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

	virtual WORD GetBattleKey() const;
	void SetBattleKey(WORD dwKey);

	virtual WORD GetUserid() const;
	void SetUserid(UINT dwID);

    void setName( char * _name , int _size );           // 名称
    void setGamesinfo( int _point, UINT _wons, UINT _failed, UINT _aways );  // 胜率
    void setMoney( int _money );            // 钱
    void getSeatInfo( char * _seatinfo, int _size );     // 坐位信息

	void CloseSession();

	/* 1. ²éÑ¯Êý¾Ý¿â£»*/
    virtual void  DBResult( WORD cate, WORD ptcl, QueryResult * pData );

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
	WORD 			m_wUserKey;         // 用户健值
    WORD 			m_wBattleKey;       // 房间号
    UINT            m_dwUserid;         // 用户ID
    char            m_szName[33];       // 名称
    int             m_iMoneys;          // 点数
    UINT            m_uiWons;           // 赢的次数
	UINT            m_uiFaileds; 	    // 失败次数
	UINT            m_uiAways; 		    // 逃跑次数

public:
	// static DWORD    m_byCloseDelay;
	static DWORD    m_dwClientDelay;
	static DWORD    m_dwServerDelay;
	DWORD 			m_dwOvertime;  // 超时时间
};

#endif // _AGENTPLAYER_H_INCLUDED_
