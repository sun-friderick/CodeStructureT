

#include "ff_queue.h"
#include <stdio.h>
#include <SDL/SDL.h>
#ifndef SDL_MUTEX
#include <pthread.h>
#else
#include <SDL/SDL_thread.h>
#endif
 
/*队列初始化*/
void ff_queue_init(PacketQueue *q) {
    memset(q, 0, sizeof(PacketQueue));
}
 
/*设置退出标志*/
void ff_queue_set_quit(PacketQueue *q){
    q->quit_flag =1;
}
 
/*将一个packet放到队列*/
int ff_queue_packet_put(PacketQueue *q, AVPacket *pkt) {
    AVPacketList *pkt1;
    if (av_dup_packet(pkt) < 0) {
        return -1;
    }
    pkt1 = av_malloc(sizeof(AVPacketList));
    if (!pkt1)
        return -1;
    pkt1->pkt = *pkt;
    pkt1->next = NULL;
#ifndef SDL_MUTEX
    pthread_mutex_lock(&q->mutex);
#else
    SDL_LockMutex(q->mutex);
#endif
    if (!q->last_pkt)
        q->first_pkt = pkt1;
    else
        q->last_pkt->next = pkt1;
    q->last_pkt = pkt1;
    q->nb_packets++;
    q->size += pkt1->pkt.size;
#ifndef SDL_MUTEX
    pthread_mutex_unlock(&q->mutex);
#else
    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
#endif
    return 0;
}
 
/*从队列中获取一个packet*/
int ff_queue_packet_get(PacketQueue *q, AVPacket *pkt, int block) {
    AVPacketList *pkt1;
    int ret;
 
#ifndef SDL_MUTEX
    pthread_mutex_lock(&q->mutex);
#else
    SDL_LockMutex(q->mutex);
#endif
    for (;;) {
        if (q->quit_flag) {
            ret = -1;
            break;
        }
        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
                q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        }else if (!block) {
            ret = 0;
            break;
        }else {
            /*百度百科：在调用pthread_cond_wait()前必须由本线程加锁（pthread_mutex_lock()），
             * 而在更新条件等待队列以前，mutex保持锁定状态，并在线程挂起进入等待前解锁。
             * 在条件满足从而离开pthread_cond_wait()之前，mutex将被重新加锁，
             * 以与进入pthread_cond_wait()前的加锁动作对应*/
#ifndef SDL_MUTEX
            pthread_cond_wait(&q->cond, &q->mutex);
#else
            SDL_CondWait(q->cond, q->mutex);
#endif
        }
    }
#ifndef SDL_MUTEX
    pthread_mutex_unlock(&q->mutex);
#else
    SDL_UnlockMutex(q->mutex);
#endif
    return ret;
}
 
 
void packet_queue_flush(PacketQueue *q) {
  AVPacketList *pkt, *pkt1;
 
#ifndef SDL_MUTEX
    pthread_mutex_lock(&q->mutex);
#else
    SDL_LockMutex(q->mutex);
#endif
 
  for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1) {
    pkt1 = pkt->next;
    av_free_packet(&pkt->pkt);
    av_freep(&pkt);
  }
  q->last_pkt = NULL;
  q->first_pkt = NULL;
  q->nb_packets = 0;
  q->size = 0;
#ifndef SDL_MUTEX
    pthread_mutex_unlock(&q->mutex);
#else
    SDL_UnlockMutex(q->mutex);
#endif
}
