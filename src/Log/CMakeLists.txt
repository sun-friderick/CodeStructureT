########################################################################################
##########         生成 loglib 库文件                                        ########### 
########################################################################################
LIST (APPEND log_LIB_SRCS
    LogC.c 
    )
    
    
########################################################################################
#############               设置 loglib 库版本号                          ############## 
########################################################################################
set(log_LIB_VERSION     "1.0.0")
set(log_LIB_SOVERSION   "1")



########################################################################################
##########            生成 functions_statisticslib 库文件                    ########### 
########################################################################################
LIST (APPEND functions_statistics_LIB_SRCS
    FunctionsStatistics.cpp
    )


########################################################################################
##########            添加 编译库文件所需的头文件目录                        ########### 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Log   
)  
  
  
########################################################################################
##########            添加 链接库文件目录                                    ########### 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  


########################################################################################
##########            生成目标库文件 并 标明版本号                           ########### 
########################################################################################
IF (COMPONENT_log)
    #生成动态库  静态库则 STATIC  
    add_library (loglib SHARED          ${log_LIB_SRCS})  
    add_library (logs STATIC            ${log_LIB_SRCS})  
    # 设置版本号，SOVERSION为API版本号
    set_target_properties(loglib   PROPERTIES 
                            VERSION  ${log_LIB_VERSION} 
                            SOVERSION ${log_LIB_SOVERSION} )
    
ELSE (COMPONENT_log)
    MESSAGE(STATUS "Not Include log module.")
ENDIF (COMPONENT_log)


########################################################################################
##########        生成目标库文件 添加所需要依赖库 设置依赖关系               ########### 
########################################################################################
IF (COMPONENT_functions_statistics)
    #生成动态库  静态库则 STATIC  
    add_library (functions_statisticslib SHARED         ${functions_statistics_LIB_SRCS})  
    add_library (functions_statistics STATIC            ${functions_statistics_LIB_SRCS})  
    # 设置依赖关系，如果当前库是最底层的共享库，则不需要设置
    add_dependencies (functions_statisticslib        pthread)
    add_dependencies (functions_statistics           pthread)
    # 添加要链接的共享库, 库的顺序就是编译器的链接时顺序
    target_link_libraries (functions_statisticslib       pthread)
    target_link_libraries (functions_statistics          pthread)

ELSE (COMPONENT_functions_statistics)
    MESSAGE(STATUS "Not Include functions_statistics module.")
ENDIF (COMPONENT_functions_statistics)



########################################################################################
#############                 生成目标库测试文件                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG)
    add_executable(Testlog.elf logTest.c)
    add_dependencies(Testlog.elf loglib functions_statisticslib pthread)
    target_link_libraries(Testlog.elf loglib functions_statisticslib pthread)

ELSE (TEST_MODULE_FLAG)
    MESSAGE(STATUS "Not Include jsoncpp module.")
ENDIF (TEST_MODULE_FLAG)



