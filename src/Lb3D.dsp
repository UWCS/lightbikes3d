# Microsoft Developer Studio Project File - Name="Lb3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Lb3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Lb3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Lb3D.mak" CFG="Lb3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Lb3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Lb3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Lb3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glu32.lib dinput.lib ws2_32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Lb3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glu32.lib dinput.lib ws2_32.lib dxguid.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\bin\Lb3d.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "Lb3D - Win32 Release"
# Name "Lb3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\LbArenaImp.cpp
# End Source File
# Begin Source File

SOURCE=.\game\LbGame.cpp
# End Source File
# Begin Source File

SOURCE=.\game\LbGameBlocks.cpp
# End Source File
# Begin Source File

SOURCE=.\game\LbLevelImp.cpp
# End Source File
# End Group
# Begin Group "graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\graphics\LbGraphics.cpp
# End Source File
# Begin Source File

SOURCE=.\graphics\LbGraphicsBike.cpp
# End Source File
# Begin Source File

SOURCE=.\graphics\LbGraphicsLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\graphics\LbGraphTexture.cpp
# End Source File
# End Group
# Begin Group "input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\input\LbInput.cpp
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sound\LbSound.cpp
# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\net\LbNet.cpp
# End Source File
# End Group
# Begin Group "oslayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\oslayer\LbOSWin32Imp.cpp
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\misc\LbMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\LbMatrix4.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\LbVector.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\LbVector4.cpp
# End Source File
# Begin Source File

SOURCE=.\misc\LbXmlParse.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\LbMain.cpp
# End Source File
# Begin Source File

SOURCE=.\oslayer\LbOSWin32InputImp.cpp
# End Source File
# Begin Source File

SOURCE=.\oslayer\LbOSWin32SoundImp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "game headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\game\LbGameImp.h
# End Source File
# End Group
# Begin Group "graphics headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\graphics\LbGraphicsImp.h
# End Source File
# End Group
# Begin Group "input headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\input\LbInputImp.h
# End Source File
# End Group
# Begin Group "sound headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sound\LbSoundImp.h
# End Source File
# End Group
# Begin Group "net headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\net\LbNetImp.h
# End Source File
# End Group
# Begin Group "oslayer headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\oslayer\LbOSWin32Imp.h
# End Source File
# Begin Source File

SOURCE=..\include\winamp_in.h
# End Source File
# Begin Source File

SOURCE=..\include\winamp_out.h
# End Source File
# End Group
# Begin Group "misc headers"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\include\LbGame.h
# End Source File
# Begin Source File

SOURCE=..\include\LbGraphics.h
# End Source File
# Begin Source File

SOURCE=..\include\LbInput.h
# End Source File
# Begin Source File

SOURCE=..\include\LbMath.h
# End Source File
# Begin Source File

SOURCE=..\include\LbMiscTypes.h
# End Source File
# Begin Source File

SOURCE=..\include\LbNet.h
# End Source File
# Begin Source File

SOURCE=..\include\LbOSLayer.h
# End Source File
# Begin Source File

SOURCE=..\include\LbPublic.h
# End Source File
# Begin Source File

SOURCE=..\include\LbSound.h
# End Source File
# Begin Source File

SOURCE=..\include\LbStandard.h
# End Source File
# Begin Source File

SOURCE=..\include\LbXmlParse.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
