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
#  For further information, see <http://www.nexusformat.org>
#
#====================================================================
LIBNEXUS=$(top_builddir)/src/libNeXus.la
LIBNEXUS77=$(top_builddir)/bindings/f77/libNeXus77.la
LIBNEXUS90=$(top_builddir)/bindings/f90/libNeXus90.la
AM_CPPFLAGS=-I. -I$(top_srcdir)/include
AM_FCFLAGS=-I$(top_builddir)/bindings/f90
man_MANS = nxdiff.1

dist_bin_SCRIPTS=nxdiff

EXTRA_DIST=SConscript $(man_MANS)

if HAVE_F90
NXDUMP = NXdump
endif
if HAVE_XML
NXINGEST = nxingest
endif
if HAVE_LIBXML2
NXTRANSLATE=NXtranslate
NXSUMMARY=NXsummary
endif
if HAVE_ANT
NXVALIDATE = NXvalidate
endif

SUBDIRS = NXbrowse NXdir NXtraverse NXconvert $(NXVALIDATE) \
	$(NXDUMP) $(NXTRANSLATE) $(NXSUMMARY) $(NXINGEST)

include $(top_srcdir)/build_rules.am
