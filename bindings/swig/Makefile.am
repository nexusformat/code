## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#  
#  Makefile for NeXus SWIG bindings
#
#  Automake version Copyright (C) 2004 Freddie Akeroyd
#  (based on original Makefile by Mark Koennecke, October 2002)
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
AM_CPPFLAGS=-I$(top_srcdir)/include
lib_LTLIBRARIES=$(LIBNXTCL) $(LIBNXPYTHON) $(LIBNXSCHEME) $(LIBNXGUILE)
bin_PROGRAMS=$(NXINTER) $(NXGUILE)

# python wrapper
if HAVE_SWIG
if HAVE_PYTHON
LIBNXPYTHON=libnxpython.la
libnxpython_la_SOURCES=$(COMMON_SRC)
nodist_libnxpython_la_SOURCES=$(PYTHONWRAP)
libnxpython_la_CFLAGS=-I$(PYTHONROOT)/include/python$(PYTHON_VERSION) $(CFLAGS)
libnxpython_la_LDFLAGS=$(top_builddir)/src/libNeXus.la $(LDFLAGS)
PYTHONWRAP=nxpython_wrap.c
endif
nxpython_wrap.c: nxinter.i nxdataset.i
	$(SWIG) -I$(srcdir) -o $@ -python -module nxpython -interface libnxpython nxinter.i 
endif

# tcl wrapper
if HAVE_SWIG
if HAVE_TCL
LIBNXTCL=libnxtcl.la
NXINTER=nxinter
TCLLIB=-L$(TCLROOT)/lib -ltcl
libnxtcl_la_SOURCES=$(COMMON_SRC)
nodist_libnxtcl_la_SOURCES=$(TCLWRAP)
libnxtcl_la_CFLAGS=-I$(TCLROOT)/include
libnxtcl_la_LDFLAGS=$(top_builddir)/src/libNeXus.la $(TCLLIB) $(LDFLAGS)

nodist_nxinter_SOURCES=libnxtcl.la
nxinter_LDFLAGS=-static libnxtcl.la $(top_builddir)/src/libNeXus.la $(TCLLIB) $(LDFLAGS)
TCLWRAP=nxinter_wrap.c
endif
nxinter_wrap.c: nxinter.i nxdataset.i
	$(SWIG) -I$(srcdir) -o $@ -tcl8 -ltclsh.i nxinter.i 
endif

# guile wrapper
if HAVE_SWIG
if HAVE_GUILE
LIBNXGUILE=libnxguile.la
NXGUILE=nxguile
GGLIB=-L$(GUILEROOT)/lib -lguile
libnxguile_la_SOURCES=$(COMMON_SRC)
nodist_libnxguile_la_SOURCES=$(GUILEWRAP)
libnxguile_la_CFLAGS=-I$(GUILEROOT)/include/guile \
  -DSWIGINIT="SCM scm_init_nxinter_module(void); scm_init_nxinter_module();"\
  $(CFLAGS)
libnxguile_la_LDFLAGS=$(top_builddir)/src/libNeXus.la $(GGLIB) $(LDFLAGS)

nodist_nxguile_SOURCES=libnxguile.la
nxguile_LDFLAGS=-static libnxguile.la $(top_builddir)/src/libNeXus.la $(GGLIB) $(LDFLAGS)
GUILEWRAP=nxguile_wrap.c
endif
nxguile_wrap.c: nxinter.i nxdataset.i
	$(SWIG) -I$(srcdir) -o $@ -guile -lguilemain.i $(SWGUILEFLAGS) nxinter.i
endif

# mzscheme wrapper
if HAVE_SWIG
if HAVE_MZSCHEME
LIBNXSCHEME=libnxscheme.la
MZINC=/data/koenneck/opt/plt
MZLIB=-L$(MZINC)/lib -lmzscheme -lmzgc $(MZINC)/lib/mzdyn.o
libnxscheme_la_SOURCES=$(COMMON_SRC)
nodist_libnxscheme_la_SOURCES=$(SCHEMEWRAP)
libnxscheme_la_CFLAGS=-I$(MZINC)/include $(CFLAGS)
libnxscheme_la_LDFLAGS=$(top_builddir)/src/libNeXus.la $(MZLIB) $(LDFLAGS)
SCHEMEWRAP=nxscheme_wrap.c
endif
nxscheme_wrap.c: nxinter.i nxdataset.i
	$(SWIG) -I$(srcdir) -o $@ -mzscheme nxinter.i 
endif

DOC_SRC		= nxinter.tex
swigdocdir 	= $(NXDOCDIR)
swigdoc_DATA	= $(DOC_OUTPUT)
if HAVE_LATEX
DOC_OUTPUT=nxinter.ps nxinter.pdf
nxinter.ps: $(DOC_SRC)
	latex nxinter.tex
	latex nxinter.tex
	dvips -f nxinter.dvi > nxinter.ps
	ps2pdf12 nxinter.ps nxinter.pdf
endif

include $(top_srcdir)/build_rules.am

# if we have XML support, we already have nxdataset
if HAVE_XML
else
NXDATASET=nxdataset.c
endif

COMMON_SRC=nxinterhelper.c $(NXDATASET)

EXTRA_DIST=nxinter.i nxdataset.i nxdataset.h nxinterhelper.h \
		$(DOC_OUTPUT) $(DOC_SRC) nxpython_test.py

BUILT_SOURCES=$(TCLWRAP) $(GUILEWRAP) $(PYTHONWRAP) $(SCHEMEWRAP)
CLEANFILES=$(TCLWRAP) $(GUILEWRAP) $(PYTHONWRAP) $(SCHEMEWRAP) nxpython.py
