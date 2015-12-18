#ifndef _LOGIN_FACTORY_H_INCLUDED_
#define _LOGIN_FACTORY_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "UserSession.h"
#include "DBSession.h"

using namespace A;

class LoginFactory : public Singleton<LoginFactory>
{
public:

	LoginFactory();
	~LoginFactory();

private:
	MemoryFactory<UserSession>			* m_pUserSessionPool;   // <Client>
	MemoryFactory<DBSession>	        * m_pDBSessionPool;     // <Server>

public:
	void Init(void);
	void Release(void);

public:
	// User
	UserSession * AllocUserSession();
	void FreeUserSession(UserSession * pUser);

	// Temp Server
	DBSession * AllocDBSession();
	void FreeDBSession(DBSession * pServerSession);
};

#endif
