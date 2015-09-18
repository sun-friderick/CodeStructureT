#include <stdio.h>

#include "log/LogC.h"
#include "LogWidget.h"
#include "Widget.h"

/**
 *  NOTE:
 *      在使用之前先调用该函数，对该模块的log组件进行初始化
 * 
 **/
int log_ui_init()
{

    //register to  module list
    g_moduleUINO = registerModule(ModuleName_UI, ModuleVersion_UI, ModuleLogType_UI);

    printf("==========================================\n");
#if 1
    uiLogFatal("=====uiLogFatal==[%d]=--------------=\n", g_moduleUINO);
    uiLogError("=====uiLogError==[%d]=--------------=\n", g_moduleUINO);
    uiLogWarning("=====uiLogWarning==[%d]=--------------=\n", g_moduleUINO);
    uiLogInfo("=====uiLogInfo==[%d]=--------------=\n", g_moduleUINO);
    uiLogVerbose("=====uiLogVerbose==[%d]=--------------=\n", g_moduleUINO);
    uiLogDebug("=====uiLogDebug==[%d]=--------------=\n", g_moduleUINO);
#endif
    return 0;
}

