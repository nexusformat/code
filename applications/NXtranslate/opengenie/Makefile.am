## Process this file with automake to produce Makefile.in
#====================================================================
# $Id$
#
# Copyright (c) 2005, Freddie Akeroyd <F.A.Akeroyd@rl.ac.uk>
#               CCLRC ISIS Facility
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
AM_CPPFLAGS=-I. -I$(top_srcdir)/include -Wall

lib_LTLIBRARIES = libnxgenie.la

libnxgenie_la_SOURCES = \
	opengenie.c geniedefs.h genie_data_access.h
libnxgenie_la_LIBADD = -lgenie
libnxgenie_la_LDFLAGS = -no-undefined -export-dynamic -module $(LDFLAGS)

EXTRA_DIST = README

include $(top_srcdir)/build_rules.am
