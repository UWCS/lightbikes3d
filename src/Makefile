# ------------------------------------
# Makefile for Lb3D	
#
# Modified by Chris Skepper 14/11/2001
# ------------------------------------

# ---------------------------------
# Turn on the auto dependency stuff
# ---------------------------------

.AUTODEPEND

# ---------------------------------------
# Varibles to hold the names of the files
# ---------------------------------------

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
    ..\include\LbMiscTypes.h \
    ..\include\LbXmlParse.h

MISC_C_FILES=\
    misc\LbMatrix.cpp \
    misc\LbMatrix4.cpp \
    misc\LbVector.cpp \
    misc\LbVector4.cpp \
    misc\LbXmlParse.cpp \
    LbMain.cpp

GAME_H_FILES=\
    game\LbGameImp.h
GAME_C_FILES=\
    game\LbGameBlocks.cpp \
    game\LbLevelImp.cpp \
    game\LbArenaImp.cpp \
    game\LbPlayer.cpp \
    game\LbGame.cpp 

GRAPHICS_H_FILES=\
    graphics\LbGraphicsImp.h
GRAPHICS_C_FILES=\
    graphics\LbGraphics.cpp \
    graphics\LbGraphicsBike.cpp \
    graphics\LbGraphicsLevel.cpp \
    graphics\LbGraphTexture.cpp

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
    oslayer\LbOSWin32InputImp.cpp \
    oslayer\LbOSWin32SoundImp.cpp

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

# -------------
# Library files
# -------------

LIB_FILES=\
    psdk\dinput.lib \
    dxextra.lib

# -------------------------
# Intermediate object files
# -------------------------

OSLAYER_OBJ_FILES=\
    ..\bin\temp\LbOSWin32Imp.obj \
    ..\bin\temp\LbOSWin32InputImp.obj \
    ..\bin\temp\LbOSWin32SoundImp.obj
 
MISC_OBJ_FILES=\
    ..\bin\temp\LbMatrix.obj \
    ..\bin\temp\LbMatrix4.obj \
    ..\bin\temp\LbVector.obj \
    ..\bin\temp\LbVector4.obj \
    ..\bin\temp\LbXmlParse.obj \
    ..\bin\temp\LbMain.obj

GAME_OBJ_FILES=\
    ..\bin\temp\LbGameBlocks.obj \
    ..\bin\temp\LbLevelImp.obj \
    ..\bin\temp\LbArenaImp.obj \
    ..\bin\temp\LbPlayer.obj \
    ..\bin\temp\LbGame.obj

GRAPHICS_OBJ_FILES=\
    ..\bin\temp\LbGraphics.obj \
    ..\bin\temp\LbGraphicsBike.obj \
    ..\bin\temp\LbGraphicsLevel.obj \
    ..\bin\temp\LbGraphTexture.obj

INPUT_OBJ_FILES=\
    ..\bin\temp\LbInput.obj

NET_OBJ_FILES=\
    ..\bin\temp\LbNet.obj

SOUND_OBJ_FILES=\
    ..\bin\temp\LbSound.obj

ALL_OBJ_FILES=\
    $(OSLAYER_OBJ_FILES)\
    $(MISC_OBJ_FILES)\
    $(GAME_OBJ_FILES)\
    $(GRAPHICS_OBJ_FILES)\
    $(INPUT_OBJ_FILES)\
    $(NET_OBJ_FILES)\
    $(SOUND_OBJ_FILES) 

# --------------
# Compiler flags
# --------------

INCLUDE_DIRS=..\include
COMP_DEFINES=-DWIN32 -DHAVE_OPENGL -5 -O2
WARN_DISABLES=-w-ccc -w-aus -w-par -w-rch -w-csu

OBJC=\
    -tW $(COMP_DEFINES) -v -c\
    $(WARN_DISABLES) \
    -I$(INCLUDE_DIRS) \
    -n"..\bin\temp" \
    $(LIB_FILES) 

# -------------------
# Actually build Lb3d
# -------------------

lb3d.exe : $(ALL_OBJ_FILES) 
    echo -eLb3D.exe > compile.rsp
    echo -tW $(COMP_DEFINES) >> compile.rsp
    echo -n"..\bin\temp" >> compile.rsp
    echo $(WARN_DISABLES)  >> compile.rsp
    echo -I$(INCLUDE_DIRS) >> compile.rsp    
    echo $(ALL_OBJ_FILES) >>compile.rsp
    echo $(LIB_FILES) >> compile.rsp          
    bcc32 @compile.rsp
    move ..\bin\temp\Lb3D.exe ..\bin\Lb3D.exe
    del compile.rsp

# -------------------
# Object dependencies
# -------------------

$(OSLAYER_OBJ_FILES) : $(OSLAYER_C_FILES) $(ALL_H_FILES)
    echo $(OBJC) $(OSLAYER_C_FILES) > compile.rsp
    bcc32 @compile.rsp
        
$(MISC_OBJ_FILES) : $(MISC_C_FILES) $(MISC_H_FILES)
    echo $(OBJC) $(MISC_C_FILES) > compile.rsp
    bcc32 @compile.rsp

$(GAME_OBJ_FILES) : $(GAME_C_FILES) $(GAME_H_FILES)
    echo $(OBJC) $(GAME_C_FILES) > compile.rsp
    bcc32 @compile.rsp
        
$(GRAPHICS_OBJ_FILES) : $(GRAPHICS_C_FILES) $(GRAPHICS_H_FILES)
    echo $(OBJC) $(GRAPHICS_C_FILES) > compile.rsp
    bcc32 @compile.rsp        

$(INPUT_OBJ_FILES) : $(INPUT_C_FILES) $(INPUT_H_FILES)
    echo $(OBJC) $(INPUT_C_FILES) > compile.rsp     
    bcc32 @compile.rsp        

$(NET_OBJ_FILES) : $(NET_C_FILES) $(NET_H_FILES)
    echo $(OBJC) $(NET_C_FILES) > compile.rsp
    bcc32 @compile.rsp    

$(SOUND_OBJ_FILES) : $(SOUND_C_FILES) $(SOUND_H_FILES)
    echo $(OBJC) $(SOUND_C_FILES) > compile.rsp
    bcc32 @compile.rsp    
   
# ----------------------------------------
# Don't think this is working or necessary
# ----------------------------------------

clean:
   -@if exist *.obj del *.obj >nul
   -@if exist *.tds del *.tds >nul
   -@if exist stderr.txt del stderr.txt >nul
   -@if exist stdout.txt del stdout.txt >nul