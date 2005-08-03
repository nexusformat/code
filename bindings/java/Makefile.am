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
#
#====================================================================
# jni definitions
lib_LTLIBRARIES		= libjnexus.la
libjnexus_la_SOURCES 	= native/hdfnativeImp.c \
                       native/hdfexceptionImp.c \
                       native/handle.c \
                       native/NexusFile.c
libjnexus_la_LIBADD 	= $(top_builddir)/src/libNeXus.la
libjnexus_la_CFLAGS 	= -I$(top_srcdir)/include -Inative @JAVAINCLUDE@ \
     			@HDF4_CPPFLAGS@ @HDF5_CPPFLAGS@

BUILT_SOURCES		= native/neutron_nexus_NexusFile.h
nodist_noinst_HEADERS 	= native/neutron_nexus_NexusFile.h 
noinst_HEADERS		= native/handle.h

AM_JAVACFLAGS	= 

JAVAROOT	= .

# jar file definitions
jnexus_ARCHIVE = jnexus.jar
jnexus_CLASS = ncsa/hdf/hdflib/HDFArray.class \
               ncsa/hdf/hdflib/ArrayDescriptor.class \
               ncsa/hdf/hdflib/HDFConstants.class \
               ncsa/hdf/hdflib/HDFException.class \
               ncsa/hdf/hdflib/HDFJavaException.class \
               ncsa/hdf/hdflib/HDFNativeData.class \
               ncsa/hdf/hdflib/HDFNotImplementedException.class \
               neutron/nexus/AttributeEntry.class \
               neutron/nexus/NexusException.class \
               neutron/nexus/NeXusFileInterface.class \
               neutron/nexus/NexusFile.class \
               neutron/nexus/NXlink.class

CLEANFILES	= $(jnexus_CLASS) $(jnexus_ARCHIVE) $(BUILT_SOURCES) 

clean-local:
		rm -rf apidoc

noinst_JAVA	= ncsa/hdf/hdflib/HDFArray.java \
		ncsa/hdf/hdflib/HDFConstants.java \
		ncsa/hdf/hdflib/HDFException.java \
		ncsa/hdf/hdflib/HDFJavaException.java \
		ncsa/hdf/hdflib/HDFNativeData.java \
		ncsa/hdf/hdflib/HDFNotImplementedException.java \
		neutron/nexus/AttributeEntry.java \
		neutron/nexus/NXlink.java \
		neutron/nexus/NeXusFileInterface.java \
		neutron/nexus/NexusException.java \
		neutron/nexus/NexusFile.java \
		test/TestJapi.java

EXTRA_DIST	= $(noinst_JAVA)

# specific targets

neutron/nexus/NexusFile.class:	neutron/nexus/NexusFile.java

native/neutron_nexus_NexusFile.h: neutron/nexus/NexusFile.class
	test -d native || mkdir native
	$(JAVAH) -jni -d native neutron.nexus.NexusFile

jnexus.jar: $(jnexus_CLASS)
	$(JAR) cvf jnexus.jar $(jnexus_CLASS)

all: $(jnexus_ARCHIVE) $(lib_LTLIBRARIES) javadoc

javadoc:	$(noinst_JAVA)
	test -d apidoc || mkdir apidoc
	$(JAVADOC) -d apidoc -windowtitle jnexus -doctitle jnexus -classpath . \
                      -sourcepath $(srcdir) neutron.nexus ncsa.hdf.hdflib

SUFFIXES = .java .class

.java.class:
	$(JAVAC) -d $(JAVAROOT) -classpath $(srcdir) $<

## include $(top_srcdir)/build_rules.am
