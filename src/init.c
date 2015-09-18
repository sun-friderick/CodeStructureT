#include <stdio.h>

#include "log/log_c.h"
#include "build_info.h"

extern struct _Version  g_version;


/**
 *  NOTE:
 *      在使用之前先调用该函数，对该模块的log组件进行初始化
 **/
int version_init()
{
    //init g_version
    g_version.Major = g_make_major_version;
    g_version.Minor = g_make_minor_version;
    g_version.BuildVersion = g_make_svn_version;

    printf("===============version_init===============\n");
    return 0;
}


int log_init()
{
    log_message_init();
    log_ui_init();
    log_player_init();
    log_utils_init();
    
    printf("===============log_init============\n");
    return 0;
}


int envirment_init()
{
    //TODO:
    //  init: sdl ffmpeg
    
    
    return 0;
}



int env_init()
{
    
    
    
    return 0;
}



