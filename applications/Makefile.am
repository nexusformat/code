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
LIBNEXUS=../src/libNeXus.a
LIBNEXUS77=../bindings/f77/libNeXus77.a
LIBNEXUS90=../bindings/f90/libNeXus90.a
AM_CPPFLAGS=-I. -I../include

if HAVE_HDF4
HDF4_TARGETS = NXtoNX4
endif
if HAVE_HDF5
HDF5_TARGETS = NXtoNX5
endif
if HAVE_F90
F90_TARGETS = NXdump NXbrowse90
endif

bin_PROGRAMS = NXbrowse NXtoXML NXtoDTD $(HDF4_TARGETS) $(HDF5_TARGETS) $(F90_TARGETS)

NXbrowse_SOURCES = NXbrowse.c
NXbrowse_LDADD = $(LIBNEXUS)

NXtoXML_SOURCES = NXtoXML.c 
NXtoXML_LDADD = $(LIBNEXUS)

NXtoDTD_SOURCES = NXtoDTD.c 
NXtoDTD_LDADD = $(LIBNEXUS)

NXtoNX4_SOURCES = NXtoNX4.c 
NXtoNX4_LDADD = $(LIBNEXUS)

NXtoNX5_SOURCES = NXtoNX5.c 
NXtoNX5_LDADD = $(LIBNEXUS)

NXdump_SOURCES = NXdump.f90
NXdump_LDADD = $(LIBNEXUS90)
NXdump_LINK = $(F90) $(F90FLAGS) $(LDFLAGS) -o $@

NXbrowse90_SOURCES = NXbrowse90.f90
NXbrowse90_LDADD = $(LIBNEXUS90)
NXbrowse90_LINK = $(F90) $(F90FLAGS) $(LDFLAGS) -o $@

include $(top_srcdir)/build_rules.am
