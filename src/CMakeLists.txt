########################################################################################
#############           ���ÿ�ִ���ļ�����                                ############## 
########################################################################################
set( TARGET_NAME myplayer.elf )



########################################################################################
#######     ����build_info.hͷ�ļ�  ׼���ڱ�������ʱ����ĵ��������ͷ�ļ�      ######## 
########################################################################################
add_custom_command(OUTPUT build_info.h
    PRE_BUILD
    COMMAND exec "${BUILD_SCRIPT_PATH}/make_version.sh"
    COMMAND exec ${BUILD_SCRIPT_PATH}/prepare_arch_libs.sh ${LIBRARY_OUTPUT_PATH} ${SRC_LIBS_PATH} ${SRC_INCLUDE_PATH} ${Module_ffmpeg_Version} ${Module_SDL2_Version}
    COMMENT "test make_version.sh"
    VERBATIM
)
add_custom_target (build_info
    ALL DEPENDS build_info.h
    COMMENT "test build_info"
    VERBATIM
    )   



########################################################################################
##########         ����Դ�ļ�����Ҫ��ͷ�ļ�Ŀ¼��ָ��ͷ�ļ���ѰĿ¼          ########### 
########################################################################################
include_directories(
	${SRC_INCLUDE_PATH}
    ${SRC_INCLUDE_PATH}/ffmpeg
    ${SRC_INCLUDE_PATH}/SDL2/include
    #/usr/local/SDL2/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Log
    ${PROJECT_SOURCE_DIR}/Monitor
    ${PROJECT_SOURCE_DIR}/Player
    ${PROJECT_SOURCE_DIR}/Utils/
    ${PROJECT_SOURCE_DIR}/Utils/ParserIni
    ${PROJECT_SOURCE_DIR}/Utils/ParserJsonCPP
    ${PROJECT_SOURCE_DIR}/Utils/ParserTinyXML2
    ${PROJECT_SOURCE_DIR}/Widget
)



########################################################################################
#########          ����Դ�ļ���������ӿ��ļ�·����ָ�����ļ�����Ŀ¼         ########## 
########################################################################################
link_directories(
    ${SRC_LIBS_PATH}
    ${LIBRARY_OUTPUT_PATH}
    ${LIBRARY_OUTPUT_PATH}/ffmpeg
    ${LIBRARY_OUTPUT_PATH}/SDL2/lib
#    /usr/local/SDL2/lib
    ${SRC_SDK_PATH}/debug
    ${SRC_SDK_PATH}/release    
) 



########################################################################################
#############           ������Ҫ�����Դ�ļ�.                             ############## 
########################################################################################
list( APPEND myplayer_SRCS
	test_cmake.c
    )

   
   
########################################################################################
#############               ����Դ����Ŀ¼                                ############## 
########################################################################################
add_subdirectory( Log )
add_subdirectory( Utils )
add_subdirectory( Monitor )
add_subdirectory( Player )
add_subdirectory( Widget )
#add_subdirectory( Test )

MESSAGE("${TARGET_NAME} status.")



########################################################################################
#############          ���ô����־�Ա������˳��.                      ############## 
########################################################################################
#GET_DIRECTORY_PROPERTY(_directory_flags INCLUDE_DIRECTORIES)
#GET_FILENAME_COMPONENT(_name ${_input} NAME)
#GET_FILENAME_COMPONENT(_path ${_input} PATH)
#SET(_IS_CURRENT_BINARY_DIR_BEFORE_INCLUDE FALSE)
#FOREACH(item ${_directory_flags})
    # �жϵ�ǰ��������Ŀ¼�Ƿ���ͷ�ļ�����·�����ײ�, ����������������.
#    IF(${item} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT _IS_CURRENT_BINARY_DIR_BEFORE_INCLUDE )
#        SET(_IS_CURRENT_BINARY_DIR_BEFORE_INCLUDE TRUE)
#    ELSEIF(NOT _IS_CURRENT_BINARY_DIR_BEFORE_INCLUDE AND NOT ${item} STREQUAL ${CMAKE_CURRENT_BINARY_DIR}) 
#        MESSAGE(FATAL_ERROR  "This is the ADD_PRECOMPILED_HEADER macro. "  
#                "CMAKE_CURREN_BINARY_DIR has to mentioned at INCLUDE_DIRECTORIES's argument list before ${_path}, where ${_name} is located"
#                )
#    ENDIF(${item} STREQUAL ${CMAKE_CURRENT_BINARY_DIR} AND NOT _IS_CURRENT_BINARY_DIR_BEFORE_INCLUDE )

#    LIST(APPEND _compile_FLAGS "-I${item}")
#ENDFOREACH(item)



########################################################################################
#############              ָ�������ļ�����                               ############## 
########################################################################################
add_executable( ${TARGET_NAME} ${myplayer_SRCS} )



########################################################################################
#############         ����Ŀ��������ϵ�����û�У�����Ҫ����            ############## 
########################################################################################
add_dependencies(${TARGET_NAME} players widgets loglib SDL2 monitorlib utilslib)



########################################################################################
#############                 ����Ҫ���ӵĹ�����                          ############## 
########################################################################################
target_link_libraries(${TARGET_NAME}    
    players
    widgets
    
    loglib 
    -finstrument-functions functions_statisticslib
    monitorlib
    utilslib
    
    jsoncpplib
    ini-parserlib

#    avcodec
#    avdevice
#    avfilter
#    avformat
#    avutil
#    swscale

    SDL2
    )
#link_libraries(${TARGET_NAME}  loglib -finstrument-functions functions_statisticslib)



########################################################################################
########          ���� �������֮��ִ�е�����   ֻ��� �����������ӹ�����       ######## 
########################################################################################
add_custom_command(TARGET ${TARGET_NAME} 
    POST_BUILD
	COMMAND sudo cp -arpf ${TARGET_NAME} ${DISTRIBUTE_PATH}/
	#COMMENT "cp ${TARGET_NAME} ${DISTRIBUTE_PATH}/${TARGET_NAME}"
    COMMAND sudo tar -zcvf ${DISTRIBUTE_PATH}/libs.tar.gz ${LIBRARY_OUTPUT_PATH}/
    COMMENT "cp ${LIBRARY_OUTPUT_PATH}/* ${DISTRIBUTE_PATH}/ "
	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
	VERBATIM
)



########################################################################################
#############                   ��װ��ִ�г����ļ�                        ############## 
########################################################################################
#install(TARGETS ${TARGET_NAME} ARCHIVE DESTINATION ${DISTRIBUTE_PATH}/debug)



########################################################################################
##########                ���Ӳ��Գ���  ����make test��ִ�в���               ########## 
########################################################################################
### ����һ����Ϊmytest�Ĳ��ԣ����ݵĲ����������������Լ����ɵĿ�ִ��Ŀ���ļ�������·��
#add_test(NAME  mytest COMMAND  ${TARGET_NAME} --config $<CONFIGURATION> --exe $<TARGET_FILE:${TARGET_NAME}> )
#enable_testing()





