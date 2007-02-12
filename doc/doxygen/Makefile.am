## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id: Makefile.am 515 2005-06-22 08:33:09Z faa59 $
#

# documenttion subdirectories
nxdocdir	= $(NXDOCDIR)

if HAVE_DOXYGEN
if HAVE_PDFLATEX
DOXY_STUFF = refman.pdf # html

refman.pdf :
	$(DOXYGEN) Doxyfile
	( cd latex; make pdf )
	cp latex/refman.pdf $@
endif
endif

nxdoc_DATA 	= $(DOXY_STUFF)

EXTRA_DIST = $(DOXY_STUFF)
