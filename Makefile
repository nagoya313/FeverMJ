CC = g++
STRIP = strip
CFLAGS = -DDX_GCC_COMPILE -DDX_LIB_NOT_DEFAULTPATH -DDX_NON_INLINE_ASM #-D_GLIBCXX_DEBUG
CFLAGS += -Wall -Werror -O3 -DNDEBUG -DNDEBUGLOG
CFLAGS += -ID:/lib/boost_1_50_0
CFLAGS += -I"C:/Users\nagoya313/Documents/DxLib_GCC/プロジェクトに追加すべきファイル_GCC(MinGW)用"
CXXFLAGS = -std=c++11
LFLAGS = -L"C:/Users\nagoya313/Documents/DxLib_GCC/プロジェクトに追加すべきファイル_GCC(MinGW)用"
LFLAGS += -lDxLib -lDxUseCLib -lDxDrawFunc -ljpeg -lpng -lzlib -ltheora_static
LFLAGS += -lvorbis_static -lvorbisfile_static -logg_static -lbulletdynamics -lbulletcollision -lbulletmath
LFLAGS += -mwindows -static-libgcc -static-libstdc++
OBJS = main.o
TARGET = FeverMJ.exe
#$(STRIP) --strip-all $(TARGET)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)
	$(STRIP) --strip-all $(TARGET)
		
.cc.o:
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

main.o: model/*.hpp view/*.hpp controller/*.hpp

clean:
	rm -f $(OBJS) $(TARGET)
