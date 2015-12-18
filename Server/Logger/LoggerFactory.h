#ifndef _LoggerFactory_INCLUDE_
#define _LoggerFactory_INCLUDE_

#include <Network.h>
#include <Utility.h>

#include "DBSession.h"
#include "LoginSession.h"
#include "LobbySession.h"
#include "AgentSession.h"
#include "GameSession.h"
#include "UserSession.h"

using namespace A;

class LoggerFactory : public Singleton<LoggerFactory>
{
public:
	LoggerFactory();
	~LoggerFactory();

private:
	MemoryFactory<DBSession>	        * m_pDBSessionPool;         // <DB>
	MemoryFactory<LoginSession>	        * m_pLoginSessionPool;      // <Lobby>
	MemoryFactory<LobbySession>	        * m_pLobbySessionPool;      // <Lobby>
    MemoryFactory<AgentSession>	        * m_pAgentSessionPool;      // <Agent>
    MemoryFactory<GameSession>	        * m_pGameSessionPool;      // <Games>
    MemoryFactory<TempSession>	        * m_pTempSessionPool;       // <Temp>

public:
	void Init(void);
	void Release(void);

public:
	// DB Server
	DBSession * AllocDBSession();
	void FreeDBSession(DBSession * pSession);

	// Login Server
	LoginSession * AllocLoginSession();
	void FreeLoginSession(LoginSession * pSession);

	// Lobby Server
	LobbySession * AllocLobbySession();
	void FreeLobbySession(LobbySession * pSession);

	// Agent Server
	AgentSession * AllocAgentSession();
	void FreeAgentSession(AgentSession * pSession);

	// Games Server
	GameSession * AllocGameSession();
	void FreeGameSession(GameSession * pSession);

	// Temp Server
	TempSession * AllocTempSession();
	void FreeTempSession(TempSession * pSession);
};

#endif
