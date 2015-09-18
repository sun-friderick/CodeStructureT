/**
 *  Support command:
        214 The following commands are recognized (* =>'s unimplemented).
        214     USER    PASS    ACCT*   CWD     XCWD    CDUP    XCUP    SMNT*   
        214     QUIT    REIN*   PORT    PASV    TYPE    STRU*   MODE*   RETR    
        214     STOR    STOU*   APPE    ALLO*   REST    RNFR    RNTO    ABOR   
        214     DELE    MDTM    RMD     XRMD    MKD     XMKD    PWD     XPWD    
        214     SIZE    LIST    NLST    SITE    SYST    STAT    HELP    NOOP    
        214 Direct comments to nolove@263.net
 **/

#include "ftpserver.h"
#include "ftpserver_function.h"


//命令 列表
struct _CmdList commandList[] = {
    {"USER",    User,   FLAG_CHECK_NOLOGIN | FLAG_NEED_PARAM  },
    {"PASS",    Pass,   FLAG_CHECK_NOLOGIN   },
    {"ACCT"},
    {"CWD",     Cwd,    FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"XCWD",    Cwd,    FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"CDUP",    Cdup,   FLAG_CHECK_LOGIN | FLAG_NO_PARAM      },
    {"XCUP",    Cdup,   FLAG_CHECK_LOGIN | FLAG_NO_PARAM      },
    {"SMNT"},
    {"QUIT",    Quit,   FLAG_NO_CHECK    },
    {"REIN"},
    {"PORT",    Port,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"PASV",    Pasv,   FLAG_CHECK_LOGIN | FLAG_NO_PARAM      },
    {"TYPE",    Type,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"STRU",    Stru,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    }, //add own
    {"MODE",    Mode,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    }, //add own
    {"RETR",    Retr,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"STOR",    Stor,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"STOU"},
    {"APPE",    Appe,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"ALLO"},
    {"REST",    Rest,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"RNFR",    Rnfr,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"RNTO",    Rnto,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"ABOR",    Abor,   FLAG_CHECK_LOGIN | FLAG_NO_PARAM      },
    {"DELE",    Dele,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"MDTM",    Mdtm,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"RMD",     Rmd,    FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"XRMD",    Rmd,    FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"MKD",     Mkd,    FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"XMKD",    Mkd,    FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"PWD",     Pwd,    FLAG_CHECK_LOGIN | FLAG_NO_PARAM      },
    {"XPWD",    Pwd,    FLAG_CHECK_LOGIN | FLAG_NO_PARAM      },
    {"SIZE",    Size,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"LIST",    List,   FLAG_CHECK_LOGIN    },
    {"NLST",    List,   FLAG_CHECK_LOGIN    },
    {"SITE",    Site,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"SYST",    Syst,   FLAG_CHECK_LOGIN },
    {"STAT",    Stat,   FLAG_CHECK_LOGIN | FLAG_NEED_PARAM    },
    {"HELP",    Help,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    
    /////own
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {"NOOP",    Noop,   FLAG_NO_CHECK    },
    {""}
};


/**
 *  FTP client端发送的命令格式：
        USER <SP> <username> <CRLF>
        PASS <SP> <password> <CRLF>
        ACCT <SP> <account-information> <CRLF>
        CWD <SP> <pathname> <CRLF>
        CDUP <CRLF>
        SMNT <SP> <pathname> <CRLF>
        QUIT <CRLF>
        REIN <CRLF>
 *  
 **/


/**
 *  加解密命令：
 *      类似：
 *          USER <SP> <username> <CRLF> 
 *          PASS <SP> <password> <CRLF>
 **/
int EncryptCommand(const char* tmpBuf, const int encryptMode, char* sendBuf)
{
    if(encryptMode <= EncryptMode_Reserved || encryptMode >= EncryptMode_Undefined){
        printf("encryptMode[%d] error.\n", encryptMode);
        return -1;
    }
    
    switch(encryptMode){
        case EncryptMode_BASE64:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_AES:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_DES:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_MD5:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_RSA:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_SHA128:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_SHA256:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private01:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private02:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private03:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private04:
            printf("encryptMode[%d].\n", encryptMode);
            break;   
        case EncryptMode_Unencrypt:
        default:
            printf("default encryptMode[%d].\n", encryptMode);
            strcpy(tmpBuf, sendBuf);
            break;
    }
    
    return 0;
}

int DecryptCommand(const char* recvBuf, const int decryptMode, char* tmpRecvBuf)
{
    if(decryptMode <= EncryptMode_Reserved || decryptMode >= EncryptMode_Undefined){
        printf("decryptMode[%d] error.\n", decryptMode);
        return -1;
    }
    
    switch(decryptMode){
        case EncryptMode_BASE64:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_AES:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_DES:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_MD5:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_RSA:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_SHA128:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_SHA256:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private01:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private02:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private03:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Private04:
            printf("encryptMode[%d].\n", encryptMode);
            break;
        case EncryptMode_Unencrypt:
        default:
            printf("default encryptMode[%d].\n", encryptMode);
            strcpy(recvBuf, tmpRecvBuf);
            break;
    }
    
    return 0;
}
 

/**
 *  从命令行输入和配置文件中解析配置参数
 *  只支持简单的格式解析，如[-p:port]
 *      sample: ftp_server -p:port -m:max_connect_count -t:time_out
 **/
int ParseOptions(const char** buf, const int count, Option* options)
{
    int i, okFlag;
    
    if(buf == NULL || count == 0){
        printf("ParseOptions, param options buf is null error.\n");
        return -1;
    }
    
    /* set option default */
    sprintf(options->userName, "%s", DEFAULT_USERNAME);
    sprintf(options->userPasswd, "%s", DEFAULT_USERPASSWD);
    options->serverPort = DEFAULT_SERVERPORT;
    options->maxConnectCount = DEFAULT_MAX_CONNECT_COUNT;
    options->timeOut = DEFAULT_TIMEOUT;

    okFlag = 1;
    /* get the param */
    for (i = 1; i < count; i++) {
        if (buf[i][0] != '-' || buf[i][2] != ':')
            okFlag = 0;
        switch (buf[i][1]) {
        case 'p':
            options->serverPort = atoi(&buf[i][3]);
            if (options->serverPort == 0)
                okFlag = 0;
            break;
        case 'm':
            options->maxConnectCount = atoi(&buf[i][3]);
            if (options->maxConnectCount == 0)
                okFlag = 0;
            break;
        case 't':
            options->timeOut = atoi(&buf[i][3]);
            if (options->timeOut == 0)
                okFlag = 0;
            break;
        default:
            okFlag = 0;
        }
        if (!okFlag)
            break;
    }

    if (!okFlag) {
        printf("Usage: %s [-p:port] [-m:maxconn] [-t:timeout]ntport: default is 21.ntmaxconn: the max client num connected.n", argv[0]);
        return -1;
    }

    return 0;
}
 
 
 

static int g_EncryptMode = 0;
 
/**
 *  服务器应答输出
 *      应答格式：
 *      sample: "225 Data connection open; no transfer in progress."
 **/
int AckResponse(const int connectFd, const int AckCode, const char* AckPromote)
{
    char tmpSendBuf[BUFSIZE] = {};
    char sendBuf[BUFSIZE] = {};
    
    //拼装应答码
    sprintf(tmpSendBuf, "%d %s\r\n", AckCode, AckPromote);
    
    //加密命令
    EncryptCommand(sendBuf, EncryptMode_Unencrypt, tmpSendBuf);

    //命令发送
    if (send(connectFd, sendBuf, strlen(sendBuf), 0) == SOCKET_ERROR) {
        printf("AckResponse,send acknowledgement error.\n");
        return -1;
    }
    
    return 0;
}



/**
 *  错误处理函数，用于命令出错处理
 **/
void ErrorHandler(int connectFd)
{
    printf("ErrorHandler:errno=[%d].\n", errno);
    switch (errno) {
    case ENOTEMPTY:
        AckResponse(connectFd, ACK_550, "Requested action not taken. Directory not empty");
        break;
    case ENOSPC:
        AckResponse(connectFd, ACK_550, "Requested action not taken. Disk full");
        break;
    case EEXIST:
        AckResponse(connectFd, ACK_550, "Requested action not taken. File exists");
        break;
    case ENAMETOOLONG:
        AckResponse(connectFd, ACK_550, "Requested action not taken. Path is too long");
        break;
    case ENOENT:
        AckResponse(connectFd, ACK_550, "Requested action not taken. No such file or directory");
        break;
    case ENOTDIR:
        AckResponse(connectFd, ACK_550, "Requested action not taken. Not a directory");
        break;
    case EISDIR:
        AckResponse(connectFd, ACK_550, "Requested action not taken. Is a directory");
        break;
    default:
        AckResponse(connectFd, ACK_550, "Requested action not taken. Permission denied");
    }

    return ;
} 



/**
 *  命令校验
 **/
int CommandVerification(const char* cmd, const char* param, const int check, const int connectFd)
{
    if (check & FLAG_FLAG_NO_CHECK) {
        printf("Verification, FLAG_FLAG_NO_CHECK.\n");
        return 1;
    }
    
    if (check & FLAG_CHECK_LOGIN && !userValid) {
        AckResponse(connectFd, ACK_530, "Please login with USER and PASS");
        return 0;
    }

    if (check & FLAG_CHECK_NOLOGIN && userValid) {
        AckResponse(connectFd, ACK_503, "You are already logged in!");
        return 0;
    }

    if (check & FLAG_NEED_PARAM) {
        if (!param)
            AckResponse(connectFd, ACK_501, "Invalid number of arguments, check more arguments.");
        return (param != NULL);
    }

    if (check & FLAG_NO_PARAM) {
        if (param)
            AckResponse(connectFd, ACK_501, "Invalid number of arguments.");
        return (param == NULL);
    }
    
    return 1;
}



/* I/O 信号处理函数 */
void SignalChldHandler()
{
    printf("SIGCHLD handler.\n");
    return ;
}

void SignalTermHandler()
{
    printf("SIGTERM handler.\n");
    
    return ;
}

int SetProgramDaemon(int* sysUid)  /* set the program a daemon */
{
    int fd;
    
    /* 注册I/O 信号处理 */
    /* register the signal handler */
    signal(SIGCHLD, SignalChldHandler);
    signal(SIGTERM, SignalTermHandler);
    
    /* 父进程退出,程序进入后台运行 */
    if(fork() != 0) 
        exit(1);
    
    /* 创建一个新的会议组 */ 
    if(setsid() < 0)
        exit(1);
    
    /* 关闭打开的文件描述符,包括标准输入、标准输出和标准错误输出 */ 
    /* getdtablesize():返回所在进程的文件描述附表的项数，即该进程打开的文件数目 */
    for (fd = 0, fdtablesize = getdtablesize(); fd < fdtablesize; fd++) 
        close(fd);
    
    open("/dev/null", O_RDONLY);
    dup2(stdin, stdout);  // 0: stdin;  1: stdout
    dup2(stdin, stderr);  // 2: stderr
    
    if ((fd = open("/dev/tty", O_RDWR)) > 0) {
        ioctl(fd, TIOCNOTTY, NULL) ;
        close(fd);
    }

    /* 子进程退出,孙进程没有控制终端了 */
    if (fork() != 0)
        exit(0);

    /* get the uid */
    *sysUid = getuid();
    
    return 1;
}



/**
 *  创建 ctrl socket链接，初始化并返回
 **/
void CreateListenSocket(const Option* options, int listenFd)
{
    int fd, i;
    struct sockaddr_in socketAddr;
    struct hostent *host;
    struct in_addr ia;

    /* init network */
    memset(&socketAddr, 0, sizeof(socketAddr));
    socketAddr.sin_family      = AF_INET;
    socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddr.sin_port        = htons(options->serverPort);

    if (gethostname(hostname, BUFSIZE) == -1)
        strcpy(hostname, "");

    i = 1;
    if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ||
        setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (const char *)&i, sizeof(int)) == -1) {
        perror("my FTP:");
        return -1;
    }

    if (bind(listenFd, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) == -1) {
        fprintf(stderr, "my FTP: Can not bind port %d.\n", options->serverPort);
        return -1;
    }
    if (listen(listenFd, options->maxConnectCount) == -1) {
        perror("my FTP:");
        return -1;
    }
    
    /* get the uid */
    //int sysUid = -1;
    SetProgramDaemon(&g_sysUid);
    
    return ;
} 
 


 
/**
 *  从接收到的消息中，解析命令和参数
 *      将buf以空格做分隔符分解成两个字符串;如： CWD <SP> <pathname> <CRLF>
 *      sample: CWD /home/hybroad/bin \r\n
 **/
int ParseCommand(const char* recvBuf, char** cmd, char** param)
{
    char* p = NULL;
    char tmpRecvBuf[BUFSIZE] = {};
    
    // 解密命令：
    DecryptCommand(tmpRecvBuf, EncryptMode_Unencrypt, recvBuf);
    
    // 解析命令参数
    if ((tmpRecvBuf[strlen(tmpRecvBuf) - 2] == '\r') && (tmpRecvBuf[strlen(tmpRecvBuf) - 1] == '\n')) {
        p = strchr(tmpRecvBuf, ' ');

        if (p != NULL) {
            strncpy(*cmd, tmpRecvBuf, p - tmpRecvBuf); //find cmd
            *cmd[strlen(cmd)] = '\0'; 
            
            strncpy(*param, p + 1, strlen(tmpRecvBuf) - (p - tmpRecvBuf) - 1); //find param
            *param[strlen(param)] = '\0';
        else {
            strcpy(*cmd, tmpRecvBuf); //find cmd
            *cmd[strlen(cmd)] = '\0';
            
            *param = NULL;
        }
    } else {
        printf("command format error.\n");
        return -1;
    }
    
    return 1;
}
 

 
/**
 *  从命令注册表中，查找函数运行并返回结果
 **/
void RunCommand(const char** cmd, const char** param, const int connectFd, char* retValue)
{
    int currentPos = 0;
    int okay = 0;
    
    if (strcmp(*cmd, "") == 0){
        printf("cmd error, is NULL.\n");
        return -1;
    }
    
    /* Make cmd UPPERCASE */
    for (i = 0; i < strlen(*cmd); i++)
        *cmd[i] = toupper(*cmd[i]);
    
    /* find command from list */
    while (strcmp(commandList[currentPos].command, "") != 0) {
        if (strcmp(commandList[currentPos].command, *cmd) == 0) {
            if (commandList[currentPos].function == NULL) {
                char tmpBuf[32] = {};
                sprintf(tmpBuf, "%s command unimplemented.", *cmd);
                AckResponse(connectFd, ACK_502, tmpBuf);
            } else {
                if (1 == CommandVerification(*cmd, *param, commandList[currentPos].check))
                    (*commandList[currentPos].function)(connectFd, *param, NULL);
                else
                    printf("RunCommand, Command Verification error.\n");
            }
            okay = 1;
            break;
        }
        currentPos++;
    }
    if (!okay) {
        char tmpBuf[32] = {};
        sprintf(tmpBuf, " command %s not understood.", *cmd);
        AckResponse(connectFd, ACK_500, tmpBuf);
    }
    
    return ;
}



void SignalAlarmHandler(int connectFd)
{
    char tmpBuf[32] = {};
    sprintf(tmpBuf, "No Transfer Timeout (%d seconds): Service not available, closing control connection.", DEFAULT_MAX_CONNECT_COUNT);
    AckResponse(connectFd, ACK_421, tmpBuf);
    fclose(file);
    close(connectFd);
    exit(0);
}

/**
 *  
 *  解析配置；
 *  创建监听socket，循环监听端口；
 *  接收到命令后，fork子进程进行处理；
 *  查找命令表，运行并返回应答码；
 *  进入到下一次的监听循环；
 *  
 **/
int main(int argc, char** argv)
{
    Option options;
    
    //TODO:解析配置参数
    ParseOptions((const char**) argv, (const int) argc, &options);
    
    //TODO:创建监听socket，进入主循环，监听端口数据
    int listenFd;
    CreateListenSocket((const Option*) &options, listenFd);
    
    //TODO: set the program in a daemon
    SetProgramDaemon(sysUid);

    //TODO:fork子进程进行命令处理
    /* main loop */
    struct sockaddr_in socketAddr;
    int len;
    FILE *fileStream;
    char recvBuf[BUFSIZE];
    char *cmd, *param;
    char retValue[BUFSIZE];
    int connectFd;
    char tmpBuf[64] = {0};
    
    while (1) {
        len = sizeof(socketAddr);
        connectFd = accept(listenfd, (struct sockaddr *)&socketAddr, &len);
        if (connectFd == -1)
            continue;
        
        //fork子进程处理命令
        if (fork() == 0) {
            len = sizeof(socketAddr);
            getsockname(connectFd, (struct sockaddr *)&socketAddr, &len);
            localPort.host = socketAddr.sin_addr.s_addr;
            fileStream = fdopen(connectFd, "rt+");
            setbuf(fileStream, (char *)NULL);  //关闭缓冲，参数设置为NULL
            getcwd(path, BUFSIZE);  //获取当前目录的绝对路径
            
            sprintf(tmpBuf, "myFTP ready, hostname: %s", hostname);
            AckResponse(connectFd, ACK_220, tmpBuf);
            
            signal(SIGALRM, SignalAlarmHandler);
            alarm(options.timeOut);
            
            /* child process's main loop */
            while (fgets(recvBuf, BUFSIZE, fileStream) != NULL) {
                /* 解析命令和参数 */
                ParseCommand((const char*)recvBuf, &cmd, &param);
                if (strcmp(cmd, "") == 0)
                    continue;
                
                /* 从命令注册表中，查找函数，校验命令和参数，运行并返回返回应答码 */
                RunCommand((const char**)&cmd, (const char**)&param, connectFd, retValue);
            }
            
            fclose(fileStream);
            exit(0);
        }
        close(connectFd);
    }
    
    return 0;
}






