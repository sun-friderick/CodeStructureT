
#ifndef __FF_QUEUE_H__
#define __FF_QUEUE_H__
 
#ifndef SDL_MUTEX
#include <pthread.h>
#else
#include <SDL/SDL_thread.h>
#endif
 
#include "libavformat/avformat.h"
 
typedef struct PacketQueue {
  AVPacketList *first_pkt, *last_pkt;
  int nb_packets;
  int size;
#ifndef SDL_MUTEX
  pthread_mutex_t mutex;
  pthread_cond_t  cond;
#else
  SDL_mutex *mutex;
  SDL_cond *cond;
#endif
  int quit_flag;/*退出标志*/
} PacketQueue;
 
/*队列初始化*/
void packet_queue_init(PacketQueue *q);
 
/*将一个packet放到队列*/
int packet_queue_put(PacketQueue *q, AVPacket *pkt);
 
/*从队列中获取一个packet, block表示是否要阻塞的获取*/
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block);
 
/*清空缓冲队列*/
void packet_queue_flush(PacketQueue *q);
 
 
#endif

