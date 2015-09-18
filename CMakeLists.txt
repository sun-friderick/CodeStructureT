#
# Main cmake script file for myplayer project.
#

#########################################################################################
#########         检查CMake版本,设置工程所需要的最低CMake版本                ############ 
#########################################################################################
cmake_minimum_required(VERSION 2.8.3)
# 使用CMAKE_POLICY命令来设置版本的兼容性，以避免一些不必要的警告信息。
#cmake_policy(VERSION 2.8)



#########################################################################################
##################         不去检查工具链是否正常                      ################## 
#########################################################################################
set (CMAKE_C_COMPILER_WORKS 1)
set (CMAKE_CXX_COMPILER_WORKS 1)



#########################################################################################
########   CMake 生成的 makefile 时把各步骤实际调用的命令、参数一一列出    ############## 
#########################################################################################
set (CMAKE_VERBOSE_MAKEFILE ON)



#########################################################################################
##################         检查 SVN 并获取SVN version                  ################## 
#########################################################################################
IF (EXISTS "${PROJECT_SOURCE_DIR}/.svn/")
    find_package(Subversion)
    IF (Subversion_FOUND)
        Subversion_WC_INFO(${CMAKE_CURRENT_LIST_DIR} myplayer)
    ELSE (Subversion_FOUND)
        MESSAGE (FATAL_ERROR "Subversion not found.")
    ENDIF (Subversion_FOUND)
ELSE (EXISTS "${PROJECT_SOURCE_DIR}/.svn/")
    MESSAGE (STATUS "Subversion not found.")
ENDIF (EXISTS "${PROJECT_SOURCE_DIR}/.svn/")
#set (BUILD_Revision         ${myplayer_WC_REVISION}  CACHE STRING "software revision")
set (BUILD_Revision         "12134"  CACHE STRING "software revision")


#########################################################################################
##################       判断gcc编译器版本 并判断是否支持预编译头      ################## 
#########################################################################################
set(CMAKE_COMPILER_IS_GNUCXX 1) 
IF(CMAKE_COMPILER_IS_GNUCXX )
    #获取GCC和G++编译器的版本号 
    EXEC_PROGRAM(
        gcc  ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
        OUTPUT_VARIABLE _gcc_compiler_output
        RETURN_VALUE _ret_value
    )
    STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1"  gcc_version  ${_gcc_compiler_output})

    EXEC_PROGRAM(
        g++  ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
        OUTPUT_VARIABLE _gcxx_compiler_output
        RETURN_VALUE _ret_value
    )
    STRING(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1"  gcxx_version  ${_gcc_compiler_output})
    MESSAGE("GCC Version: ${gcc_version} ; G++ Version:${gcxx_version};")
    
    #判断GNU编译器是否支持预编译头
    IF(gcc_version MATCHES "4\\.[0-9](\\.[0-9])?")
        SET(PCHSupport_FOUND TRUE)
        MESSAGE("GCC Version: ${gcc_version}, PCHSupport_FOUND id TRUE")
    ELSE(gcc_version MATCHES "4\\.[0-9](\\.[0-9])?")
        IF(gcc_version MATCHES "3\\.4(\\.[0-9])?")
            SET(PCHSupport_FOUND TRUE)
            MESSAGE("GCC Version: ${gcc_version}, PCHSupport_FOUND id TRUE")
        ENDIF(gcc_version MATCHES "3\\.4(\\.[0-9])?")
    ENDIF(gcc_version MATCHES "4\\.[0-9](\\.[0-9])?")

ELSE(CMAKE_COMPILER_IS_GNUCXX )
    IF(WIN32)
        SET(PCHSupport_FOUND TRUE) # for experimental msvc support
        MESSAGE("MSVC for experimental msvc support, PCHSupport_FOUND id TRUE")
    ELSE(WIN32)
        SET(PCHSupport_FOUND FALSE)
        MESSAGE("OTHER for un-support, PCHSupport_FOUND id FALSE")
    ENDIF(WIN32)
    
ENDIF(CMAKE_COMPILER_IS_GNUCXX )



########################################################################################
#############                 项目定名以及目录                            ############## 
########################################################################################
project(myplayer CXX C)
set (EXECUTABLE_OUTPUT_PATH ${myplayer_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH    ${myplayer_BINARY_DIR}/libs)
set (PROJECT_TOP_DIR        ${CMAKE_CURRENT_LIST_DIR})
set (PROJECT_SOURCE_DIR     ${CMAKE_CURRENT_LIST_DIR}/src)



########################################################################################
#########        set some PATHs of Source && Options for Compiler             ########## 
########################################################################################
set (BUILD_SCRIPT_PATH           ${CMAKE_CURRENT_SOURCE_DIR}/config/scripts)
set (BUILD_CMAKE_PATH            ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake)
set (BUILD_TOOLS_PATH            ${CMAKE_CURRENT_SOURCE_DIR}/config/tools)

##set (SRC_SDK_PATH        ${CMAKE_CURRENT_SOURCE_DIR}/build/arch_sdk)
set (SRC_LIBS_PATH       ${CMAKE_CURRENT_SOURCE_DIR}/src/libs)
set (SRC_INCLUDE_PATH    ${CMAKE_CURRENT_SOURCE_DIR}/src/includes)

set (DISTRIBUTE_PATH       ${CMAKE_CURRENT_SOURCE_DIR}/dist)



########################################################################################
#########                  设置项目安装路径                                   ########## 
########################################################################################
set (CMAKE_INSTALL_PREFIX   ${DISTRIBUTE_PATH}/debug)



########################################################################################
#########                    包含 CMAKE 脚本                                  ########## 
########################################################################################
include (${BUILD_CMAKE_PATH}/common_PlatformOptions.cmake)
include (${BUILD_CMAKE_PATH}/PlatformPrepare_ArchSDK_${TARGET_SYSTEM}.cmake)
include (${BUILD_CMAKE_PATH}/common_ModulesList.cmake)
include (${BUILD_CMAKE_PATH}/common_CompileOptions.cmake)



########################################################################################
#########                 添加项目代码子目录                                  ########## 
########################################################################################
#link_directories ("This_is_test_link_directory")
#include (src/CMakeLists.txt)
add_subdirectory(src)



