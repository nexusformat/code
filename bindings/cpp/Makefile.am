## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id: Makefile.am 598 2005-08-19 16:19:15Z faa59 $
#  
#  Makefile for NeXus C++ bindings
#
#  Copyright (C) 2008 Freddie Akeroyd
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
LIBNEXUS=$(top_builddir)/src/libNeXus.la
lib_LTLIBRARIES=libNeXusCPP.la

libNeXusCPP_la_SOURCES=NeXusFile.hpp NeXusFile.cpp nexus_util.hpp nexus_util.cpp
libNeXusCPP_la_LIBADD=$(LIBNEXUS)
libNeXusCPP_la_LDFLAGS=@SHARED_LDFLAGS@ $(LDFLAGS)

AM_CPPFLAGS=-I. -I$(top_srcdir)/include

include $(top_srcdir)/build_rules.am
