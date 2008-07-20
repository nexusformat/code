## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id: Makefile.am 515 2005-06-22 08:33:09Z faa59 $
#

# documenttion subdirectories
nxdoxydir	= $(NXDOCDIR)/doxygen
CLEANFILES =  doxy.remove_me_to_remake

if HAVE_DOXYGEN
if HAVE_PDFLATEX
if HAVE_DOT
all : doxy.remove_me_to_remake

doxy.remove_me_to_remake :
	rm -fr latex html
	$(DOXYGEN) Doxyfile
	( cd latex; make pdf )
	rm -f refman.pdf
	ln -s latex/refman.pdf .
	touch doxy.remove_me_to_remake
endif
endif
endif

clean-local :
	rm -fr html latex refman.pdf

dist-hook :
	cp $(srcdir)/refman.pdf $(distdir)
	test -d $(srcdir)/html && cp -r $(srcdir)/html $(distdir)
	find $(distdir)/html -depth -type d -name '.svn' -exec rm -fr {} \;

install-data-local :
	$(mkinstalldirs) $(DESTDIR)$(nxdoxydir)
	cp $(srcdir)/refman.pdf $(DESTDIR)$(nxdoxydir)
	test -d $(srcdir)/html && cp -r $(srcdir)/html $(DESTDIR)$(nxdoxydir)
	find $(DESTDIR)$(nxdoxydir) -type f -exec chmod 0644 {} \;
	find $(DESTDIR)$(nxdoxydir) -type d -exec chmod 0755 {} \;

uninstall-local :
	rm -fr $(DESTDIR)$(nxdoxydir)

