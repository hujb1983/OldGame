#ifndef _CnpokerFactory_H_INCLUDED_
#define _CnpokerFactory_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "ServerSession.h"
#include "TempSession.h"
#include "DBSession.h"
#include "AgentSession.h"
#include "LobbyUser.h"

using namespace A;

class LobbyFactory : public Singleton<LobbyFactory>
{

public:
	LobbyFactory();
	~LobbyFactory();

private:

	MemoryFactory<DBSession> 	    * m_pDBSessionPool;
	MemoryFactory<AgentSession> 	* m_pAgentSessionPool;
	MemoryFactory<TempSession>	    * m_pTempSessionPool;
	MemoryFactory<LobbyUser>	    * m_pLobbyUserPool;

public:
	void Init(void);
	void Release(void);

	// GameSession
	AgentSession * AllocAgentSession();
	void FreeAgentSession(AgentSession * pSession);

	// UserSession
	DBSession * AllocDBSession();
	void FreeDBSession(DBSession * pSession);

	// Server
	TempSession * AllocTempSession();
	void FreeTempSession(TempSession * pSession);

	// User
	LobbyUser * AllocUserSession();
	void FreeUserSession(LobbyUser * pUser);
};

#endif
