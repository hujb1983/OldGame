#include "CnpokerFactory.h"

CnpokerFactory::CnpokerFactory()
{
    m_pDBSessionPool    = NULL;
	m_pAgentSessionPool = NULL;
	m_pTempSessionPool  = NULL;
	m_pGameTablePool    = NULL;
}

CnpokerFactory::~CnpokerFactory()
{
    if (m_pDBSessionPool) 	    delete m_pDBSessionPool;
	if (m_pAgentSessionPool) 	delete m_pAgentSessionPool;
	if (m_pTempSessionPool) 	delete m_pTempSessionPool;
	if (m_pGameTablePool) 	    delete m_pGameTablePool;
}

void CnpokerFactory::Init()
{
    m_pDBSessionPool    = new MemoryFactory<DBSession>;
	m_pAgentSessionPool = new MemoryFactory<AgentSession>;
	m_pTempSessionPool 	= new MemoryFactory<TempSession>;
	m_pGameTablePool    = new MemoryFactory<GameTable>;

	// UserSession
	m_pDBSessionPool->Initialize(1,1);      // 已经认证用户；
	m_pAgentSessionPool->Initialize(1,1);      // 已经认证用户；
	m_pTempSessionPool->Initialize(1,1);      // 未认证用户；
	m_pGameTablePool->Initialize(100,1);
}

// UserSession
DBSession * CnpokerFactory::AllocDBSession() {
	if (m_pDBSessionPool == NULL) {
		return NULL;
	}
	return m_pDBSessionPool->Alloc();
}

void CnpokerFactory::FreeDBSession(DBSession * pUser) {
	return m_pDBSessionPool->Free(pUser);
}


// UserSession
AgentSession * CnpokerFactory::AllocAgentSession() {
	if (m_pAgentSessionPool == NULL) {
		return NULL;
	}
	return m_pAgentSessionPool->Alloc();
}

void CnpokerFactory::FreeAgentSession(AgentSession * pUser) {
	return m_pAgentSessionPool->Free(pUser);
}

// Server
TempSession * CnpokerFactory::AllocTempSession() {
	if ( m_pTempSessionPool == NULL ) {
		return NULL;
	}

	TempSession * pSession = m_pTempSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}

	return pSession;
}

void CnpokerFactory::FreeTempSession(TempSession * pServerSession) {
	return m_pTempSessionPool->Free(pServerSession);
}

// Server
GameTable * CnpokerFactory::AllocGameTable()
{
	if ( m_pGameTablePool == NULL ) {
		return NULL;
	}
	return m_pGameTablePool->Alloc();
}

void CnpokerFactory::FreeGameTable(GameTable * pTable) {
	return m_pGameTablePool->Free(pTable);
}

