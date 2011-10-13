## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id: Makefile.am 1239 2009-04-15 14:37:08Z Freddie Akeroyd $
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
#  For further information, see <http://www.nexusformat.org>
#
#====================================================================
LIBNEXUS=$(top_builddir)/src/libNeXus.la
AM_CPPFLAGS=-I. -I$(top_srcdir)/include 

#EXTRA_DIST=SConscript


bin_PROGRAMS = nxtraverse

nxtraverse_SOURCES = nxtraverse.cpp main.cpp nxtraverse.h
nxtraverse_LDADD = $(LIBNEXUS)
nxtraverse_LDFLAGS = -static @HDF4_LDFLAGS@ @HDF5_LDFLAGS@ @XML_LDFLAGS@ $(LDFLAGS)

include $(top_srcdir)/build_rules.am
