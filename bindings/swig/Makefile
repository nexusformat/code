#--------------------------------------------------------------------------
# This is a makefile for the NeXus SWIG interface functions. An example if
# you wish, really.
#
# Mark Koennecke, October 2002
#-----------------------------------------------------------------------
.SUFFIXES:
.SUFFIXES: .i .c .o

CC=cc
CFLAGS=-c -I/afs/psi.ch/project/sinq/tru64/include -DHDF4 -DHDF5
LDFLAGS=-L/afs/psi.ch/project/sinq/tru64/lib -lNeXus -lmf -ldf -ljpeg -lhdf5 -lz -lm

#----------- change this for other script languages
SCRIPTFLAG=-tcl8 -ltclsh.i
SCRIPTFLAG2=-tcl8 
SCRIPTLIB=-L/data/lnslib/lib -ltcl8.0
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

.c.o:
	$(CC) $(CFLAGS) $*.c


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
	$(CC) -o nxinter  $(TCLOBJ) $(OBJ) $(SCRIPTLIB) $(LDFLAGS)
	$(CC) -shared -o nxinter.so  $(OBJ) $(SCRIPTLIB) $(LDFLAGS)

nxscheme: $(OBJ) $(MZOBJ)
	$(CC) -shared -o nxscheme.so $(MZOBJ) $(OBJ) $(MZLIB) $(LDFLAGS) 

nxguile: $(OBJ) $(GGOBJ)
	$(CC) -non_shared -o nxguile $(GGOBJ) $(OBJ) $(GGLIB) $(LDFLAGS) 

nxinter_wrap.o: nxinter.i nxdataset.i
	/data/lnslib/bin/swig $(SCRIPTFLAG) nxinter.i 
	$(CC) $(CFLAGS) nxinter_wrap.c

nxscheme_wrap.o: nxinter.i nxdataset.i
	swig $(MZFLAG) -o nxscheme_wrap.c  nxinter.i
	$(CC) $(CFLAGS) -ieee_with_inexact -assume noaligned_objects \
	-I$(MZINC)/include  nxscheme_wrap.c

nxguile_wrap.o: nxinter.i nxdataset.i
	/data/lnslib/bin/swig $(GGFLAG) -o nxguile_wrap.c nxinter.i
	$(CC) $(CFLAGS) -I$(GGINC)/include/guile  \
  -DSWIGINIT="SCM scm_init_nxinter_module(void); scm_init_nxinter_module();"\
	nxguile_wrap.c



