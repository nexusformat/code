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
AM_LDFLAGS=-version-info $(NXLTVERSINFO)

libNeXus90_la_SOURCES = NXmodule.f90 NXUmodule.f90
libNeXus90_la_LIBADD = $(top_builddir)/src/libNeXus.la
libNeXus90_la_LDFLAGS = @SHARED_LDFLAGS@ $(LDFLAGS)
libNeXus90_la_LINK = $(FCLINK)

NXUmodule.o: NXUmodule.f90 NXmodule.o

# This is needed as f90 is not supported as a valid tag in libtool
# and the relink command (used during an install) which is written to
# libNeXus90.la will not work. 
#all: all-am libNeXus90.la_orig

#libNeXus90.la_orig: libNeXus90.la
#	mv libNeXus90.la $@
#	sed -e 's/--mode=relink/--mode=relink @LTF90LINKTAG@/' < $@ > libNeXus90.la
#	touch $@

include $(top_srcdir)/build_rules.am

CLEANFILES = libNeXus90.la_orig nxmodule.mod nxumodule.mod

