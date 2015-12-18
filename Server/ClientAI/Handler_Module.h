#ifndef _Handler_Module_H_INCLUDED_
#define _Handler_Module_H_INCLUDED_

#include <Utility.h>
#include <Common.h>
#include <Network.h>
#include "JsonParser.h"

void MSG_Handler_EnteryType_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_Login_ANC		( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_GamePacket_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_RoomInfo_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_TableInfo_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_Onlines_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_WRankInfo_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_DRankInfo_ANC	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_JoinGame_BRD	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_StartGame_BRD	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_InitCards_BRD	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_ShowCards_BRD	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_CalledBank_BRD	( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_CreateBank_BRD ( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_Discard_BRD	( int cnSock, char * pInput, WORD wSize);

void MSG_Handler_CalledLicense_BRD	 ( int cnSock, char * pInput, WORD wSize);
void MSG_Handler_DiscardsLicense_BRD ( int cnSock, char * pInput, WORD wSize);

#endif // _AGENTPLAYER_H_INCLUDED_