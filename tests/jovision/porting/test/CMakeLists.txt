

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/porting/include)

PROJECT(TEST)

SET(TESTSRC 
					testmain.c
					stream.c        )

LINK_DIRECTORIES(
					${IPCAM_SOURCE_DIR}/libs
					${IPCAM_SOURCE_DIR}/arch/${CHIP}/libs
					${IPCAM_BINARY_DIR}/porting
					${IPCAM_BINARY_DIR}/module
					)


ADD_EXECUTABLE(test ${TESTSRC})
 
TARGET_LINK_LIBRARIES(test 
					porting 
					expat 
					memmgr 
					sharedbuffmgr 
					xmlmgr 
					xmlwrapper
					rt
					pthread
					stdc++
					)

