## Process this file with automake to produce Makefile.in
#====================================================================
#
# $Id: Makefile.am 515 2005-06-22 08:33:09Z faa59 $
#

# documenttion subdirectories
nxdoxydir	= $(NXDOCDIR)/doxygen
CLEANFILES =  doxy.remove_me_to_remake c-api.tag

all : doxy.remove_me_to_remake

doxy.remove_me_to_remake :
if HAVE_DOXYGEN
if HAVE_PDFLATEX
if HAVE_DOT
	rm -fr latex html latex-* html-*
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_c_brief
	( cd latex-c; make pdf )
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_cpp
	( cd latex-cpp; ) # ( cd latex-cpp; make pdf )
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_java
	( cd latex-java; ) # ( cd latex-java; make pdf )
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_python
	( cd latex-python; ) # ( cd latex-python; make pdf )
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_f90
	( cd latex-f90; ) # ( cd latex-f90; make pdf )
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_f77
	( cd latex-f77; ) # ( cd latex-f77; make pdf )
	env TOPSRC=$(top_srcdir) $(DOXYGEN) $(srcdir)/Doxyfile_nxvalidate
	( cd latex-nxvalidate; ) # ( cd latex-nxvalidate; make pdf )
	rm -f refman.pdf
	ln -s latex-c/refman.pdf .
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
	if test -d $(srcdir)/html-c; then \
	  for i in $(srcdir)/html-*; do \
	    if test -d $$i; then \
		cp -r $$i $(distdir); \
	    fi; \
	  done; \
	  find $(distdir)/html-* -depth -type d -name '.svn' -exec rm -fr {} \; ; \
	fi
endif
endif
endif

install-data-local :
	$(mkinstalldirs) $(DESTDIR)$(nxdoxydir)
	if test -f $(srcdir)/refman.pdf; then \
	    cp $(srcdir)/refman.pdf $(DESTDIR)$(nxdoxydir); \
        fi
	if test -d $(srcdir)/html-c; then \
	  for i in $(srcdir)/html-*; do \
		cp -r $$i $(DESTDIR)$(nxdoxydir); \
	  done; \
	  find $(DESTDIR)$(nxdoxydir) -type f -exec chmod 0644 {} \; ;\
	  find $(DESTDIR)$(nxdoxydir) -type d -exec chmod 0755 {} \; ;\
	fi

clean-local :
	rm -fr html-* latex-* refman.pdf

uninstall-local :
	rm -fr $(DESTDIR)$(nxdoxydir)

EXTRA_DIST=$(srcdir)/Doxyfile_*
