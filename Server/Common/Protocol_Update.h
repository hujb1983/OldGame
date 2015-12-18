#ifndef _PROTOCOLCA_UPDATE_H_INCLUDED_
#define _PROTOCOLCA_UPDATE_H_INCLUDED_

#include "Protocol.h"

enum eUPDATE_PROTOCOL
{
  	RoomInfo_SYN			= 600,      // 房间信息
	RoomInfo_DBR,
	RoomInfo_ANC,

	TableInfo_SYN           = 610,      // 桌子信息
	TableInfo_DBR,
	TableInfo_ANC,

	OnlineInfo_SYN          = 620,      // 在线信息
	OnlineInfo_DBR,
	OnlineInfo_ANC,

	WRankInfo_SYN           = 630,      // 周排行版信息
	WRankInfo_DBR,
	WRankInfo_ANC,

	DRankInfo_SYN           = 640,      // 日排行版信息
	DRankInfo_DBR,
	DRankInfo_ANC,
};

#endif // _PROTOCOLCA_H_INCLUDED_
