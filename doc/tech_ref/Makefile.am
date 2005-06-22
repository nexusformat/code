## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id$
#
IMAGES=tofnpd.jpg tofnpd.gif
DOC_SRC=NeXus_definitions.docbook terminology.docbook methods.docbook base_classes.docbook tofnpd.docbook tofnref.docbook monoref.docbook tofdgs.docbook monotas.docbook

nxtechrefdocdir		= $(NXDOCDIR)/tech_ref
nxtechrefdoc_DATA		= $(DOC_OUTPUT)
EXTRA_DIST 			= $(DOC_OUTPUT) $(DOC_SRC)

if HAVE_DOCBOOK
DOC_OUTPUT=NeXus_definitions.pdf NeXus_definitions.txt
NeXus_definitions.pdf: $(DOC_SRC) $(IMAGES)
	$(DOCBOOK2PDF) NeXus_definitions.docbook

NeXus_definitions.txt: $(DOC_SRC)
	$(DOCBOOK2TXT) NeXus_definitions.docbook
endif
