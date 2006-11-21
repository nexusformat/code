## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  Makefile for building the core NeXus library
#
#  $Id$
#
#  Copyright (C) 2004 Freddie Akeroyd
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
#
#====================================================================

AM_CPPFLAGS=-I$(top_srcdir)/include @HDF4_CPPFLAGS@ @HDF5_CPPFLAGS@ @XML_CPPFLAGS@ -DIN_NEXUS_LIBRARY
AM_CFLAGS=-prefer-pic

if HAVE_HDF4
H4SRC = napi4.c
endif

if HAVE_HDF5
H5SRC = napi5.c
endif

if HAVE_XML
XMLSRC = nxxml.c nxio.c nxio.h nxdataset.h nxdataset.c
endif
##  the LIBS variable should contain all needed libs for the HDF support
##  It will be filled during the configuration run

lib_LTLIBRARIES = libNeXus.la

nexus_symbols.sym : nexus_symbols.txt
	cp $(srcdir)/nexus_symbols.txt nexus_symbols.sym

# Windows import library for DLL

if MINGW_MSYS
MINGW_SRC=napi_exports.c napi_exports2.c napi_exports.h
msimplibdir = ${libdir}
libNeXus.def: libNeXus.la
	pexports .libs/libNeXus-0.dll > libNeXus.def
if HAVE_MS_LIB
msimplib_DATA = libNeXus.dll.lib libNeXus.dll.exp libNeXus.def
libNeXus.dll.exp: libNeXus.dll.lib
libNeXus.dll.lib: libNeXus.def
	rm -f libNeXus.dll.lib libNeXus.dll.exp
	$(MS_LIB) /MACHINE:I386 /DEF:libNeXus.def /OUT:libNeXus.dll.lib
else
msimplib_DATA = libNeXus.def
endif
endif

#SUBDIRS=nxdict

libNeXus_la_SOURCES = napi.c napiu.c nxstack.c nxstack.h stptok.c $(H4SRC) $(H5SRC) $(XMLSRC) $(MINGW_SRC)
libNeXus_la_LDFLAGS = -export-symbols nexus_symbols.sym @SHARED_LDFLAGS@ @HDF4_LDFLAGS@ @HDF5_LDFLAGS@ @XML_LDFLAGS@ $(LDFLAGS)
libNeXus_la_DEPENDENCIES = nexus_symbols.sym

## The following part is the solution for automake >= 1.7
## 
## libNeXus_la_LIBADD = -lz
## if HAVE_HDF4 
## libNeXus_la_SOURCES += napi4.c
## libNeXus_la_LIBADD += -lmfhdf -ldf -ljpeg
## endif

## if HAVE_HDF5
## libNeXus_la_SOURCES += napi5.c
## libNeXus_la_LIBADD += -lhdf5 
## endif

EXTRA_DIST=nexus_symbols.txt
CLEANFILES=libNeXus.def libNeXus.dll.lib libNeXus.dll.exp nexus_symbols.sym

include $(top_srcdir)/build_rules.am
