#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  Makefile for building the NeXus library with the HDF5 libraries
#  
#  Copyright (C) 2002 Mark Koennecke
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
# 
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free 
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
#  MA  02111-1307  USA
#             
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
# $Id: Makefile,v 2.00 2001/27/07 11:00:02
#
#====================================================================

#Edit "make_general" to set compiler flags
include ../../make_general

F77=f77
LIBNEXUS_OBJ=../../src/napi.o napif.o

all: libNeXusf77.a 

.c.o :
	$(CC) -c $(NXFLAGS) -I. -I../../include $*.c

.f.o:
	$(F77) -c $(NXFLAGS) -I. $*.f

../../src/napi.o: ../../src/napi.c ../../src/napi4.c ../../src/napi5.c
	$(CC) -c $(NXFLAGS) -I../../include -I../../src -o ../../src/napi.o \
	../../src/napi.c
		
libNeXusf77.a : $(LIBNEXUS_OBJ)
	@echo "*** Creating NeXus interface (libNeXus.a) ***"
	- rm $@
	ar -cr $@ $(LIBNEXUS_OBJ)
	ranlib $@

clean:
	- rm *.o	
	- rm *.a

