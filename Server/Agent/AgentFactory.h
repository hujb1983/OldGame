#ifndef _AgentFactory_H_INCLUDED_
#define _AgentFactory_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "ServerSession.h"
#include "TempSession.h"
#include "UserSession.h"
#include "GameSession.h"
#include "LobbySession.h"

using namespace A;

class AgentFactory : public Singleton<AgentFactory>
{

public:
	AgentFactory();
	~AgentFactory();

private:

	/*  1. �Ѿ������ϣ�δ��֤��
        2. ʱ��̫��Ҫ�Ͽ��� */
	MemoryFactory<UserSession> 	* m_pUserSessionPool;

	/*  1. �Ѿ������ϣ�δ��֤��
        2. ʱ��̫��Ҫ�Ͽ��� */
	MemoryFactory<GameSession> 	* m_pGameSessionPool;

	/*  1. �Ѿ����ӣ�δ��֤��
        2. ʱ��̫��Ҫ�Ͽ��� */
	MemoryFactory<LobbySession>	* m_pLobbySessionPool;

	/*  1. �Ѿ����ӣ�δ��֤��
        2. ʱ��̫��Ҫ�Ͽ��� */
	MemoryFactory<TempSession>	* m_pTempSessionPool;

public:
	void Init(void);
	void Release(void);

	// UserSession
	UserSession * AllocUserSession();
	void FreeUserSession(UserSession * pSession);

	// GameSession
	GameSession * AllocGameSession();
	void FreeGameSession(GameSession * pSession);

	// LobbySession
	LobbySession * AllocLobbySession();
	void FreeLobbySession(LobbySession * pSession);

	// Server
	TempSession * AllocTempSession();
	void FreeTempSession(TempSession * pSession);
};

#endif
