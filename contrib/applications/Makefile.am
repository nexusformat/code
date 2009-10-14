## Process this file with automake to produce Makefile.in
#====================================================================
#  
# $Id$
#
# @configure_input@
#
#====================================================================
#if HAVE_LIBXML2
#NXTRANSLATE=NXtranslate
#endif
#SUBDIRS = NXdir $(NXTRANSLATE)
if HAVE_CBFLIB
CBFLIB=CBFLib
endif
#if HAVE_LIBMXMLPLUS
NXEXTRACT=NXextract
#endif
SUBDIRS=$(CBFLIB) $(NXEXTRACT)
