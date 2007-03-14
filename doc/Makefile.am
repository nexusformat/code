## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id$
#

# documenttion subdirectories
if HAVE_DOXYGEN
if HAVE_PDFLATEX
if HAVE_DOT
DOXYSUB=doxygen
endif
endif
endif
SUBDIRS=api nxdict tech_ref $(DOXYSUB)

nxdocdir		= $(NXDOCDIR)

# add files and example code here from this directory
nxdoc_DATA		= README.doc

EXTRA_DIST		= README.doc
