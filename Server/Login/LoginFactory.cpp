#include "LoginFactory.h"

LoginFactory::LoginFactory()
{
	m_pUserSessionPool  = NULL;
	m_pDBSessionPool	= NULL;
}

LoginFactory::~LoginFactory()
{
	if (m_pUserSessionPool) 	delete m_pUserSessionPool;      // Client ->
	if (m_pDBSessionPool) 	    delete m_pDBSessionPool;        // Server ->
}

void LoginFactory::Init()
{
	m_pUserSessionPool	= new MemoryFactory<UserSession>;
	m_pDBSessionPool	= new MemoryFactory<DBSession>;
	m_pUserSessionPool->Initialize(999,1);
	m_pDBSessionPool->Initialize(1,1);
}

// User
UserSession * LoginFactory::AllocUserSession() {
	assert ( m_pUserSessionPool );
	UserSession * pSession = m_pUserSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Release();
	}
	return pSession;
}
void LoginFactory::FreeUserSession(UserSession * pServerSession) {
	return m_pUserSessionPool->Free(pServerSession);
}

// Temp Server
DBSession * LoginFactory::AllocDBSession() {
	assert ( m_pDBSessionPool );
	DBSession * pSession = m_pDBSessionPool->Alloc();
	if ( pSession != NULL ) {
		pSession->Clear();
	}
	return pSession;
}
void LoginFactory::FreeDBSession(DBSession * pServerSession) {
	return m_pDBSessionPool->Free(pServerSession);
}


