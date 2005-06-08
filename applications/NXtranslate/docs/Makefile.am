## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id$
#
DOC_SRC=NXtranslate.docbook overview.docbook \
		translation.docbook retrievers.docbook
DOC_OUTPUT=NXtranslate.pdf NXtranslate.txt

nxtranslatedocdir		= $(NXDOCDIR)/NXtranslate
nxtranslatedoc_DATA		= $(DOC_OUTPUT)
EXTRA_DIST 			= $(DOC_OUTPUT) $(DOC_SRC)

NXtranslate.pdf: $(DOC_SRC)
	docbook2pdf NXtranslate.docbook

NXtranslate.txt: $(DOC_SRC)
	docbook2txt NXtranslate.docbook
