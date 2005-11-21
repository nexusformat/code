## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#  
#  Makefile for building NeXus applications
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
#====================================================================
LIBNEXUS=$(top_builddir)/src/libNeXus.la
LIBNEXUS77=$(top_builddir)/bindings/f77/libNeXus77.la
LIBNEXUS90=$(top_builddir)/bindings/f90/libNeXus90.la
AM_CPPFLAGS=-I. -I$(top_srcdir)/include

if HAVE_LIBXML2
NXTRANSLATE=NXtranslate
endif
SUBDIRS = NXdir $(NXTRANSLATE)
#if HAVE_HDF4
#HDF4_TARGETS = NXtoNX4
#endif
#if HAVE_HDF5
#HDF5_TARGETS = NXtoNX5
#endif
if HAVE_F90
F90_TARGETS = NXdump NXbrowse90
## F90_TARGETS =
endif

bin_PROGRAMS = nxbrowse nx2dtd nxconvert $(F90_TARGETS)

nxconvert_SOURCES = nxconvert.c
nxconvert_LDADD = $(LIBNEXUS)
nxconvert_LDFLAGS = -static $(LDFLAGS)

nxbrowse_SOURCES = NXbrowse.c
nxbrowse_LDADD = $(LIBNEXUS)
nxbrowse_LDFLAGS = -static $(LDFLAGS)

#NXtoXML_SOURCES = NXtoXML.c 
#NXtoXML_LDADD = $(LIBNEXUS)
#NXtoXML_LDFLAGS = -static $(LDFLAGS)

nx2dtd_SOURCES = NXtoDTD.c 
nx2dtd_LDADD = $(LIBNEXUS)
nx2dtd_LDFLAGS = -static $(LDFLAGS)

#NXtoNX4_SOURCES = NXtoNX4.c 
#NXtoNX4_LDADD = $(LIBNEXUS)

#NXtoNX5_SOURCES = NXtoNX5.c 
#NXtoNX5_LDADD = $(LIBNEXUS)

NXdump_SOURCES = NXdump.f90
NXdump_LDADD = $(LIBNEXUS90)
NXdump_LINK = $(FCLINK)

NXbrowse90_SOURCES = NXbrowse90.f90
NXbrowse90_LDADD = $(LIBNEXUS90)
NXbrowse90_LINK = $(FCLINK)

include $(top_srcdir)/build_rules.am
