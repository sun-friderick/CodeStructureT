

#ifndef __FF_PLAY_H__
#define __FF_PLAY_H__
 
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/error.h"
 
/*错误码*/
#define EFF_PLAY_BASE 1000
#define EFF_PLAY_FIND_CODEC    EFF_PLAY_BASE + 1
#define EFF_PLAY_OPEN_CODEC    EFF_PLAY_BASE + 2
 
/*注册函数*/
void ff_play_register_all(void);
 
/*free all resource*/
int ff_play_FreeAll(void);
 
/*save frame to ppm   path 是形如 /mnt/sdcard/这种形式  */
void ff_play_SaveFrame(AVFrame *pFrame, char*  path, int width, int height, int iFrame);
 
/*save sound to pcm , path 是形如 /mnt/sdcard/这种形式 */
void ff_play_SaveAudio(AVFrame *pFrame, char*  path, int bufsize);
 
/*打开一个文件或者流 并初始化上下文*/
int ff_play_open_and_initctx(const char *pathStr);
 
/*获取音频包放到 arg 所指定的buf中播放*/
int ff_play_getaudiopkt2play(void *audio_buf, int max_len);
 
/*获取一个视频包显示*/
void ff_play_getvideopkt2display(void *arg);
 
/*从stream里面取包挂到queue上面*/
void* ff_play_readpkt_thread(void *arg);
 
/*创建取包线程*/
int ff_play_begin_read_thread(void);
 
/*把pixels 指向的缓冲区作为 FrameVideoRGB的 填充区*/
int  ff_play_picture_fill(void* pixels,int width,int height,enum PixelFormat pix_fmt);
 
/*获取宽、高、采样率*/
int ff_play_getvideo_height(void);
int ff_play_getvideo_width(void);
int ff_play_getaudio_samplerate(void);
int ff_play_getaudio_channels(void);
 
/*快进或 快退跳转*/
void ff_play_jump(int second);
 
#endif