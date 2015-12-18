#include "LobbyUpdate.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include <JsonParser.h>


void LobbyUpdate::JoinAgentSession ( AgentSession * pSession )
{
    WORD _wIndex = this->AllocSessionKey();
    pSession->SetIndexKey(_wIndex);
    if ( _wIndex != 0 ) {
        pSession->SetIndexKey( _wIndex );
        mapAgentObject[_wIndex] = pSession;
        m_pAllAgent.push_back( pSession );
    }
    printf("[LobbyUpdate::JoinAgentSession] pSession = %d \n", _wIndex);
}

void LobbyUpdate::QuitAgentSession ( AgentSession * pSession )
{
    WORD _wIndex = pSession->GetIndexKey();
    if ( _wIndex != 0 ) {
        mapAgentObject[_wIndex] = NULL;
        m_pAllAgent.remove( pSession );
        this->FreeSessionKey( _wIndex );
        printf( "[LobbyUpdate::QuitAgentSession] pSession = %d \n", _wIndex );
    }
}

/***********************************************

************************************************/
void LobbyUpdate::JoinLobby ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    if (_userport != 0) {
        iOnlinesInfo++;
        LobbyUser * pUser = LobbyFactory::Instance()->AllocUserSession();
        if (pUser) {
            pUser->SetHashKey(_userport);
        }

        AgentSession * pSession = (AgentSession*) pServerSession;
        if (pSession) {
            pSession->AddUser(_userport, pUser);
            printf( "[LobbyUpdate::JoinLobby] userkey = %d \n", _userport );
        }
    }
}

void LobbyUpdate::QuitLobby ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    if (_userport != 0) {
        AgentSession * pSession = (AgentSession*) pServerSession;
        if (pSession) {
            pSession->RemoveUser( _userport );
            printf( "[LobbyUpdate::QuitLobby] userkey = %d \n", _userport );
        }
    }
}

void LobbyUpdate::JoinTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _agentkey;
    js_map.ReadInteger( "agentkey", _agentkey );
    SendToAgentSession( (WORD)_agentkey, (BYTE*) pMsg, wSize );
}

void LobbyUpdate::QuitTable ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    printf("LobbyUpdate::QuitTable\n");
    char szMsg[1024] = {0};
    snprintf( szMsg, sizeof(szMsg), " { \"protocol\":\"%d\", \"userkey\":\"%d\" } ",
            MAKEDWORD( Games_Protocol, QuitTable_REQ), _userport );

    int nLen = strlen(szMsg);
    g_LobbyServer->SendToDBServer( (BYTE*) szMsg, nLen );
}

void LobbyUpdate::RoomInfo_REQ  (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    int _userroom;
    js_map.ReadInteger( "userkey",  _userport );

    char szInfo[1024] = {0};
    int nLen = JsonRoomInfo( szInfo, sizeof(szInfo) );
    if ( nLen > 0) {
        char buff[4096] = {0};
        snprintf( buff, sizeof(buff), szInfo, _userport);
        pServerSession->Send( (BYTE*)buff, strlen(buff) );
    }
}

void LobbyUpdate::TableInfo_REQ (ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize)
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    int _userroom;
    js_map.ReadInteger( "userkey", _userport );
    js_map.ReadInteger( "roomid",  _userroom );

    char szInfo[1024] = {0};
    int nLen = JsonTableInfo( _userroom, szInfo, sizeof(szInfo) );
    if ( nLen > 0) {
        char buff[4096] = {0};
        snprintf( buff, sizeof(buff), szInfo, _userport);
        pServerSession->Send( (BYTE*)buff, strlen(buff) );
    }
}


void LobbyUpdate::Onlines_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    char szInfo[1024] = {0};
    int nLen = JsonOnlineInfo( szInfo, sizeof(szInfo) );

    MSG_ONLINEINFO_ANC send;
    char buff[4096] = {0};
    snprintf( buff, sizeof(szInfo), "{ \"protocol\":\"%d\", \"userkey\":\"%d\", %s }",
             send.m_dwProtocol, _userport, szInfo);

    nLen = strlen (szInfo);
    if ( nLen > 0) {
        pServerSession->Send( (BYTE*)buff, strlen(buff) );
    }
}

void LobbyUpdate::WRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    char szInfo[8192] = {0};
    int nLen = JsonWeekRank( szInfo, sizeof(szInfo) );
    if ( nLen > 0) {
        char buff[4096] = {0};
        snprintf( buff, sizeof(buff), szInfo, _userport);
        pServerSession->Send( (BYTE*)buff, strlen(buff) );
    }
}

void LobbyUpdate::DRankInfo_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pMsg ) == -1 ) {
        return;
    }

    int _userport;
    js_map.ReadInteger( "userkey", _userport );

    char szInfo[8192] = {0};
    int nLen = JsonDayRank( szInfo, sizeof(szInfo) );
    if ( nLen > 0) {
        char buff[4096] = {0};
        snprintf( buff, sizeof(buff), szInfo, _userport);
        pServerSession->Send( (BYTE*)buff, strlen(buff) );
    }
}

