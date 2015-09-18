#! /bin/sh

#最简单的基于FFmpeg的视频播放器----命令行编译
#Simplest FFmpeg Player----Compile in Shell 
#
#雷霄骅 Lei Xiaohua
#leixiaohua1020@126.com
#中国传媒大学/数字电视技术
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.out \
-I /usr/local/SDL2/include -L /usr/local/SDL2/lib -lSDLmain -lSDL \
-I /usr/include -L /usr/lib -lavformat -lavcodec -lavutil -lswscale

