PRJ_ROOT=/home/hl/linphone
CC=gcc
CFLAGS= -g  -I./  -I./ffmpeg -DSDL_MUTEX
CPPFLAGS= 
LDFLAGS= -L./ -lavcodec -lavformat -lswscale -lavutil -lpthread `sdl-config --cflags --libs`
 
 
###########源文件，每增加一个目标，依样增加下面一段#############
#源文件列表1
i:=1
SOURCES_CPP_$(i):= 
SOURCES_C_$(i):=    \
    ff_queue.c \
    ff_play.c  \
    myplayer.c  
TARGET_$(i):=myplayer
OBJS_CPP_$(i):=$(patsubst %.cpp,%.o, $(SOURCES_CPP_$(i)))
OBJS_C_$(i):= $(patsubst %.c,%.o, $(SOURCES_C_$(i)))
OBJS_$(i):= $(OBJS_CPP_$(i)) $(OBJS_C_$(i))
 
 
#源文件列表2 
i:=2
SOURCES_CPP_$(i):= 
SOURCES_C_$(i):= 
TARGET_$(i):=
OBJS_CPP_$(i):=$(patsubst %.cpp,%.o, $(SOURCES_CPP_$(i)))
OBJS_C_$(i):= $(patsubst %.c,%.o, $(SOURCES_C_$(i)))
OBJS_$(i):= $(OBJS_CPP_$(i)) $(OBJS_C_$(i))
 
 
#######目标和清除 每增加一个目标，依样增加一个target##########
all: $(TARGET_1) $(TARGET_2)
    @echo "outputfile : $(TARGET_1) $(TARGET_2)"
clean:
    rm -f *.o  *.d  $(TARGET_1) $(TARGET_2)
 
 
##########目标， 每增加一个目标，依样增加下面一段#############
#目标1 
$(TARGET_1):$(OBJS_1)
    $(CC) $(OBJS_1) $(LDFLAGS) -o $(TARGET_1)
 
#目标2
$(TARGET_2):$(OBJS_2)
    $(CC) $(OBJS_2) $(LDFLAGS) -o $(TARGET_2)
 
###########包含 每增加一个目标，依样增加下面一行#############
sinclude $(OBJS_1:.o=.d)
sinclude $(OBJS_2:.o=.d)
 
 
#下面这边都是获取依赖关系 ，属于约定俗成的写法
%.d: %.c    
    @rm -f $@;
    @$(CC) -MM $< > $@.1111; \
    sed 's,/($*/)/.o[ :]*,/1.o $@ : ,g' < $@.1111 > $@;  \
    rm -f $@.1111
 
%.d: %.cpp
    @rm -f $@;
    @$(CC) -MM $< > $@.1111; \
    sed 's,/($*/)/.o[ :]*,/1.o $@ : ,g' < $@.1111 > $@;  \
    rm -f $@.1111


