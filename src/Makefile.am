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

if HAVE_HDF4
H4SRC = napi4.c
endif

if HAVE_HDF5
H5SRC = napi5.c
endif

if HAVE_XML
XMLSRC = nxxml.c nxio.c nxdataset.c stptok.c
endif
##  the LIBS variable should contain all needed libs for the HDF support
##  It will be filled during the configuration run

lib_LTLIBRARIES = libNeXus.la

# Windows import library for DLL

if MINGW_MSYS
msimplibdir = ${libdir}
msimplib_DATA = libNeXus.dll.lib libNeXus.dll.exp libNeXus.def
libNeXus.def: libNeXus.la
	pexports .libs/libNeXus-0.dll > libNeXus.def
libNeXus.dll.exp: libNeXus.dll.lib
libNeXus.dll.lib: libNeXus.def
	LIB /MACHINE:I386 /DEF:libNeXus.def /OUT:libNeXus.dll.lib
endif

#SUBDIRS=nxdict

libNeXus_la_SOURCES = napi.c napiu.c $(H4SRC) $(H5SRC) $(XMLSRC)
libNeXus_la_LDFLAGS = @SHARED_LDFLAGS@ @HDF4_LDFLAGS@ @HDF5_LDFLAGS@ @XML_LDFLAGS@ $(LDFLAGS)

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

EXTRA_DIST=nxio.h nxdataset.h
CLEANFILES=libNeXus.def libNeXus.dll.lib libNeXus.dll.exp

include $(top_srcdir)/build_rules.am
