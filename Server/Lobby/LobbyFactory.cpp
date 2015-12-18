#include "LobbyFactory.h"

LobbyFactory::LobbyFactory()
{
    m_pDBSessionPool      = NULL;
    m_pAgentSessionPool = NULL;
    m_pTempSessionPool    = NULL;
    m_pLobbyUserPool      = NULL;
}

LobbyFactory::~LobbyFactory()
{
	if (m_pDBSessionPool) 	    delete m_pDBSessionPool;
	if (m_pAgentSessionPool) 	delete m_pAgentSessionPool;
	if (m_pTempSessionPool)     delete m_pTempSessionPool;
	if (m_pLobbyUserPool)       delete m_pLobbyUserPool;
}

void LobbyFactory::Init()
{
	m_pDBSessionPool 	    = new MemoryFactory<DBSession>;
	m_pAgentSessionPool 	= new MemoryFactory<AgentSession>;
	m_pTempSessionPool 	    = new MemoryFactory<TempSession>;
	m_pLobbyUserPool 	    = new MemoryFactory<LobbyUser>;

	// UserSession
	m_pDBSessionPool->Initialize(1,1);
	m_pAgentSessionPool->Initialize(9,1);
	m_pTempSessionPool->Initialize(9,1);
	m_pLobbyUserPool->Initialize(999,1);
}

// UserSession
DBSession * LobbyFactory::AllocDBSession() {
	if (m_pDBSessionPool == NULL) {
		return NULL;
	}
	return m_pDBSessionPool->Alloc();
}

void LobbyFactory::FreeDBSession(DBSession * pUser) {
	return m_pDBSessionPool->Free(pUser);
}


// UserSession
AgentSession * LobbyFactory::AllocAgentSession() {
	if (m_pAgentSessionPool == NULL) {
		return NULL;
	}
	return m_pAgentSessionPool->Alloc();
}

void LobbyFactory::FreeAgentSession(AgentSession * pUser) {
	return m_pAgentSessionPool->Free(pUser);
}

// Server
TempSession * LobbyFactory::AllocTempSession() {
	if ( m_pTempSessionPool == NULL ) {
		return NULL;
	}
	TempSession * pSession = m_pTempSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}

	return pSession;
}

void LobbyFactory::FreeTempSession(TempSession * pServerSession) {
	return m_pTempSessionPool->Free(pServerSession);
}

LobbyUser * LobbyFactory::AllocUserSession() {
	if ( m_pLobbyUserPool == NULL ) {
		return NULL;
	}
	return m_pLobbyUserPool->Alloc();
}

void LobbyFactory::FreeUserSession(LobbyUser * pUser) {
	return m_pLobbyUserPool->Free(pUser);
}
