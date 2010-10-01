## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id: Makefile.am 1474 2010-05-09 15:30:02Z Freddie Akeroyd $
#  
#  Makefile for nxvalidate
#
#  Copyright (C) 2010 Freddie Akeroyd
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
#  For further information, see <http://www.nexusformat.org/>
#
#====================================================================


nxexampledir=$(NXEXAMPLEDIR)
nxjavadocdir=$(NXDOCDIR)/java
# jar file definitions
javadir		= $(datadir)/java
javalibdir		= $(javadir)/lib
java_DATA = dist/NXvalidate.jar
javalib_DATA = lib/saxon9he.jar lib/jhall.jar
bin_SCRIPTS = nxvalidate

#nxjavadoc_DATA = README.JNEXUS COPYING.NCSA

EXTRA_DIST	= src lib build.xml manifest.mf nbproject

dist/NXvalidate.jar ::
	[ -r build.xml ] || ln -s $(srcdir)/build.xml .
	[ -d nbproject ] || ln -s $(srcdir)/nbproject .
	ant

#dist-hook :
#	if test -d $(srcdir)/apidoc; then \
#	    cp -r $(srcdir)/apidoc $(distdir); \
#	fi

#install-data-local :
#	$(mkinstalldirs) $(DESTDIR)$(nxjavadocdir)
#	if test -d $(srcdir)/apidoc; then \
#	  cp -r $(srcdir)/apidoc $(DESTDIR)$(nxjavadocdir); \
#	  find $(DESTDIR)$(nxjavadocdir)/apidoc -type f -exec chmod 0644 {} \; ;\
#	  find $(DESTDIR)$(nxjavadocdir)/apidoc -type d -exec chmod 0755 {} \; ;\
#	fi

#clean-local :
#	rm -rf apidoc

#uninstall-local :
#	rm -fr $(DESTDIR)$(nxjavadocdir)/apidoc

