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
AM_CPPFLAGS=-I$(top_srcdir)/include

# define which libraries to use
if HAVE_HDF4
LIB4_FLAG = -DHDF4
endif

if HAVE_HDF5
LIB5_FLAG = -DHDF5
endif


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

lib_LIBRARIES = libjnexus.so

libjnexus_so_SOURCES = native/neutron_nexus_NexusFile.h \
                       native/hdfnativeImp.c \
                       native/hdfexceptionImp.c \
                       native/handle.c \
                       native/NexusFile.c

native/NexusFile.o: native/NexusFile.c native/neutron_nexus_NexusFile.h

JAVAROOT = /usr/local/java
JINCLUDE = -I$(JAVAROOT)/include -I$(JAVAROOT)/include/linux


CFLAGS = -g -c $(AM_CPPFLAGS) $(JINCLUDE) -I../../include -Inative
LFLAGS = -g -shared -L../../src -lNeXus $(LIB4_FLAG) $(LIB5_FLAG)

.c.o:
	$(CC) $(CFLAGS) -o $*.o $*.c

native/neutron_nexus_NexusFile.h: neutron/nexus/NexusFile.class
	$(JAVAH) -jni -d native neutron.nexus.NexusFile

jnexus.jar: $(jnexus_CLASS)
	$(JAR) cvf jnexus.jar $(jnexus_CLASS)

#libjnexus.so: native/neutron_nexus_NexusFile.h $(jnexus_COBJ)
#	$(CC) -o libjnexus.so $(jnexus_COBJ) $(LFLAGS) -lz -lm

test: test/TestJapi.class

javadoc:
	$(JAVADOC) -d apidoc -windowtitle jnexus -doctitle jnexus neutron.nexus ncsa.hdf.hdflib

all: jnexus.jar native/neutron_nexus_NexusFile.h libjnexus.so javadoc

clean:
	rm -f $(jnexus_CLASS) native/neutron_nexus_NexusFile.h
	rm -rf apidoc/*

include $(top_srcdir)/build_rules.am
