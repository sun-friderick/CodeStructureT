

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/arch/${CHIP}/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/libs/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/porting/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/module/include)

PROJECT(MODULELIB)

SET(MODULESRC 
				src/maccount.c
				src/malarmout.c
				src/mdetect.c
				src/mlog.c
				src/mosd.c
				src/mprivacy.c
				src/mrecord.c
				src/mstorage.c
				src/mstream.c
				src/mtimer.c
        )

ADD_LIBRARY(module STATIC ${MODULESRC})
 
 
