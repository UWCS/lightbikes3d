MISC_H_FILES=\
	../include/LbStandard.h \
	../include/LbPublic.h \
	../include/LbGraphics.h \
	../include/LbInput.h \
	../include/LbSound.h \
	../include/LbNet.h \
	../include/LbGame.h \
	../include/LbMath.h \
	../include/LbOSLayer.h \
	../include/LbMiscTypes.h

MISC_C_FILES=\
	misc/LbMatrix.cpp \
	misc/LbMatrix4.cpp \
	misc/LbVector.cpp \
	misc/LbVector4.cpp \
	LbMain.cpp

GAME_H_FILES=\
	game/LbGameImp.h
GAME_C_FILES=\
	game/LbGame.cpp


GRAPHICS_H_FILES=\
	graphics/LbGraphicsImp.h
GRAPHICS_C_FILES=\
	graphics/LbGraphics.cpp

INPUT_H_FILES=\
	input/LbInputImp.h
INPUT_C_FILES=\
	input/LbInput.cpp

NET_H_FILES=\
	net/LbNetImp.h
NET_C_FILES=\
	net/LbNet.cpp

SOUND_H_FILES=\
	sound/LbSoundImp.h
SOUND_C_FILES=\
	sound/LbSound.cpp

OSLAYER_H_FILES=\
	oslayer/LbOSLinuxImp.h
OSLAYER_C_FILES=\
	oslayer/LbOSLinuxImp.cpp \
        oslayer/LbOSLinuxInputImp.cpp \
        oslayer/LbOSLinuxSoundImp.cpp

ALL_C_FILES=\
	$(MISC_C_FILES) \
	$(GAME_C_FILES) \
	$(GRAPHICS_C_FILES) \
	$(INPUT_C_FILES) \
	$(NET_C_FILES) \
	$(SOUND_C_FILES) \
	$(OSLAYER_C_FILES)

ALL_H_FILES=\
	$(MISC_H_FILES) \
	$(GAME_H_FILES) \
	$(GRAPHICS_H_FILES) \
	$(INPUT_H_FILES) \
	$(NET_H_FILES) \
	$(SOUND_H_FILES) \
	$(OSLAYER_H_FILES)


INCLUDE_DIRS=../include /usr/local/include /usr/include
COMP_DEFINES=-DLINUX -DHAVE_OPENGL  -o Lb3D -O2
WARN_DISABLES=-w-ccc -w-aus -w-par -w-rch -w-csu

lb3d: $(ALL_C_FILES) $(ALL_H_FILES)
	g++     $(COMP_DEFINES)     \
              -I$(INCLUDE_DIRS)     \
                $(COMP_DEFINES)     \
              -I$(INCLUDE_DIRS)     \
	        $(MISC_C_FILES)     \
	        $(GAME_C_FILES)     \
	        $(GRAPHICS_C_FILES) \
	        $(INPUT_C_FILES)    \
	        $(NET_C_FILES)      \
	        $(SOUND_C_FILES)    \
	        $(OSLAYER_C_FILES)  
	mv Lb3D ..\bin

#clean:
#   @[ -e  *.obj ] &&  del *.obj >nul
#   @if exist *.tds del *.tds >nul
#   @if exist stderr.txt del stderr.txt >nul
#   @if exist stdout.txt del stdout.txt >nul	
