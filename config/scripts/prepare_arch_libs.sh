#!/bin/sh

echo "----------------------------------------------------------------------------"
echo "                             prepare arch libs                              "
echo "----------------------------------------------------------------------------"
export LANG="en_US.utf8"



########################################################################################
###########        prepare ffmpeg && SDL2 libs && includes path           ##############
########################################################################################
LIBRARY_OUTPUT_PATH=$1
SRC_LIBS_PATH=$2
SRC_INCLUDE_PATH=$3
FFMPEG_LIBS_VERSION=$4
SDL2_LIBS_VERSION=$5

FFMPEG_LIBS_PATH=${SRC_LIBS_PATH}/ffmpeg-${FFMPEG_LIBS_VERSION}-libs.tar.gz
FFMPEG_INCLUDE_PATH=${SRC_INCLUDE_PATH}/ffmpeg-${FFMPEG_LIBS_VERSION}-includes.tar.gz

SDL2_LIBS_PATH=${SRC_LIBS_PATH}/SDL2-${SDL2_LIBS_VERSION}-libs.tar.gz
SDL2_INCLUDE_PATH=${SRC_INCLUDE_PATH}/SDL2-${SDL2_LIBS_VERSION}-includes.tar.gz

echo ${SDL2_LIBS_PATH}
echo ${SDL2_INCLUDE_PATH}



########################################################################################
###########        unzip ffmpeg && SDL2 libs && includes files           ##############
########################################################################################
if [ -d ${LIBRARY_OUTPUT_PATH} ]; then 
    rm -rf ${LIBRARY_OUTPUT_PATH}/ffmpeg  ${LIBRARY_OUTPUT_PATH}/SDL2 
    mkdir -p ${LIBRARY_OUTPUT_PATH}/ffmpeg  ${LIBRARY_OUTPUT_PATH}/SDL2 
fi
if [ -d ${SRC_INCLUDE_PATH} ]; then 
    rm -rf ${SRC_INCLUDE_PATH}/ffmpeg  ${SRC_INCLUDE_PATH}/SDL2 
    mkdir -p ${SRC_INCLUDE_PATH}/ffmpeg  ${SRC_INCLUDE_PATH}/SDL2 
fi


###################      unzip ffmpeg libs && includes files     ######################
if [ -f ${FFMPEG_LIBS_PATH}  ]; then 
    tar -zxvf ${FFMPEG_LIBS_PATH}  -C ${LIBRARY_OUTPUT_PATH}/ffmpeg 
fi
if [ -f ${FFMPEG_INCLUDE_PATH} ]; then 
    tar -zxvf ${FFMPEG_INCLUDE_PATH}  -C ${SRC_INCLUDE_PATH}/ffmpeg
fi
echo "${FFMPEG_LIBS_PATH}  ${FFMPEG_INCLUDE_PATH} done.............. "


###################       unzip SDL2 libs && includes files      ######################
if [ -f ${SDL2_LIBS_PATH} ]; then 
    tar -zxf ${SDL2_LIBS_PATH}  -C ${LIBRARY_OUTPUT_PATH}/SDL2 
fi
if [ -f ${SDL2_INCLUDE_PATH} ]; then 
    tar -zxf ${SDL2_INCLUDE_PATH}  -C ${SRC_INCLUDE_PATH}/SDL2
fi
echo "${SDL2_LIBS_PATH}  ${SDL2_INCLUDE_PATH} done.............. "


echo "-- written to "

