#---------------------------------------------------------------------------
#  Makefile for standard NeXus applications
#
#  Copyright (C) 2003 Mark Koennecke
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
#-------------------------------------------------------------------------
include ../make_general


all: NXbrowse NXtoDTD NXtoNX4 NXtoNX5 NXtoXML napi_test

.c.o:
	$(CC) -c -g -I../include $(NXFLAGS) $*.c

#--------------------------------------------------------------------------
# NOTE: define EXFLAG on the make line if you need special link options.
#  On True64Unix, for instance EXFLAG=-non_shared is necessary in order
# to prevent linking against a dodgy shared jpeg library
#--------------------------------------------------------------------------
NXbrowse: NXbrowse.o
	$(CC) $(EXFLAG) -o NXbrowse NXbrowse.o -L../src -lNeXus $(NXLDFLAGS)

NXtoDTD: NXtoDTD.o
	$(CC) $(EXFLAG) -o NXtoDTD NXtoDTD.o -L../src -lNeXus $(NXLDFLAGS)

NXtoNX4: NXtoNX4.o
	$(CC) $(EXFLAG) -o NXtoNX4 NXtoNX4.o -L../src -lNeXus $(NXLDFLAGS)

NXtoNX5: NXtoNX5.o
	$(CC) $(EXFLAG) -o NXtoNX5 NXtoNX5.o -L../src -lNeXus $(NXLDFLAGS)

NXtoXML: NXtoXML.o
	$(CC) $(EXFLAG) -o NXtoXML NXtoXML.o -L../src -lNeXus $(NXLDFLAGS)

napi_test: napi_test.o
	$(CC) $(EXFLAG) -o napi_test napi_test.o -L../src -lNeXus $(NXLDFLAGS)
	- cp napi_test napi5_test
clean:
	-rm *.o
	-rm Nbrowse NXtoDTD NXtoNX4 NXtoNX5 NXtoXML
