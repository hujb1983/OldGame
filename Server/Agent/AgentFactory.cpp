#include "AgentFactory.h"

AgentFactory::AgentFactory()
{
    m_pGameSessionPool = NULL;
    m_pLobbySessionPool = NULL;
	m_pUserSessionPool = NULL;
	m_pUserGamePool = NULL;
	m_pTempSessionPool = NULL;
}

AgentFactory::~AgentFactory()
{
    if (m_pGameSessionPool) 	delete m_pGameSessionPool;
	if (m_pLobbySessionPool) 	delete m_pLobbySessionPool;
	if (m_pUserSessionPool) 	delete m_pUserSessionPool;
	if (m_pUserGamePool) 	    delete m_pUserGamePool;
	if (m_pTempSessionPool) 	delete m_pTempSessionPool;
}

void AgentFactory::Init()
{
    m_pGameSessionPool 	= new MemoryFactory<GameSession>;
	m_pLobbySessionPool = new MemoryFactory<LobbySession>;
	m_pUserSessionPool 	= new MemoryFactory<UserSession>;
	m_pUserGamePool 	= new MemoryFactory<UserGame>;
	m_pTempSessionPool 	= new MemoryFactory<TempSession>;

	// UserSession
	m_pUserSessionPool->Initialize(999,1);      // 已经认证用户；
	m_pUserGamePool->Initialize(999,1);         // 进入游戏的用户；
	m_pGameSessionPool->Initialize(1,1);        // 已经认证用户；
	m_pLobbySessionPool->Initialize(1,1);       // 未认证用户；
	m_pTempSessionPool->Initialize(1,1);        // 未认证服务器；
}


// UserSession
UserSession * AgentFactory::AllocUserSession() {
	if (m_pUserSessionPool == NULL) {
		return NULL;
	}
	return m_pUserSessionPool->Alloc();
}
void AgentFactory::FreeUserSession(UserSession * pUser) {
	return m_pUserSessionPool->Free(pUser);
}

// UserGame
UserGame * AgentFactory::AllocUserGame() {
	if (m_pUserGamePool == NULL) {
		return NULL;
	}
	return m_pUserGamePool->Alloc();
}

void AgentFactory::FreeUserGame(UserGame * pUser) {
    return m_pUserGamePool->Free(pUser);
}

// GameSession
GameSession * AgentFactory::AllocGameSession() {
	if (m_pGameSessionPool == NULL) {
		return NULL;
	}
	return m_pGameSessionPool->Alloc();
}
void AgentFactory::FreeGameSession(GameSession * pUser) {
	return m_pGameSessionPool->Free(pUser);
}


// LobbySession
LobbySession * AgentFactory::AllocLobbySession() {
	if (m_pLobbySessionPool == NULL) {
		return NULL;
	}
	return m_pLobbySessionPool->Alloc();
}
void AgentFactory::FreeLobbySession(LobbySession * pUser) {
	return m_pLobbySessionPool->Free(pUser);
}


// Server
TempSession * AgentFactory::AllocTempSession() {
	if ( m_pTempSessionPool == NULL ) {
		return NULL;
	}
	TempSession * pSession = m_pTempSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}

void AgentFactory::FreeTempSession(TempSession * pServerSession) {
	return m_pTempSessionPool->Free(pServerSession);
}
