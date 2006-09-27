## Process this file with automake to produce Makefile.in
#====================================================================
# $Id: Makefile.am,v 1.3 2005/11/15 22:05:48 pfp Exp $
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
AM_CPPFLAGS=-I. -I$(top_srcdir)/include

noinst_LTLIBRARIES = libEdf.la

libEdf_la_SOURCES = \
	edf_reader.cpp  edf_reader.h  \
	edf_retriever.cpp  edf_retriever.h

#EXTRA_DIST = CHANGES LICENSE README TODO

include $(top_srcdir)/build_rules.am
