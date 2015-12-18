#include "DBFactory.h"

DBFactory::DBFactory()
{
    m_pGameSessionPool	 = NULL;
	m_pLobbySessionPool	 = NULL;
	m_pLoginSessionPool	 = NULL;
	m_pTempSessionPool   = NULL;
}

DBFactory::~DBFactory()
{
    if (m_pGameSessionPool) 	delete m_pGameSessionPool;
	if (m_pLobbySessionPool) 	delete m_pLobbySessionPool;
	if (m_pLoginSessionPool) 	delete m_pLoginSessionPool;
	if (m_pTempSessionPool) 	delete m_pTempSessionPool;
}

void DBFactory::Init()
{
    m_pGameSessionPool  = new MemoryFactory<GameSession>;
	m_pLobbySessionPool = new MemoryFactory<LobbySession>;
	m_pLoginSessionPool = new MemoryFactory<LoginSession>;
	m_pTempSessionPool 	= new MemoryFactory<TempSession>;

	// UserSession
	m_pGameSessionPool->Initialize(1,1);
	m_pLobbySessionPool->Initialize(1,1);      // 已经认证用户；
	m_pLoginSessionPool->Initialize(1,1);
	m_pTempSessionPool->Initialize(1,1);       // 未认证用户；
}

// UserSession
GameSession * DBFactory::AllocGameSession() {
	if (m_pGameSessionPool == NULL) {
		return NULL;
	}
	return m_pGameSessionPool->Alloc();
}

void DBFactory::FreeGameSession(GameSession * pSession) {
	return m_pGameSessionPool->Free(pSession);
}


// UserSession
LobbySession * DBFactory::AllocLobbySession() {
	if (m_pLobbySessionPool == NULL) {
		return NULL;
	}
	return m_pLobbySessionPool->Alloc();
}

void DBFactory::FreeLobbySession(LobbySession * pSession) {
	return m_pLobbySessionPool->Free(pSession);
}

LoginSession * DBFactory::AllocLoginSession() {
	if (m_pLoginSessionPool == NULL) {
		return NULL;
	}
	return m_pLoginSessionPool->Alloc();
}

void DBFactory::FreeLoginSession(LoginSession * pSession) {
	return m_pLoginSessionPool->Free(pSession);
}

// Server
TempSession * DBFactory::AllocTempSession() {
	if ( m_pTempSessionPool == NULL ) {
		return NULL;
	}
	TempSession * pSession = m_pTempSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}

	return pSession;
}

void DBFactory::FreeTempSession(TempSession * pServerSession) {
	return m_pTempSessionPool->Free(pServerSession);
}
