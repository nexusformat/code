#
# $Id$
#
# Makefile for building the distribution kit
#
# See the README files for building the examples
#
#====================================================================
# Override compiler definitions if needed e.g.
#CC=gcc
#FC=g77
#CFLAGS=-g
#FFLAGS=-g
#====================================================================
#The following compiler flags work with Absoft Pro Fortran on Linux
#CFLAGS=-D__ABSOFT
#FFLAGS=-f
#F90=f90
#F90FLAGS=-YEXT_NAMES=LCS
#====================================================================
HDFROOT=/usr/local/hdf
FILES=napi.c napi.h napi_test.c napif.f napif_test.f napif.inc \
	napi_test_read.c NXmodule.f90 NXUmodule.f90 NXtest.f90 \
	NXbrowse.f90 README README.FORTRAN README.FORTRAN90 \
	make_vms.com
NAPITEST_OBJ=napi_test.o napi.o
NAPITESTREAD_OBJ=napi_test_read.o napi.o
NAPIFTEST_OBJ=napif_test.o napif.o napi.o
NXTEST_OBJ=NXtest.o NXmodule.o napi.o
NXBROWSE_OBJ=NXbrowse.o NXUmodule.o NXmodule.o napi.o
LIBNEXUS_OBJ=napif.o napi.o
#====================================================================

default :
	@ echo "Possible targets are: all lib kit napi_test napi_test_read \
	napif_test NXtest NXbrowse"


all : lib napi_test napi_test_read napif_test NXtest NXbrowse

lib : libNeXus.a

kit : tar zip

clean :
	rm -f *.o libNeXus.a napi.tar napi.zip
tar :
	rm -f napi.tar
	tar cf napi.tar $(FILES)

zip :
	rm -f napi.zip
	zip napi.zip $(FILES)

libNeXus.a : $(LIBNEXUS_OBJ)
	@echo "*** Creating NeXus interface library libNeXus.a ***"
	ar -cr $@ $(LIBNEXUS_OBJ)
	ranlib $@

napi_test : $(NAPITEST_OBJ)
	$(CC) -o $@ $(NAPITEST_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz -ljpeg

napi_test_read : $(NAPITESTREAD_OBJ)
	$(CC) -o $@ $(NAPITESTREAD_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz -ljpeg

napif_test : $(NAPIFTEST_OBJ)
	$(FC) -o $@ $(NAPIFTEST_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz -ljpeg

NXtest : $(NXTEST_OBJ)
	$(F90) -o $@ $(NXTEST_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz -ljpeg
	
NXbrowse : $(NXBROWSE_OBJ)
	$(F90) -o $@ $(NXBROWSE_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz -ljpeg
	
NXbrowse.o : NXbrowse.f90
	$(F90) $(F90FLAGS) -c $?

NXtest.o : NXtest.f90
	$(F90) $(F90FLAGS) -c $?

NXUmodule.o : NXUmodule.f90
	$(F90) $(F90FLAGS) -c $?

NXmodule.o : NXmodule.f90
	$(F90) $(F90FLAGS) -c $?

.c.o :
	$(CC) $(CFLAGS) -I$(HDFROOT)/include -c $<

.f.o :
	$(FC) $(FFLAGS) -c $<
