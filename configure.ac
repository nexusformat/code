dnl====================================================================
dnl  NeXus - Neutron & X-ray Common Data Format
dnl  
dnl  Autoconf (configure script) generation file
dnl  
dnl  $Id$
dnl
dnl  Copyright (C) 2004 Freddie Akeroyd
dnl  
dnl  This library is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public
dnl  License as published by the Free Software Foundation; either
dnl  version 2 of the License, or (at your option) any later version.
dnl 
dnl  This library is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  Lesser General Public License for more details.
dnl 
dnl  You should have received a copy of the GNU Lesser General Public
dnl  License along with this library; if not, write to the Free 
dnl  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
dnl  MA  02111-1307  USA
dnl             
dnl  For further information, see <http://www.neutron.anl.gov/NeXus/>
dnl
dnl                       -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.
dnl

AC_PREREQ(2.52)
AC_REVISION($Revision$)
AC_INIT([NeXus Library], [3.0.1], [nexus-developers@anl.gov], [nexus])
AC_COPYRIGHT([Copyright (C) 2004 Freddie Akeroyd 
This software is covered by the GNU LESSER GENERAL PUBLIC LICENSE
see file COPYING for further information])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/napi.c])
AC_CANONICAL_TARGET

dnl $EGREP is used in macros included from acinclude.m4
AC_PROG_EGREP

dnl check for correct autoconf version - the same check is done in autogen.sh
if test ! -z "$srcdir"; then
    . $srcdir/autoversion.sh
else
    . ./autoversion.sh
fi

AM_INIT_AUTOMAKE([1.6])


AM_CONFIG_HEADER(include/nxconfig.h:include/nxconfig_h.in)
dnl Checks for programs.
AC_ARG_WITH([cc], AC_HELP_STRING([--with-cc=c compiler], [Specify name of C compiler]),
	[], [with_cc=yes])
if test x"$with_cc" != x"no" ; then
	if test x"$with_cc" != x"yes" ; then CC="$with_cc"; fi
	AC_PROG_CC
	dnl Check C compiler options
	AC_CHECK_CPP_OPTION([-fno-common])
else
	AC_MSG_ERROR([You need a C compiler to compile this package])
fi

AC_ARG_WITH([cxx], AC_HELP_STRING([--with-cxx=c++ compiler], [Specify name of C++ compiler]), [], [with_cxx=yes])
if test x"$with_cxx" != x"no" ; then
	if test x"$with_cxx" != x"yes" ; then CXX="$with_cxx"; fi
	AC_PROG_CXX
else
	CXX=
fi

AC_ARG_WITH([f90], AC_HELP_STRING([--with-f90=f90 compiler], [Specify name of FORTRAN 90 compiler]),
	[], [with_f90=no])
if test x"$with_f90" != x"no" ; then
	if test x"$with_f90" = x"yes" ; then
		AC_CHECK_PROGS(FC, [g95 f90 f95 ifort])
	else
		AC_CHECK_PROGS(FC, ["$with_f90"])
	fi
	dnl f90 has a "module path" option - hard to get as you really need a
	dnl ready compiled module to point the option at so for now we are just
	dnl check that the option takes a directory and the compiler works
	mytop=`pwd`
	FCFLAGS="$FCFLAGS -I$mytop/bindings/f90"
	dnl AC_CHECK_F90_OPTION([-p $mytop/bindings/f90],[      INCLUDE 'NXmodule.f90'])
	dnl AC_CHECK_F90_OPTION([-p $mytop/bindings/f90],[      INCLUDE 'NXmodule.f90'])
	dnl AC_CHECK_F90_OPTION([-qextname])
	AC_FC_LIBRARY_LDFLAGS
#        if test x$with_f77=xno; then with_f77=$FC; fi
        if test x$with_f77=xno; then with_f77=yes; fi
else
	FC=
fi

AC_ARG_WITH([f77], AC_HELP_STRING([--with-f77=f77 compiler], [Specify name of FORTRAN 77 compiler]),
	[], [with_f77=no])
if test x"$with_f77" != x"no" ; then
	if test x"$with_f77" != x"yes" ; then F77="$with_f77"; fi
	AC_PROG_F77
	dnl Check FORTRAN compiler options
	dnl 	AC_CHECK_F77_OPTION([-Wno-globals])
	dnl 	AC_CHECK_F77_OPTION([-fno-common])  # use with caution
	mytop=`pwd`
	dnl get right F77 include option
	AC_CHECK_F77_OPTION([-I$mytop/bindings/f77],[      INCLUDE 'napif.inc'])
	AC_F77_LIBRARY_LDFLAGS
else
	F77=
fi

AC_ARG_WITH([swig], AC_HELP_STRING([--with-swig=swig compiler], [Specify name of swig compiler]), [], [with_swig=no])
if test x$with_swig != xno; then
    if test x$with_swig != xyes; then
	SWIG=$with_swig
    fi
    AC_CHECK_PROGS(SWIG,[swig])
else
    SWIG=
fi

java_host="linux"
javaroot=""
with_java=no
AC_ARG_WITH([java-home], AC_HELP_STRING([--with-java-home=JAVA SDK Home], [Specify location of top directory of JAVA SDK i.e. where the bin, lib and include directories live]),
	[], [with_java_home=$withval])
if test -d "$with_java_home"; then
	javaroot="$with_java_home"
	AC_MSG_NOTICE([Java SDK home is $javaroot])
	JAVAINCLUDE="-I$javaroot/include"
	AC_PATH_PROGS(JAVAC,javac,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAVADOC,javadoc,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAR,jar,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAVA,java,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAVAH,javah,,[$javaroot/bin:$PATH])
fi

#AC_ARG_WITH([java], AC_HELP_STRING([--with-java=java compiler], [Specify name of Java compiler]),
#	[], [with_java=yes])
#if test x$with_java != xno ; then
#	if test -z "$javaroot"; then
#	    javapath=`which java`
#	    if test ! -z "$javapath"; then javaroot=`dirname $javapath`/..; fi
#	fi
#	if test -z "$javaroot"; then 
#	    with_java=no
#	    AC_MSG_ERROR([Unable to determine JAVA SDK directory - please specify this using  --with-java-home=/sdk/directory  or instead use  --without-java])
#	fi
#	AC_MSG_NOTICE([Java SDK home is $javaroot])
#	if test x"$with_java" = x"yes" ; then
#		AC_PATH_PROGS(JAVAC,javac,,[$javaroot/bin:$PATH])
#	else
#		AC_PATH_PROGS(JAVAC,["$with_java"],,[$javaroot/bin:$PATH])
#	fi
#	AC_PATH_PROGS(JAVADOC,javadoc,,[$javaroot/bin:$PATH])
#	AC_PATH_PROGS(JAR,jar,,[$javaroot/bin:$PATH])
#	AC_PATH_PROGS(JAVA,java,,[$javaroot/bin:$PATH])
#	AC_PATH_PROGS(JAVAH,javah,,[$javaroot/bin:$PATH])
#	JAVAINCLUDE="-I$javaroot/include"
#else
#	JAVAC=
#	JAVAINCLUDE=
#fi
AC_SUBST(SHARED_LDFLAGS)
AC_SUBST(JAVAINCLUDE)
AC_SUBST(PACKAGE_RELEASE)
test -z "$PACKAGE_RELEASE" && PACKAGE_RELEASE=1

NXDOCDIR='${prefix}/nexus/doc'
NXEXAMPLEDIR='${prefix}/nexus/examples'
NXTESTDIR='${prefix}/nexus/tests'
AC_SUBST(NXDOCDIR)
AC_SUBST(NXEXAMPLEDIR)
AC_SUBST(NXTESTDIR)

AM_PROG_GCJ
AM_PATH_PYTHON(,, :)

AC_PROG_LIBTOOL

AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_CHECK_PROGS(DOCBOOK2PDF,[docbook2pdf])
AC_CHECK_PROGS(DOCBOOK2TXT,[docbook2txt])
AC_CHECK_PROGS(TCLSH,[tclsh])
AC_CHECK_PROGS(GUILE,[guile])
AC_CHECK_PROGS(LATEX,[latex])

AC_CHECK_ROOT([tcl],[TCLROOT],[/usr /usr/local],[include/tcl.h])
AC_CHECK_ROOT([guile],[GUILEROOT],[/usr /usr/local],[include/libguile.h])
AC_CHECK_ROOT([python],[PYTHONROOT],[/usr /usr/local],[include/python$PYTHON_VERSION/Python.h])
AC_CHECK_ROOT([opengenie],[OPENGENIEROOT],[/usr/local],[genie/genie.so])

dnl  flags for building swig guile bindings
AC_SUBST(SWGUILEFLAGS)  
if ! test -z "$GUILEROOT"; then
    if test `$SWIG -guile -help 2>&1 | grep -i '\-linkage' | wc -l` -gt 0; then
	SWGUILEFLAGS="-Linkage ltdlmod"
    else
	SWGUILEFLAGS=""
    fi
fi

dnl These are used in build_rules.am to pass any approptiate 
dnl tag to libtool for f90 building
#if test `$EGREP "available_tags=.*F77" libtool | wc -l` -gt 0; then
#    LTF90COMPILETAG="--tag=F77"
#    LTF90LINKTAG="--tag=CC"
#else
#    LTF90COMPILETAG=""
#    LTF90LINKTAG=""
#fi
#AC_SUBST(LTF90COMPILETAG)
#AC_SUBST(LTF90LINKTAG)

dnl
dnl First any host specific bits
dnl
MINGW_MSYS=no
HDF_EXT=a # library file extent to look for with HDF libraries
case $host in
    alpha*-dec-osf*)
	java_host="alpha"
	if test "$CC" = "cc" -o "$CC" = "c89"; then
	    CPPFLAGS="$CPPFLAGS -pthread"
	fi
	if test "$CXX" = "cxx"; then
	    CPPFLAGS="$CPPFLAGS -pthread -D__USE_STD_IOSTREAM"
	    CXXFLAGS="$CXXFLAGS -std gnu"
	fi
	;;

    *mingw*)
	SHARED_LDFLAGS="-no-undefined"
	MINGW_MSYS=yes
	HDF_EXT=dll
	;;

    *darwin*)
	dnl -no-cpp-precomp needed on MacOS-X (and others?)
	AC_CHECK_C_OPTION([-no-cpp-precomp])
	CPPFLAGS="$CPPFLAGS -D__unix"
	CFLAGS="$CFLAGS -g"
	FFLAGS="$FFLAGS -g -Wno-globals"
dnl This is a hack for libtool - for some reason the F77 tag
dnl does not get set with the commands to create a shared library
	AC_MSG_NOTICE([Patching libtool for F77 shared linking])
	sed -e "s/^archive_cmds=\"\"//" < libtool > libtool.$$
	mv -f libtool.$$ libtool
	chmod +x libtool
	JAVAINCLUDE="-I/System/Library/Frameworks/JavaVM.framework/Headers"
dnl	F90FLAGS="$F90FLAGS -g -cons -YEXT_NAMES=LCS -YEXT_SFX=_"
	FCFLAGS="$FCFLAGS -qextname"
	SHARED_LDFLAGS="-Wl,-single_module"
#	LTF90COMPILETAG="--tag=F77"
#	LTF90LINKTAG="--tag=CC"
dnl	LDFLAGS="$LDFLAGS -flat_namespace"
dnl     if $CC -dumpspecs 2>&1 | grep 'single_module' >/dev/null ; then
dnl           lt_int_apple_cc_single_mod=yes
dnl     fi
dnl     if test "X$lt_int_apple_cc_single_mod" = Xyes ; then
dnl           archive_cmds='$CC -dynamiclib -single_module'
dnl	fi
	;;
esac

if test ! -z "$JAVAINCLUDE" -a ! -z "$java_host"; then
    JAVAINCLUDE="$JAVAINCLUDE -I$javaroot/include/$java_host"
fi

dnl
dnl * locate path to HDF libraries *
dnl
dnl first see if one has been given
dnl
H4ROOT=""
H4SUBROOT=""
H5ROOT=""
MXMLROOT=""
AC_SUBST(H4ROOT)
AC_SUBST(H5ROOT)
AC_SUBST(MXMLROOT)
AC_ARG_WITH([hdf4],
	AC_HELP_STRING([--with-hdf4=/path/to/hdf4],
                       [Specify location of HDF4 files]),
	[if test $withval != no; then H4ROOT=$withval; fi])
AC_ARG_WITH([hdf5],
	AC_HELP_STRING([--with-hdf5=/path/to/hdf5],
                       [Specify location of HDF5 files]),
	[if test $withval != no; then H5ROOT=$withval; fi])
AC_ARG_WITH([xml],
	AC_HELP_STRING([--with-xml=/path/to/mxml],
                       [Specify location of MXML library files]),
	[if test $withval != no; then MXMLROOT=$withval; fi])
dnl otherwise try and find HDF path, but not if
dnl e.g. --without-hdf4 has been specified (hence check on $with_hdf4 != no )
dnl first HDF4
HDF4SEARCH="/usr/local/hdf4 /usr/local/hdf /usr/local /sw /usr"
if test "$with_hdf4" != "no" -a -z "$H4ROOT"; then
    AC_MSG_CHECKING(for location of HDF4 libraries)
    for i in $HDF4SEARCH; do
        for j in lib64 lib; do
	    if test -z "$H4ROOT" -a -r ${i}/$j/libdf.$HDF_EXT; then H4ROOT=$i; fi
        done
    done
    for i in lib64 lib; do
        if test -r /usr/$i/hdf/libdf.$HDF_EXT; then
	    H4ROOT=/usr
	    H4SUBROOT=hdf
        fi
    done
    if test -z "$H4ROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($H4ROOT)
    fi
fi
if test "$H4ROOT"; then
    H4VERSION=`grep LIBVER_STRING ${H4ROOT}/include/${H4SUBROOT}/hfile.h | cut -d '"' -f 2 | cut -d ' ' -f4,6 | tr ' ,' '. '`
    case $H4VERSION in
	4.[[12]]*) 
    		HDF4_LDFLAGS="-lmfhdf -ldf";
		for j in lib lib64; do
		    if test -d $H4ROOT/$j/$H4SUBROOT; then LDFLAGS="-L$H4ROOT/$j/$H4SUBROOT $LDFLAGS"; fi
		done
    		HDF4_CPPFLAGS="-I$H4ROOT/include/$H4SUBROOT -DHDF4" ;;
	*) 	AC_MSG_WARN([The HDF 4 installation has not the right version ($H4VERSION). You need at least 4.1])
		H4ROOT="";;
    esac		
fi
AC_SUBST(HDF4_LDFLAGS)
AC_SUBST(HDF4_CPPFLAGS)

dnl HDF5
HDF5SEARCH="/usr/local/hdf5 /usr/local/hdf /usr/local /sw /usr"
if test "$with_hdf5" != "no" -a -z "$H5ROOT"; then
    AC_MSG_CHECKING(for location of HDF5 libraries)
    for i in $HDF5SEARCH; do
	for j in lib lib64; do
	    if test -z "$H5ROOT" -a -r ${i}/$j/libhdf5.$HDF_EXT; then H5ROOT=$i; fi
        done
    done
    if test -z "$H5ROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($H5ROOT)
    fi
fi
if test "$H5ROOT"; then
    H5VERSION=`grep H5_VERS_INFO ${H5ROOT}/include/H5public.h | cut -d '"' -f 2 | cut -d ' ' -f 4`
    case $H5VERSION in
	1.[[6]]*)
    		HDF5_LDFLAGS="-lhdf5 -lz"
		for j in lib lib64; do
		    if test -d $H5ROOT/$j; then LDFLAGS="-L$H5ROOT/$j $LDFLAGS"; fi
		done
    		HDF5_CPPFLAGS="-I$H5ROOT/include -DHDF5";;
	*)	AC_MSG_WARN([The HDF 5 installation has not the right version ($H5VERSION). You need at least 1.6]) 
		H5ROOT="";;
    esac
fi
AC_SUBST(HDF5_LDFLAGS)
AC_SUBST(HDF5_CPPFLAGS)

dnl XML
MXMLSEARCH="/usr /usr/local"
if test "$with_xml" != "no" -a -z "$MXMLROOT"; then
    AC_MSG_CHECKING(for location of mxml package)
    for i in $MXMLSEARCH; do
	if test -z "$MXMLROOT" -a -r ${i}/include/mxml.h; then MXMLROOT=$i; fi
    done
    if test -z "$MXMLROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($MXMLROOT)
    fi
fi
if test "$MXMLROOT"; then
    XML_LDFLAGS="-lmxml"
    LDFLAGS="-L$XMLROOT/lib $LDFLAGS"
    XML_CPPFLAGS="-I$MXMLROOT/include -DNXXML"
fi
AC_SUBST(XML_LDFLAGS)
AC_SUBST(XML_CPPFLAGS)
if test -d /usr/local/lib; then
    LDFLAGS="$LDFLAGS -L/usr/local/lib"
fi
AC_SUBST(LIBG2C)

dnl finally set LIBS variable if we 
#AC_CHECK_LIB(g2c,c_sqrt,LIBG2C="-lg2c",LIBG2C="")
AC_CHECK_LIB(SystemStubs, fprintf$LDBLStub)
case $host in
    *mingw*)
	;;
    *)
	AC_CHECK_LIB(m, log)
esac
AC_CHECK_LIB(rpc, xdr_float)
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(z, gzopen)
AC_CHECK_LIB(jpeg, jpeg_CreateCompress)
AC_CHECK_LIB(sz, SZ_Compress)
AC_CHECK_LIB(df, Hopen)
AC_CHECK_LIB(mfhdf, ncopen)
AC_CHECK_LIB(hdf5, H5open)
AC_CHECK_LIB(xml2, xmlParseDocument)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([ftime memset strchr strdup strrchr strstr tzset])

AM_CONDITIONAL(HAVE_F77, [test ! -z "$F77"])
AM_CONDITIONAL(HAVE_F90, [test ! -z "$FC"])
AM_CONDITIONAL(HAVE_PYTHON, [test ! -z "$PYTHONROOT"])
AM_CONDITIONAL(HAVE_TCL, [test ! -z "$TCLROOT"])
AM_CONDITIONAL(HAVE_GUILE, [test ! -z "$GUILEROOT"])
AM_CONDITIONAL(HAVE_SWIG, [test ! -z "$SWIG"])
AM_CONDITIONAL(HAVE_JAVA, [test ! -z "$JAVA"])
AM_CONDITIONAL(HAVE_JAVAC, [test ! -z "$JAVAC"])
AM_CONDITIONAL(HAVE_JAVADOC, [test ! -z "$JAVADOC"])
AM_CONDITIONAL(HAVE_HDF4, [test ! -z "$H4ROOT"])
AM_CONDITIONAL(HAVE_HDF5, [test ! -z "$H5ROOT"])
AM_CONDITIONAL(HAVE_XML, [test ! -z "$MXMLROOT"])
AM_CONDITIONAL(HAVE_LIBXML2, [ test "$ac_cv_lib_xml2_xmlParseDocument" = "yes" ])
AM_CONDITIONAL(HAVE_DOCBOOK, [test ! -z "$DOCBOOK2TXT"])
AM_CONDITIONAL(HAVE_LATEX, [test ! -z "$LATEX"])
#AM_CONDITIONAL(INSTALL_NXPYTHON, [test ! -z "$INSTALL_NXPYTHON"])
AM_CONDITIONAL(HAVE_MZSCHEME, [test ! -z "$INSTALL_NXPYTHON"])
AM_CONDITIONAL(MINGW_MSYS, [test x$MINGW_MSYS = xyes])
AM_CONDITIONAL(HAVE_OPENGENIE, [test ! -z "$OPENGENIEROOT"])

AC_CONFIG_TESTDIR(test)
AC_CONFIG_FILES(test/Makefile test/atlocal)
AM_MISSING_PROG([AUTOM4TE], [autom4te])

LINUX_DISTRIBUTION

AC_CONFIG_FILES([Makefile
		nexus.spec:nexus_spec.in
		build_rpm
		include/Makefile
		src/Makefile 
		applications/Makefile
		applications/NXdir/Makefile
		applications/NXtranslate/Makefile
		applications/NXtranslate/IPNS_CPP/Makefile
		applications/NXtranslate/FRM2/Makefile
		applications/NXtranslate/opengenie/Makefile
		applications/NXtranslate/text_collist/Makefile
		applications/NXtranslate/text_plain/Makefile
		applications/NXtranslate/text_xml/Makefile
		applications/NXtranslate/sns_histogram/Makefile
		applications/NXtranslate/docs/Makefile
		examples/Makefile
		doc/Makefile
		doc/api/Makefile
		doc/nxdict/Makefile
		doc/tech_ref/Makefile
		bindings/Makefile 
		bindings/f77/Makefile
		bindings/f90/Makefile
		bindings/java/Makefile
		bindings/swig/Makefile
		contrib/Makefile
		contrib/applications/Makefile
		contrib/bindings/Makefile
		scripts/Makefile
		scripts/nexus.pc:scripts/nexus_pc.in
		scripts/nxbuild])
dnl src/nxdict/Makefile
AC_OUTPUT

HDF4SUPPORT=`if test -n "$H4ROOT" ; then echo yes ; else echo no ; echo "                found version $H4VERSION"; fi`
HDF5SUPPORT=`if test -n "$H5ROOT" ; then echo yes ; else echo no ; echo "                found version $H5VERSION"; fi`
XMLSUPPORT=`if test -n "$MXMLROOT" ; then echo yes ; else echo no ; fi`
F77BINDING=`if test -n "$F77" ; then echo yes ; else echo no ; fi`
F90BINDING=`if test -n "$FC" ; then echo yes ; else echo no ; fi`
JAVABINDING=`if test -n "$JAVAC" ; then echo yes ; else echo no ; fi` 
if test -n "$SWIG" ; then 
    SWIGBINDING="yes ("
    test -n "$TCLROOT" && SWIGBINDING="$SWIGBINDING tcl"
    test -n "$GUILEROOT" && SWIGBINDING="$SWIGBINDING guile"
    test -n "$PYTHONROOT" && SWIGBINDING="$SWIGBINDING python"
    SWIGBINDING="$SWIGBINDING )"
else
    SWIGBINDING=no
fi

if test -z "$javaroot"; then
    javaroot="not specified"
fi
AC_MSG_RESULT([

Configuration (NeXus):

        Source code location:  ${srcdir}
        Version:               ${VERSION}
        Compiler:              ${CC},${CXX},${F77},${FC}

build:
        NeXus with:
                HDF4 support:  ${HDF4SUPPORT} 
                HDF5 support:  ${HDF5SUPPORT}
                XML support:   ${XMLSUPPORT}

        bindings:
                F77 :          ${F77BINDING} 
                F90 :          ${F90BINDING} 
                JAVA:          ${JAVABINDING} (--with-java-home=${javaroot})
		SWIG:	       ${SWIGBINDING}

Please check whether the configuration I detected matches what you
would like to have.
])
