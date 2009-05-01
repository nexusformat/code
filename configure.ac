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

AC_PREREQ(2.61)
AC_REVISION($Revision$)
AC_INIT([NeXus Library], [4.3.0], [nexus-developers@nexusformat.org], [nexus])
AC_COPYRIGHT([Copyright (C) 2004 NeXus International Advisory Committee
This software is covered by the GNU LESSER GENERAL PUBLIC LICENSE
see file COPYING for further information])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/napi.c])
AC_CANONICAL_TARGET
AC_CANONICAL_BUILD

dnl $EGREP is used in macros included from acinclude.m4
AC_PROG_EGREP

dnl check for correct autoconf version - the same check is done in autogen.sh
if test ! -z "$srcdir"; then
    . $srcdir/autoversion.sh
else
    . ./autoversion.sh
fi

FCFLAGS="$F90FLAGS $FCFLAGS" # for compatibility with nexus-3.0

AM_INIT_AUTOMAKE([1.6])


AM_CONFIG_HEADER(include/nxconfig.h:include/nxconfig_h.in)

AC_ARG_ENABLE([debug], AC_HELP_STRING([--enable-debug],[Turn off optimisation]), [enable_debug=$enableval],[enable_debug=no])

AC_ARG_ENABLE([32bit], AC_HELP_STRING([--enable-32bit],[build 32bit binary on 64 bit computer]), [enable_32bit=$enableval],[enable_32bit=no])

dnl Checks for programs.
AC_ARG_WITH([cc], AC_HELP_STRING([--with-cc=c compiler], [Specify name of C compiler]),
	[with_cc=$withval], [with_cc=yes])
if test x"$with_cc" != x"no" ; then
	if test x"$with_cc" != x"yes" ; then CC="$with_cc"; fi
	AC_PROG_CC
	dnl Check C compiler options
	AC_CHECK_CPP_OPTION([-fno-common])
else
	AC_MSG_ERROR([You need a C compiler to compile this package])
fi

AC_ARG_WITH([cxx], AC_HELP_STRING([--with-cxx=c++ compiler], [Specify name of C++ compiler]), [with_cxx=$withval], [with_cxx=yes])
if test x"$with_cxx" != x"no" ; then
	if test x"$with_cxx" != x"yes" ; then CXX="$with_cxx"; fi
	AC_PROG_CXX
else
	CXX=
fi

AC_ARG_WITH([f77], AC_HELP_STRING([--with-f77=f77 compiler], [Specify name of FORTRAN 77 compiler]),
	[with_f77=$withval], [with_f77=no])
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

AC_ARG_WITH([f90], AC_HELP_STRING([--with-f90=f90 compiler], [Specify name of FORTRAN 90 compiler]),
	[with_f90=$withval], [with_f90=no])
if test x"$with_f90" != x"no" ; then
	if test x"$with_f90" = x"yes" ; then
		AC_CHECK_PROGS(FC, [g95 f90 f95 ifort])
	else
		FC="$with_f90"
		AC_CHECK_PROGS(FC, [$with_f90])
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
#        if test x$with_f77=xno; then with_f77=yes; fi
else
	FC=
fi

java_host="linux"
JAVAROOT=""
AC_ARG_WITH([java-home], AC_HELP_STRING([--with-java-home=JAVA SDK Home], [Specify location of top directory of JAVA SDK i.e. where the bin, lib and include directories live]),
	[], [with_java_home=$withval])
if test -d "$with_java_home"; then
	JAVAROOT="$with_java_home"
	AC_MSG_NOTICE([Java SDK home is $JAVAROOT])
	JAVAINCLUDE="-I$JAVAROOT/include"
	AC_PATH_PROGS(JAVAC,gcj javac,,[$JAVAROOT/bin])
	if test `basename "$JAVAC"` = "gcj"; then
	    JAVACFLAGS="-C"	# force creation of .class files
	fi
	AC_PATH_PROGS(JAVA,gij java,,[$JAVAROOT/bin])
	AC_PATH_PROGS(JAVAH,gcjh javah,,[$JAVAROOT/bin])
	AC_PATH_PROGS(JAVADOC,javadoc,,[$JAVAROOT/bin:$PATH])
	AC_PATH_PROGS(JAR,jar fastjar,,[$JAVAROOT/bin:$PATH])
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

AC_ARG_WITH([contrib], AC_HELP_STRING([--with-contrib=names_of_apps_to_build], [Specify a list of targets from the contrib directory to build]),
	[CONTRIB=$withval], [CONTRIB=no])
AM_CONDITIONAL(BUILD_CONTRIB, [test "$CONTRIB" != "no"])
AC_SUBST(CBFLIBROOT)
AC_ARG_WITH([cbflib],
	AC_HELP_STRING([--with-cbflib=/path/to/cbflib],
                       [Specify location of CBFLib files]),
	[if test $withval != no; then CBFLIBROOT=$withval; fi])
AM_CONDITIONAL(HAVE_CBFLIB, [test "$CBFLIBROOT" != ""])

AC_SUBST(IDLROOT)
AC_ARG_WITH([idlroot],
	AC_HELP_STRING([--with-idlroot=/path/to/idlroot],
                       [Specify root directory of system idl installation]),
	[if test $withval != no; then IDLROOT=$withval; fi])
AM_CONDITIONAL(HAVE_IDL, [test "$IDLROOT" != ""])

IDLDLM="\${prefix}/lib"
AC_SUBST(IDLDLM)
AC_ARG_WITH([idldlm],
	AC_HELP_STRING([--with-idldlm=/path/to/idldlm],
                       [Specify location ito install idl dlm files to]),
	[if test $withval != no; then IDLDLM=$withval; fi])

AC_ARG_WITH([doxygen], AC_HELP_STRING([--with-doxygen=/path/to/doxygen], [Specify path to doxygen]), [with_doxygen=$withval], [with_doxygen=no])
if test x"$with_doxygen" != x"no" ; then
	if test x"$with_doxygen" != x"yes" ; then DOXYGEN="$with_doxygen"; fi
	AC_CHECK_PROGS(DOXYGEN,[doxygen])
	AC_CHECK_PROGS(DOT,[dot])
	AC_CHECK_PROGS(PDFLATEX,[pdflatex])
	AC_PROG_CXX
else
	DOXYGEN=
	PDFLATEX=
	DOT=
fi

AC_SUBST(FCFLAGS)
AC_SUBST(JAVACFLAGS)
AC_SUBST(SHARED_LDFLAGS)
AC_SUBST(JAVAINCLUDE)
AC_SUBST(PACKAGE_RELEASE)
test -z "$PACKAGE_RELEASE" && PACKAGE_RELEASE=1

NXDOCDIR='${datadir}/nexus/doc'
NXEXAMPLEDIR='${datadir}/nexus/examples'
NXTESTDIR='${prefix}/nexus/tests'
AC_SUBST(NXDOCDIR)
AC_SUBST(NXEXAMPLEDIR)
AC_SUBST(NXTESTDIR)

AM_PROG_GCJ
AM_PROG_CC_C_O

AC_PROG_LIBTOOL
dnl AC_LIBTOOL_WIN32_DLL

AC_PROG_MAKE_SET
AC_PROG_RANLIB

if test x$with_tcl != xno; then
    AC_CHECK_ROOT([tcl],[TCLROOT],[/usr /usr/local],[include/tcl.h])
fi
if test x$with_guile != xno; then
    AC_CHECK_ROOT([guile],[GUILEROOT],[/usr /usr/local],[include/libguile.h])
fi
if test x$with_opengenie != xno; then
    AC_CHECK_ROOT([opengenie],[OPENGENIEROOT],[/usr/local],[genie/genie.so])
fi
if test x$with_python != xno; then
    AC_CHECK_ROOT([python],[PYTHONROOT],[/usr /usr/local],[bin/python])
    if test x$PYTHONROOT != x; then
	if test -r "$PYTHONROOT/bin/python"; then PYTHON="$PYTHONROOT/bin/python"; fi
	if test -r "$PYTHONROOT/python"; then PYTHON="$PYTHONROOT/python"; fi
    fi
    AM_PATH_PYTHON(,, :)
fi

AC_ARG_WITH([swig], AC_HELP_STRING([--with-swig=swig compiler], [Specify path of swig compiler]), [with_swig=$withval], [with_swig=])
if test x$with_tcl != xno -o x$with_guile != xno; then
    if test x$with_swig != xyes; then
	SWIG=$with_swig
    fi
    AC_CHECK_PROGS(SWIG,[swig])
else
    SWIG=
fi

AC_CHECK_PROGS(DOCBOOK2PDF,[docbook2pdf])
AC_CHECK_PROGS(DOCBOOK2TXT,[docbook2txt])
AC_CHECK_PROGS(TCLSH,[tclsh])
AC_CHECK_PROGS(GUILE,[guile])
AC_CHECK_PROGS(LATEX,[latex])
AC_CHECK_PROGS(WGET,[wget])
AC_CHECK_PROGS(XMLLINT,[xmllint])


dnl  flags for building swig guile bindings
AC_SUBST(SWGUILEFLAGS)  
if test x$GUILEROOT != x -a x$SWIG != x; then
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

dnl microsoft LIB command (used in MinGW makefile to create .LIB file)
AC_CHECK_PROGS(MS_LIB, [LIB])
AM_CONDITIONAL(HAVE_MS_LIB, [test ! -z "$MS_LIB"])

# strict aliasing causes issues with casts from NXmalloc()
CFLAGS="$CFLAGS -fno-strict-aliasing"
CXXFLAGS="$CXXFLAGS -fno-strict-aliasing"

# check for debug
if test "$enable_debug" = "yes"; then
    CFLAGS="$CFLAGS -O0 -g"
    CXXFLAGS="$CXXFLAGS -O0 -g"
fi

if test "$enable_32bit" = "yes"; then
    CFLAGS="$CFLAGS -m32"
    CXXFLAGS="$CXXFLAGS -m32"
fi

# enable compiler warnings if gcc
if test "$CC" = "gcc"; then
    CFLAGS="$CFLAGS -Wall -Wno-unused-variable -Wno-sign-compare -Wno-comment"
fi

if test "$CXX" = "g++"; then
    CXXFLAGS="$CXXFLAGS -Wall -Wno-unused-variable -Wno-sign-compare -Wno-comment"
fi

dnl extra FCFLAGS for Absoft f90
if test ! -z "$FC"; then
    fcpathlc=`which $FC 2>/dev/null | tr 'A-Z' 'a-z'`
    case "$fcpathlc" in
	*absoft*)
	    echo "Absoft f90 compiler detected - adjusting FCFLAGS"
	    FCFLAGS="$FCFLAGS -p$mytop/bindings/f90 -g -cons -YEXT_NAMES=LCS -YEXT_SFX=_"
	    case "$host_cpu" in
		*powerpc*)
	    	    FCFLAGS="$FCFLAGS -N11"
		    ;;
	    esac
	    ;;
    esac
fi

dnl extra FFLAGS for Absoft f77
if test ! -z "$F77"; then
    f77pathlc=`which $F77 2>/dev/null | tr 'A-Z' 'a-z'`
    case "$f77pathlc" in
	*absoft*)
	    echo "Absoft f77 compiler detected - adjusting FFLAGS"
	    FFLAGS="$FFLAGS -g -f -N15"
	    ;;
    esac
fi

HDF_EXT=a # library file extent to look for with HDF libraries
HDF5_EXT=so # library file extent to look for with HDF5 libraries

AC_SUBST(LIBXML2_CFLAGS)
LIBXML2_CFLAGS='`xml2-config --cflags`'

# set up for linux here - mac is covered in the case $host statement
AS_CASE([$build_cpu], [x86|i386|i686], [IDLCPU=x86], [IDLCPU=$build_cpu])
if test $enable_32bit = yes; then
    IDL_HOST="bin.linux.x86"	# force 32 bit on a 64 bit machine
else
    IDL_HOST="bin.linux.$IDLCPU"
fi
AC_SUBST(IDL_HOST)

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
	SHARED_LDFLAGS="-no-undefined" #  -Wl,--add-stdcall-alias
	MINGW_MSYS=yes
	HDF_EXT=dll
	HDF5_EXT=dll
	LIBXML2_CFLAGS=-I/usr/local/include
	java_host="win32"
	;;

    *cygwin*)
#	SHARED_LDFLAGS="-no-undefined" #  -Wl,--add-stdcall-alias
	;;

    *darwin*)
	dnl -no-cpp-precomp needed on MacOS-X (and others?)
	AC_CHECK_C_OPTION([-no-cpp-precomp])
	CPPFLAGS="$CPPFLAGS -D__unix"
	CFLAGS="$CFLAGS -g"
	FFLAGS="$FFLAGS -g"
	HDF5_EXT=dylib
dnl This is a hack for libtool - for some reason the F77 tag
dnl does not get set with the commands to create a shared library
	AC_MSG_NOTICE([Patching libtool for F77 shared linking])
	sed -e "s/^archive_cmds=\"\"//" < libtool > libtool.$$
	mv -f libtool.$$ libtool
	chmod +x libtool
	JAVAINCLUDE="-I/System/Library/Frameworks/JavaVM.framework/Headers"
dnl	F90FLAGS="$F90FLAGS -g -cons -YEXT_NAMES=LCS -YEXT_SFX=_"
dnl	FCFLAGS="$FCFLAGS -qextname"
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
	if test $enable_32bit = yes; then
	    IDL_HOST="bin.darwin.i386" # force 32bit build
	else
	    IDL_HOST="bin.darwin.x86_64"
	fi
	;;
esac

if test ! -z "$JAVAINCLUDE" -a ! -z "$java_host"; then
# avoid adding /usr/include/linux to include path
    if test "$JAVAROOT" != "/usr" -a "$JAVAROOT" != "/usr/"; then
        JAVAINCLUDE="$JAVAINCLUDE -I$JAVAROOT/include/$java_host"
    fi
fi

dnl
dnl * locate path to HDF libraries *
dnl
dnl first see if one has been given
dnl
H4ROOT=""
H4INCSUBROOT=""
H4LIBSUBROOT=""
H5ROOT=""
MXMLROOT=""
AC_SUBST(H4ROOT)
AC_SUBST(H5ROOT)
AC_SUBST(MXMLROOT)
AC_ARG_WITH([hdf4],
	AC_HELP_STRING([--with-hdf4=/path/to/hdf4],
                       [Specify location of HDF4 files]),
	[if test $withval != no -a $withval != yes; then H4ROOT=$withval; fi])
AC_ARG_WITH([hdf5],
	AC_HELP_STRING([--with-hdf5=/path/to/hdf5],
                       [Specify location of HDF5 files]),
	[if test $withval != no -a $withval != yes; then H5ROOT=$withval; fi])
AC_ARG_WITH([xml],
	AC_HELP_STRING([--with-xml=/path/to/mxml],
                       [Specify location of MXML library files]),
	[if test $withval != no -a $withval != yes; then MXMLROOT=$withval; fi])
dnl otherwise try and find HDF path, but not if
dnl e.g. --without-hdf4 has been specified (hence check on $with_hdf4 != no )
dnl first HDF4
EXTRA_LDPATH=""
EXTRA_LD_LIBRARY_PATH=""
AC_SUBST(EXTRA_LD_LIBRARY_PATH)
HDF4SEARCH="/usr/local/hdf4 /usr/local/hdf /usr/local /sw /opt/local /usr"
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
	    H4LIBSUBROOT=hdf
        fi
    done
    if test -r $H4ROOT/include/hdf; then
        H4INCSUBROOT=hdf
    fi
    if test -z "$H4ROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($H4ROOT)
    fi
fi
if test "$H4ROOT"; then
    H4MAJORVERSION=`grep LIBVER_MAJOR ${H4ROOT}/include/${H4INCSUBROOT}/hfile.h | awk '{ print $3}'`
    H4MINORVERSION=`grep LIBVER_MINOR ${H4ROOT}/include/${H4INCSUBROOT}/hfile.h | awk '{ print $3}'`
    H4RELEASE=`grep LIBVER_RELEASE ${H4ROOT}/include/${H4INCSUBROOT}/hfile.h | awk '{ print $3}'`
    H4VERSION=$H4MAJORVERSION.$H4MINORVERSION.$H4RELEASE
    case $H4VERSION in
	4.[[12]]*) 
    		HDF4_LDFLAGS="";
		for j in lib lib64; do
		    if test -d $H4ROOT/$j/$H4LIBSUBROOT; then 
			HDF4_LDFLAGS="-L$H4ROOT/$j/$H4LIBSUBROOT $HDF4_LDFLAGS"
		        EXTRA_LD_LIBRARY_PATH=$EXTRA_LD_LIBRARY_PATH:$H4ROOT/$j/$H4LIBSUBROOT
		    fi
		done
		EXTRA_LDPATH="$EXTRA_LDPATH $HDF4_LDFLAGS"
    		HDF4_LDFLAGS="$HDF4_LDFLAGS -lmfhdf -ldf";
		if test -z "$H4INCSUBROOT"; then
    		    HDF4_CPPFLAGS="-I$H4ROOT/include -DHDF4"
		else
    		    HDF4_CPPFLAGS="-I$H4ROOT/include/$H4INCSUBROOT -DHDF4"
		fi
		;;
	*) 	AC_MSG_WARN([The HDF 4 installation has not the right version ($H4VERSION). You need at least 4.1])
		H4ROOT=""
		;;
    esac		
fi
AC_SUBST(HDF4_LDFLAGS)
AC_SUBST(HDF4_CPPFLAGS)

dnl HDF5
HDF5SEARCH="/usr/local/hdf5 /usr/local/hdf /usr/local /sw /opt/local /usr"
if test "$with_hdf5" != "no" -a -z "$H5ROOT"; then
    AC_MSG_CHECKING(for location of HDF5 libraries)
    for i in $HDF5SEARCH; do
	for j in lib lib64; do
	    if test -z "$H5ROOT" -a -r ${i}/$j/libhdf5.$HDF5_EXT; then H5ROOT=$i; fi
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
	1.[[68]]*)
    		HDF5_LDFLAGS=""
		for j in lib lib64; do
		    if test -d $H5ROOT/$j; then 
			HDF5_LDFLAGS="-L$H5ROOT/$j $HDF5_LDFLAGS"
		    	EXTRA_LD_LIBRARY_PATH=$EXTRA_LD_LIBRARY_PATH:$H5ROOT/$j
		    fi
		done
		EXTRA_LDPATH="$EXTRA_LDPATH $HDF5_LDFLAGS"
    		HDF5_LDFLAGS="$HDF5_LDFLAGS -lhdf5 -lz"
    		HDF5_CPPFLAGS="-I$H5ROOT/include -DHDF5 -DH5_USE_16_API";;
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
if test ! -z "$MXMLROOT"; then
    XML_LDFLAGS="-lmxml"
    XML_LDFLAGS="-L$MXMLROOT/lib $XML_LDFLAGS"
    XML_CPPFLAGS="-I$MXMLROOT/include -DNXXML"
# meed to allow for lib64
#    EXTRA_LDPATH="$EXTRA_LDPATH $XML_LDFLAGS"
#    export LD_LIBRARY_PATH=$MXMLROOT/lib:$LD_LIBRARY_PATH
fi
AC_SUBST(XML_LDFLAGS)
AC_SUBST(XML_CPPFLAGS)

if test ! -z "$EXTRA_LDPATH"; then
    LDFLAGS="$LDFLAGS $EXTRA_LDPATH"
fi
if test -d /usr/local/lib; then
    LDFLAGS="$LDFLAGS -L/usr/local/lib"
fi
# for Mac/fink when hdf and hdf5 are not taken from fink, but jpeg is
if test -d /sw/lib; then
    LDFLAGS="$LDFLAGS -L/sw/lib"
fi
# for Mac/Macports when you are not using fink at all.
if test -d /opt/local/lib; then
    LDFLAGS="$LDFLAGS -L/opt/local/lib"
fi

# remove initial :
if test ! -z "$EXTRA_LD_LIBRARY_PATH"; then
    EXTRA_LD_LIBRARY_PATH=`expr substr "$EXTRA_LD_LIBRARY_PATH" 2 1024`
fi
if test ! -z "$LD_LIBRARY_PATH"; then
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${EXTRA_LD_LIBRARY_PATH}
else
    export LD_LIBRARY_PATH=${EXTRA_LD_LIBRARY_PATH}
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
AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(z, gzopen)
AC_CHECK_LIB(jpeg, jpeg_CreateCompress)
AC_CHECK_LIB(sz, SZ_Compress)
AC_CHECK_LIB(df, Hopen)
AC_CHECK_LIB(mfhdf, SDcreate)
AC_CHECK_LIB(hdf5, H5open)
AC_CHECK_LIB(xml2, xmlParseDocument)

dnl Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([stdlib.h string.h stdint.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_LONG_LONG_INT
AC_TYPE_UNSIGNED_LONG_LONG_INT
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long int])
AC_CHECK_SIZEOF([long long int])
AC_STRUCT_TM

AC_SYS_LARGEFILE
case "$ac_cv_sys_file_offset_bits" in
  "no" | "unknown" | "") 
    ;;
  *)
    CFLAGS="$CFLAGS -D_FILE_OFFSET_BITS=$ac_cv_sys_file_offset_bits"
    CXXFLAGS="$CXXFLAGS -D_FILE_OFFSET_BITS=$ac_cv_sys_file_offset_bits"
    ;;
esac

if test $MINGW_MSYS = yes; then
AC_DEFINE([PRINTF_INT64],["I64d"],[Set to printf format for int64_t])
AC_DEFINE([PRINTF_UINT64],["I64u"],[Set to printf format for uint64_t])
else
AC_DEFINE([PRINTF_INT64],["lld"],[Set to printf format for int64_t])
AC_DEFINE([PRINTF_UINT64],["llu"],[Set to printf format for uint64_t])
fi

dnl Checks for library functions.
#this can cause link errors with rpl_malloc undefined
#AC_FUNC_MALLOC
#this no needed
#AC_FUNC_MEMCMP
#this hangs on FC9 at the moment
#AC_FUNC_MKTIME

#AC_FUNC_STRFTIME

AC_CHECK_FUNCS([ftime memset strchr strdup strrchr strstr tzset mkstemp])

AM_CONDITIONAL(HAVE_CPP, [test ! -z "$CXX"])
AM_CONDITIONAL(HAVE_F77, [test "$with_f77" != "no"])
AM_CONDITIONAL(HAVE_F90, [test "$with_f90" != "no"])
AM_CONDITIONAL(HAVE_PYTHON, [test ! -z "$PYTHONROOT"])
AM_CONDITIONAL(HAVE_TCL, [test ! -z "$TCLROOT"])
AM_CONDITIONAL(HAVE_GUILE, [test ! -z "$GUILEROOT"])
AM_CONDITIONAL(HAVE_SWIG, [test ! -z "$SWIG"])
AM_CONDITIONAL(HAVE_JAVA, [test ! -z "$JAVA" -a ! -z "$JAVAROOT"])
AM_CONDITIONAL(HAVE_JAVAC, [test ! -z "$JAVAC" -a ! -z "$JAVAROOT"])
AM_CONDITIONAL(HAVE_JAVADOC, [test ! -z "$JAVADOC" -a ! -z "$JAVAROOT"])
AM_CONDITIONAL(HAVE_HDF4, [test ! -z "$H4ROOT"])
AM_CONDITIONAL(HAVE_HDF5, [test ! -z "$H5ROOT"])
AM_CONDITIONAL(HAVE_XML, [test ! -z "$MXMLROOT"])
AM_CONDITIONAL(HAVE_LIBXML2, [ test "$ac_cv_lib_xml2_xmlParseDocument" = "yes" ])
AM_CONDITIONAL(HAVE_DOCBOOK, [test ! -z "$DOCBOOK2TXT" -a ! -z "$DOCBOOK2PDF"])
AM_CONDITIONAL(HAVE_LATEX, [test ! -z "$LATEX"])
#AM_CONDITIONAL(INSTALL_NXPYTHON, [test ! -z "$INSTALL_NXPYTHON"])
AM_CONDITIONAL(HAVE_MZSCHEME, [test ! -z "$INSTALL_NXPYTHON"])
AM_CONDITIONAL(MINGW_MSYS, [test x$MINGW_MSYS = xyes])
AM_CONDITIONAL(HAVE_OPENGENIE, [test ! -z "$OPENGENIEROOT"])
AM_CONDITIONAL(HAVE_DOXYGEN, [test ! -z "$DOXYGEN"])
AM_CONDITIONAL(HAVE_PDFLATEX, [test ! -z "$PDFLATEX"])
AM_CONDITIONAL(HAVE_DOT, [test ! -z "$DOT"])

AM_CONDITIONAL(HAVE_WGET, [test ! -z "$WGET"])
AM_CONDITIONAL(HAVE_XMLLINT, [test ! -z "$XMLLINT"])

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
		applications/NXbrowse/Makefile
		applications/NXconvert/Makefile
		applications/NXvalidate/Makefile
		applications/NXdump/Makefile
		applications/nxingest/Makefile
		applications/NXsummary/Makefile
		applications/NXtranslate/Makefile
		applications/NXtranslate/IPNS_CPP/Makefile
		applications/NXtranslate/FRM2/Makefile
		applications/NXtranslate/opengenie/Makefile
		applications/NXtranslate/text_collist/Makefile
		applications/NXtranslate/text_plain/Makefile
		applications/NXtranslate/text_xml/Makefile
		applications/NXtranslate/sns_histogram/Makefile
		applications/NXtranslate/loopy/Makefile
		applications/NXtranslate/binary/Makefile
		applications/NXtranslate/spec/Makefile
		applications/NXtranslate/esrf_edf/Makefile
		applications/NXtranslate/docs/Makefile
		examples/Makefile
		doc/Makefile
		doc/doxygen/Makefile
		doc/doxygen/Doxyfile
		doc/api/Makefile
		doc/nxdict/Makefile
		doc/tech_ref/Makefile
		bindings/Makefile 
		bindings/cpp/Makefile
		bindings/f77/Makefile
		bindings/f90/Makefile
		bindings/java/Makefile
		bindings/swig/Makefile
		bindings/idl/Makefile
		bindings/python/Makefile
		third_party/Makefile
		third_party/tclap/Makefile
		contrib/Makefile
		contrib/applications/Makefile
		contrib/applications/CBFLib/Makefile
		contrib/bindings/Makefile
		contrib/bindings/python/Makefile
		macosx_install_kit/pkg.config
		scripts/Makefile
		scripts/nexus.pc:scripts/nexus_pc.in
		scripts/nxbuild])
dnl src/nxdict/Makefile
AC_OUTPUT

if test ! -z "$DOXYGEN" -a ! -z "$PDFLATEX" -a ! -z "$DOT"; then
    DOXYGENDOCS="yes"
else
    if test x"$with_doxygen" != x"no" ; then
        DOXYGENDOCS="no  - missing "
        if test -z "$DOXYGEN"; then DOXYGENDOCS="$DOXYGENDOCS doxygen"; fi
        if test -z "$PDFLATEX"; then DOXYGENDOCS="$DOXYGENDOCS pdflatex"; fi
        if test -z "$DOT"; then DOXYGENDOCS="$DOXYGENDOCS graphviz"; fi
    else
        DOXYGENDOCS="no    --with-doxygen  not specified"
    fi
fi

if test ! -z "$DOCBOOK2TXT" -a ! -z "$DOCBOOK2PDF"; then
    DOCBOOKDOCS="yes"
else
    DOCBOOKDOCS="no  - missing "
    if test -z "$DOCBOOK2TXT"; then DOCBOOKDOCS="$DOCBOOKDOCS docbook2txt"; fi
    if test -z "$DOCBOOK2PDF"; then DOCBOOKDOCS="$DOCBOOKDOCS docbook2pdf"; fi
fi

HDF4SUPPORT=`if test -n "$H4ROOT" ; then echo yes ; else echo no ; echo "                found version $H4VERSION"; fi`
HDF5SUPPORT=`if test -n "$H5ROOT" ; then echo yes ; else echo no ; echo "                found version $H5VERSION"; fi`
XMLSUPPORT=`if test -n "$MXMLROOT" ; then echo yes ; else echo no ; fi`
CPPBINDING=`if test -n "$CXX" ; then echo yes ; else echo no ; fi`
F77BINDING=`if test -n "$F77" ; then echo yes ; else echo no ; fi`
F90BINDING=`if test -n "$FC" ; then echo yes ; else echo no ; fi`
JAVABINDING=`if test -n "$JAVAROOT" ; then echo yes ; else echo no ; fi` 
IDLBINDING=`if test -n "$IDLROOT" ; then echo yes ; else echo no ; fi` 
PYTHONBINDING=`if test -n "$PYTHONROOT" ; then echo yes; else echo no ; fi` 
if test -n "$SWIG" ; then 
    SWIGBINDING="yes ("
    test -n "$TCLROOT" && SWIGBINDING="$SWIGBINDING tcl"
    test -n "$GUILEROOT" && SWIGBINDING="$SWIGBINDING guile"
    SWIGBINDING="$SWIGBINDING ) SWIG=${SWIG}"
else
    SWIGBINDING=no
fi

if test -z "$JAVAROOT"; then
    JAVAROOT="not specified"
fi
AC_MSG_RESULT([

Configuration (NeXus):

	Install prefix:        ${prefix}
        Source code location:  ${srcdir}
        Version:               ${VERSION}
        Compiler:              ${CC},${CXX},${F77},${FC}

build:
        NeXus with:
                HDF4 support:  ${HDF4SUPPORT} ($H4VERSION)
                HDF5 support:  ${HDF5SUPPORT} ($H5VERSION)
                XML support:   ${XMLSUPPORT}

        bindings:
                C++ :          ${CPPBINDING} (--with-cxx=${CXX})
                F77 :          ${F77BINDING} (--with-f77=${F77})
            F90/F95 :          ${F90BINDING} (--with-f90=${FC})
                JAVA:          ${JAVABINDING} (--with-java-home=${JAVAROOT}, JAVA=${JAVA}, JAVAC=${JAVAC}, JAVAH=${JAVAH})
		SWIG:	       ${SWIGBINDING}
		 IDL:	       ${IDLBINDING} (--with-idlroot=${IDLROOT}, --with-idldlm=${IDLDLM})
	      PYTHON:	       ${PYTHONBINDING} (PYTHONROOT=${PYTHONROOT})

	documentation:
	     Doxygen:	       ${DOXYGENDOCS}
	     Docbook:	       ${DOCBOOKDOCS}

Please check whether the configuration I detected matches what you would
like to have. If not, re-run configure adding the appropriate  --with  option
or setting the relevant environment variable.

For a list of available options type    ./configure --help
])
