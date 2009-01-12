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

# so   "make test"   is the same as   "make check"
test: check

LIBNEXUS=$(top_builddir)/src/libNeXus.la
LIBNEXUS77=$(top_builddir)/bindings/f77/libNeXus77.la
LIBNEXUS90=$(top_builddir)/bindings/f90/libNeXus90.la
LIBNEXUSCPP=$(top_builddir)/bindings/cpp/libNeXusCPP.la

AM_CPPFLAGS=-I. -I$(top_srcdir)/include -I$(top_srcdir)/bindings/cpp
AM_FFLAGS=-I$(top_srcdir)/bindings/f77
AM_FCFLAGS=-I$(top_builddir)/bindings/f90
CLEANFILES=NXtest.nx4 NXtest.nx5 NXtest.nxs NXtest.xml NXtest-table.xml \
	leak_test.nxs leak_test1.nxs leak_test2_*.nxs \
	nxtranslate nxsummary nxconvert nxvalidate nxdir nxbrowse


## testdir=$(prefix)/nexus/test

HDF4_TARGETS = napi_test-hdf4 napi_test-hdf4-static napi_test_cpp-hdf4

HDF5_TARGETS = napi_test-hdf5 napi_test-hdf5-static napi_test_cpp-hdf5

XML_TARGETS = napi_test-xml napi_test-xml-static \
		napi_test-xml-table-static \
		napi_test_cpp-xml napi_test_cpp-xml-table

F77_TARGETS = napif_test-hdf4 napif_test-hdf5

F90_TARGETS = NXtest

if HAVE_CPP
CPP_TARGETS = leak_test2 leak_test3
endif

EXTRA_DIST	= testsuite.at $(TESTSUITE_AT) testsuite package.m4 \
			NXtest.f90 napif_test.f README.tests \
			napi_test.c napif4_test.f napif5_test.f \
			setup_test $(srcdir)/data/dmc* \
			nxbrowse.txt napi_test_cpp.cxx

TESTSUITE	= $(srcdir)/testsuite

TESTSUITE_AT	= ## dbase.at devices.at starter.at

AUTOTEST	= $(AUTOM4TE) --language=autotest

check_PROGRAMS = run_test skip_test $(HDF4_TARGETS) $(HDF5_TARGETS) $(F77_TARGETS) $(F90_TARGETS) $(XML_TARGETS) $(CPP_TARGETS) leak_test1

#nxtestdir=$(NXTESTDIR)
#nxtest_PROGRAMS = $(HDF4_TARGETS) $(HDF5_TARGETS) $(F77_TARGETS) $(F90_TARGETS) $(XML_TARGETS) leak_test1 leak_test2 leak_test3
#nxtest_DATA = README.tests

nxexampledir=$(NXEXAMPLEDIR)
nxexample_DATA = napi_test.c napif4_test.f napif5_test.f napif_test.f \
	NXtest.f90 napi_test_cpp.cxx

nxsummary:
	rm -f $@
	if test -r ../applications/NXsummary/$@$(EXEEXT); then \
	    ln -s ../applications/NXsummary/$@$(EXEEXT) $@; \
	fi

nxconvert:
	rm -f $@
	ln -s ../applications/$@$(EXEEXT) $@

nxvalidate:
	rm -f $@
	ln -s ../applications/$@$(EXEEXT) $@

nxbrowse:
	rm -f $@
	ln -s ../applications/$@$(EXEEXT) $@

nxdir:
	rm -f $@
	ln -s ../applications/NXdir/$@$(EXEEXT) $@

nxtranslate:
	rm -f $@
	if test -r ../applications/NXtranslate/$@$(EXEEXT); then \
	    ln -s ../applications/NXtranslate/$@$(EXEEXT) $@; \
	fi

# for running tests via   "make check"
## check_PROGRAMS = $(test_PROGRAMS)	# what to build
## TESTS = $(test_PROGRAMS)		# what to run

# this sets the test running environment - in case we
# have got built with shared HDF libraries we need to set LD_LIBRARY_PATH
TESTS_ENVIRONMENT=env LD_LIBRARY_PATH=@abs_top_builddir@/src/.libs:@abs_top_builddir@/bindings/cpp/.libs:$${LD_LIBRARY_PATH}:@EXTRA_LD_LIBRARY_PATH@:/usr/local/lib DYLD_LIBRARY_PATH=@abs_top_builddir@/src/.libs:@abs_top_builddir@/bindings/cpp/.libs:$${DYLD_LIBRARY_PATH}:@EXTRA_LD_LIBRARY_PATH@:/usr/local/lib

run_test_SOURCES=run_test.c
run_test_LDFLAGS=-static $(LDFLAGS)

skip_test_SOURCES=skip_test.c
skip_test_LDFLAGS=-static $(LDFLAGS)

leak_test1_SOURCES=leak_test1.c
leak_test1_LDADD=$(LIBNEXUS)
leak_test1_LDFLAGS=-static $(LDFLAGS)

leak_test2_SOURCES=leak_test2.cxx
leak_test2_LDADD=$(LIBNEXUS)
leak_test2_LDFLAGS=-static $(LDFLAGS)

leak_test3_SOURCES=leak_test3.cxx
leak_test3_LDADD=$(LIBNEXUS)
leak_test3_LDFLAGS=-static $(LDFLAGS)


if HAVE_XML
napi_test_xml_SOURCES = napi_test.c
napi_test_xml_CPPFLAGS = -I$(top_srcdir)/include @XML_CPPFLAGS@
napi_test_xml_LDADD = $(LIBNEXUS) @XML_LDFLAGS@
napi_test_xml_static_SOURCES = napi_test.c
napi_test_xml_static_CPPFLAGS = -I$(top_srcdir)/include @XML_CPPFLAGS@
napi_test_xml_static_LDADD = $(LIBNEXUS) @XML_LDFLAGS@
napi_test_xml_static_LDFLAGS = -static $(LDFLAGS)
napi_test_xml_table_static_SOURCES = napi_test.c
napi_test_xml_table_static_CPPFLAGS = -I$(top_srcdir)/include @XML_CPPFLAGS@
napi_test_xml_table_static_LDADD = $(LIBNEXUS) @XML_LDFLAGS@
napi_test_xml_table_static_LDFLAGS = -static $(LDFLAGS)
napi_test_cpp_xml_SOURCES = napi_test_cpp.cxx
napi_test_cpp_xml_LDADD = $(LIBNEXUSCPP)
napi_test_cpp_xml_table_SOURCES = napi_test_cpp.cxx
napi_test_cpp_xml_table_LDADD = $(LIBNEXUSCPP)
else
napi_test_xml_SOURCES = dummy.c
napi_test_xml_static_SOURCES = dummy.c
napi_test_xml_table_static_SOURCES = dummy.c
napi_test_xml_static_LDFLAGS = $(LDFLAGS)
napi_test_cpp_xml_SOURCES = dummy.c
napi_test_cpp_xml_table_SOURCES = dummy.c
endif

if HAVE_HDF4
napi_test_hdf4_SOURCES = napi_test.c
napi_test_hdf4_CPPFLAGS = -I$(top_srcdir)/include @HDF4_CPPFLAGS@
napi_test_hdf4_LDADD = $(LIBNEXUS) @HDF4_LDFLAGS@
napi_test_hdf4_static_SOURCES = napi_test.c
napi_test_hdf4_static_CPPFLAGS = -I$(top_srcdir)/include @HDF4_CPPFLAGS@
napi_test_hdf4_static_LDADD = $(LIBNEXUS) @HDF4_LDFLAGS@
napi_test_hdf4_static_LDFLAGS = -static $(LDFLAGS)
napi_test_cpp_hdf4_SOURCES = napi_test_cpp.cxx
napi_test_cpp_hdf4_LDADD = $(LIBNEXUSCPP)
if HAVE_F77
napif_test_hdf4_SOURCES = napif4_test.f
napif_test_hdf4_LDADD = $(LIBNEXUS77) # $(FLIBS)
napif_test_hdf4_LINK = $(F77LINK)
napif_test_hdf4_LDFLAGS = -static $(LDFLAGS)
else
napif_test_hdf4_SOURCES = dummy.c
napif_test_hdf4_LINK = $(LINK)
endif
else
napi_test_hdf4_SOURCES = dummy.c
napi_test_hdf4_static_SOURCES = dummy.c
napi_test_hdf4_static_LDFLAGS = $(LDFLAGS)
napif_test_hdf4_SOURCES = dummy.c
napif_test_hdf4_LINK = $(LINK)
napi_test_cpp_hdf4_SOURCES = dummy.c
endif

if HAVE_HDF5
napi_test_hdf5_SOURCES = napi_test.c
napi_test_hdf5_CPPFLAGS = -I$(top_srcdir)/include @HDF5_CPPFLAGS@
napi_test_hdf5_LDADD = $(LIBNEXUS) @HDF5_LDFLAGS@
napi_test_hdf5_static_SOURCES = napi_test.c
napi_test_hdf5_static_CPPFLAGS = -I$(top_srcdir)/include @HDF5_CPPFLAGS@
napi_test_hdf5_static_LDADD = $(LIBNEXUS) @HDF5_LDFLAGS@
napi_test_hdf5_static_LDFLAGS = -static $(LDFLAGS)
napi_test_cpp_hdf5_SOURCES = napi_test_cpp.cxx
napi_test_cpp_hdf5_LDADD=$(LIBNEXUSCPP)
if HAVE_F77
napif_test_hdf5_SOURCES = napif5_test.f
napif_test_hdf5_LDADD = $(LIBNEXUS77) # $(FLIBS)
napif_test_hdf5_LINK = $(F77LINK)
napif_test_hdf5_LDFLAGS = -static $(LDFLAGS)
else
napif_test_hdf5_SOURCES = dummy.c
napif_test_hdf5_LINK = $(LINK)
endif
else
napi_test_hdf5_SOURCES = dummy.c
napi_test_hdf5_static_SOURCES = dummy.c
napi_test_hdf5_static_LDFLAGS = $(LDFLAGS)
napif_test_hdf5_SOURCES = dummy.c
napif_test_hdf5_LINK = $(LINK)
napi_test_cpp_hdf5_SOURCES = dummy.c
endif

if HAVE_F90
NXtest_SOURCES = NXtest.f90
NXtest_LDADD = $(LIBNEXUS90)
NXtest_LINK = $(FCLINK)
NXtest_LDFLAGS = -static $(LDFLAGS)
else
NXtest_SOURCES = dummy.c
NXtest_LINK = $(LINK)
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

check-local: atconfig atlocal nxconvert nxvalidate nxsummary nxdir nxbrowse nxtranslate $(TESTSUITE) 
	$(TESTS_ENVIRONMENT) $(SHELL) $(TESTSUITE)

$(TESTSUITE):	$(srcdir)/testsuite.at $(TESTSUITE_AT) $(srcdir)/package.m4
	$(AUTOTEST) -I $(srcdir) $@.at -o $@.tmp
	mv $@.tmp $@

atconfig:	$(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@

clean-local:
	-$(SHELL) $(TESTSUITE) --clean


DISTCLEANFILES = atconfig
