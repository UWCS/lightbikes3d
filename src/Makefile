MISC_H_FILES=\
	..\include\LbStandard.h \
	..\include\LbPublic.h \
	..\include\LbGraphics.h \
	..\include\LbInput.h \
	..\include\LbSound.h \
	..\include\LbNet.h \
	..\include\LbGame.h \
	..\include\LbMath.h \
	..\include\LbOSLayer.h \
	..\include\LbMiscTypes.h

MISC_C_FILES=\
	misc\LbMatrix.cpp \
	misc\LbMatrix4.cpp \
	misc\LbVector.cpp \
	misc\LbVector4.cpp \
	LbMain.cpp

GAME_H_FILES=\
	game\LbGameImp.h
GAME_C_FILES=\
	game\LbGame.cpp


GRAPHICS_H_FILES=\
	graphics\LbGraphicsImp.h
GRAPHICS_C_FILES=\
	graphics\LbGraphics.cpp

INPUT_H_FILES=\
	input\LbInputImp.h
INPUT_C_FILES=\
	input\LbInput.cpp

NET_H_FILES=\
	net\LbNetImp.h
NET_C_FILES=\
	net\LbNet.cpp

SOUND_H_FILES=\
	sound\LbSoundImp.h
SOUND_C_FILES=\
	sound\LbSound.cpp

OSLAYER_H_FILES=\
	oslayer\LbOSWin32Imp.h
OSLAYER_C_FILES=\
	oslayer\LbOSWin32Imp.cpp \
	oslayer\LbOSWin32InputImp.cpp

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

LIB_FILES=\
    psdk\dinput.lib \
    dxextra.lib

INCLUDE_DIRS=..\include
COMP_DEFINES=-DWIN32 -DHAVE_OPENGL  -eLb3D.exe -5 -O2
WARN_DISABLES=-w-ccc -w-aus -w-par -w-rch -w-csu

lb3d.exe: $(ALL_C_FILES) $(ALL_H_FILES)
        echo -tW $(COMP_DEFINES) > compile.rsp
        echo $(WARN_DISABLES)  >> compile.rsp
        echo -I$(INCLUDE_DIRS) >> compile.rsp
        echo $(MISC_C_FILES) >> compile.rsp
        echo $(GAME_C_FILES) >> compile.rsp
        echo $(GRAPHICS_C_FILES) >> compile.rsp
        echo $(INPUT_C_FILES) >> compile.rsp
        echo $(NET_C_FILES) >> compile.rsp
        echo $(SOUND_C_FILES) >> compile.rsp
        echo $(OSLAYER_C_FILES) >> compile.rsp
        echo $(LIB_FILES) >> compile.rsp
        bcc32 @compile.rsp
	move Lb3D.exe ..\bin

clean:
   -@if exist *.obj del *.obj >nul
   -@if exist *.tds del *.tds >nul
   -@if exist stderr.txt del stderr.txt >nul
   -@if exist stdout.txt del stdout.txt >nul	
