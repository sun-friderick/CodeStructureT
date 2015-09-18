########################################################################################
#############              生成 monitor 库文件                               ############## 
########################################################################################
LIST (APPEND monitor_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/logMonitor.c
    
    #${CMAKE_CURRENT_SOURCE_DIR}/Server/ftpserver_function.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Server/ftpserver.c
    #${CMAKE_CURRENT_SOURCE_DIR}/Util/logCrypto.c
    )



########################################################################################
#############               设置 monitor 库版本号                          ############## 
########################################################################################
set(monitor_LIB_VERSION     "1.0.0")
set(monitor_LIB_SOVERSION   "1")



########################################################################################
#############              添加头文件目录                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Monitor
    ${PROJECT_SOURCE_DIR}/Monitor/Util
    ${PROJECT_SOURCE_DIR}/Monitor/Server
)  



########################################################################################
#############               添加 链接库文件目录                           ############## 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  



########################################################################################
#############           生成目标库文件 并 标明版本号                      ############## 
########################################################################################
IF (COMPONENT_monitor)
    #生成动态库  静态库则 STATIC  
    add_library (monitorlib SHARED      ${monitor_LIB_SRCS})  
    add_library (monitors STATIC        ${monitor_LIB_SRCS})  
    # 设置版本号，SOVERSION为API版本号
    set_target_properties(monitorlib   PROPERTIES 
                            VERSION ${monitor_LIB_VERSION} 
                            SOVERSION ${monitor_LIB_SOVERSION} )
ELSE (COMPONENT_monitor)
    MESSAGE(STATUS "Not Include monitor module.")
ENDIF (COMPONENT_monitor)



########################################################################################
#############                 生成目标库测试文件                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG)
    add_executable(TestMonitor.elf monitorTest.c)
    add_dependencies(TestMonitor.elf monitorlib loglib)
    target_link_libraries(TestMonitor.elf monitorlib loglib)

ELSE (TEST_MODULE_FLAG)
    MESSAGE(STATUS "Not Include jsoncpp module.")
ENDIF (TEST_MODULE_FLAG)




