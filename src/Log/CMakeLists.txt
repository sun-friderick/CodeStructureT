########################################################################################
##########         ���� loglib ���ļ�                                        ########### 
########################################################################################
LIST (APPEND log_LIB_SRCS
    LogC.c 
    )
    
    
########################################################################################
#############               ���� loglib ��汾��                          ############## 
########################################################################################
set(log_LIB_VERSION     "1.0.0")
set(log_LIB_SOVERSION   "1")



########################################################################################
##########            ���� functions_statisticslib ���ļ�                    ########### 
########################################################################################
LIST (APPEND functions_statistics_LIB_SRCS
    FunctionsStatistics.cpp
    )


########################################################################################
##########            ���� ������ļ������ͷ�ļ�Ŀ¼                        ########### 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Log   
)  
  
  
########################################################################################
##########            ���� ���ӿ��ļ�Ŀ¼                                    ########### 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  


########################################################################################
##########            ����Ŀ����ļ� �� �����汾��                           ########### 
########################################################################################
IF (COMPONENT_log)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (loglib SHARED          ${log_LIB_SRCS})  
    add_library (logs STATIC            ${log_LIB_SRCS})  
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(loglib   PROPERTIES 
                            VERSION  ${log_LIB_VERSION} 
                            SOVERSION ${log_LIB_SOVERSION} )
    
ELSE (COMPONENT_log)
    MESSAGE(STATUS "Not Include log module.")
ENDIF (COMPONENT_log)


########################################################################################
##########        ����Ŀ����ļ� ��������Ҫ������ ����������ϵ               ########### 
########################################################################################
IF (COMPONENT_functions_statistics)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (functions_statisticslib SHARED         ${functions_statistics_LIB_SRCS})  
    add_library (functions_statistics STATIC            ${functions_statistics_LIB_SRCS})  
    # ����������ϵ�������ǰ������ײ�Ĺ����⣬����Ҫ����
    add_dependencies (functions_statisticslib        pthread)
    add_dependencies (functions_statistics           pthread)
    # ����Ҫ���ӵĹ�����, ���˳����Ǳ�����������ʱ˳��
    target_link_libraries (functions_statisticslib       pthread)
    target_link_libraries (functions_statistics          pthread)

ELSE (COMPONENT_functions_statistics)
    MESSAGE(STATUS "Not Include functions_statistics module.")
ENDIF (COMPONENT_functions_statistics)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG)
    add_executable(Testlog.elf logTest.c)
    add_dependencies(Testlog.elf loglib functions_statisticslib pthread)
    target_link_libraries(Testlog.elf loglib functions_statisticslib pthread)

ELSE (TEST_MODULE_FLAG)
    MESSAGE(STATUS "Not Include jsoncpp module.")
ENDIF (TEST_MODULE_FLAG)


