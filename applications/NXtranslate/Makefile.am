## Process this file with automake to produce Makefile.in
#====================================================================
# $Id$
#
# Copyright (c) 2004, P.F.Peterson <petersonpf@ornl.gov>
#               Spallation Neutron Source at Oak Ridge National Laboratory
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# @configure_input@
#
#====================================================================
LIBNEXUS=$(top_builddir)/src/libNeXus.la

AM_CPPFLAGS=-I. -I$(top_srcdir)/include -IIPNS_CPP -DIPNS_RETRIEVER -DFRM2_RETRIEVER -DTEXT_COLLIST_RETRIEVER -DTEXT_PLAIN_RETRIEVER -DTEXT_XML_RETRIEVER -DDYNAMIC_RETRIEVER -DSNS_HISTOGRAM_RETRIEVER -DLOOPY_RETRIEVER `xml2-config --cflags` @HDF4_CPPFLAGS@ @HDF5_CPPFLAGS@ @XML_CPPFLAGS@

if HAVE_OPENGENIE
GENIESUBDIR=opengenie
endif
SUBDIRS=IPNS_CPP text_collist text_plain text_xml sns_histogram docs \
	FRM2 loopy $(GENIESUBDIR)

bin_PROGRAMS = nxtranslate
nxtranslate_SOURCES = \
	attr.cpp main.cpp nexus_retriever.cpp  nexus_util.cpp node.cpp \
	node_util.cpp retriever.cpp string_util.cpp xml_parser.cpp \
        xml_util.h xml_util.cpp \
	attr.h nexus_retriever.h nexus_util.h node.h node_util.h \
	nxtranslate_debug.h Ptr.h retriever.h string_util.h xml_parser.h \
	tree.hh dynamic_retriever.cpp dynamic_retriever.h

EXTRA_nxtranslate_SOURCES=IPNS_CPP/ipns_retriever.h \
	text_collist/collist_retriever.h \
	text_plain/retriever.h \
	text_xml/retriever.h \
        sns_histogram/retriever.h \
	FRM2/frm2_retriever.h \
        loopy/retriever.h

nxtranslate_LDADD = $(LIBNEXUS) -lxml2 \
	IPNS_CPP/libIPNSRunfile.la text_collist/libTextCollist.la \
	text_plain/libTextPlain.la text_xml/libTextXML.la \
	sns_histogram/libSNShistogram.la \
	FRM2/libFRM2.la \
        loopy/libLoopy.la

nxtranslate_LDFLAGS = -static $(LDFLAGS)

#EXTRA_DIST = CHANGES LICENSE README TODO

include $(top_srcdir)/build_rules.am

test_dynamic.nxs: nxtranslate test_dynamic.xml test_dynamic.so
	env LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:. ./nxtranslate test_dynamic.xml

test_dynamic.so : test_dynamic.c
	gcc -I$(top_srcdir)/include -shared -o $@ test_dynamic.c

test_opengenie: nxtranslate test_opengenie.xml
	( cd opengenie; for i in *.raw; do \
             echo "Processing  $$i"; \
	     for j in 4 5; do \
		env LD_LIBRARY_PATH=.:.libs:../../../src/.libs:@OPENGENIEROOT@/genie:$$LD_LIBRARY_PATH GENIE_SMALLTALK_IMAGE=@OPENGENIEROOT@/genie/genie.im GENIE_GROWTO_IMAGE_SIZE=64 NXGENIE_INIT=nxgenie.gcl ../nxtranslate --hdf$$j -DINPUT_FILE=$$i -o $$i.nx$$j ../test_opengenie.xml; \
          done; done )

test_simple.nxs: nxtranslate test_simple.xml
	./nxtranslate test_simple.xml

test_nexus.nxs: nxtranslate test_nexus.xml
	./nxtranslate test_nexus.xml

test_nexus_macro.nxs: nxtranslate test_nexus_macro.xml
	./nxtranslate test_nexus_macro.xml -DFILE=test_simple.nxs

test_ipns.nxs: nxtranslate test_ipns.xml hrcs1797.run
	./nxtranslate test_ipns.xml -DfileName=hrcs1797.run

test_text_plain.nxs: nxtranslate test_text_plain.xml test_simple.xml
	./nxtranslate test_text_plain.xml

test-clean:
	rm -f $(TEST_RESULT)


