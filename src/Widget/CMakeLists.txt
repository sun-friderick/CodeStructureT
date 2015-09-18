########################################################################################
#############                生成 widget 库文件                           ############## 
########################################################################################
LIST (APPEND widget_LIB_SRCS
    LogWidget.c 
    )
    
    
########################################################################################
#############               设置 widget 库版本号                          ############## 
########################################################################################
set(widget_LIB_VERSION     "1.0.0")
set(widget_LIB_SOVERSION   "1")



########################################################################################
#############              添加头文件目录                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Widget   
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
IF (COMPONENT_widget)
    #生成动态库  静态库则 STATIC  
    add_library (widgetlib     SHARED        ${widget_LIB_SRCS})  
    add_library (widgets       STATIC       ${widget_LIB_SRCS})  

    # 设置依赖关系，如果当前库是最底层的共享库，则不需要设置
    #add_dependencies(widgetlib     pthread)
    #add_dependencies(widgets     pthread)
    # 添加要链接的共享库, 库的顺序就是编译器的链接时顺序
    #target_link_libraries(widgetlib pthread)
    #target_link_libraries(widgets pthread)

    # 设置版本号，SOVERSION为API版本号
    set_target_properties(widgetlib    PROPERTIES 
                            VERSION  ${widget_LIB_VERSION}   
                            SOVERSION   ${widget_LIB_SOVERSION} )
ELSE (COMPONENT_widget)
    MESSAGE(STATUS "Not Include COMPONENT_widget module.")
ENDIF (COMPONENT_widget)

