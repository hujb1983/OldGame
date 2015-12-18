#ifndef _AGENTINFO_H_INCLUDED_
#define _AGENTINFO_H_INCLUDED_

#include <Common.h>
#include <Network.h>
#include <Utility.h>
#include <Public.h>
using namespace std;


class AgentInfo
{
public:
	AgentInfo(void);
	~AgentInfo(void);

	// 初始化
	BOOL	Init(char * strFileName);

	BOOL	GetString( char * strSection, char * szName, char * szDefault, char *pszValue);
	int 	GetInt( char * strSection, char * szName, char * szDefault );

	// 保存服务器端口信息
	int SaveAgentInfo();
	// 获取服务器端口信息
	SERVER_INFO GetAgentInfo( int nNum );

private:
	Yond_ini * pIniFile;
	
	std::map<DWORD, std::string> m_mapAgentSrvList;
};

extern AgentInfo g_AgentInfo;

#endif // _AGENTINFO_H_INCLUDED_
