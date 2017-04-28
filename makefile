!include "../global.mak"

ALL : "$(OUTDIR)\MQ2Linkdb.dll"

CLEAN :
	-@erase "$(INTDIR)\MQ2Linkdb.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\MQ2Linkdb.dll"
	-@erase "$(OUTDIR)\MQ2Linkdb.exp"
	-@erase "$(OUTDIR)\MQ2Linkdb.lib"
	-@erase "$(OUTDIR)\MQ2Linkdb.pdb"


LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib $(DETLIB) ..\Release\MQ2Main.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\MQ2Linkdb.pdb" /debug /machine:I386 /out:"$(OUTDIR)\MQ2Linkdb.dll" /implib:"$(OUTDIR)\MQ2Linkdb.lib" /OPT:NOICF /OPT:NOREF 
LINK32_OBJS= \
	"$(INTDIR)\MQ2Linkdb.obj" \
	"$(OUTDIR)\MQ2Main.lib"

"$(OUTDIR)\MQ2Linkdb.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) $(LINK32_FLAGS) $(LINK32_OBJS)


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MQ2Linkdb.dep")
!INCLUDE "MQ2Linkdb.dep"
!ELSE 
!MESSAGE Warning: cannot find "MQ2Linkdb.dep"
!ENDIF 
!ENDIF 


SOURCE=.\MQ2Linkdb.cpp

"$(INTDIR)\MQ2Linkdb.obj" : $(SOURCE) "$(INTDIR)"

