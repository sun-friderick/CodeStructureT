#ifndef __LOG_UI_H__
#define __LOG_UI_H__

#include "log/LogC.h"
#include "build_info.h"

#define ModuleName_UI       "ui"
#define ModuleVersion_UI    g_make_svn_version
#define ModuleLogType_UI    LogType_SECURITY

/** 
* g_moduleUINO： 记录当前模块在模块列表中的位置
* g_uiLogLevel： 设置当前模块的log输出等级，每个模块都可设置独立的log输出等级 
**/
int g_moduleUINO;
static int g_uiLogLevel = LOG_LEVEL_Debug;

#define uiLogFatal(args...)     LogFatal(g_moduleUINO, g_uiLogLevel, args)
#define uiLogError(args...)     LogError(g_moduleUINO, g_uiLogLevel, args)
#define uiLogWarning(args...)   LogWarning(g_moduleUINO, g_uiLogLevel, args)
#define uiLogInfo(args...)      LogInfo(g_moduleUINO, g_uiLogLevel, args)
#define uiLogVerbose(args...)   LogVerbose(g_moduleUINO, g_uiLogLevel, args)
#define uiLogDebug(args...)     LogDebug(g_moduleUINO, g_uiLogLevel, args)



#endif

