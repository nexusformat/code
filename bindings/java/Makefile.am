## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#  
#  Makefile for NeXus Java bindings
#
#  Copyright (C) 2004 Peter Peterson
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
# Modified after change to org.nexusformat package name change: 
# Mark Koennecke, December 2006
#
#====================================================================

AM_JAVACFLAGS	= -classpath .

JAVAROOT	= .

# jar file definitions
javadir		= $(prefix)/nexus/java
java_DATA = jnexus.jar test/TestJapi.class README.JNEXUS COPYING.NCSA
jnexus_jar_ARCHIVE = jnexus.jar

jnexus_jar_CLASS = ncsa/hdf/hdflib/HDFException.class \
               ncsa/hdf/hdflib/HDFJavaException.class \
               ncsa/hdf/hdflib/HDFConstants.class \
               ncsa/hdf/hdflib/HDFNativeData.class \
               ncsa/hdf/hdflib/HDFArray.class \
               ncsa/hdf/hdflib/HDFNotImplementedException.class \
               org/nexusformat/AttributeEntry.class \
               org/nexusformat/NexusException.class \
               org/nexusformat/NXlink.class \
               org/nexusformat/NeXusFileInterface.class \
               org/nexusformat/NexusFile.class

# jni definitions
lib_LTLIBRARIES		= libjnexus.la
libjnexus_la_SOURCES 	= native/hdfnativeImp.c \
                       native/hdfexceptionImp.c \
                       native/handle.c \
                       native/NexusFile.c \
		       native/hdfexceptionImp.h
libjnexus_la_LIBADD 	= $(top_builddir)/src/libNeXus.la
libjnexus_la_LDFLAGS 	= @SHARED_LDFLAGS@ $(LDFLAGS) $(JNEXUS_LDFLAGS)
libjnexus_la_CFLAGS 	= -I$(top_srcdir)/include -Inative @JAVAINCLUDE@ \
     			@HDF4_CPPFLAGS@ @HDF5_CPPFLAGS@ $(JNEXUS_CFLAGS)

BUILT_SOURCES		= native/org_nexusformat_NexusFile.h
nodist_noinst_HEADERS 	= native/org_nexusformat_NexusFile.h 
noinst_HEADERS		= native/handle.h

CLEANFILES	= $(jnexus_jar_CLASS) $(jnexus_jar_ARCHIVE) $(BUILT_SOURCES) \
	libjnexus.def libjnexus.dll.lib libjnexus.dll.exp jnexus.dll \
               ncsa/hdf/hdflib/ArrayDescriptor.class

clean-local:
		rm -rf apidoc

noinst_JAVA	= ncsa/hdf/hdflib/HDFArray.java \
		ncsa/hdf/hdflib/HDFConstants.java \
		ncsa/hdf/hdflib/HDFException.java \
		ncsa/hdf/hdflib/HDFJavaException.java \
		ncsa/hdf/hdflib/HDFNativeData.java \
		ncsa/hdf/hdflib/HDFNotImplementedException.java \
		org/nexusformat/AttributeEntry.java \
		org/nexusformat/NXlink.java \
		org/nexusformat/NeXusFileInterface.java \
		org/nexusformat/NexusException.java \
		org/nexusformat/NexusFile.java \
		test/TestJapi.java

EXTRA_DIST	= $(noinst_JAVA) README.JNEXUS COPYING.NCSA 

# specific targets
# org/nexusformat/NexusFile.class:	org/nexusformat/NexusFile.java

native/org_nexusformat_NexusFile.h: jnexus.jar # org/nexusformat/NexusFile.class
	test -d native || mkdir native
	$(JAVAH) -jni -d native -classpath . org.nexusformat.NexusFile

jnexus.jar: $(jnexus_jar_CLASS)
	$(JAR) cvf jnexus.jar $(jnexus_jar_CLASS) \
               ncsa/hdf/hdflib/ArrayDescriptor.class

#all: $(jnexus_jar_ARCHIVE) $(lib_LTLIBRARIES) javadoc

javadoc:	$(noinst_JAVA)
	test -d apidoc || mkdir apidoc
	$(JAVADOC) -d apidoc -windowtitle jnexus -doctitle jnexus -classpath . \
                      -sourcepath $(srcdir) org.nexusformat ncsa.hdf.hdflib

SUFFIXES = .java .class

.java.class:
	$(JAVAC) -d $(JAVAROOT) $(AM_JAVACFLAGS) $(JAVACFLAGS) $<

# Windows import library for DLL

if MINGW_MSYS
JNEXUS_CFLAGS=-D_JNI_IMPLEMENTATION_
JNEXUS_LDFLAGS=-Wl,--kill-at
msimplibdir = ${libdir}
msimplib_DATA = libjnexus.dll.lib libjnexus.dll.exp libjnexus.def
libjnexus.def: libjnexus.la
	pexports .libs/libjnexus-0.dll > libjnexus.def
libjnexus.dll.exp: libjnexus.dll.lib
libjnexus.dll.lib: libjnexus.def
	LIB /MACHINE:I386 /DEF:libjnexus.def /OUT:libjnexus.dll.lib
jnexusbindir = ${bindir}
jnexusbin_DATA = jnexus.dll
jnexus.dll : libjnexus.la
	ln -sf .libs/libjnexus-0.dll jnexus.dll
endif

## include $(top_srcdir)/build_rules.am
