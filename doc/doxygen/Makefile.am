## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id: Makefile.am 515 2005-06-22 08:33:09Z faa59 $
#

# documenttion subdirectories
nxdoxydir	= $(NXDOCDIR)/doxygen
CLEANFILES =  doxy.remove_me_to_remake

all : doxy.remove_me_to_remake

doxy.remove_me_to_remake :
if HAVE_DOXYGEN
if HAVE_PDFLATEX
if HAVE_DOT
	rm -fr latex html
	$(DOXYGEN) Doxyfile
	( cd latex; ) # ( cd latex; make pdf )
	rm -f refman.pdf
	ln -s latex/refman.pdf .
endif
endif
endif
	touch doxy.remove_me_to_remake

dist-hook :
if HAVE_DOXYGEN
if HAVE_PDFLATEX
if HAVE_DOT
	if test -f $(srcdir)/refman.pdf; then \
	    cp $(srcdir)/refman.pdf $(distdir); \
	fi
	if test -d $(srcdir)/html; then \
		cp -r $(srcdir)/html $(distdir); \
		find $(distdir)/html -depth -type d -name '.svn' -exec rm -fr {} \; ;\
	fi
endif
endif
endif

install-data-local :
if HAVE_DOXYGEN
if HAVE_PDFLATEX
if HAVE_DOT
	$(mkinstalldirs) $(DESTDIR)$(nxdoxydir)
	test -f $(srcdir)/refman.pdf && cp $(srcdir)/refman.pdf $(DESTDIR)$(nxdoxydir)
	if test -d $(srcdir)/html; then \
		cp -r $(srcdir)/html $(DESTDIR)$(nxdoxydir); \
		find $(DESTDIR)$(nxdoxydir) -type f -exec chmod 0644 {} \; ;\
		find $(DESTDIR)$(nxdoxydir) -type d -exec chmod 0755 {} \; ;\
	fi
endif
endif
endif

clean-local :
	rm -fr html latex refman.pdf

uninstall-local :
	rm -fr $(DESTDIR)$(nxdoxydir)

