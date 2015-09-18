

INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/arch/${CHIP}/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/libs/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/porting/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/module/include)
INCLUDE_DIRECTORIES(${IPCAM_SOURCE_DIR}/share/smtp/include)

PROJECT(SMTPLIB)

SET(SMTPSRC 
				src/Base64Coder.c
				src/SMTP.c
        )

ADD_LIBRARY(SendMail STATIC ${SMTPSRC})
 
#INSTALL(TARGETS SendMail DESTINATION libs)
#INSTALL(FILES include/Const.h 
#	include/smtp_interface.h
#	DESTINATION libs/include)


