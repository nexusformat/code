# Microsoft Developer Studio Generated NMAKE File, Based on hdf45_5.dsp
!IF "$(CFG)" == ""
CFG=hdf45_5 - Win32 Release
!MESSAGE No configuration specified. Defaulting to hdf45_5 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "hdf45_5 - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdf45_5.mak" CFG="hdf45_5 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdf45_5 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\hdf45_5.exe"


CLEAN :
	-@erase "$(INTDIR)\napi45.obj"
	-@erase "$(INTDIR)\napi45_5test.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\hdf45_5.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "HDF5" /D "_HDF5USEDLL_" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\hdf45_5.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hdf45_5.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=hdf5dll.lib zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\hdf45_5.pdb" /machine:I386 /out:"$(OUTDIR)\hdf45_5.exe" 
LINK32_OBJS= \
	"$(INTDIR)\napi45.obj" \
	"$(INTDIR)\napi45_5test.obj"

"$(OUTDIR)\hdf45_5.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("hdf45_5.dep")
!INCLUDE "hdf45_5.dep"
!ELSE 
!MESSAGE Warning: cannot find "hdf45_5.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "hdf45_5 - Win32 Release"
SOURCE=..\napi45.c

"$(INTDIR)\napi45.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\napi45_5test.c

"$(INTDIR)\napi45_5test.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

