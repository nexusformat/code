## Process this file with automake to produce Makefile.in
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#  
#  Makefile for generating test programs
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
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
#====================================================================
LIBNEXUS=$(top_builddir)/src/libNeXus.la
LIBNEXUS77=$(top_builddir)/bindings/f77/libNeXus77.la
LIBNEXUS90=$(top_builddir)/bindings/f90/libNeXus90.la

AM_CPPFLAGS=-I. -I$(top_srcdir)/include
AM_FFLAGS=-I$(top_srcdir)/bindings/f77
CLEANFILES=NXtest.nx4 NXtest.nx5 NXtest.nxs

## testdir=$(prefix)/nexus/test

HDF4_TARGETS = napi_test-hdf4

HDF5_TARGETS = napi_test-hdf5

F77_TARGETS = napif_test

F90_TARGETS = NXtest


EXTRA_DIST	= testsuite.at $(TESTSUITE_AT) testsuite package.m4

TESTSUITE	= $(srcdir)/testsuite

TESTSUITE_AT	= ## dbase.at devices.at starter.at

AUTOTEST	= $(AUTOM4TE) --language=autotest

check_PROGRAMS = $(HDF4_TARGETS) $(HDF5_TARGETS) $(F77_TARGETS) $(F90_TARGETS)

# for running tests via   "make check"
## check_PROGRAMS = $(test_PROGRAMS)	# what to build
## TESTS = $(test_PROGRAMS)		# what to run

# this sets the test running environment - in case we
# have got built with shared HDF libraries we need to set LD_LIBRARY_PATH
TESTS_ENVIRONMENT=env LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$(H4ROOT)/lib:$(H5ROOT)/lib

napi_test_hdf4_SOURCES = napi_test.c
napi_test_hdf4_CPPFLAGS = @HDF4_CPPFLAGS@ -I$(top_srcdir)/include
napi_test_hdf4_LDADD = $(LIBNEXUS) @HDF4_LDFLAGS@

napi_test_hdf5_SOURCES = napi_test.c
napi_test_hdf5_CPPFLAGS = @HDF5_CPPFLAGS@ -I$(top_srcdir)/include
napi_test_hdf5_LDADD = $(LIBNEXUS) @HDF5_LDFLAGS@

if HAVE_F77
napif_test_SOURCES = napif_test.f
napif_test_LDADD = $(LIBNEXUS77) # $(FLIBS)
else
napif_test_SOURCES = dummy.c
endif

if HAVE_F90
NXtest_SOURCES = NXtest.f90
NXtest_LDADD = $(LIBNEXUS90)
NXtest_LINK = $(F90LINK)
else
NXtest_SOURCES = dummy.c
endif

include $(top_srcdir)/build_rules.am

$(srcdir)/package.m4:	$(top_srcdir)/configure.ac
	{\
		echo '# Signature of the current package.'; \
		echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])'; \
		echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])'; \
		echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])'; \
		echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])'; \
		echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} > $(srcdir)/package.m4

check-local:	atconfig atlocal $(TESTSUITE) 
	$(SHELL) $(TESTSUITE)

$(TESTSUITE):	$(srcdir)/testsuite.at $(TESTSUITE_AT) $(srcdir)/package.m4
	$(AUTOTEST) -I $(srcdir) $@.at -o $@.tmp
	mv $@.tmp $@

atconfig:	$(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@

clean-local:
	$(SHELL) $(TESTSUITE) --clean

DISTCLEANFILES = atconfig
