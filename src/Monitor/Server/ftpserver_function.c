#include <shadow.h>
#include <unistd.h>
#include <crypt.h>
#include <pwd.h>
#include <time.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <utime.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <malloc.h>
#include <sys/file.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "ftpserver.h"
#include "ftpserver_function.h"





/** 输出到客户端 */
extern int AckResponse(const int connectFd, const int AckCode, const char* AckPromote);

/** 将buf以空格做分隔符分解成两个字符串 */
void explode(char *buf, char **cmd, char **param)
{
    printf("%s\n", param);
    return ;
}

/**
 *  HELP <command>	    
 *  返回指定命令信息
 **/
void Help(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    
    if (!param) {
        AckResponse(connectFd, ACK_214, " The following commands are recognized (* =>'s unimplemented).");
        AckResponse(connectFd, ACK_214, "   USER    PASS    ACCT*   CWD     XCWD    CDUP    XCUP    SMNT*   ");
        AckResponse(connectFd, ACK_214, "   QUIT    REIN*   PORT    PASV    TYPE    STRU*   MODE*   RETR    ");
        AckResponse(connectFd, ACK_214, "   STOR    STOU*   APPE    ALLO*   REST    RNFR    RNTO    ABOR    ");
        AckResponse(connectFd, ACK_214, "   DELE    MDTM    RMD     XRMD    MKD     XMKD    PWD     XPWD    ");
        AckResponse(connectFd, ACK_214, "   SIZE    LIST    NLST    SITE    SYST    STAT    HELP    NOOP    ");
        AckResponse(connectFd, ACK_214, " Direct comments to nolove@263.net");
    } else {
        AckResponse(ACK_214, " Sorry, I haven't write the topic help.");
    }

    return ;
}



//使用json文件存储用户名密码，只要用户登录，将用户信息贮存在内存结构体链表中
//  "ftp01":{passwd:"123456","homedir":"/home/ftp1"},"ftp02":{passwd:"123456","homedir":"/home/ftp2"},"ftp03":{passwd:"123456","homedir":"/home/ftp3"},
typedef struct UserInfoNode{
    char username[32]; //用户账号
    char passwd[32];   //用户密码
    char userHomedir[128]; //用户家目录路径
    char userInfo[128];
}  UserInfo;


//贮存在 内存
typedef struct Node{
    char userName[32]; //用户账号
    char passwd[32];   //用户密码
    int loginStatus;   //控制登陆流程：0-init status, 1-has input username, 2-has input passwd, 3-success logged in(has input username && passwd)
    int ftpUid;        //用户识别码
    char userHomeDir[128]; //用户家目录路径
    char userInfo[128];
    struct Node * pNext;  //指针域
} MemUserInfo, * pMenUserInfo;

MemUserInfo* g_userListHead = NULL;


 //用户账号 //用户密码 //用户家目录路径 //用户信息 //登陆流程状态 //用户识别码 
extern int addUserToMemList(char* userName, char* passwd, char* userHomeDir, char* userInfo, int loginStatus, int ftpUid);


//boolen
int g_userHasLoginFlag = 0;
int g_userAnonymousLoginFlag = 0;
int g_inputUserFlag = 0;

//TODO:: 修改为 用户信息结构体链表
char username[100];

/**
 *  USER <username>	
 *  系统登录的用户名 
 **/
void User(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    if(g_userHasLoginFlag == 1) {
        AckResponse(connectFd, ACK_503, "Bad sequence of commands: You are already logged in!");
        return ;
    }
    if(!param){
        AckResponse(connectFd, ACK_500, "Syntax error, command unrecognized: 'USER' command requires a parameter.");
        printf("\n");
        return ;
    }
    
    strncpy(username, param, 100);
    //sprintf(username, "\"%s\"", param);
    //TODO::查找比较json配置文件中username
    /**
     *  查找比较json配置文件中username：
     *          若json配置文件中存在，则查找比较 内存中全局列表：
     *                      若存在，返回已经登陆，
     *                      若不存在，则拷贝添加到全局列表，设置登录状态
     *         若不存在，返回错误信息
     **/
    
    if(strstr(json_file, username) == NULL){
        printf("json[%s] not found [%s].\n", json_file, username);
        return ;
    }
    
    char *out;
    cJSON *json;
    FILE *jsonFileFd = fopen(json_filename, "rb");
    fseek(jsonFileFd, 0, SEEK_END);
    long len = ftell(jsonFileFd);
    fseek(jsonFileFd, 0, SEEK_SET);
	char *data = (char*)malloc(len + 1);
    fread(data, 1, len, jsonFileFd);
    fclose(jsonFileFd);
	json = cJSON_Parse(data);
	if (!json) {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    } else {
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
	free(data);
    
    while(json->next){
        //查找比较json配置文件中username：
        if(strcasecmp(json->string, username) == 0){
            cJSON tmpJson;
            memcpy(tmpJson, *json);
            int hasLoginFlag = 0;
            pUserList = g_inputUserFlag;
            //若json配置文件中存在，查找比较 内存中全局列表
            while(pUserList->pNext){
                if(strcasecmp(pUserList->userName, username) == 0){
                    printf("内存中全局列表中已经存在，已经登陆\n");
                    printf("Has Login , error\n");
                    hasLoginFlag = 1;
                    break;
                }
                pUserList++;
            }
            if(hasLoginFlag == 1){ //已经登陆
                printf("\n");
            } else { //否则，不存在，把这一信息 追加到内存中全局列表；
                printf("add user[%d] into userlist[%p].\n", tmpJson.username, g_inputUserFlag);
                addUserToMemList(tmpJson.username, tmpJson.passwd, tmpJson.userHomedir, tmpJson.userInfo, 1, 0);
            }
            break;
        }
    }
    

    if (strcasecmp(username, "anonymous") == 0) {
        g_userAnonymousLoginFlag = 1;
        strncpy(username, "ftp", 100);
        AckResponse(connectFd, ACK_331, "Anonymous login ok, send your complete e-mail address as password.");
    } else {
        char tmpBuf = {};
        sprintf(tmpBuf, "Password required for %s.", username);
        AckResponse(connectFd, ACK_331, "Anonymous login ok, send your complete e-mail address as password.");
    }

    g_inputUserFlag = 1;
    return ;
}

char path[PATH_MAX];
/**
 *  PASS <password>	    
 *  系统登录密码
 **/
void Pass(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    if (!g_inputUserFlag) {
        AckResponse(connectFd, ACK_503, "Bad sequence of commands: Login with USER first.");
        return;
    }
    char tmpBuf[BUFSIZE] = {};

    // 解密密码：
    DecryptCommand(param, EncryptMode_Unencrypt, tmpBuf);

    char *passwd, salt[13];

    /* judge and chdir to its home directory */
    if (!g_userAnonymousLoginFlag) {
        if (g_sysUid != 0) {    //当使用非root 身份执行程序时
            AckResponse(connectFd, ACK_530, "Not logged in: Login incorrect.");
            g_inputUserFlag = 0;
            return;
        }

        //get current user info; get passwd
        /**
         *  先判断登陆状态：
         *          若状态正确，从用户信息结构体链表中 获取用户登录相关信息，判断密码是否存在：
         *                  若结构体链表中密码存在：判断密码是否正确：
         *                          若密码正确，设置登陆状态，并判断家目录是否存在：若不存在则创建，返回提示；
         *                          若密码错误，返回错误提示；
         *                  若结构体链表中密码不存在：
         *  
         *          若状态错误，返回错误提示；
         *  
         **/
        setuid(pw->pw_uid);

        MemUserInfo* userInfo = NULL;
        
        //跟还到用户家目录
        strcpy(path, "");
        if (userInfo->userHomeDir)
            strncpy(path, userInfo->userHomeDir, PATH_MAX);
        else
            strcpy(path, "/");
        chdir(path);
        getcwd(userInfo->userHomeDir, PATH_MAX);  //将当前工作目录的绝对路径复制到参数path所指的内存空间中

        g_userHasLoginFlag = 1;
        char tmpBuf[32] = {};
        sprintf(tmpBuf, "User logged in, proceed: User %s logged in.", username);
        AckResponse(connectFd, ACK_230, tmpBuf);
    } else { // user Anonymous Login
        if ((pw = getpwuid(g_sysUid)) == NULL) {
            AckResponse(connectFd, ACK_530, "Not logged in: Login incorrect.");
            g_inputUserFlag = 0;
            return;
        }
        
        //更换到用户家目录
        if (pw->pw_dir)
            strncpy(basedir, pw->pw_dir, PATH_MAX);
        else
            strcpy(basedir, "");
        strcpy(path, "/");
        chdir(basedir);
        getcwd(basedir, PATH_MAX);
        
        g_userHasLoginFlag = 1;
        AckResponse(connectFd, ACK_230, "User logged in, proceed: Anonymous access granted, restrictions apply.");
    }
    g_userHasLoginFlag = 1;
    AckResponse(connectFd, ACK_230, "User logged in, proceed: Anonymous access granted, restrictions apply.");
    
    return ;
}

/**
 *  PWD	                
 *  显示当前工作目录
 **/
void Pwd(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  CWD  <dir path>	    
 *  改变服务器上的工作目录
 **/
void Cwd(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  CDUP <dir path> 	
 *  改变服务器上的父目录
 **/
void Cdup(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  MKD  <directory>	
 *  在服务器上建立指定目录
 **/
void Mkd(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  RMD  <directory>	
 *  在服务器上删除指定目录
 **/
void Rmd(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  TYPE <data type>	
 *  数据类型（A=ASCII，E=EBCDIC，I=binary）
 **/
void Type(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  
 **/
void Size(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  STAT <directory>	
 *  在当前程序或目录上返回信息
 **/
void Stat(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  DELE <filename>	   
 *   删除服务器上的指定文件
 **/
void Dele(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  QUIT	            
 *  从 FTP 服务器上退出登录
 **/
void Quit(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  NOOP	            
 *  无动作，除了来自服务器上的承认
 **/
void Noop(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  LIST <name>	        
 *  如果是文件名列出文件信息，如果是目录则列出文件列表
 **/
void List(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  NLST <directory>	
 *  列出指定目录内容
 **/
void Nlst(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  RETR <filename>	    
 *  从服务器上找回（复制）文件
 **/
void Retr(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  STOR <filename> 	
 *  储存（复制）文件到服务器上
 **/
void Stor(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  APPE <filename>	    
 *  添加文件到服务器同名文件
 **/
void Appe(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/** 
 *  ABOR
 *  中断数据连接程序
 **/
void Abor(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  
 **/
void Mdtm(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  SYST	            
 *  返回服务器使用的操作系统
 **/
void Syst(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  REST <offset>	    
 *  由特定偏移量重启文件传递
 **/
void Rest(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  RNFR <old path>	    
 *  对旧路径重命名
 **/
void Rnfr(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  RNTO <new path>	    
 *  对新路径重命名
 **/
void Rnto(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  SITE <params>	    
 *  由服务器提供的站点特殊参数
 **/
void Site(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  PORT <address>	    
 *  IP 地址和两字节的端口 ID
 **/
void Port(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  PASV	            
 *  请求服务器等待数据连接
 **/
void Pasv(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}


/**
 *  STRU <type>	        
 *  数据结构（F=文件，R=记录，P=页面）
 **/
void Stru(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  MODE <mode>	        
 *  传输模式（S=流模式，B=块模式，C=压缩模式）
 **/
void Mode(int connectFd, char *param, void* reserved)
{
    printf("%s\n", param);
    return ;
}

/**
 *  STOU <filename>	    
 *  储存文件到服务器名称上
 **/

 
/**
 *   REIN	            
 *  重新初始化登录状态连接
 **/
 
 
/**
 *   SMNT <pathname>	    
 *  挂载指定文件结构
 **/

 
/**
 *   ALLO <bytes>	    
 *  为服务器上的文件存储器分配字节
 **/
 
 
/**
 *   ACCT <account>	    
 *  系统特权帐号
 **/
 








