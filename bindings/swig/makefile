#--------------------------------------------------------------------------
# This is a makefile for the NeXus SWIG interface functions. An example if
# you wish, really.
#
# Mark Koennecke, October 2002
#-----------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .i .c .o

include ../../make_general

#-------- where is your swig?
#SWIG=/data/lnslib/bin/swig
SWIG=/usr/bin/swig

#----------- change this for other script languages
SCRIPTFLAG=-tcl8 -ltclsh.i
SCRIPTFLAG2=-tcl8 
SCRIPTLIB=-L/data/lnslib/lib -ltcl
TCLOBJ=nxinter_wrap.o

#---------- these define for mzscheme
MZFLAG=-mzscheme
MZOBJ=nxscheme_wrap.o
MZINC=/data/koenneck/opt/plt
MZLIB=-L$(MZINC)/lib -lmzscheme -lmzgc $(MZINC)/lib/mzdyn.o

#--------- defines for guile
GGFLAG=-guile -lguilemain.i  -Linkage ltdlmod 
GGOBJ=nxguile_wrap.o 
GGINC=/data/lnslib
GGLIB=-L/data/lnslib/lib -lguile

OBJ=nxinterhelper.o nxdataset.o

CFLAGS=-g -c -I../../include
.c.o:
	$(CC) $(CFLAGS) $(NXFLAGS) $*.c


all:nxinter nxguile nxscheme

clean:
	- rm *.o
	- rm nxinter.so
	- rm nxinter
	- rm nxguile
	- rm nxscheme.so
	- rm nxinter_wrap.c
	- rm nxscheme_wrap.c  

nxinter: $(OBJ) $(TCLOBJ)
	$(CC) $(EXFLAG) -g -o nxinter  $(TCLOBJ) $(OBJ) $(SCRIPTLIB) \
	-L../../src -lNeXus $(NXLDFLAGS)
	$(CC) -shared -o nxinter.so  $(OBJ) $(TCLOBJ) $(SCRIPTLIB) \
	-L../../src -lNeXus $(NXLDFLAGS)

nxscheme: $(OBJ) $(MZOBJ)
	$(CC) -shared -o nxscheme.so $(MZOBJ) $(OBJ) $(MZLIB) \
	-L../../src -lNeXus $(NXLDFLAGS)

nxguile: $(OBJ) $(GGOBJ)
	$(CC) -non_shared -o nxguile $(GGOBJ) $(OBJ) $(GGLIB) \
	-L../../src -lNeXus $(NXLDFLAGS)  

nxinter_wrap.o: nxinter.i nxdataset.i
	$(SWIG) $(SCRIPTFLAG) $(NXFLAG) nxinter.i 
	$(CC) $(CFLAGS) $(NXFLAGS) nxinter_wrap.c

nxscheme_wrap.o: nxinter.i nxdataset.i
	$(SWIG) $(MZFLAG) -o nxscheme_wrap.c  nxinter.i
	$(CC) $(CFLAGS) -ieee_with_inexact -assume noaligned_objects \
	-I$(MZINC)/include $(NXFLAGS) nxscheme_wrap.c

nxguile_wrap.o: nxinter.i nxdataset.i
	$(SWIG) $(GGFLAG) -o nxguile_wrap.c nxinter.i
	$(CC) $(CFLAGS) -I$(GGINC)/include/guile $(NXFLAGS)  \
  -DSWIGINIT="SCM scm_init_nxinter_module(void); scm_init_nxinter_module();"\
	nxguile_wrap.c



