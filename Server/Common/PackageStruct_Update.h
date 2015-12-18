#ifndef _PACKAGESTRUCT_UPDATE_H_INCLUDED_
#define _PACKAGESTRUCT_UPDATE_H_INCLUDED_

#include "PackageStruct.h"
#include "Protocol_Update.h"
#include "CommStruct.h"

#pragma pack(push, 1)


struct MSG_GAMEPACKET_SYN : public MSG_BASE_FORWARD
{
	MSG_GAMEPACKET_SYN() {
		memset( this, 0, sizeof(MSG_GAMEPACKET_SYN) );
		m_byCategory = Games_Protocol;
		m_byProtocol = GamePacket_SYN;
	}
};

struct MSG_GAMEPACKET_ANC : public MSG_BASE_FORWARD
{
	MSG_GAMEPACKET_ANC() {
		memset( this, 0, sizeof(MSG_GAMEPACKET_ANC) );
		m_byCategory = Games_Protocol;
		m_byProtocol = GamePacket_ANC;
	}
};


// Login Req
struct MSG_TABLEINFO_SYN : public MSG_BASE_FORWARD
{
	MSG_TABLEINFO_SYN() {
		memset( this, 0, sizeof(MSG_TABLEINFO_SYN) );
		m_byCategory = Update_Protocol;
		m_byProtocol = TableInfo_SYN;
	}
};

// Login Req
struct MSG_TABLEINFO_ANC : public MSG_BASE_FORWARD
{
	MSG_TABLEINFO_ANC() {
		memset( this, 0, sizeof(MSG_TABLEINFO_ANC) );
		m_byCategory = Update_Protocol;
		m_byProtocol = TableInfo_ANC;
	}
};

// Login Req
struct MSG_ONLINEINFO_SYN : public MSG_BASE_FORWARD
{
	MSG_ONLINEINFO_SYN() {
		memset( this, 0, sizeof(MSG_ONLINEINFO_SYN) );
		m_byCategory = Update_Protocol;
		m_byProtocol = OnlineInfo_SYN;
	}
};

// Login Req
struct MSG_ONLINEINFO_ANC : public MSG_BASE_FORWARD
{
	MSG_ONLINEINFO_ANC() {
		memset( this, 0, sizeof(MSG_ONLINEINFO_ANC) );
		m_byCategory = Update_Protocol;
		m_byProtocol = OnlineInfo_ANC;
	}
};

// Login Anc
struct MSG_WRANKING_SYN : public MSG_BASE_FORWARD
{
	MSG_WRANKING_SYN() {
		memset( this, 0, sizeof(MSG_WRANKING_SYN) );
		m_byCategory = Update_Protocol;
		m_byProtocol = WRankInfo_SYN;
	}
};

// Login Anc
struct MSG_WRANKING_ANC : public MSG_BASE_FORWARD
{
	MSG_WRANKING_ANC() {
		memset( this, 0, sizeof(MSG_WRANKING_ANC) );
		m_byCategory = Update_Protocol;
		m_byProtocol = WRankInfo_ANC;
	}
};

// Login Anc
struct MSG_DRANKING_SYN : public MSG_BASE_FORWARD
{
	MSG_DRANKING_SYN() {
		memset( this, 0, sizeof(MSG_DRANKING_SYN) );
		m_byCategory = Update_Protocol;
		m_byProtocol = DRankInfo_SYN;
	}
};

// Login Anc
struct MSG_DRANKING_ANC : public MSG_BASE_FORWARD
{
	MSG_DRANKING_ANC() {
		memset( this, 0, sizeof(MSG_DRANKING_ANC) );
		m_byCategory = Update_Protocol;
		m_byProtocol = DRankInfo_ANC;
	}
};

#pragma pack(pop)

#endif // _PACKAGESTRUCT_GA_H_INCLUDED_
