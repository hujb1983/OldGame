#ifndef _GameUser_H_INCLUDED_
#define _GameUser_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"

#define MAX_POKER (54)

class GameUser
{
public:
	GameUser(void);
	~GameUser(void);
	
public:
	int	 	m_nBattleid;
	int 	m_nSeatid;
	int 	m_nSeatKey;
	int 	m_nSeatName[3];
	BYTE 	m_byPokers[MAX_POKER+1];
};

// --- 游戏用户
extern GameUser g_GameUser;

#endif
