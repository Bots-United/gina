# Microsoft Developer Studio Project File - Name="bot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=bot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "bot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "bot.mak" CFG="bot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "bot - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "bot - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BOT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GR /GX /O2 /I ".\hlsdk" /I ".\metamod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BOT_EXPORTS" /D "VALVE_DLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386

!ELSEIF  "$(CFG)" == "bot - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BOT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I ".\hlsdk" /I ".\metamod" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BOT_EXPORTS" /D "VALVE_DLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "bot - Win32 Release"
# Name "bot - Win32 Debug"
# Begin Group "interface"

# PROP Default_Filter ".cpp"
# Begin Group "hlsdk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlsdk\archtypes.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\cbase.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\const.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\eiface.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\enginecallback.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\entity_state.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\extdll.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\progdefs.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\studio.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\usercmd.h
# End Source File
# Begin Source File

SOURCE=.\hlsdk\util.h
# End Source File
# End Group
# Begin Group "metamod"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\metamod\dllapi.h
# End Source File
# Begin Source File

SOURCE=.\metamod\engine_api.h
# End Source File
# Begin Source File

SOURCE=.\metamod\meta_api.h
# End Source File
# Begin Source File

SOURCE=.\metamod\mutil.h
# End Source File
# Begin Source File

SOURCE=.\metamod\plinfo.h
# End Source File
# Begin Source File

SOURCE=.\metamod\sdk_util.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\dllapi.cpp
# End Source File
# Begin Source File

SOURCE=.\engine_api.cpp
# End Source File
# Begin Source File

SOURCE=.\engine_util.cpp
# End Source File
# Begin Source File

SOURCE=.\h_export.cpp
# End Source File
# Begin Source File

SOURCE=.\linkgame.cpp
# End Source File
# Begin Source File

SOURCE=.\meta_api.cpp
# End Source File
# Begin Source File

SOURCE=.\namefunc.cpp
# End Source File
# Begin Source File

SOURCE=.\sdk_util.cpp
# End Source File
# End Group
# Begin Group "bot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bot.cpp
# End Source File
# Begin Source File

SOURCE=.\bot.h
# End Source File
# Begin Source File

SOURCE=.\bot_ai.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_ainet.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_chat.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_chat.h
# End Source File
# Begin Source File

SOURCE=.\bot_combat.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_globals.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_globals.h
# End Source File
# Begin Source File

SOURCE=.\bot_hlengine.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_manager.h
# End Source File
# Begin Source File

SOURCE=.\bot_navigate.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_navigate.h
# End Source File
# Begin Source File

SOURCE=.\world.cpp
# End Source File
# Begin Source File

SOURCE=.\world.h
# End Source File
# End Group
# Begin Group "mods"

# PROP Default_Filter ""
# Begin Group "cstrike"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cstrike\bot_cs.cpp
# End Source File
# Begin Source File

SOURCE=.\cstrike\bot_cs.h
# End Source File
# Begin Source File

SOURCE=.\cstrike\botai_cs.cpp
# End Source File
# Begin Source File

SOURCE=.\cstrike\botai_cs.h
# End Source File
# Begin Source File

SOURCE=.\cstrike\buy.cpp
# End Source File
# Begin Source File

SOURCE=.\cstrike\buy.h
# End Source File
# Begin Source File

SOURCE=.\cstrike\mod_specific.h
# End Source File
# Begin Source File

SOURCE=.\cstrike\server_cs.cpp
# End Source File
# Begin Source File

SOURCE=.\cstrike\server_cs.h
# End Source File
# Begin Source File

SOURCE=.\cstrike\util_cs.cpp
# End Source File
# Begin Source File

SOURCE=.\cstrike\util_cs.h
# End Source File
# End Group
# Begin Group "gearbox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gearbox\bot_gearbox.cpp
# End Source File
# Begin Source File

SOURCE=.\gearbox\bot_gearbox.h
# End Source File
# Begin Source File

SOURCE=.\gearbox\bot_gearbox_ctf.cpp
# End Source File
# Begin Source File

SOURCE=.\gearbox\bot_gearbox_ctf.h
# End Source File
# Begin Source File

SOURCE=.\gearbox\mod_specific.h
# End Source File
# Begin Source File

SOURCE=.\gearbox\server_gearbox.cpp
# End Source File
# Begin Source File

SOURCE=.\gearbox\server_gearbox.h
# End Source File
# Begin Source File

SOURCE=.\gearbox\server_gearbox_ctf.cpp
# End Source File
# Begin Source File

SOURCE=.\gearbox\server_gearbox_ctf.h
# End Source File
# End Group
# Begin Group "ricochet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ricochet\bot_ricochet.cpp
# End Source File
# Begin Source File

SOURCE=.\ricochet\bot_ricochet.h
# End Source File
# Begin Source File

SOURCE=.\ricochet\mod_specific.h
# End Source File
# Begin Source File

SOURCE=.\ricochet\server_ricochet.cpp
# End Source File
# Begin Source File

SOURCE=.\ricochet\server_ricochet.h
# End Source File
# End Group
# Begin Group "valve"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\valve\bot_valve.cpp
# End Source File
# Begin Source File

SOURCE=.\valve\bot_valve.h
# End Source File
# Begin Source File

SOURCE=.\valve\mod_specific.h
# End Source File
# Begin Source File

SOURCE=.\valve\server_valve.cpp
# End Source File
# Begin Source File

SOURCE=.\valve\server_valve.h
# End Source File
# End Group
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ini.cpp
# End Source File
# Begin Source File

SOURCE=.\ini.h
# End Source File
# Begin Source File

SOURCE=.\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.h
# End Source File
# Begin Source File

SOURCE=.\general.cpp
# End Source File
# Begin Source File

SOURCE=.\general.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\server.cpp
# End Source File
# Begin Source File

SOURCE=.\server.h
# End Source File
# End Target
# End Project
