#include "LobbyUpdate.h"
#include "LobbyFactory.h"
#include "LobbyServer.h"
#include <JsonParser.h>

LobbyUpdate g_LobbyUpdate;

LobbyUpdate::LobbyUpdate() {
    memset( this, 0x0, sizeof(this) );
}

LobbyUpdate::~LobbyUpdate() {
}

/* 从数据库查询到所有信息 */
int LobbyUpdate::Init()
{
    printf("LobbyUpdate::Init\n");
    m_cObjKey.Create(1, 0xFFFF);

    memset( this, 0x0, sizeof(this) );
    this->QueryTextInfo();
    this->QueryRoomInfo();
    this->QueryTableInfo();
    this->QueryOnlineInfo();
    this->QueryWeekRank();
    this->QueryDayRank();
}

int LobbyUpdate::Update(DWORD dwDeltaTick)
{
    if ( m_dwUpdateTime < dwDeltaTick ) {
        printf("%d, %d \n", dwDeltaTick, m_dwUpdateTime);
        m_dwUpdateTime = dwDeltaTick + 50000;
        SendOnlinesToAllUser();
    }
    return TRUE;
}

int LobbyUpdate::SendToAgentSession( WORD wport, BYTE * pMsg, WORD wSize )
{
    if ( mapAgentObject[wport] ) {
        mapAgentObject[wport]->Send( pMsg, wSize );
    }
}

WORD LobbyUpdate::AllocSessionKey() {
    return m_cObjKey.GetKey();
}

void LobbyUpdate::FreeSessionKey( WORD _wIndex ) {
    m_cObjKey.RestoreKey( _wIndex );
}

int LobbyUpdate::SendAllUser( BYTE * pMsg, WORD wSize )
{
    AgentSession * pAgent = m_pAllAgent.get_frist();
    for ( ; pAgent ; pAgent = pAgent->get_next() ) {
        pAgent->SendAllUser(pMsg, wSize);
    }
}

/* 发送在线人数到所有用户列表 */
int LobbyUpdate::SendOnlinesToAllUser()
{
    char szInfo[1024] = {0};
    int nLen = JsonOnlineInfo( szInfo, sizeof(szInfo) );
    SendAllUser( (BYTE*) szInfo, nLen);
}

void LobbyUpdate::SendUpdateTableOnlinesToAllUser ( )
{
    /* char szInfo[1024] = {0};
    int nLen = JsonOnlineInfo( szInfo, sizeof(szInfo) );
    SendAllUser( (BYTE*) szInfo, nLen); */
}


