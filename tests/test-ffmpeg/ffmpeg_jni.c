

#include <string.h>
#include <jni.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "ff_play.h"
 
#ifdef ANDROID_LOG
#include <android/log.h>
#define LOGI(format,...)  //__android_log_print(ANDROID_LOG_INFO ,"hello_hl","file[%s] line[%d] "format"",__FILE__, __LINE__ ,##__VA_ARGS__)
#define LOGE(format,...)  __android_log_print(ANDROID_LOG_ERROR,"hello_hl","file[%s] line[%d] "format"",__FILE__, __LINE__ ,##__VA_ARGS__)
#else
#define LOGI(format,...)  // printf("file[%s] line[%d] "format"\n",__FILE__, __LINE__ ,##__VA_ARGS__)
#define LOGE(format,...)  printf("file[%s] line[%d] "format"\n",__FILE__, __LINE__ ,##__VA_ARGS__)
#endif
 
#define PRINT_DURATION  print_duration(__FUNCTION__,__LINE__)
 
unsigned int clk_before=0;
int framecount=0;
 
 
void print_duration(const char *func, int line){
    LOGE("func[%s] line[%d]  time duration is [%d]",func, line, (clock()- clk_before)/1000 );
    clk_before = clock();
}
 
/*jstring 转换成string
 * 注意C 和 C++ 的env 不同， C里面要 (*env)->xxx C++里面要env->xxx
 * 这个是通用的 C调用java的方法 就是利用了GetMethodID */
char* jstringTostring(JNIEnv* env, jstring jstr)
{
       char* rtn = NULL;
       jclass clsstring = (*env)->FindClass(env,"java/lang/String");
       jstring strencode = (*env)->NewStringUTF(env,"utf-8");
       jmethodID mid = (*env)->GetMethodID(env,clsstring, "getBytes", "(Ljava/lang/String;)[B");
       jbyteArray barr= (jbyteArray)(*env)->CallObjectMethod(env,jstr, mid, strencode);
       jsize alen = (*env)->GetArrayLength(env,barr);
       jbyte* ba = (*env)->GetByteArrayElements(env,barr, JNI_FALSE);
       if (alen > 0)
       {
            rtn = (char*)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
       }
       (*env)->ReleaseByteArrayElements(env,barr, ba, 0);
       return rtn;
}
 
/*初始化 open file , get stream, open codec*/
jint Java_com_example_player_PlayActivity_InitStream(JNIEnv* env,
        jobject this, jstring path) {
    int res = 1, i;
    int numBytes;
    char *pathStr;
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    /*this func will apply space for pFormatCtx,just like "new"*/
    if( (pathStr = jstringTostring(env,path)) == NULL )
        return  -1999;
    /*open and init ctx */
    res = ff_play_open_and_initctx(pathStr);
    if(res < 0){
        LOGE("_open_and_initctx res[%d]",res);
        return res;
    }
    /*分配帧*/
    ff_play_allocFrame();
    /*启动读线程*/
    res = ff_play_begin_read_thread();
    LOGE("begin thread res[%d]",res);
    return res;
}
 
jint Java_com_example_player_PlayActivity_getFrameHeight(JNIEnv * env, jobject this){
    return ff_play_getvideo_height();
}
 
jint Java_com_example_player_PlayActivity_getFrameWidth(JNIEnv * env, jobject this){
    return ff_play_getvideo_width();
}
 
/*free all resource*/
jint Java_com_example_player_PlayActivity_frameFree(JNIEnv * env, jobject this){
    return ff_play_FreeAll();
}
 
/*TODO 根据流的内容绘图*/
void Java_com_example_player_PlayActivity_drawFrame(JNIEnv * env, jobject this,
        jstring bitmap) {
    AndroidBitmapInfo info;
    void* pixels;
    int ret;
    uint8_t *pktdata;
    int pktsize;
    int len_get = AVCODEC_MAX_AUDIO_FRAME_SIZE*100;
    int data_size=0;
    int handle_finished,frameFinished = 0;
    AVPacket packet;
    static struct SwsContext *img_convert_ctx;
    int64_t seek_target;
    int target_width = 0;
    int target_height = 0;
 
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    LOGE("Checked on the bitmap");
 
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }
    LOGE("Grabbed the pixels addr[0x%x]", pixels);
 
    /*point frame->data to pixels; if already, do nothing*/
    ret = ff_play_picture_fill(pixels,info.width, info.height,PIX_FMT_RGBA);
    if(ret <0){
        AndroidBitmap_unlockPixels(env, bitmap);
        return;
    }
    PRINT_DURATION;
    handle_finished = 0;
    ff_play_getvideopkt2display(NULL);
    AndroidBitmap_unlockPixels(env, bitmap);
}
 
jint Java_com_example_player_FFAudio_getAudioSamplerate(void) {
     return ff_play_getaudio_samplerate();
}
 
jint Java_com_example_player_FFAudio_getAudioBuf(JNIEnv * env, jobject this,jbyteArray jAudioBuf) {
    int ret;
    int len_get = 0;
    int data_size=0;
    uint8_t *audio_output, *pBuf;
    void *pDebug=NULL;
    int handle_finished=0,frameFinished = 0;
    AVPacket packet;
 
    audio_output = (*env)->GetByteArrayElements(env,jAudioBuf,JNI_FALSE);
//  PRINT_DURATION;
    data_size = ff_play_getaudiopkt2play((void *)audio_output,0);
 
    (*env)->ReleaseByteArrayElements(env,jAudioBuf,audio_output,0);
    PRINT_DURATION;
    return data_size;
}