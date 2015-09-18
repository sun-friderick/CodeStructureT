#ifndef __LOG_MAIN_H__
#define __LOG_MAIN_H__

#include "log/log_c.h"

#define ModuleName_LOG_MAIN       "logMain"
#define ModuleVersion_LOG_MAIN    "1111"
#define ModuleLogType_LOG_MAIN    LogType_SECURITY

/** 
* g_moduleLogMainNO： 记录当前模块在模块列表中的位置
* g_logMainLogLevel： 设置当前模块的log输出等级，每个模块都可设置独立的log输出等级 
**/
int g_moduleLogMainNO = 0;
static int g_logMainLogLevel = LOG_LEVEL_Debug;

#define logMainLogFatal(args...)     LogFatal(g_moduleLogMainNO, g_logMainLogLevel, args)
#define logMainLogError(args...)     LogError(g_moduleLogMainNO, g_logMainLogLevel, args)
#define logMainLogWarning(args...)   LogWarning(g_moduleLogMainNO, g_logMainLogLevel, args)
#define logMainLogInfo(args...)      LogInfo(g_moduleLogMainNO, g_logMainLogLevel, args)
#define logMainLogVerbose(args...)   LogVerbose(g_moduleLogMainNO, g_logMainLogLevel, args)
#define logMainLogDebug(args...)     LogDebug(g_moduleLogMainNO, g_logMainLogLevel, args)


#endif
