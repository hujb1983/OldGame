#include "AgentInfo.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

AgentInfo g_AgentInfo;

AgentInfo::AgentInfo(void)
{
	pIniFile = NULL;
	
	m_mapAgentSrvList.clear();
}

AgentInfo::~AgentInfo(void)
{
	SAFE_DELETE(pIniFile);
	m_mapAgentSrvList.clear();
}

// 初始化
BOOL AgentInfo::Init( char *strFileName )
{
	pIniFile = new Yond_ini( strFileName );
	if (!pIniFile)
		return FALSE;

	if ( !pIniFile->m_isOpenFile ) {
		return FALSE;
	}

	return TRUE;
}

BOOL AgentInfo::GetString( char * strSection, char * szName, char * szDefault, char *pszValue )
{
	char szBuffer[256];

	const char *pChar = NULL;
	
	pChar = pIniFile->GetString( strSection, szName, szDefault );
	if(!pChar)
		return FALSE;

	memset( szBuffer, 0x0, sizeof(szBuffer) );
	strncpy( szBuffer, pChar, min( 256, strlen(pChar) ) );
	
	int nLen = (int)strlen( szBuffer ); 
	if ( nLen > 0 )
	{
		strcpy( pszValue, szBuffer );
		return TRUE;
	}

	return FALSE;
}

int AgentInfo::GetInt( char * strSection, char * szName, char * szDefault )
{	
	int retInt = 0;
	retInt = pIniFile->GetInteger( strSection, szName, -1 );
	return retInt;
}

int AgentInfo::SaveAgentInfo()
{
	int _max = pIniFile->GetInteger("AGENT_SIZE", "MAX_SIZE");
	//_min = pIniFile->GetInteger("AGENT_SIZE", "MIN_SIZE");
	
	char szBuf[16] = {0};
	for(int i=0; i<_max; ++i)
	{
		memset( szBuf, 0, sizeof(szBuf) );
		sprintf(szBuf, "AGENT_SERVER_%d", i+1);
		//printf("i = %d, szBuf = %s\n", i, szBuf);
		//m_mapAgentSrvList.insert( make_pair(i, szBuf) );
		m_mapAgentSrvList[i] = szBuf;	
	}
	return _max;
}

// 获取服务器端口信息
SERVER_INFO AgentInfo::GetAgentInfo( int nNum )
{
	SERVER_INFO sInfo;
	std::map<DWORD, std::string>::iterator itSrv = m_mapAgentSrvList.find(nNum);
	if (itSrv == m_mapAgentSrvList.end())
	{
		return sInfo;
	}
	
	char szName[32] = {0};
	strcpy(szName, itSrv->second.c_str());
	GetString(szName, "IP", "", sInfo.m_strIp);
	sInfo.m_dwPort = GetInt(szName, "PORT", "0");
	
	return sInfo;
}
