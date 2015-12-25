#ifndef _SysLog_INCLUDED_
#define _SysLog_INCLUDED_


// the log print to what place;
#define     LOG_FILE        0x1000          // 写到文件
#define     LOG_OUT         0x2000          // 打印到面上
#define     LOG_DB          0x4000          // 写到数据库中


// the Log print all type;
#define     LVL_INFO        0x0001          // 正常信息:连接类;
#define     LVL_TRACE       0x0002          // 痕迹信息:消息接收与发送;
#define     LVL_DEBUG       0x0004          // 调试信息:打印特定参数;
#define     LVL_ERROR       0x00F0          // 错误信息:打印错误提示;


// 日志消息接口
void DEBUG_MSG(int _level, const char * _format, ...);


#endif // _SysInfo_INCLUDED_
