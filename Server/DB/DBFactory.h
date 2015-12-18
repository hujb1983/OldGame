#ifndef _DBFactory_H_INCLUDED_
#define _DBFactory_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "ServerSession.h"
#include "TempSession.h"
#include "LobbySession.h"
#include "GameSession.h"
#include "LoginSession.h"

using namespace A;

class DBFactory : public Singleton<DBFactory>
{

public:
	DBFactory();
	~DBFactory();

private:

	MemoryFactory<GameSession> 	* m_pGameSessionPool;
	MemoryFactory<LobbySession> * m_pLobbySessionPool;
	MemoryFactory<LoginSession>	* m_pLoginSessionPool;
	MemoryFactory<TempSession>	* m_pTempSessionPool;

public:
	void Init(void);
	void Release(void);

	// GameSession
	GameSession * AllocGameSession();
	void FreeGameSession(GameSession * pSession);

	// UserSession
	LobbySession * AllocLobbySession();
	void FreeLobbySession(LobbySession * pSession);

	// Server
	LoginSession * AllocLoginSession();
	void FreeLoginSession(LoginSession * pSession);

	// Server
	TempSession * AllocTempSession();
	void FreeTempSession(TempSession * pSession);
};

#endif
