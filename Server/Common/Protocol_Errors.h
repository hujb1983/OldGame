#ifndef _PROTOCOLCA_ERRORS_H_INCLUDED_
#define _PROTOCOLCA_ERRORS_H_INCLUDED_

#include "Protocol.h"

/***** Client -> Login *****/
enum eERRORS_PROTOCOL
{
    Correct_NAK		    =      0,      // 正确通知
	ClientLogin_NAK		= 0xFFFF,      // 登陆失败
};

#endif // _PROTOCOLCA_H_INCLUDED_
