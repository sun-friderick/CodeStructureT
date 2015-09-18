########################################################################################
#############           添加所要编译的源文件.                             ############## 
########################################################################################
list( APPEND utils_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/logParseFiles.c
    )


########################################################################################
#############               添加源码子目录                                ############## 
########################################################################################
add_subdirectory( ParserIni )
add_subdirectory( ParserJsonCPP )
add_subdirectory( ParserTinyXML2 )

MESSAGE("${CMAKE_CURRENT_SOURCE_DIR} status.")

    
########################################################################################
#############           生成目标库文件 并 标明版本号                      ############## 
########################################################################################
IF (COMPONENT_utils)
    #生成动态库  静态库则 STATIC  
    add_library (utilslib SHARED      ${utils_SRCS})  
    add_library (utilss STATIC        ${utils_SRCS})  
    # 设置版本号，SOVERSION为API版本号
    set_target_properties(utilslib   PROPERTIES VERSION 1.0.0 SOVERSION 1 )

ELSE (COMPONENT_utils)
    MESSAGE(STATUS "Not Include COMPONENT_utils module.")
ENDIF (COMPONENT_utils)


