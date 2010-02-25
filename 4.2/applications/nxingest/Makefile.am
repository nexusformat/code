## Process this file with automake to produce Makefile.in
LIBNEXUS=$(top_builddir)/src/libNeXus.la
AM_CPPFLAGS=-I. -I$(top_srcdir)/include $(XML_CPPFLAGS)

bin_PROGRAMS = nxingest
nxingestdocdir = $(NXDOCDIR)/nxingest
nxingestdoc_DATA = nxingest.txt gpl.txt

nxingest_SOURCES = nxingest_main.cpp nxingest_utils.cpp \
   nxingest_debug.cpp nxingest_nexus.cpp nxingest_parse.cpp nxingest_time.cpp \
   nxingest_debug.h nxingest.h nxingest_main.h nxingest_nexus.h \
   nxingest_parse.h nxingest_time.h nxingest_utils.h
nxingest_LDADD = $(LIBNEXUS)
nxingest_LDFLAGS = -static $(HDF4_LDFLAGS) $(HDF5_LDFLAGS) $(XML_LDFLAGS) $(LDFLAGS)

EXTRA_DIST=nxingest.txt gpl.txt

include $(top_srcdir)/build_rules.am
