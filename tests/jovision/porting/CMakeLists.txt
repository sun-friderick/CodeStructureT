

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/arch/${CHIP}/include)

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/libs/include)

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/porting/include)

PROJECT(PORTINGLIB)

SET(PORTINGSRC 
				src/jv_common.c
				src/jv_mdetect.c
				src/jv_osddrv.c
				src/jv_snapshot.c
				src/jv_stream.c
				src/nxp_vencmsg.c
				src/nxp_xmlsettings.c
				src/jv_cmd.c
        )

#test add macro definition
ADD_DEFINITIONS(-DTEST1 -DTEST2)

ADD_LIBRARY(porting STATIC ${PORTINGSRC})
 
ADD_SUBDIRECTORY(test)

