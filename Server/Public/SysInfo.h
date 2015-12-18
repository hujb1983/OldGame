#ifndef _SysInfo_INCLUDED_
#define _SysInfo_INCLUDED_

#include <Utility.h>

// 外接系统
struct sRemoteServerSystem {
	char  m_ip[16];
	WORD  m_port;  
};

// 接收系统
struct sAcceptSystem{
	WORD  m_session;
	WORD  m_threads;
};

// 连接系统
struct sConnectSystem{
	WORD  m_session;
	WORD  m_threads;
};

// 如Agent/Login配置外接系统
struct sClientSystem {
	char  			m_ip[16];
	WORD  			m_port;  
	BYTE  			m_iothd;
	UINT  			m_send;
	UINT  			m_recv;
	sAcceptSystem  	m_accept;
	sConnectSystem  m_connect;
};

// 本地系统
struct sLocalListenServerSystem {
	char  			m_ip[16];
	WORD  			m_port;  
	BYTE  			m_iothd;
	UINT  			m_send;
	UINT  			m_recv;
	sAcceptSystem  	m_accept;
	sConnectSystem  m_connect;
};

// 系统名称
extern char g_SysInfo_Name[80];		

// 系统编号
extern char g_SysInfo_Index[80];		

// LobbyServer
extern sRemoteServerSystem g_SysInfo_Connector_LobbyServer;

// LoggerServer
extern sRemoteServerSystem g_SysInfo_Connector_LoggerServer;

// DBServer
extern sRemoteServerSystem g_SysInfo_Connector_DBServer;

// AgentServer
extern sRemoteServerSystem g_SysInfo_ConnectTo_AgentServer;

// LocalClientListen
extern sLocalListenServerSystem g_SysInfo_Acceptor_Client;

// LocalServerListen
extern sLocalListenServerSystem g_SysInfo_Acceptor_Server;


// SysInfo
class SysInfo
{
public:
	SysInfo();
	virtual ~SysInfo();

public:
	static void SystemFile( const char * paths );
};

#endif // _SysInfo_INCLUDED_
