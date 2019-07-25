# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=SexShop - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to SexShop - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SexShop - Win32 Release" && "$(CFG)" !=\
 "SexShop - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "SexShop.mak" CFG="SexShop - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SexShop - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SexShop - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "SexShop - Win32 Debug"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SexShop - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\SexShop.exe"

CLEAN : 
	-@erase "$(INTDIR)\Cabina.obj"
	-@erase "$(INTDIR)\Canal.obj"
	-@erase "$(INTDIR)\Comm.obj"
	-@erase "$(INTDIR)\ConfTimeCoin.obj"
	-@erase "$(INTDIR)\Dia.obj"
	-@erase "$(INTDIR)\DialogClrCab.obj"
	-@erase "$(INTDIR)\DialogClrChan.obj"
	-@erase "$(INTDIR)\Historial.obj"
	-@erase "$(INTDIR)\Imprimiendo.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Monitor.obj"
	-@erase "$(INTDIR)\NewPassDlg.obj"
	-@erase "$(INTDIR)\Password.obj"
	-@erase "$(INTDIR)\Present.obj"
	-@erase "$(INTDIR)\Report.obj"
	-@erase "$(INTDIR)\SexShop.obj"
	-@erase "$(INTDIR)\SexShop.pch"
	-@erase "$(INTDIR)\SexShop.res"
	-@erase "$(INTDIR)\SexShopDoc.obj"
	-@erase "$(INTDIR)\SexShopView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysStatus.obj"
	-@erase "$(OUTDIR)\SexShop.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SexShop.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0xc0a /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0a /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0xc0a /fo"$(INTDIR)/SexShop.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SexShop.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/SexShop.pdb" /machine:I386 /out:"$(OUTDIR)/SexShop.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Cabina.obj" \
	"$(INTDIR)\Canal.obj" \
	"$(INTDIR)\Comm.obj" \
	"$(INTDIR)\ConfTimeCoin.obj" \
	"$(INTDIR)\Dia.obj" \
	"$(INTDIR)\DialogClrCab.obj" \
	"$(INTDIR)\DialogClrChan.obj" \
	"$(INTDIR)\Historial.obj" \
	"$(INTDIR)\Imprimiendo.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Monitor.obj" \
	"$(INTDIR)\NewPassDlg.obj" \
	"$(INTDIR)\Password.obj" \
	"$(INTDIR)\Present.obj" \
	"$(INTDIR)\Report.obj" \
	"$(INTDIR)\SexShop.obj" \
	"$(INTDIR)\SexShop.res" \
	"$(INTDIR)\SexShopDoc.obj" \
	"$(INTDIR)\SexShopView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysStatus.obj"

"$(OUTDIR)\SexShop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SexShop - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\SexShop.exe"

CLEAN : 
	-@erase "$(INTDIR)\Cabina.obj"
	-@erase "$(INTDIR)\Canal.obj"
	-@erase "$(INTDIR)\Comm.obj"
	-@erase "$(INTDIR)\ConfTimeCoin.obj"
	-@erase "$(INTDIR)\Dia.obj"
	-@erase "$(INTDIR)\DialogClrCab.obj"
	-@erase "$(INTDIR)\DialogClrChan.obj"
	-@erase "$(INTDIR)\Historial.obj"
	-@erase "$(INTDIR)\Imprimiendo.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Monitor.obj"
	-@erase "$(INTDIR)\NewPassDlg.obj"
	-@erase "$(INTDIR)\Password.obj"
	-@erase "$(INTDIR)\Present.obj"
	-@erase "$(INTDIR)\Report.obj"
	-@erase "$(INTDIR)\SexShop.obj"
	-@erase "$(INTDIR)\SexShop.pch"
	-@erase "$(INTDIR)\SexShop.res"
	-@erase "$(INTDIR)\SexShopDoc.obj"
	-@erase "$(INTDIR)\SexShopView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysStatus.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SexShop.exe"
	-@erase "$(OUTDIR)\SexShop.ilk"
	-@erase "$(OUTDIR)\SexShop.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SexShop.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0xc0a /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc0a /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0xc0a /fo"$(INTDIR)/SexShop.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SexShop.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/SexShop.pdb" /debug /machine:I386 /out:"$(OUTDIR)/SexShop.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Cabina.obj" \
	"$(INTDIR)\Canal.obj" \
	"$(INTDIR)\Comm.obj" \
	"$(INTDIR)\ConfTimeCoin.obj" \
	"$(INTDIR)\Dia.obj" \
	"$(INTDIR)\DialogClrCab.obj" \
	"$(INTDIR)\DialogClrChan.obj" \
	"$(INTDIR)\Historial.obj" \
	"$(INTDIR)\Imprimiendo.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Monitor.obj" \
	"$(INTDIR)\NewPassDlg.obj" \
	"$(INTDIR)\Password.obj" \
	"$(INTDIR)\Present.obj" \
	"$(INTDIR)\Report.obj" \
	"$(INTDIR)\SexShop.obj" \
	"$(INTDIR)\SexShop.res" \
	"$(INTDIR)\SexShopDoc.obj" \
	"$(INTDIR)\SexShopView.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysStatus.obj"

"$(OUTDIR)\SexShop.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "SexShop - Win32 Release"
# Name "SexShop - Win32 Debug"

!IF  "$(CFG)" == "SexShop - Win32 Release"

!ELSEIF  "$(CFG)" == "SexShop - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "SexShop - Win32 Release"

!ELSEIF  "$(CFG)" == "SexShop - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SexShop.cpp

!IF  "$(CFG)" == "SexShop - Win32 Release"

DEP_CPP_SEXSH=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\Dia.h"\
	".\MainFrm.h"\
	".\Monitor.h"\
	".\NewPassDlg.h"\
	".\Password.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\SexShopView.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\SexShop.obj" : $(SOURCE) $(DEP_CPP_SEXSH) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


!ELSEIF  "$(CFG)" == "SexShop - Win32 Debug"

DEP_CPP_SEXSH=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\Dia.h"\
	".\MainFrm.h"\
	".\Monitor.h"\
	".\NewPassDlg.h"\
	".\Password.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\SexShopView.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\SexShop.obj" : $(SOURCE) $(DEP_CPP_SEXSH) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "SexShop - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SexShop.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SexShop.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "SexShop - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SexShop.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SexShop.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\Dia.h"\
	".\MainFrm.h"\
	".\Monitor.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\SexShopView.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SexShopDoc.cpp
DEP_CPP_SEXSHO=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\Dia.h"\
	".\DialogClrCab.h"\
	".\DialogClrChan.h"\
	".\MainFrm.h"\
	".\Monitor.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\SexShopView.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\SexShopDoc.obj" : $(SOURCE) $(DEP_CPP_SEXSHO) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SexShopView.cpp
DEP_CPP_SEXSHOP=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\ConfTimeCoin.h"\
	".\Dia.h"\
	".\Historial.h"\
	".\MainFrm.h"\
	".\Monitor.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\SexShopView.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\SexShopView.obj" : $(SOURCE) $(DEP_CPP_SEXSHOP) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SexShop.rc
DEP_RSC_SEXSHOP_=\
	".\res\bitmap1.bmp"\
	".\res\bitmap2.bmp"\
	".\res\bitmap3.bmp"\
	".\res\bitmap4.bmp"\
	".\res\bitmap5.bmp"\
	".\res\bitmap6.bmp"\
	".\res\bitmap7.bmp"\
	".\res\bmp00001.bmp"\
	".\res\luces.bmp"\
	".\res\SexShop.ico"\
	".\res\SexShop.rc2"\
	".\res\SexShopDoc.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\SexShop.res" : $(SOURCE) $(DEP_RSC_SEXSHOP_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cabina.cpp
DEP_CPP_CABIN=\
	".\Cabina.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Cabina.obj" : $(SOURCE) $(DEP_CPP_CABIN) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Canal.cpp
DEP_CPP_CANAL=\
	".\Canal.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Canal.obj" : $(SOURCE) $(DEP_CPP_CANAL) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Comm.cpp
DEP_CPP_COMM_=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\Dia.h"\
	".\MainFrm.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\Comm.obj" : $(SOURCE) $(DEP_CPP_COMM_) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ConfTimeCoin.cpp
DEP_CPP_CONFT=\
	".\ConfTimeCoin.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ConfTimeCoin.obj" : $(SOURCE) $(DEP_CPP_CONFT) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Dia.cpp
DEP_CPP_DIA_C=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Dia.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Dia.obj" : $(SOURCE) $(DEP_CPP_DIA_C) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DialogClrCab.cpp
DEP_CPP_DIALO=\
	".\DialogClrCab.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DialogClrCab.obj" : $(SOURCE) $(DEP_CPP_DIALO) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DialogClrChan.cpp
DEP_CPP_DIALOG=\
	".\DialogClrChan.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DialogClrChan.obj" : $(SOURCE) $(DEP_CPP_DIALOG) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Historial.cpp
DEP_CPP_HISTO=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Comm.h"\
	".\Dia.h"\
	".\Historial.h"\
	".\Imprimiendo.h"\
	".\MainFrm.h"\
	".\Present.h"\
	".\report.h"\
	".\SexShop.h"\
	".\SexShopDoc.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\Historial.obj" : $(SOURCE) $(DEP_CPP_HISTO) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Imprimiendo.cpp
DEP_CPP_IMPRI=\
	".\Imprimiendo.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Imprimiendo.obj" : $(SOURCE) $(DEP_CPP_IMPRI) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Monitor.cpp
DEP_CPP_MONIT=\
	".\Monitor.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Monitor.obj" : $(SOURCE) $(DEP_CPP_MONIT) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NewPassDlg.cpp
DEP_CPP_NEWPA=\
	".\NewPassDlg.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NewPassDlg.obj" : $(SOURCE) $(DEP_CPP_NEWPA) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Password.cpp
DEP_CPP_PASSW=\
	".\Password.h"\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\Password.obj" : $(SOURCE) $(DEP_CPP_PASSW) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Present.cpp
DEP_CPP_PRESE=\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Present.obj" : $(SOURCE) $(DEP_CPP_PRESE) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Report.cpp
DEP_CPP_REPOR=\
	".\Cabina.h"\
	".\Canal.h"\
	".\Dia.h"\
	".\Imprimiendo.h"\
	".\Present.h"\
	".\report.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Report.obj" : $(SOURCE) $(DEP_CPP_REPOR) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysStatus.cpp
DEP_CPP_SYSST=\
	".\Present.h"\
	".\SexShop.h"\
	".\StdAfx.h"\
	".\SysStatus.h"\
	

"$(INTDIR)\SysStatus.obj" : $(SOURCE) $(DEP_CPP_SYSST) "$(INTDIR)"\
 "$(INTDIR)\SexShop.pch"


# End Source File
# End Target
# End Project
################################################################################
