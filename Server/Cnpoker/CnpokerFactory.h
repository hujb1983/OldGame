#ifndef _CnpokerFactory_H_INCLUDED_
#define _CnpokerFactory_H_INCLUDED_

#include <Network.h>
#include <Utility.h>

#include "ServerSession.h"
#include "TempSession.h"
#include "AgentSession.h"
#include "DBSession.h"
#include "GameTable.h"

using namespace A;

class CnpokerFactory : public Singleton<CnpokerFactory>
{

public:
	CnpokerFactory();
	~CnpokerFactory();

private:
	MemoryFactory<DBSession> 	    * m_pDBSessionPool;
	MemoryFactory<AgentSession> 	* m_pAgentSessionPool;
	MemoryFactory<TempSession>	    * m_pTempSessionPool;
	MemoryFactory<GameTable> 	    * m_pGameTablePool;

public:
	void Init(void);
	void Release(void);

	// DBSession
	DBSession * AllocDBSession();
	void FreeDBSession(DBSession * pSession);

	// UserSession
	AgentSession * AllocAgentSession();
	void FreeAgentSession(AgentSession * pSession);

	// Server
	TempSession * AllocTempSession();
	void FreeTempSession(TempSession * pSession);

	// Server
	GameTable * AllocGameTable();
	void FreeGameTable(GameTable * pTable);
};

#endif
