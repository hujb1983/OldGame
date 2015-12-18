#include "LoggerFactory.h"

LoggerFactory::LoggerFactory()
{
	m_pUserSessionPool  = NULL;
	m_pDBSessionPool	= NULL;
}

LoggerFactory::~LoggerFactory()
{
	if (m_pDBSessionPool) 	    delete m_pDBSessionPool;        // DBSession ->
	if (m_pLoginSessionPool) 	delete m_pLoginSessionPool;        // Server ->
	if (m_pLobbySessionPool) 	delete m_pLobbySessionPool;      // Client ->
	if (m_pAgentSessionPool) 	delete m_pAgentSessionPool;        // Server ->
	if (m_pGameSessionPool) 	delete m_pGameSessionPool;      // Client ->
	if (m_pTempSessionPool) 	delete m_pTempSessionPool;        // Server ->
}

void LoggerFactory::Init()
{
	m_pDBSessionPool	= new MemoryFactory<DBSession>;
	m_pLoginSessionPool	= new MemoryFactory<LoginSession>;
    m_pLobbySessionPool	= new MemoryFactory<LobbySession>;
	m_pAgentSessionPool	= new MemoryFactory<AgentSession>;
    m_pGameSessionPool	= new MemoryFactory<GameSession>;
	m_pTempSessionPool	= new MemoryFactory<TempSession>;
	m_pDBSessionPool->Initialize(1,1);
	m_pLoginSessionPool->Initialize(1,1);
    m_pLobbySessionPool->Initialize(1,1);
	m_pAgentSessionPool->Initialize(1,1);
    m_pGameSessionPool->Initialize(1,1);
	m_pTempSessionPool->Initialize(1,1);
}

// DB Server
DBSession * LoggerFactory::AllocDBSession() {
	assert ( m_pDBSessionPool );
	DBSession * pSession = m_pDBSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoggerFactory::FreeUserSession(DBSession * pSession) {
    return m_pDBSessionPool->Free(pSession);
}

// Login Server
LoginSession * LoggerFactory::AllocLoginSession() {
    assert ( m_pLoginSessionPool );
	LoginSession * pSession = m_pLoginSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoggerFactory::FreeLoginSession(LoginSession * pSession) {
    return m_pLoginSessionPool->Free(pSession);
}

// Lobby Server
LobbySession * LoggerFactory::AllocLobbySession() {
    assert ( m_pLobbySessionPool );
	LobbySession * pSession = m_pLobbySessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LobbyFactory::FreeLobbySession( LobbySession * pSession ) {
    return m_pLobbySessionPool->Free(pSession);
}

// Agent Server
AgentSession * LoggerFactory::AllocAgentSession() {
    assert ( m_pAgentSessionPool );
	AgentSession * pSession = m_pAgentSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoggerFactory::FreeAgentSession(AgentSession * pSession) {
    return m_pAgentSessionPool->Free(pSession);
}

// Games Server
GameSession * LoggerFactory::AllocGameSession() {
    assert ( m_pGameSessionPool );
	GameSession * pSession = m_pGameSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoggerFactory::FreeGameSession(GameSession * pSession) {
    return m_pGameSessionPool->Free(pSession);
}

// Temp Server
TempSession * LoggerFactory::AllocTempSession() {
    assert ( m_pTempSessionPool );
	TempSession * pSession = m_pTempSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoggerFactory::FreeTempSession( TempSession * pSession ) {
    return m_pTempSessionPool->Free(pSession);
}
