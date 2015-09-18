/**
 *  按功能分组的响应代码：
200 Command okay. （命令 OK）
500 Syntax error, command unrecognized. （语法错误，命令不能被识别）可能包含因为命令行太长的错误。
501 Syntax error in parameters or arguments. （参数语法错误）
202 Command not implemented, superfluous at this site. （命令没有实现，对本站点冗余）
502 Command not implemented. （命令没有实现）
503 Bad sequence of commands. （命令顺序错误）
504 Command not implemented for that parameter. （没有实现这个命令参数）


110 Restart marker reply. （重新开始标记响应）对于这种情况，文本应该是明确的，无需进行特殊实现；必须形如：MARK yyyy = mmmm ； yyyy 是用户进程数据流标记，mmmm服务器的等效标记（注意，标记间的空格和“=“）
211 System status, or system help reply. （系统状态，或者系统帮助响应。）
212 Directory status. （目录状态）
213 File status. （文件状态）
214 Help message. （帮助信息）关于如何使用服务器，或者特殊的非标准的命令的意义。只对人类用户有用。
215 NAME system type. （系统类型名称）这里的 NAME 指在 Assigned Numbers 文档中列出的正式名称。


120 Service ready in nnn minutes. （服务将在 nnn 分钟后准备完成）
220 Service ready for new user. （接受新用户服务准备完成）
221 Service closing control connection. （服务关闭控制连接）已注消
421 Service not available, closing control connection. （服务不可用，关闭控制连接）如果服务器知道它必须关闭，应该以 421 作为任何命令的响应代码。


125 Data connection already open; transfer starting. （数据连接已打开，传输开始）
225 Data connection open; no transfer in progress. （数据连接打开，没有传输）
425 Can't open data connection. （不能打开数据连接）
226 Closing data connection. （关闭数据连接）请求文件动作成功（例如，文件传输或者放弃）
426 Connection closed; transfer aborted. （连接关闭，放弃传输）


227 Entering Passive Mode (h1,h2,h3,h4,p1,p2). （进入被动模式）
230 User logged in, proceed. (用户成功登录，继续）
530 Not logged in. （没有登录成功）


331 User name okay, need password. （用户名 OK，需要密码）
332 Need account for login. （需要帐户才能登录）
532 Need account for storing files. （需要帐户来存储文件）


150 File status okay; about to open data connection. （文件状态 OK，将打开数据连接）
250 Requested file action okay, completed. （请求文件动作 OK，完成）
257 "PATHNAME" created. （创建了“PATHNAME”）
350 Requested file action pending further information. （请求文件动作需要进一步的信息）
450 Requested file action not taken. （请求文件动作没有执行）文件不可使用（例如，文件忙）
550 Requested action not taken. (请求的动作没有执行） 文件不可用 （例如， 没有找到文件 ，没有访问权限）


451 Requested action aborted. Local error in processing. （请求动作放弃，处理中发生本地错误）
551 Requested action aborted. Page type unknown. （请求动作放弃，未知的页面类型）


452 Requested action not taken. （请求动作未执行）系统存储空间不足。
552 Requested file action aborted. (请求文件动作被放弃)超出存储分配空间 （当前的路径或者数据集）
553 Requested action not taken. （请求动作未获得）文件名不允许。
 *  
 **/

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <signal.h> 
#include <sys/file.h> 


#define BUFSIZE         (1024)

#define DEFAULT_USERNAME            "anounse"
#define DEFAULT_USERPASSWD          "123456" 
#define DEFAULT_MAX_CONNECT_COUNT   32767 
#define DEFAULT_SERVERPORT          21 
#define DEFAULT_TIMEOUT             300


///响应代码	        解释说明
enum _AckCode{
    ACK_110	= 110,  //  新文件指示器上的重启标记              110 Restart marker reply. （重新开始标记响应）对于这种情况，文本应该是明确的，无需进行特殊实现；必须形如：MARK yyyy = mmmm ； yyyy 是用户进程数据流标记，mmmm服务器的等效标记（注意，标记间的空格和“=“）
    ACK_120 = 120,  //	服务器准备就绪的时间（分钟数）        120 Service ready in nnn minutes. （服务将在 nnn 分钟后准备完成）
    ACK_125	= 125,  //  打开数据连接，开始传输                125 Data connection already open; transfer starting. （数据连接已打开，传输开始）
    ACK_150 = 150,  //	打开连接                              150 File status okay; about to open data connection. （文件状态 OK，将打开数据连接）
    ACK_200 = 200,  //	成功              200 Command okay. （命令 OK）
    ACK_202 = 202,  //	命令没有执行      202 Command not implemented, superfluous at this site. （命令没有实现，对本站点冗余）
    ACK_211 = 211,  //	系统状态回复      211 System status, or system help reply. （系统状态，或者系统帮助响应。）
    ACK_212,        //	目录状态回复      212 Directory status. （目录状态）
    ACK_213,        //	文件状态回复      213 File status. （文件状态）
    ACK_214,        //	帮助信息回复      214 Help message. （帮助信息）关于如何使用服务器，或者特殊的非标准的命令的意义。只对人类用户有用。
    ACK_215,        //	系统类型回复      215 NAME system type. （系统类型名称）这里的 NAME 指在 Assigned Numbers 文档中列出的正式名称。
    ACK_220 = 220,  //	服务就绪          220 Service ready for new user. （接受新用户服务准备完成）
    ACK_221,        //	退出网络          221 Service closing control connection. （服务关闭控制连接）已注消
    ACK_225 = 225,  //	打开数据连接      225 Data connection open; no transfer in progress. （数据连接打开，没有传输）
    ACK_226,        //	结束数据连接      226 Closing data connection. （关闭数据连接）请求文件动作成功（例如，文件传输或者放弃）
    ACK_227,        //	进入被动模式      227 Entering Passive Mode (h1,h2,h3,h4,p1,p2). （进入被动模式）（IP 地址、ID 端口）
    ACK_230 = 230,  //	登录因特网        230 User logged in, proceed. (用户成功登录，继续）
    ACK_250 = 250,  //	文件行为完成      250 Requested file action okay, completed. （请求文件动作 OK，完成）
    ACK_257 = 257,  //	路径名建立        257 "PATHNAME" created. （创建了“PATHNAME”）
    ACK_331 = 331,  //	要求密码              331 User name okay, need password. （用户名 OK，需要密码）
    ACK_332,        //	要求帐号              332 Need account for login. （需要帐户才能登录）
    ACK_350 = 350,  //	文件行为暂停          350 Requested file action pending further information. （请求文件动作需要进一步的信息）
    ACK_421 = 421,  //	服务关闭          421 Service not available, closing control connection. （服务不可用，关闭控制连接）如果服务器知道它必须关闭，应该以 421 作为任何命令的响应代码。
    ACK_425 = 425,  //	无法打开数据连接  425 Can't open data connection. （不能打开数据连接）
    ACK_426,        //	结束连接          426 Connection closed; transfer aborted. （连接关闭，放弃传输）
    ACK_450 = 450,  //	文件不可用        450 Requested file action not taken. （请求文件动作没有执行）文件不可使用（例如，文件忙）
    ACK_451,        //	遇到本地错误      451 Requested action aborted. Local error in processing. （请求动作放弃，处理中发生本地错误）
    ACK_452,        //	磁盘空间不足      452 Requested action not taken. （请求动作未执行）系统存储空间不足。
    ACK_500 = 500,  //	无效命令              500 Syntax error, command unrecognized. （语法错误，命令不能被识别）可能包含因为命令行太长的错误。
    ACK_501,        //	错误参数              501 Syntax error in parameters or arguments. （参数语法错误）
    ACK_502,        //	命令没有执行          502 Command not implemented. （命令没有实现）
    ACK_503,        //	错误指令序列          503 Bad sequence of commands. （命令顺序错误）
    ACK_504,        //	无效命令参数          504 Command not implemented for that parameter. （没有实现这个命令参数）
    ACK_530 = 530,  //	未登录网络            530 Not logged in. （没有登录成功）
    ACK_532 = 532,  //	存储文件需要帐号      532 Need account for storing files. （需要帐户来存储文件）
    ACK_550 = 550,  //	文件不可用            550 Requested action not taken. (请求的动作没有执行） 文件不可用 （例如， 没有找到文件 ，没有访问权限）
    ACK_551,        //	不知道的页类型        551 Requested action aborted. Page type unknown. （请求动作放弃，未知的页面类型）
    ACK_552,        //	超过存储分配          552 Requested file action aborted. (请求文件动作被放弃)超出存储分配空间 （当前的路径或者数据集）
    ACK_553,        //	文件名不允许          553 Requested action not taken. （请求动作未获得）文件名不允许。
};


typedef struct _options{
    char userName[32]; 
    char userPasswd[32]; 
    int serverPort;
    int maxConnectCount; 
    int timeOut;  //unit: seconds
} Option;


//boolen type
int userValid = 0;

//static int connectFd;
char path[BUFSIZE];

struct _port {
        uint32_t host;
        uint16_t port;
} localPort;

int g_sysUid;


//加解密方式
enum _encryptMode{
    EncryptMode_Reserved = 0,
    EncryptMode_BASE64,
    EncryptMode_AES,
    EncryptMode_DES,
    EncryptMode_MD5,
    EncryptMode_RSA,
    EncryptMode_SHA128,
    EncryptMode_SHA256,
    EncryptMode_Private01,
    EncryptMode_Private02,
    EncryptMode_Private03,
    EncryptMode_Private04,
    EncryptMode_Unencrypt,
    EncryptMode_Undefined
} EncryptMode;


#define FLAG_NO_CHECK                1 //1
#define FLAG_NEED_PARAM              1<<1 //2
#define FLAG_NO_PARAM                1<<2 //4
#define FLAG_CHECK_LOGIN             1<<3 //8
#define FLAG_CHECK_NOLOGIN           1<<4 //16
#define FLAG_NO_TRANSFER             1<<5 //32
struct _CmdList {
        char *command;
        void (*function)(int connectFd, char *param, void* reserved);
        int check;
};



