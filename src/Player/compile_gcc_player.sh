#! /bin/sh

#��򵥵Ļ���FFmpeg����Ƶ������----�����б���
#Simplest FFmpeg Player----Compile in Shell 
#
#������ Lei Xiaohua
#leixiaohua1020@126.com
#�й���ý��ѧ/���ֵ��Ӽ���
#Communication University of China / Digital TV Technology
#http://blog.csdn.net/leixiaohua1020
#
#compile
gcc simplest_ffmpeg_player.cpp -g -o simplest_ffmpeg_player.out \
-I /usr/local/SDL2/include -L /usr/local/SDL2/lib -lSDLmain -lSDL \
-I /usr/include -L /usr/lib -lavformat -lavcodec -lavutil -lswscale

