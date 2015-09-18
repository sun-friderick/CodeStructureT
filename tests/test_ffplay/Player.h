



class UltraPlayer : public RefCnt, public ResourceUser, public MessageHandler {
public:
    UltraPlayer(UltraPlayerClient *, BrowserPlayerReporter *, Program *);
    ~UltraPlayer();

    static int registerPlayer(int pIndex, UltraPlayer *player);
    static int unregisterPlayer(int pIndex, UltraPlayer *player);
    static UltraPlayer *lockPlayer(int pIndex);
    static void unlockPlayer(int pIndex, UltraPlayer *player);

    /* for resource manager */
    ResourceUser::Type type() { return mResourceUserType; }
    int getRequirement() { return mResourceRequirement; }
    
    /// 播放操作
    virtual int open() = 0;
    virtual int play(unsigned int startTime) = 0;
    virtual int seekTo(unsigned int playTime) = 0;
    virtual int fastForward(int) = 0;
    virtual int fastRewind(int) = 0;
    virtual int pause() = 0;
    virtual int resume() = 0;
    virtual int stop() = 0;
    virtual int close(int mode) = 0;
    virtual unsigned int getTotalTime() = 0;
    virtual unsigned int getCurrentTime() = 0;


    /// 声音控制 
    void SetVolume(int);
    int  GetVolume(void);
    void SetMute(int);
    int  GetMute(void);
    void SetAudioChannel(int);    //声道    LL LR RL RR
    int  GetAudioChannel(void);
    void SetAudioTrack(int);      //音轨    同一片源可以有多个音轨
    int  GetAudioTrack(void);
    int  GetTrackInfo(char *pTrackInfo);
    
    void SelectSubtitle(int subtitlePid);
    int  SubtitleIndex();
    void SetHDCPFlag(int HDCPFlag);
    

    int mPlaylistMode;        //0、只播放该片源，1、按列表播放模式，默认是0
    int mPlayCycleFlag;       //0、为播放一次，1、为循环播放，默认为1
    int mPlayRandomFlag;      //0、为顺序播放，1、为随机播放.默认为0
    int mDisplayMode;      //显示模式：0、为小窗口播放，1、为全屏播放，2、自定义大小；
    
    
    int m_subtitleFlag;
    int m_muteFlag;
    
    ///播放器位置
    int m_VideoDisplaytop;
    int m_VideoDisplayleft;
    int m_VideoDisplaywidth;
    int m_VideoDisplayheight;
    
    
    int m_PlayerInstanceType;    /*0,Main; 1, PIP; 2, MOSAIC */

protected:
    uint32_t mMagicNumber;
    APP_TYPE mMediaType;

    /* for resource manager */
    ResourceUser::Type mResourceUserType;
    int mResourceRequirement;



public:
    int mCurrentStatus;
    int mCurrentSpeed;
    static PlayStateWidget *mPlayState;
    static ChannelNOWidget *mChannelNO;
    static ProgressBarWidget *mProgressBar;
    static AudioMuteWidget *mAudioMute;
    static AudioTrackWidget *mAudioTrack;
    static AudioVolumeWidget *mAudioVolume;
    static DolbyWidget *mDolbyIcon;
    static DolbyDownmixWidget *mDolbyDownmixIcon;
    //static AudioBGGraphWidget *mAudioBGGraph;
   	int mInstanceId;
};



