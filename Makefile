CC = g++
STRIP = strip
CFLAGS = -DDX_GCC_COMPILE -DDX_LIB_NOT_DEFAULTPATH -DDX_NON_INLINE_ASM
CFLAGS += -DBOOST_SPIRIT_USE_PHOENIX_V3=1
#CFLAGS += -DNDEBUGLOG
#CFLAGS += -DNDEBUG
#CFLAGS += -D_GLIBCXX_DEBUG
CFLAGS += -Wall #-Werror
CFLAGS += -O3 
CFLAGS += -ID:/lib/boost_1_51_0
CFLAGS += -I"C:/Users\nagoya313/Documents/DxLib_GCC/プロジェクトに追加すべきファイル_GCC(MinGW)用"
CXXFLAGS = -std=c++11
LFLAGS = -L"C:/Users\nagoya313/Documents/DxLib_GCC/プロジェクトに追加すべきファイル_GCC(MinGW)用"
LFLAGS += -lDxLib -lDxUseCLib -lDxDrawFunc -ljpeg -lpng -lzlib -ltheora_static
LFLAGS += -lvorbis_static -lvorbisfile_static -logg_static -lbulletdynamics -lbulletcollision -lbulletmath
LFLAGS += -mwindows -static-libgcc -static-libstdc++
OBJS = main.o sequence.o 
#OBJS = open_reach_test.o
TARGET = FeverMJ.exe
VPATH = controller

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)
	$(STRIP) --strip-all $(TARGET)
		
.cc.o:
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

main.o: model/*.hpp view/*.hpp controller/*.hpp utility/*.hpp
sequence.o: model/*.hpp view/*.hpp controller/*.hpp utility/*.hpp
#open_reach_test.o: model/*.hpp view/*.hpp controller/*.hpp utility/*.hpp

clean:
	rm -f $(OBJS) $(TARGET)
