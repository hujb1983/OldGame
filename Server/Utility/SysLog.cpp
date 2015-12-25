#include <Utility.h>
#include "SysLog.h"


void debug_printf_times(char * _format, int _size )
{
    if ( _size > 20 )
    {
        time_t now = time(NULL);
        struct tm * sysTime = localtime( &now );
        sprintf( _format, "%d.%d.%d %d:%d:%d ",
                sysTime->tm_year + 1900,
                sysTime->tm_mon  + 1,
                sysTime->tm_mday,
                sysTime->tm_hour,
                sysTime->tm_min,
                sysTime->tm_sec );
    }
}

// 打印类型
int debug_printf_type(int _level, char * _format, int _size )
{
    if ( _level == LVL_INFO  ) {
        strcat( _format, " [INFO] " );
        return 0;
    }
    else if ( _level == LVL_TRACE ) {
        strcat( _format, " [TRACE] " );
        return 0;
    }
    else if ( _level == LVL_DEBUG  ) {
        strcat( _format, " [DEBUG] " );
        return 0;
    }
    else if ( _level == LVL_ERROR ) {
        strcat( _format, " [ERROR] " );
        return 0;
    }
    return -1;
}

// 打印方法
int debug_printf_log( int _log_type )
{
    if ( (_log_type & LOG_FILE) == LOG_FILE  ) {
        return 0;
    }
    else if ( (_log_type & LOG_OUT) == LOG_OUT ) {
        return 0;
    }
    else if ( (_log_type & LOG_DB) == LOG_DB ) {
        return 0;
    }
    return -1;
}

void DEBUG_MSG(int _level, const char * _format, ...)
{
    if ( debug_printf_log( LOG_OUT ) != 0 ) {
        return;
    }

    char szMsg[4096] = {0};
    char szBuff[2048] = {0};

    // 打印时间
    debug_printf_times( szMsg, sizeof( szMsg ) );

    if ( debug_printf_type( _level, szMsg, sizeof(szMsg) ) != 0) {
        return;
    }

    va_list args;
    va_start(args, _format);
    vsnprintf(szBuff, sizeof(szBuff), _format, args);
    va_end(args);

    strcat( szMsg, szBuff );
    strcat( szMsg, ". \n" );
    printf(szMsg);
}
