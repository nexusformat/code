## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#  
#  Makefile for generating test programs
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
LIBNEXUS90=$(top_builddir)/bindings/f90/libNeXus90.a
AM_CPPFLAGS=-I. -I$(top_srcdir)/include
AM_FFLAGS=-I$(top_srcdir)/bindings/f77

if HAVE_HDF4
HDF4_TARGETS = napi4_test
endif
if HAVE_HDF5
HDF5_TARGETS = napi5_test
endif
if HAVE_F77
F77_TARGETS = napif_test
endif
if HAVE_F90
F90_TARGETS = NXtest
endif

bin_PROGRAMS = $(HDF4_TARGETS) $(HDF5_TARGETS) $(F77_TARGETS) $(F90_TARGETS)

napi4_test_SOURCES = napi4_test.c
napi4_test_LDADD = $(LIBNEXUS)

napi5_test_SOURCES = napi5_test.c
napi5_test_LDADD = $(LIBNEXUS)

napif_test_SOURCES = napif_test.f
napif_test_LDADD = $(LIBNEXUS77)
## napif_test_LINK = $(F77) $(F77FLAGS) $(LDFLAGS) -o $@

NXtest_SOURCES = NXtest.f90
NXtest_LDADD = $(LIBNEXUS90)
## NXtest_LINK = $(F90) $(F90FLAGS) $(LDFLAGS) -o $@

include $(top_srcdir)/build_rules.am
