#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include "ff_queue.h"
#include "ff_play.h"
 
#ifdef ANDROID_LOG
#include <android/log.h>
#define LOGI(format,...)  //__android_log_print(ANDROID_LOG_INFO ,"hello_hl","file[%s] line[%d] "format"",__FILE__, __LINE__ ,##__VA_ARGS__)
#define LOGE(format,...)  __android_log_print(ANDROID_LOG_ERROR,"hello_hl","file[%s] line[%d] "format"",__FILE__, __LINE__ ,##__VA_ARGS__)
#else
#define LOGI(format,...)  // printf("file[%s] line[%d] "format"\n",__FILE__, __LINE__ ,##__VA_ARGS__)
#define LOGE(format,...)  printf("file[%s] line[%d] "format"\n",__FILE__, __LINE__ ,##__VA_ARGS__)
#endif
 
/*上下文*/
AVFormatContext *FormatCtx = NULL;
AVCodecContext *VideoCodecCtx=NULL;
AVCodecContext *AudioCodecCtx=NULL;
AVCodec *Codec=NULL;
/*帧*/
AVFrame *FrameVideo=NULL;
AVFrame *FrameVideoRGB=NULL;
AVFrame *FrameAudio=NULL;
/*视频像素格式*/
enum PixelFormat FrameRGB_Pix_Fmt;
/*视频流*/
int StreamVideo = -1;
int StreamAudio = -1;
/*音频测试缓冲区和文件*/
FILE* File_Audio_Record=NULL;
/*音视频队列*/
PacketQueue Queue_Audio ={0};
PacketQueue Queue_Video ={0};
/*线程定义*/
pthread_t Task_Read =0;
int Isrun_Read = 0; /*读线程状态*/
/*时钟定义*/
double Clock_Audio;
double Clock_Video;
unsigned char  Delay_Muti=0;  /*视频延时倍率*/
#define DELAY_BASE 50   /*视频延时基准,单位豪秒*/
#define INTERVAL_MAX 0.5    /*音视频 最大时钟差，单位秒*/
#define INTERVAL_ALERT 0.1 /*视频过快超过此时间，则开始增加延迟倍率*/
int Seek_Time=0;   /*播放的前进后退时间，单位秒*/
 
/*缓冲清刷标志位*/
int Flush_Video=0;
int Flush_Audio=0;
 
//#define  ONLY_I_FRAME         /*是否只取I帧*/
 
 
/*save frame to ppm   path 是形如 /mnt/sdcard/这种形式  */
void ff_play_SaveFrame(AVFrame *FrameVideo, char*  path, int width, int height, int iFrame) {
    FILE *pFile;
    char szFilename[100];
    int  y;
    // Open file
    sprintf(szFilename, "%sframe%05d.ppm", path, iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;
    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(FrameVideo->data[0]+y*FrameVideo->linesize[0], 1, width*3, pFile);
    // Close file
    fclose(pFile);
}
 
/*save sound to pcm , path 是形如 /mnt/sdcard/这种形式 */
void ff_play_SaveAudio(AVFrame *FrameVideo, char*  path, int bufsize) {
    FILE *pFile;
    char szFilename[100];
    int  y;
    // Open file
    if(File_Audio_Record == NULL){
        sprintf(szFilename, "%saudio.pcm", path);
        File_Audio_Record=fopen(szFilename, "wb");
        if(File_Audio_Record==NULL)
            return;
    }
    // Write data
    fwrite(FrameVideo->data[0],1,bufsize,File_Audio_Record);//pcm记录
    fflush(File_Audio_Record);
}
 
void ff_play_init_queue(void){
    ff_queue_init(&Queue_Audio);
    ff_queue_init(&Queue_Video);
}
 
void ff_play_allocFrame(){
    FrameVideo = avcodec_alloc_frame();
    FrameVideoRGB = avcodec_alloc_frame();
    FrameAudio = avcodec_alloc_frame();
}
 
/*free all resource*/
int ff_play_FreeAll(void){
    Isrun_Read = 0;
    /*等待读线程结束*/
    while(Isrun_Read >= 0);
    LOGE("free all 111111  !!!");
    if(FrameVideo != NULL)
        av_free(FrameVideo);
    if(FrameVideoRGB != NULL)
        av_free(FrameVideoRGB);
    if(FrameAudio != NULL)
            av_free(FrameAudio);
    if(VideoCodecCtx != NULL)
        avcodec_close(VideoCodecCtx);
    if(AudioCodecCtx != NULL)
            avcodec_close(AudioCodecCtx);
    if(FormatCtx!=NULL)
        avformat_close_input(&FormatCtx);
    if(File_Audio_Record != NULL)
        fclose(File_Audio_Record);
    /*init global vira*/
    FrameVideo=FormatCtx=FrameVideoRGB=VideoCodecCtx=AudioCodecCtx=File_Audio_Record=NULL;
    Flush_Video = Flush_Audio = Seek_Time = Clock_Video = Clock_Audio = Delay_Muti =0;
    StreamVideo= StreamAudio = -1;
    LOGE("free all 22222  !!!");
    return 0;
}
 
/*注册函数*/
void ff_play_register_all(){
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
}
 
 
/*打开一个文件或者流 并初始化上下文*/
int ff_play_open_and_initctx(const char *pathStr){
    int res = 1, i;
    int numBytes;
    /*open stream or file*/
    res = avformat_open_input(&FormatCtx, pathStr, NULL, NULL);
    if (res < 0) {
        return res;
    }
    /*find stream*/
    res = avformat_find_stream_info(FormatCtx, NULL);
    if (res < 0) {
        return res;
    }
    /* find the first video and audio stream */
    for (i=0; i < FormatCtx->nb_streams; i++) {
        if (FormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            StreamVideo = i;
        }
        if (FormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            StreamAudio = i;
        }
        if(StreamVideo >=0 && StreamAudio >=0)
            break;
    }
    /*find the decoder for the video stream*/
    if(StreamVideo >=0 ){
        VideoCodecCtx = FormatCtx->streams[StreamVideo]->codec;
        Codec = avcodec_find_decoder(VideoCodecCtx->codec_id);
        if (Codec == NULL) {
            return AVERROR(EFF_PLAY_FIND_CODEC);
        } /*supprt TRUNCATE? */
        if (Codec->capabilities & CODEC_CAP_TRUNCATED) {
            VideoCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
        }
        if(avcodec_open2(VideoCodecCtx, Codec, NULL) <0){
            return AVERROR(EFF_PLAY_OPEN_CODEC);
        }
        LOGE("have video stream[%d]",StreamVideo);
    }
    /*find the decoder for the audio stream*/
    if(StreamAudio >=0 ){
        AudioCodecCtx = FormatCtx->streams[StreamAudio]->codec;
        Codec = avcodec_find_decoder(AudioCodecCtx->codec_id);
        if (Codec == NULL) {
            return AVERROR(EFF_PLAY_FIND_CODEC);
        } /*supprt TRUNCATE? */
        if (Codec->capabilities & CODEC_CAP_TRUNCATED) {
            AudioCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
        }
        if(avcodec_open2(AudioCodecCtx, Codec, NULL) <0){
            return AVERROR(EFF_PLAY_OPEN_CODEC);
        }
        LOGE("have audio stream[%d]",StreamAudio);
    }
    ff_play_init_queue();
    ff_play_allocFrame();
    return 0;
}
 
int  ff_play_picture_fill(void* pixels,int width,int height,enum PixelFormat pix_fmt){
    int res;
    /*point frame->data to pixels; if already, do nothing*/
    if(FrameVideoRGB->data[0] != pixels){
        res = avpicture_fill((AVPicture *) FrameVideoRGB, pixels, pix_fmt,
                width, height) <0;
        FrameRGB_Pix_Fmt = pix_fmt;
        return res;
    }
    return 0;
}
 
int ff_play_getvideo_height(void){
    return VideoCodecCtx != NULL? VideoCodecCtx->height:0;
}
int ff_play_getvideo_width(void){
    return VideoCodecCtx != NULL? VideoCodecCtx->width:0 ;
}
int ff_play_getaudio_samplerate(void){
    return AudioCodecCtx != NULL ? AudioCodecCtx->sample_rate:0;
}
 
int ff_play_getaudio_channels(void){
    return AudioCodecCtx != NULL ? AudioCodecCtx->channels:0;
}
 
/*从音频队列上取一个包 写入audiobuf, 这里的audiobuf 可以是音频设备的缓冲也可是单纯的buf*/
int ff_play_getaudiopkt2play(void *audio_buf, int max_len){
    int ret;
    int len_get = 0,len_left=0;
    int data_size=0;
    uint8_t *pbuf;
    int pkt_size;
    int handle_finished=0,frameFinished = 0;
    AVPacket packet;
    uint8_t *FR_pkt_data_org; /*用于保存packet.data指向的原地址*/
 
    av_init_packet(&packet);
    pbuf = (uint8_t*)audio_buf;
 
    do{/*清刷缓冲区*/
        if(Flush_Audio == 1){
            avcodec_flush_buffers(AudioCodecCtx);
            Flush_Audio = 0;
            LOGI("FLUSH OK!!!");
        }
        LOGI("11111!!!");
 
        ret = ff_queue_packet_get(&Queue_Audio,&packet,0);
        if(ret<=0){
            break;
        }
        LOGI("22222 stream_idx[%d], stream_audio[%d]!!!",packet.stream_index ,StreamAudio );
        if (packet.stream_index != StreamAudio){
            av_free_packet(&packet);
            continue;
        }
        LOGI("33333!!!");
 
        FR_pkt_data_org = packet.data; /*保留packet指向的缓冲区初始地址,这个地址如果不对，将来释放的时候会出错*/
        len_left = (max_len > 0 ? max_len : 1000000 );
        while(packet.size>0 && len_left>0) { /*音频解码的时候可能一次不能把pkt全解完，要多次*/
            /*解码, 这里的len get是packet的偏移 而不是解码出来的长度， 切不要弄错！！*/
            len_get = avcodec_decode_audio4(AudioCodecCtx,FrameAudio,&frameFinished,&packet);
            if (len_get<0){
                break;
            }
            if(frameFinished){  /*获取了一帧 */
                /*data_size 才是解码的长度*/
                data_size = av_samples_get_buffer_size(FrameAudio->linesize, AudioCodecCtx->channels,
                        FrameAudio->nb_samples,AudioCodecCtx->sample_fmt, 0);
                data_size = ((data_size > len_left && len_left >0 )?len_left:data_size);
                memmove((void *)pbuf,(void *)FrameAudio->data[0],data_size);
            }/*注意，指向缓冲区的指针后移了，释放的时候要把它重置*/
            packet.data += len_get;
            packet.size -= len_get;
            pbuf += data_size;  /*pbuf 向后移动，向输出缓冲填入新的声音*/
            len_left -= data_size; /*缓冲区剩余空间减少*/
            LOGI("55555 decode len_get[%d],len_left[%d] data_size[%d] frameFinished[%d]!!!", \
                    len_get, len_left,data_size,frameFinished);
        }
        /*获取音频时间基准保存到 Clock_Audio*/
        if(packet.pts != AV_NOPTS_VALUE){
            Clock_Audio = av_q2d(FormatCtx->streams[StreamAudio]->time_base)*packet.pts;
        }
        handle_finished = 1;
 
        packet.data = FR_pkt_data_org; /*重置*/
        av_free_packet(&packet);
 
    }while(handle_finished <1);
    ret = pbuf - (uint8_t*)audio_buf;
//  LOGE("44444 ret[%d]!!!", ret);
    return  ret;
}
 
 
/*从Queue_Video里面取一个包显示*/
void ff_play_getvideopkt2display(void *arg){
    AVPacket packet;
    struct SwsContext *img_convert_ctx;
    int handle_finished,frameFinished = 0,ret;
    static int FS_discard_frame=0;  /*丢帧标志*/
 
    do{/*清刷缓冲区*/
        if(Flush_Video == 1){
            avcodec_flush_buffers(VideoCodecCtx);
            Flush_Video = 0;
        }
        ret = ff_queue_packet_get(&Queue_Video,&packet,0);
        if(ret<=0){
            break;
        }
        if(packet.stream_index != StreamVideo){
            av_free_packet(&packet);
            continue;
        }/*获取视频时间基准保存到 Clock_Video*/
        if(packet.pts != AV_NOPTS_VALUE){
            Clock_Video = av_q2d(FormatCtx->streams[StreamVideo]->time_base)*packet.pts;
        }
        LOGI("Discard flag[%d], PKT_FLAG[%d], clk_diff[%f] clk_video[%f]，head[0x %x %x %x %x %x %x %x]", \
            FS_discard_frame, packet.flags & AV_PKT_FLAG_KEY, Clock_Video-Clock_Audio, Clock_Video, \
            packet.data[0],packet.data[1],packet.data[2],packet.data[3],packet.data[4],packet.data[5],packet.data[6]);
        /*视频太超前，增加延时, 差距越大，增加延迟越多*/
        if( (StreamAudio >=0 ) && Clock_Video - Clock_Audio > INTERVAL_ALERT){
            if(Clock_Video - Clock_Audio > INTERVAL_ALERT*(Delay_Muti+1) ){
                Delay_Muti++;
            }
            usleep( Delay_Muti * DELAY_BASE * 1000);
            LOGI("DELAY_MUTI[%d] diff[%f]", Delay_Muti, Clock_Video-Clock_Audio);
        }
        /*视频时间太落后，进入丢帧状态, 同时延时标志置为0*/
        if( ( StreamAudio >=0 ) &&  (Clock_Video < Clock_Audio - INTERVAL_MAX) && (packet.flags & AV_PKT_FLAG_KEY)){
            Delay_Muti = 0;
            FS_discard_frame =1;
        }
 
        /*丢帧状态中，判断是否要继续丢帧， 超过音频则不再丢*/
        if(FS_discard_frame){
            if( (StreamAudio >=0 ) &&  (Clock_Video >= Clock_Audio ) && (packet.flags & AV_PKT_FLAG_KEY)){
                FS_discard_frame =0;
            } /*丢掉关键帧之外的帧*/
            else if(!(packet.flags & AV_PKT_FLAG_KEY) ){
                    av_free_packet(&packet);
                    continue;
            }
        }
        avcodec_decode_video2(VideoCodecCtx, FrameVideo, &frameFinished, &packet);
        LOGI("PKT_FLAG[%d] , pic type is[%d]",packet.flags,FrameVideo->pict_type);
        if(frameFinished <=0){
            av_free_packet(&packet);
            continue;
        }
        /*测试只取关键帧*/
        /*  if(FrameVideo->key_frame != 1) */
        img_convert_ctx = sws_getContext(VideoCodecCtx->width,
                VideoCodecCtx->height, VideoCodecCtx->pix_fmt, VideoCodecCtx->width,
                VideoCodecCtx->height, FrameRGB_Pix_Fmt, SWS_BICUBIC, NULL, NULL,
                NULL);
        if (img_convert_ctx == NULL) { /*获取不到对应上下文直接返回*/
            av_free_packet(&packet);
            return;
        }
        sws_scale(img_convert_ctx,
                (const uint8_t* const *) FrameVideo->data, FrameVideo->linesize,
                0, VideoCodecCtx->height, FrameVideoRGB->data,
                FrameVideoRGB->linesize);
        handle_finished = 1;
        av_free_packet(&packet);
    }while(handle_finished <1);
}
 
void ff_play_seek_to(int seek_seconds){
    double  clock;
    int64_t seek_pos;
    int stream_index = -1;
    int seek_flags;
 
    /*获取时钟*/
    if (StreamVideo >= 0){
        stream_index = StreamVideo;
        clock = Clock_Video;
    }else if(StreamAudio >= 0){
        stream_index = StreamAudio;
        clock = Clock_Audio;
    }
    seek_pos = (clock + seek_seconds) * AV_TIME_BASE;
 
    if(stream_index<0)
        return;
 
    /*这里av_rescale_q(a,b,c)是用来把时间戳从一个时基调整到另外一个时基时候用的函数。
     * 它基本的动作是计算a*b/c ——+ 这是从网上抄的 具体作用不明*/
    seek_pos= av_rescale_q(seek_pos, AV_TIME_BASE_Q,
        FormatCtx->streams[stream_index]->time_base);
 
    seek_flags = seek_seconds < 0 ? AVSEEK_FLAG_BACKWARD : 0;
    LOGI("stream_index[%d] ,seek_pos[%lld] ,seek_flags[%d]",
            stream_index, seek_pos, seek_flags);
    if(av_seek_frame(FormatCtx, stream_index,seek_pos, seek_flags) < 0){
        LOGI("error when seeking!!!!");
    }
}
 
 
/*从stream里面取包挂到queue上面*/
void* ff_play_readpkt_thread(void *arg) {
    AVPacket packet;
    int res=-1;
    double pkt_pts;
    int stream_index= -1;
    int64_t seek_target;
 
    while ( Isrun_Read >0 ) {
        /*视频前进后退 ,移动Format上下文之后清空队列和缓存*/
        if(Seek_Time != 0){
            ff_play_seek_to(Seek_Time);
            packet_queue_flush(&Queue_Video);
            packet_queue_flush(&Queue_Audio);
            /*设置清刷标志*/
            Flush_Video = 1;
            Flush_Audio = 1;
            LOGI("hahahaha  seektime [%d]!!!!", Seek_Time);
            Seek_Time = 0;
        }
        res = av_read_frame(FormatCtx, &packet);
        //LOGE("read frame res[%d] stream_idx[%d]",res ,packet.stream_index);
        if(res < 0) continue;
        // Is this a packet from the video stream?
        if (packet.stream_index == StreamVideo) {
            ff_queue_packet_put(&Queue_Video, &packet);
        } else if (packet.stream_index == StreamAudio) {
            ff_queue_packet_put(&Queue_Audio, &packet);
        } else {
            av_free_packet(&packet);
        }
    }
    packet_queue_flush(&Queue_Video);
    packet_queue_flush(&Queue_Audio);
    Isrun_Read = -1;
    return NULL;
}
 
/*启动读包线程*/
int ff_play_begin_read_thread(void){
    int res;
    res = pthread_create(&Task_Read, NULL, ff_play_readpkt_thread, NULL);
    Isrun_Read = 1;
    return res;
}
 
/*快进或 快退跳转*/
void ff_play_jump(int second){
    Seek_Time =  second;
}