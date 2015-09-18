#include <stdio.h>
#include "LogC.h"

#include "upgrade.h"
#include "example.h"

#if 0
#include "Player/LogPlayer.h"
#include "Widget/LogUI.h"
#include "Monitor/logMonitor.h"
#include "Utils/logParseFiles.h"
#else
#include "Player.h"
#include "Widget.h"
#include "Monitor.h"
#include "Utils.h"
#endif


extern struct _Version  g_version;

int test ()
{
    //init g_version
    g_version.Major = 9;
    g_version.Minor = 88;
    g_version.BuildVersion = 777;

    //register to  module list
    g_moduleUpgradeNO = registerModule(ModuleName_UPGRADE, ModuleVersion_UPGRADE, ModuleLogType_UPGRADE);
    g_moduleBuildNO =  registerModule(ModuleName_BUILD, ModuleVersion_BUILD, ModuleLogType_BUILD);
    
    upgradeLogFatal("=====upgradeLogFatal==[%d]=--------------=\n", g_moduleUpgradeNO);
    upgradeLogError("=====upgradeLogError==[%d]=--------------=\n", g_moduleUpgradeNO);
    upgradeLogWarning("=====upgradeLogWarning==[%d]=--------------=\n", g_moduleUpgradeNO);
    upgradeLogInfo("=====upgradeLogInfo==[%d]=--------------=\n", g_moduleUpgradeNO);
    upgradeLogVerbose("=====upgradeLogVerbose==[%d]=--------------=\n", g_moduleUpgradeNO);
    upgradeLogDebug("=====upgradeLogDebug==[%d]=--------------=\n", g_moduleUpgradeNO);
    
    printf("==========================================\n");
    buildLogFatal("=====buildLogFatal==[%d]=--------------=\n", g_moduleBuildNO);
    buildLogError("=====buildLogError==[%d]=--------------=\n", g_moduleBuildNO);
    buildLogWarning("=====buildLogWarning==[%d]=--------------=\n", g_moduleBuildNO);
    buildLogInfo("=====buildLogInfo==[%d]=--------------=\n", g_moduleBuildNO);
    buildLogVerbose("=====buildLogVerbose==[%d]=--------------=\n", g_moduleBuildNO);
    buildLogDebug("=====buildLogDebug==[%d]=--------------=\n", g_moduleBuildNO);

    
    return 0;
}


extern int log_monitor_init();
extern int log_player_init();
extern int log_utils_init();
extern int log_ui_init();;

int main()
{
    test();
    
    log_monitor_init();
    log_player_init();
    log_utils_init();
    log_ui_init();;
    
    return 0;
}






