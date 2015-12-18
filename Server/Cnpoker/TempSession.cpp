#include "TempSession.h"
#include "CnpokerServer.h"

TempSession::TempSession()
	: m_bFirst(TRUE)
{
}

TempSession::~TempSession()
{

}

void TempSession::Clear()
{
	m_bFirst = TRUE;
	ServerSession::Clear();
}

void TempSession::OnRecv(BYTE *pMsg, WORD wSize)
{
	assert( m_bFirst == TRUE);
	if ( !m_bFirst) {
		return;
	}

	CnpokerFactory::Instance()->FreeTempSession(this);
}

void TempSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{

}

void TempSession::OnLogString( char * pszLog)
{

}

