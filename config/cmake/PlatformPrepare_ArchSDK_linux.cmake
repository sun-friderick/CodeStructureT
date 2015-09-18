########################################################################################
###############              根据平台判断所需的SDK以及库文件              ##############
########################################################################################

add_custom_command( TARGET arch_libs 
    PRE_BUILD
    COMMENT " Start to prepare arch_libs ...."
    COMMAND if [ -d ${LIBRARY_OUTPUT_PATH} ]; then mkdir -p ${LIBRARY_OUTPUT_PATH}/ffmpeg  ${LIBRARY_OUTPUT_PATH}/SDL2 \; fi
    COMMAND if [ -f ${SRC_LIBS_PATH}/ffmpeg-${Module_ffmpeg_Version}-libs.tar.gz ]; then tar -zxf ffmpeg-${Module_ffmpeg_Version}-libs.tar.gz -C ${LIBRARY_OUTPUT_PATH}/ffmpeg \; fi
    COMMAND if [ -f ${SRC_LIBS_PATH}/ffmpeg-${Module_ffmpeg_Version}-libs.tar.gz ]; then tar -zxf SDL2-${Module_ffmpeg_Version}-libs.tar.gz -C ${LIBRARY_OUTPUT_PATH}/SDL2 \; fi
    COMMENT " Prepare arch_libs end ......"
    VERBATIM
)

#add_custom_target ( arch_libs
#    
#    COMMENT "test arch_libs"
#    COMMAND if [ -d ${LIBRARY_OUTPUT_PATH} ]; then mkdir -p ${LIBRARY_OUTPUT_PATH}/ffmpeg  ${LIBRARY_OUTPUT_PATH}/SDL2 \; fi
#    COMMAND if [ -f ${SRC_LIBS_PATH}/ffmpeg-${Module_ffmpeg_Version}-libs.tar.gz ]; then tar -zxf ffmpeg-${Module_ffmpeg_Version}-libs.tar.gz -C ${LIBRARY_OUTPUT_PATH}/ffmpeg \; tar -zxf SDL2-${Module_ffmpeg_Version}-libs.tar.gz -C ${LIBRARY_OUTPUT_PATH}/SDL2 \; fi
#        COMMENT " Prepare arch_libs end ......"

#    VERBATIM
#    ) 
    
SET(RUN_HELLO_WORLD_COMMAND_FILE    "${CMAKE_BINARY_DIR}/run-hello-world.cmake")
FILE(WRITE   ${RUN_HELLO_WORLD_COMMAND_FILE}  "SET(ENV{LANG en})\n")

FILE(APPEND  ${RUN_HELLO_WORLD_COMMAND_FILE}  "EXECUTE_PROCESS(COMMAND \"./hello-world\" WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}/bin\")\n")

FILE(APPEND  ${RUN_HELLO_WORLD_COMMAND_FILE}  "EXECUTE_PROCESS(COMMAND \"${GCOVR_EXECUTABLE}\" --html  -r \"${CMAKE_SOURCE_DIR}\" --output \"${COVERAGE_HTML_FILE}\" WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\")\n")

EXECUTE_PROCESS(COMMAND   hello-world   WORKING_DIRECTORY    ${CMAKE_BINARY_DIR})

ADD_CUSTOM_TARGET(run-hello-world
    COMMAND ${CMAKE_COMMAND} ARGS -P ${RUN_HELLO_WORLD_COMMAND_FILE}
    DEPENDS hello-world
    )

 
