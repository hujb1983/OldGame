#include <Utility.h>
#include "Sysconfig.h"
#include "JsonParser.h"

// 系统名称
char g_SysInfo_Name[80];

// 系统编号
char g_SysInfo_Index[80];

// LobbyServer
sRemoteServerSystem g_SysInfo_LobbyServer;

// LoggerServer
sRemoteServerSystem g_SysInfo_LoggerServer;

// DBServer
sRemoteServerSystem g_SysInfo_DBServer;

// AgentServer
sRemoteServerSystem g_SysInfo_AgentServer;

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
void get_SysInfo_RemoteServerSystem( cJSON * js_root,  sRemoteServerSystem & _server )
{
	if ( js_root==NULL ) {
		return -1;
	}

	JsonMap js_map;
	js_map.set_json(js_root);
	js_map.ReadString ( "ip",   _server.m_ip, sizeof(_server.m_ip) );
	js_map.ReadInteger( "port", _server.port );
	return 0;
}

// 代理服务器
void get_SysInfo_AgentServer( cJSON * js_root ) {
	get_SysInfo_RemoteServerSystem( js_root, g_SysInfo_ConnectTo_AgentServer );
}

// 代理服务器
void get_SysInfo_LobbyServer( cJSON * js_root ) {
	get_SysInfo_RemoteServerSystem( js_root, g_SysInfo_Connector_LobbyServer );
}

void get_SysInfo_DBServer( cJSON * js_root ) {
	get_SysInfo_RemoteServerSystem( js_root, g_SysInfo_Connector_LobbyServer );
}

void get_SysInfo_LoggerServer( cJSON * js_root ) {
	get_SysInfo_RemoteServerSystem( js_root, g_SysInfo_Connector_LoggerServer );
}

// Client的资料
void get_SysInfo_LocalListenServerSystem( cJSON * js_root, sLocalListenServerSystem & listen )
{
	if ( js_root==NULL ) {
		return -1;
	}

	sAcceptSystem  & _accept  = listen.m_accept;
	sConnectSystem & _connect = listen.m_connect;
	WORD  _port;
	BYTE  _iothd;
	UINT  _send;
	UINT  _recv;
	WORD  _session;
	WORD  _threads;

	JsonMap js_map;
	js_map.set_json(js_root);
	js_map.ReadString  ( "ip", listen.m_ip, sizeof(listen.m_ip) );
	js_map.ReadInteger ( "port",   _port  );
	js_map.ReadInteger ( "_iothd", _iothd );

	// 3个字项
	JsonMap js_buff, js_accp, js_connect;
	js_map.ReadJson ( "buff",    js_buff    );
	js_map.ReadJson ( "accept",  js_accp    );
	js_map.ReadJson ( "connect", js_connect );

	// 缓存
	js_map.ReadInteger  ( "send",   _send );
	js_map.ReadInteger  ( "recv",   _recv );

	// 接受队列
	js_map.ReadInteger  ( "session",   _session );
	js_map.ReadInteger  ( "threads",   _threads );

	// 连接队列
	js_map.ReadInteger  ( "session",   _session );
	js_map.ReadInteger  ( "threads",   _threads );
}

// 取得所有的配置文件资料
void SysInfo::SystemConfig( const char * paths )
{


}
