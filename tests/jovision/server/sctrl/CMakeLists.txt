

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/arch/${CHIP}/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/libs/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/porting/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/module/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/share/smtp/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/share/network/include)

PROJECT(SCTRL)

SET(SCTRLSRC 
					main.c
					mfirmup.c
					MRemoteCfg.c
					mtransmit.c
					rcmodule.c
					SYSFuncs.c        )

LINK_DIRECTORIES(
					${IPCAM_SOURCE_DIR}/libs
					${IPCAM_SOURCE_DIR}/arch/${CHIP}/libs
					${IPCAM_BINARY_DIR}/porting
					${IPCAM_BINARY_DIR}/module
					)


ADD_EXECUTABLE(sctrl ${SCTRLSRC})
 
TARGET_LINK_LIBRARIES(sctrl 
					module 
					porting 
					${IPCAM_BINARY_DIR}/share/smtp/libSendMail.a
					${IPCAM_BINARY_DIR}/share/network/libMNetwork.a
					jvserver
					expat 
					memmgr 
					sharedbuffmgr 
					xmlmgr 
					xmlwrapper
					rt
					pthread
					stdc++
					)

