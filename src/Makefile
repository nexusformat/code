#
# $Id$
#
# Makefile for building the distribution kit
#
# See the README files for building the examples
#
# Override compiler definitions if needed e.g.
#CC=gcc
#FC=g77
#CFLAGS=-g
#FFLAGS=-g
#
HDFROOT=/usr/local/hdf
FILES=napi.c napi.h napi_test.c napif.f napif_test.f napif.inc README \
	README.FORTRAN NXdict.tex napi.tex
NAPITEST_OBJ=napi_test.o napi.o
NAPIFTEST_OBJ=napif_test.o napif.o napi.o

all :
	@ echo "Possible targets are: kit napi_test napif_test"


kit : tar zip

tar :
	rm -f napi.tar
	tar cf napi.tar $(FILES)

zip :
	rm -f napi.zip
	zip napi.zip $(FILES)

napi_test : $(NAPITEST_OBJ)
	$(CC) -o $@ $(NAPITEST_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz

napif_test : $(NAPIFTEST_OBJ)
	$(FC) -o $@ $(NAPIFTEST_OBJ) -L$(HDFROOT)/lib -lmfhdf -ldf -lz

.c.o :
	$(CC) $(CFLAGS) -I$(HDFROOT)/include -c $<

.f.o :
	$(FC) $(FFLAGS) -c $<
