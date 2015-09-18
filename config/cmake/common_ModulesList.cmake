########################################################################################
######################             配置包含的库文件                 ####################
########################################################################################
set (Module_ffmpeg_Version      "0.8.15"    CACHE STRING "ffmpeg version .")
set (Module_SDL2_Version        "2.0.3"     CACHE STRING "SDL2 version .")




########################################################################################
####################          一般情况下所 必须 包含的模块           ###################
########################################################################################
option  (COMPONENT_player                      "Enable player module."              ON)
option  (COMPONENT_widget                      "Enable widget module."              ON)
option  (COMPONENT_utils                       "Enable utils module."               ON)
option  (COMPONENT_log                         "Enable log module."                 ON)
option  (COMPONENT_functions_statistics        "ENABLE functions_statistics "       ON)
option  (COMPONENT_monitor                     "Enable monitor"                     ON)




########################################################################################
##################               配置所包含的功能模块                 ##################
########################################################################################
option  (MODULE_base64                      "ENABLE base64"                     OFF)
option  (MODULE_jsoncpp                     "ENABLE json"                       ON)
option  (MODULE_ini-parser                  "Enable ini-parser"                 ON)
option  (MODULE_xml                         "Enable xml"                        ON)


########################################################################################
##################               功能模块 测试标志位                  ##################
########################################################################################
option  (TEST_MODULE_FLAG                   "Enable module test"                ON)





