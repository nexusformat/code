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
man_MANS = nxvalidate.1

#nxjavadoc_DATA = README.JNEXUS COPYING.NCSA

EXTRA_DIST	= $(man_MANS) src lib build.xml manifest.mf $(srcdir)/nbproject/*

dist/NXvalidate.jar ::
	[ -r build.xml ] || ln -s $(srcdir)/build.xml .
	[ -d nbproject ] || ln -s $(srcdir)/nbproject .
	$(ANT_PROG)

#apidoc : $(noinst_JAVA)
#	test -d apidoc || mkdir apidoc
#	$(JAVADOC) -d apidoc -windowtitle jnexus -doctitle jnexus -classpath . \
#                      -sourcepath $(srcdir) org.nexusformat ncsa.hdf.hdflib

#dist-hook :
#	if test -d $(srcdir)/apidoc; then \
#	    cp -r $(srcdir)/apidoc $(distdir); \
#	fi

install-data-local :
	$(mkinstalldirs) $(DESTDIR)$(nxjavadocdir)
	if test -d $(builddir)/dist/javadoc; then \
	  cp -r $(builddir)/dist/javadoc $(DESTDIR)$(nxjavadocdir)/nxvalidate; \
	  find $(DESTDIR)$(nxjavadocdir) -type f -exec chmod 0644 {} \; ;\
	  find $(DESTDIR)$(nxjavadocdir) -type d -exec chmod 0755 {} \; ;\
	fi

clean-local :
if HAVE_ANT
	$(ANT_PROG) clean
endif

#uninstall-local :
#	rm -fr $(DESTDIR)$(nxjavadocdir)/apidoc

