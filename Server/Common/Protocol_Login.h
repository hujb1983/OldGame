#ifndef _PROTOCOLCA_LOGIN_H_INCLUDED_
#define _PROTOCOLCA_LOGIN_H_INCLUDED_

#include "Protocol.h"

enum eLOGIN_PROTOCOL
{
	// 登陆
	Login_REQ			= 100,
	Login_DBR,
	Login_ANC,
	Login_NAK,

    PreLogin_REQ        = 110,
	PreLogin_DBR,
	PreLogin_ANC,
	PreLogin_NAK,

	Relogin_REQ         = 120,
	Relogin_DBR,
	Relogin_ANC,
	Relogin_NAK,
	Relogin_BRD,

	GamePacket_SYN      = 130,
	GamePacket_DBR,
	GamePacket_ANC,

	Logout_REQ          = 140,
	Logout_DBR,
	Logout_ANC,
	Logout_NAK,

	Offline_NAK         = 150, 	 // 离线
	Offline_BRD,
};

#endif // _PROTOCOLCA_H_INCLUDED_
