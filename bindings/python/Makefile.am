## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id: Makefile.am 715 2005-12-16 18:11:19Z faa59 $
#  
#  Makefile for NeXus python bindings
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

pydocdir 	= $(NXDOCDIR)/python
pydoc_DATA	= README.html
#nxspythondir	= $(pkgpythondir)/nxs
nxspythondir	= $(pythondir)/nxs
#pkgpython_PYTHON = nxstest.py
nxspython_PYTHON = nxs/__init__.py nxs/napi.py
EXTRA_DIST	= README.html nxstest.py

install-data-hook :
	sed -e "s|nxprefix = .*|nxprefix = \'${prefix}\'|" < ${srcdir}/nxs/napi.py > $(DESTDIR)${nxspythondir}/napi.py
	if test -z "$(DESTDIR)"; then \
	    $(py_compile) --basedir "$(nxspythondir)" napi.py; \
	else \
	    $(py_compile) --destdir "$(DESTDIR)" --basedir "$(nxspythondir)" napi.py; \
	fi

# python setup.py install --root=$(DESTDIR)

include $(top_srcdir)/build_rules.am
