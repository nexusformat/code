## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#  
#  Makefile for NeXus FORTRAN 90 bindings
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
lib_LTLIBRARIES = libNeXus90.la

libNeXus90_la_SOURCES = NXmodule.f90 NXUmodule.f90
libNeXus90_la_LIBADD = $(top_builddir)/bindings/f77/libNeXus77.la \
			$(top_builddir)/src/libNeXus.la
libNeXus90_la_LDFLAGS = @SHARED_LDFLAGS@
libNeXus90_la_LINK = $(F90LINK)

NXUmodule.o: NXUmodule.f90 NXmodule.o

include $(top_srcdir)/build_rules.am

