/**
 *  @file  main.c
 *  @version  1.0
 *  @brief  Brief this file mian.c for ...
 *  
 *  @auther  Frederick Sun
 *  @mail  mrsun.echo@gmail.com / echo_eric@yeah.net 
 *  @date  Date&Time
 **/

 
#include <stdio.h> 
#include <stdlib.h> 
#include "log/log_c.h"
//#include "SDL.h"
#include <SDL.h>

//#include "ffmpeg/ffmpeg.h"



#define WINDOW_WIDTH   640
#define WINDOW_HEIGHT  480
#define WINDOW_TITLE   "PlayerUI"
#define SCREEN_BPP   32

SDL_Surface *gpScreen;//显示表面

/*
struct video_surface{
    width, height, video_bpp, videoflags
    
}
 */
 

 
// 
int main(int argc, char** argv)
{
    
    //TODO: 系统初始化
    /*
        1、设置参数；
        2、加载配置文件；
        3、解析传入参数；
    */
    //定义一个结构体，用于加载ini配置文件中的参数作为初始化的默认值
    //iniparse_load();
    
    //解析通过命令行传入的参数，修改ini配置结构体并保存到配置文件
    //parse_options();
    //iniparse_save();
    
    
    
    //TODO: UI线程
    /*
        1、SDL 窗口设置;
        2、创建UI主线程；
    */
    int SDL_initflags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTTHREAD;
    if (SDL_Init (SDL_initflags) == -1) {  //初始化SDL子系统
        printf("Could not initialize SDL - %s\n (Did you set the DISPLAY variable?)\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);// 注册SDL_Quit，当退出时调用，使得退出时程序自动清理

    //创建32位640*480窗口
    gpScreen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF );
    if(!gpScreen) { 
        exit(1);
    }
    SDL_WM_SetCaption( WINDOW_TITLE, 0 );  //设置窗口标题
    
    //UI主线程，用来显示整个窗体上的播放控制UI，刷新消息直接由主事件循环处理，然后由专门的ui刷新线程处理
    //ui_display();
    
    
    
    
    
    
    //TODO: 音视频解码
    /*
        创建音视频解码主线程；
    */
    //stream_open();

    
    
    //TODO: 事件监听主循环
    /*
        1、监听UI事件： 鼠标点击、拖动、按键
        2、监听解码显示消息：刷新屏幕、更改窗口大小、
    */
    //event_loop();

    return 0;
}



