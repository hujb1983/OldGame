#include <Utility.h>
#include "Yond_json.h"

// 系统名称
char g_SysInfo_Name[80];		

// 系统编号
char g_SysInfo_Index[80];		

// LobbyServer
sExternServerSystem g_SysInfo_LobbyServer;

// LoggerServer
sExternServerSystem g_SysInfo_LoggerServer;

// DBServer
sExternServerSystem g_SysInfo_DBServer;

// AgentServer
sExternServerSystem g_SysInfo_AgentServer;

// LocalClientListen
sLocalListenServerSystem g_SysInfo_Client;

// LocalServerListen
sLocalListenServerSystem g_SysInfo_Server;

// ... name .... index 
int get_SysInfo_Base( cJSON * js_root )
{
	if ( js_root==NULL ) {
		return -1; 
	}
	
	JsonMap js_map;
	js_map.set_json(js_root);
	js_map.ReadString ( "name",  g_SysInfo_Name, sizeof(g_SysInfo_Name) );
	js_map.ReadInteger( "index", g_SysInfo_Index );
	return 0;
}

// Lobby的资料
void get_SysInfo_LobbyServer( cJSON * js_root )
{
	if ( js_root==NULL ) {
		return -1; 
	}
	
	JsonMap js_map;
	js_map.set_json(js_root);
	js_map.ReadString ( "ip",   g_SysInfo_Connector_LobbyServer.m_ip, 
						 sizeof(g_SysInfo_Connector_LobbyServer.m_ip) );
	js_map.ReadInteger( "port", g_SysInfo_Connector_LobbyServer.port );
	return 0;
}

// Client的资料
void get_SysInfo_AgentServer( cJSON * js_root )
{
	if ( js_root==NULL ) {
		return -1; 
	}
	
	JsonMap js_map;
	js_map.set_json(js_root);
	js_map.ReadString ( "ip",   g_SysInfo_ConnectTo_AgentServer.m_ip, 
						 sizeof(g_SysInfo_ConnectTo_AgentServer.m_ip) );
	js_map.ReadInteger( "port", g_SysInfo_ConnectTo_AgentServer.port );
	return 0;
}

// Client的资料
void get_SysInfo_Client( cJSON * js_root )
{
	if ( js_root==NULL ) {
		return -1; 
	}
	
	JsonMap js_map;
	js_map.set_json(js_root);
	js_map.ReadString ( "name",  g_SysInfo_Name, sizeof(g_SysInfo_Name) );
	js_map.ReadInteger( "index", g_SysInfo_Index );
	return 0;
}

// 取得所有的配置文件资料
void SysInfo::SystemFile( const char * paths )
{
	
	
}
