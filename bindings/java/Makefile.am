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
lib_LTLIBRARIES=libjnexus.la
### List of all classes needed for the jar file. The order matters
### since ArrayDescripter is defined inside HDFArray
#jnexus_jar_CLASS = ncsa/hdf/hdflib/HDFArray.java \
#                     ncsa/hdf/hdflib/ArrayDescriptor.java \
#                     ncsa/hdf/hdflib/HDFConstants.java \
#                     ncsa/hdf/hdflib/HDFException.java \
#                     ncsa/hdf/hdflib/HDFJavaException.java \
#                     ncsa/hdf/hdflib/HDFNativeData.java \
#                     ncsa/hdf/hdflib/HDFNotImplementedException.java \
#                     neutron/nexus/AttributeEntry.java \
#                     neutron/nexus/NexusException.java \
#                     neutron/nexus/NeXusFileInterface.java \
#                     neutron/nexus/NexusFile.java \
#                     neutron/nexus/NXlink.java
## List of all classes needed for the jar file. The order matters
## since ArrayDescripter is defined inside HDFArray
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

## native/neutron_nexus_NexusFile.h \
libjnexus_la_SOURCES = native/hdfnativeImp.c \
                       native/hdfexceptionImp.c \
                       native/handle.c \
                       native/NexusFile.c
libjnexus_la_LIBADD = $(top_builddir)/src/libNeXus.la
libjnexus_la_CFLAGS = -Inative -I/usr/local/java/include \
                      -I/usr/local/java/include/linux
noinst_HEADERS = native/neutron_nexus_NexusFile.h

jnexus_ARCHIVE = jnexus.jar

native/neutron_nexus_NexusFile.h: neutron/nexus/NexusFile.class
	$(JAVAH) -jni -d native neutron.nexus.NexusFile

jnexus.jar: $(jnexus_CLASS)
	$(JAR) cvf jnexus.jar $(jnexus_CLASS)

all: $(jnexus_ARCHIVE) $(lib_LTLIBRARIES) javadoc

javadoc:
	$(JAVADOC) -d apidoc -windowtitle jnexus -doctitle jnexus \
                      neutron.nexus ncsa.hdf.hdflib

##### second try
#mylibdir=./
#mylib_PROGRAMS = libjnexus.so
#libjnexus_so_SOURCES = native/neutron_nexus_NexusFile.h \
#                       native/hdfnativeImp.c \
#                       native/hdfexceptionImp.c \
#                       native/handle.c \
#                       native/NexusFile.c
#
## this is to find neutron_nexus_NexusFile.h, jni.h, and jni_md.h
#libjnexus_so_CFLAGS = -Inative -I/usr/local/java/include \
#                      -I/usr/local/java/include/linux
#libjnexus_so_LDFLAGS = -shared -lz -lm
#libjnexus_so_LDADD = $(top_builddir)/src/libNeXus.la
#
#native/neutron_nexus_NexusFile.h: neutron/nexus/NexusFile.class
#	$(JAVAH) -jni -d native neutron.nexus.NexusFile

###
include $(top_srcdir)/build_rules.am

##### start of old version
#AM_CPPFLAGS=-I$(top_srcdir)/include
#
## define which libraries to use
#if HAVE_HDF4
#LIB4_FLAG = -DHDF4
#endif
#
#if HAVE_HDF5
#LIB5_FLAG = -DHDF5
#endif
#
#
### List of all classes needed for the jar file. The order matters
### since ArrayDescripter is defined inside HDFArray
#jnexus_CLASS = ncsa/hdf/hdflib/HDFArray.class \
#               ncsa/hdf/hdflib/ArrayDescriptor.class \
#               ncsa/hdf/hdflib/HDFConstants.class \
#               ncsa/hdf/hdflib/HDFException.class \
#               ncsa/hdf/hdflib/HDFJavaException.class \
#               ncsa/hdf/hdflib/HDFNativeData.class \
#               ncsa/hdf/hdflib/HDFNotImplementedException.class \
#               neutron/nexus/AttributeEntry.class \
#               neutron/nexus/NexusException.class \
#               neutron/nexus/NeXusFileInterface.class \
#               neutron/nexus/NexusFile.class \
#               neutron/nexus/NXlink.class
#
##lib_LIBRARIES = libjnexus.so
#
##libjnexus_so_SOURCES = native/neutron_nexus_NexusFile.h \
##                       native/hdfnativeImp.c \
##                       native/hdfexceptionImp.c \
##                       native/handle.c \
##                       native/NexusFile.c
#
#jnexus_COBJ = native/hdfnativeImp.o \
#              native/hdfexceptionImp.o \
#              native/handle.o \
#              native/NexusFile.o
#
#all: jnexus.jar native/neutron_nexus_NexusFile.h libjnexus.so javadoc
#
#native/NexusFile.o: native/NexusFile.c native/neutron_nexus_NexusFile.h
#
#JAVAROOT = /usr/local/java
#JINCLUDE = -I$(JAVAROOT)/include -I$(JAVAROOT)/include/linux
#
#
#CFLAGS = -g -c $(AM_CFLAGS) $(LIB4_FLAG) $(LIB5_FLAG) $(JINCLUDE) -I../../include -Inative
#LFLAGS = -g -shared $(LIB4_FLAG) $(LIB5_FLAG) -lz -lm
#
#.c.o:
#	$(CC) $(CFLAGS) -o $*.o $*.c
#
#native/neutron_nexus_NexusFile.h: neutron/nexus/NexusFile.class
#	$(JAVAH) -jni -d native neutron.nexus.NexusFile
#
#jnexus.jar: $(jnexus_CLASS)
#	$(JAR) cvf jnexus.jar $(jnexus_CLASS)
#
#libjnexus.so: native/neutron_nexus_NexusFile.h $(jnexus_COBJ)
#	$(CC) $(LFLAGS) -o libjnexus.so ../../src/libNeXus.la $(jnexus_COBJ)
#
#test: test/TestJapi.class
#
#
#clean:
#	rm -f $(jnexus_CLASS) native/neutron_nexus_NexusFile.h
#	rm -rf apidoc/*
#
#include $(top_srcdir)/build_rules.am
