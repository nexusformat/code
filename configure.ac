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
AC_INIT([NeXus Library], [2.1.0], [nexus-developers@anl.gov], [nexus])
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

AM_CONFIG_HEADER([include/nxconfig.h])
dnl Checks for programs.
AC_ARG_WITH([cc], AC_HELP_STRING([--with-cc=c compiler], [Specify name of C compiler]),
	[], [with_cc=yes])
if test x"$with_cc" != x"no" ; then
	if test x"$with_cc" = x"yes" ; then
		AC_PROG_CC
	else
		AC_CHECK_PROGS(CC, ["$with_cc"])
	fi
	dnl Check C compiler options
	AC_CHECK_C_OPTION([-fno-common])
else
	AC_MSG_ERROR([You need a C compiler to compile this package])
fi

AC_ARG_WITH([f77], AC_HELP_STRING([--with-f77=f77 compiler], [Specify name of FORTRAN 77 compiler]),
	[], [with_f77=yes])
if test x"$with_f77" != x"no" ; then
	if test x"$with_f77" = x"yes" ; then
		AC_PROG_F77
	else
		AC_CHECK_PROGS(F77, ["$with_f77"])
	fi
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

AC_ARG_WITH([f90], AC_HELP_STRING([--with-f90=f90 compiler], [Specify name of FORTRAN 90 compiler]),
	[], [with_f90=yes])
if test x"$with_f90" != x"no" ; then
	if test x"$with_f90" = x"yes" ; then
		AC_CHECK_PROGS(F90, [f90 f95 ifort])
	else
		AC_CHECK_PROGS(F90, ["$with_f90"])
	fi
	dnl f90 has a "module path" option - hard to get as you really need a
	dnl ready compiled module to point the option at so for now we are just
	dnl check that the option takes a directory and the compiler works
	mytop=`pwd`
	F90FLAGS="$F90FLAGS -I$mytop/bindings/f90"
	dnl AC_CHECK_F90_OPTION([-p $mytop/bindings/f90],[      INCLUDE 'NXmodule.f90'])
	dnl AC_CHECK_F90_OPTION([-p $mytop/bindings/f90],[      INCLUDE 'NXmodule.f90'])
	dnl AC_CHECK_F90_OPTION([-qextname])
	AC_SUBST(F90)
	AC_SUBST(F90FLAGS)
else
	F90=
fi

java_host="linux"
javaroot=""
AC_ARG_WITH([java-home], AC_HELP_STRING([--with-java-home=JAVA SDK Home], [Specify location of top directory of JAVA SDK]),
	[], [with_java_home=$withval])
if test -d "$with_java_home"; then
	javaroot="$with_java_home"
fi


AC_ARG_WITH([java], AC_HELP_STRING([--with-java=java compiler], [Specify name of Java compiler]),
	[], [with_java=yes])
if test x$with_java != xno ; then
	if test -z "$javaroot"; then
	    javapath=`which java`
	    if test ! -z "$javapath"; then javaroot=`dirname $javapath`/..; fi
	fi
	if test -z "$javaroot"; then 
	    AC_MSG_ERROR([Unable to determine JAVA SDK directory - please specify this using  --with-java-home=/sdk/directory  or instead use  --without-java])
	fi
	AC_MSG_NOTICE([Java SDK home is $javaroot])
	if test x"$with_java" = x"yes" ; then
		AC_PATH_PROGS(JAVAC,javac,,[$javaroot/bin:$PATH])
	else
		AC_PATH_PROGS(JAVAC,["$with_java"],,[$javaroot/bin:$PATH])
	fi
	AC_PATH_PROGS(JAVADOC,javadoc,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAR,jar,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAVA,java,,[$javaroot/bin:$PATH])
	AC_PATH_PROGS(JAVAH,javah,,[$javaroot/bin:$PATH])
	JAVAINCLUDE="-I$javaroot/include"
else
	JAVAC=
	JAVAINCLUDE=
fi
AC_SUBST(SHARED_LDFLAGS)
AC_SUBST(JAVAINCLUDE)

AC_PROG_LIBTOOL

AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_CHECK_PROG(SWIG,swig)
AC_PROG_CXX

dnl These are used in build_rules.am to pass any approptiate 
dnl tag to libtool for f90 building
if test `$EGREP "available_tags=.*F77" libtool | wc -l` -gt 0; then
    LTF90COMPILETAG="--tag=F77"
    LTF90LINKTAG="--tag=CC"
else
    LTF90COMPILETAG=""
    LTF90LINKTAG=""
fi
AC_SUBST(LTF90COMPILETAG)
AC_SUBST(LTF90LINKTAG)

dnl
dnl First any host specific bits
dnl
case $host in
    alpha*-dec-osf*)
	java_host="alpha"
	if test "$CC" = "cc" -o "$CC" = "c89"; then
	    CFLAGS="$CFLAGS -pthread"
	fi
	if test "$CXX" = "cxx"; then
	    CXXFLAGS="$CXXFLAGS -pthread -std gnu -D__USE_STD_IOSTREAM"
	fi
	;;

    *darwin*)
	dnl -no-cpp-precomp needed on MacOS-X (and others?)
	AC_CHECK_C_OPTION([-no-cpp-precomp])
	CFLAGS="$CFLAGS -g -D__unix"
	FFLAGS="$FFLAGS -g -Wno-globals"
dnl This is a hack for libtool - for some reason the F77 tag
dnl does not get set with the commands to create a shared library
	AC_MSG_NOTICE([Patching libtool for F77 shared linking])
	sed -e "s/^archive_cmds=\"\"//" < libtool > libtool.$$
	mv -f libtool.$$ libtool
	chmod +x libtool
	JAVAINCLUDE="-I/System/Library/Frameworks/JavaVM.framework/Headers"
dnl	F90FLAGS="$F90FLAGS -g -cons -YEXT_NAMES=LCS -YEXT_SFX=_"
	F90FLAGS="$F90FLAGS -qextname"
	SHARED_LDFLAGS="-Wl,-single_module"
	LTF90COMPILETAG="--tag=F77"
	LTF90LINKTAG="--tag=CC"
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
	if test -z "$H4ROOT" -a -r ${i}/lib/libdf.a; then H4ROOT=$i; fi
    done
    if test -z "$H4ROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($H4ROOT)
    fi
fi
if test "$H4ROOT"; then
    H4VERSION=`grep LIBVER_STRING ${H4ROOT}/include/hfile.h | cut -d '"' -f 2 | cut -d ' ' -f4,6 | tr ' ,' '. '`
    case $H4VERSION in
	4.[[12]]*) 
    		HDF4_LDFLAGS="-L$H4ROOT/lib -lmfhdf -ldf -ljpeg";
    		HDF4_CPPFLAGS="-I$H4ROOT/include -DHDF4" ;;
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
	if test -z "$H5ROOT" -a -r ${i}/lib/libhdf5.a; then H5ROOT=$i; fi
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
    		HDF5_LDFLAGS="-L$H5ROOT/lib -lhdf5 -lz -lm"
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
    XML_LDFLAGS="-L$MXMLROOT/lib -lmxml"
    XML_CPPFLAGS="-I$MXMLROOT/include -DNXXML"
fi
AC_SUBST(XML_LDFLAGS)
AC_SUBST(XML_CPPFLAGS)

dnl finally set LIBS variable if we 
AC_CHECK_LIB(z, gzopen)
AC_CHECK_LIB(jpeg, jpeg_CreateCompress)
AC_CHECK_LIB(sz, SZ_Compress)
AC_CHECK_LIB(df, Hopen)
AC_CHECK_LIB(mfhdf, ncopen)
AC_CHECK_LIB(hdf5, H5open)

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

AM_CONDITIONAL([HAVE_F77], [test ! -z "$F77"])
AM_CONDITIONAL([HAVE_F90], [test ! -z "$F90"])
AM_CONDITIONAL([HAVE_JAVA], [test ! -z "$JAVA"])
AM_CONDITIONAL([HAVE_JAVAC], [test ! -z "$JAVAC"])
AM_CONDITIONAL([HAVE_JAVADOC], [test ! -z "$JAVADOC"])
AM_CONDITIONAL([HAVE_HDF4], [test ! -z "$H4ROOT"])
AM_CONDITIONAL([HAVE_HDF5], [test ! -z "$H5ROOT"])
AM_CONDITIONAL([HAVE_XML], [test ! -z "$MXMLROOT"])

AC_CONFIG_TESTDIR(test)
AC_CONFIG_FILES(test/Makefile test/atlocal)
AM_MISSING_PROG([AUTOM4TE], [autom4te])

LINUX_DISTRIBUTION

AC_CONFIG_FILES([Makefile
		nexus.spec
		build_rpm
		include/Makefile
		src/Makefile 
		applications/Makefile
		bindings/Makefile 
		bindings/f77/Makefile
		bindings/f90/Makefile
                bindings/java/Makefile
		contrib/Makefile
		contrib/applications/Makefile
		contrib/applications/NXdir/Makefile
		scripts/Makefile
		scripts/nexus.pc
		scripts/nxbuild])
dnl src/nxdict/Makefile
AC_OUTPUT

HDF4SUPPORT=`if test -n "$H4ROOT" ; then echo yes ; else echo no ; echo "                found version $H4VERSION"; fi`
HDF5SUPPORT=`if test -n "$H5ROOT" ; then echo yes ; else echo no ; echo "                found version $H5VERSION"; fi`
XMLSUPPORT=`if test -n "$MXMLROOT" ; then echo yes ; else echo no ; fi`
F77BINDING=`if test -n "$F77" ; then echo yes ; else echo no ; fi`
F90BINDING=`if test -n "$F90" ; then echo yes ; else echo no ; fi`
JAVABINDING=`if test -n "$JAVAC" ; then echo yes ; else echo no ; fi` 
AC_MSG_RESULT([

Configuration (NeXus):

        Source code location:  ${srcdir}
        Version:               ${VERSION}
        Compiler:              ${CC},${CXX},${F77},${F90}

build:
        NeXus with:
                HDF4 support:  ${HDF4SUPPORT} 
                HDF5 support:  ${HDF5SUPPORT}
                XML support:   ${XMLSUPPORT}

        bindings:
                F77 :          ${F77BINDING} 
                F90 :          ${F90BINDING} 
                JAVA:          ${JAVABINDING}

Please check whether the configuration I detected matches what you
would like to have.
])
