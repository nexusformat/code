#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  Autoconf (configure script) generation file
#  
#  $Id$
#
#  Copyright (C) 2004 Freddie Akeroyd
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
# 
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free 
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
#  MA  02111-1307  USA
#             
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
#                       -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#
AC_PREREQ(2.57)
AC_REVISION($Revision$)
AC_INIT([NeXus Library], [2.0.0], [nexus-developers@anl.gov], [nexus])
AC_COPYRIGHT([Copyright (C) 2004 Freddie Akeroyd 
This software is covered by the GNU LESSER GENERAL PUBLIC LICENSE
see file COPYING for further information])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/napi.c])
AM_INIT_AUTOMAKE([1.6])
AM_CONFIG_HEADER([include/nxconfig.h])
AC_CANONICAL_HOST
AC_PROG_LIBTOOL
# Checks for programs.
AC_ARG_WITH([cc],
	AC_HELP_STRING([--with-cc=c_compiler],
                       [Specify name of C compiler]),
	[if test $withval != no; then CC=$withval; fi])
AC_ARG_WITH([f90],
	AC_HELP_STRING([--with-f90=f90_compiler],
                       [Specify name of FORTRAN 90 compiler]),
	[if test $withval != no; then F90=$withval; fi])
AC_ARG_WITH([f77],
	AC_HELP_STRING([--with-f77=f77_compiler],
                       [Specify name of FORTRAN 77 compiler]),
	[if test $withval != no; then F77=$withval; fi])
AC_PROG_CC
AC_PROG_F77
AC_CHECK_PROGS(F90,f90 f95)
AC_SUBST(F90)
AC_SUBST(F90FLAGS)
#
AC_PROG_EGREP
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Check C compiler options
# -no-cpp-precomp needed on MacOS-X (and others?)
AC_CHECK_C_OPTION([-no-cpp-precomp])
AC_CHECK_C_OPTION([-fno-common])

# Check FORTRAN compiler options
#AC_CHECK_F77_OPTION([-Wno-globals])
#AC_CHECK_F77_OPTION([-fno-common])  # use with caution

#
# First any host specific bits
#
case $host in
    alpha*-dec-osf*)
	LDFLAGS="$LDFLAGS -non_shared"
	;;
    *darwin*)
	CFLAGS="$CFLAGS -g -D__unix"
	FFLAGS="$FFLAGS -g -Wno-globals"
	F90FLAGS="$F90FLAGS -g -cons -YEXT_NAMES=LCS -YEXT_SFX=_"
	LDFLAGS="$LDFLAGS -flat_namespace"
	;;
esac
#
# * locate path to HDF libraries *
#
# first see if one has been given
#
H4ROOT=""
H5ROOT=""
AC_SUBST(H4ROOT)
AC_SUBST(H5ROOT)
AC_ARG_WITH([hdf4],
	AC_HELP_STRING([--with-hdf4=/path/to/hdf4],
                       [Specify location of HDF4 files]),
	[if test $withval != no; then H4ROOT=$withval; fi])
AC_ARG_WITH([hdf5],
	AC_HELP_STRING([--with-hdf5=/path/to/hdf5],
                       [Specify location of HDF5 files]),
	[if test $withval != no; then H5ROOT=$withval; fi])
# otherwise try and find HDF path, but not if
# e.g. --without-hdf4 has been specified (hence check on $with_hdf4 != no )
# first HDF4
HDF4SEARCH="/usr/local/hdf4 /usr/local/hdf /sw"
if test "$with_hdf4" != "no" -a -z "$H4ROOT"; then
    AC_MSG_CHECKING(for location of HDF4 libraries)
    for i in $HDF4SEARCH; do
	if test -r ${i}/lib/libdf.a; then H4ROOT=$i; fi
    done
    if test -z "$H4ROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($H4ROOT)
    fi
fi
if test ! -z "$H4ROOT"; then
    LDFLAGS="$LDFLAGS -L$H4ROOT/lib"
    CPPFLAGS="$CPPFLAGS -I$H4ROOT/include -DHDF4"
fi
# HDF5
HDF5SEARCH="/usr/local/hdf5 /usr/local/hdf /sw"
if test "$with_hdf5" != "no" -a -z "$H5ROOT"; then
    AC_MSG_CHECKING(for location of HDF5 libraries)
    for i in $HDF5SEARCH; do
	if test -r ${i}/lib/libhdf5.a; then H5ROOT=$i; fi
    done
    if test -z "$H5ROOT"; then 
	AC_MSG_RESULT(unknown)
    else 
	AC_MSG_RESULT($H5ROOT)
    fi
fi
if test ! -z "$H5ROOT"; then
    LDFLAGS="$LDFLAGS -L$H5ROOT/lib"
    CPPFLAGS="$CPPFLAGS -I$H5ROOT/include -DHDF5"
fi
# finally set LIBS variable if we 
AC_CHECK_LIB(z, gzopen)
AC_CHECK_LIB(jpeg, jpeg_CreateCompress)
AC_CHECK_LIB(df, Hopen)
AC_CHECK_LIB(mfhdf, ncopen)
AC_CHECK_LIB(hdf5, H5open)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([ftime memset strchr strdup strrchr strstr tzset])

AM_CONDITIONAL([HAVE_F77], [test ! -z "$F77"])
AM_CONDITIONAL([HAVE_F90], [test ! -z "$F90"])
AM_CONDITIONAL([HAVE_HDF4], [test ! -z "$H4ROOT"])
AM_CONDITIONAL([HAVE_HDF5], [test ! -z "$H5ROOT"])

AC_CONFIG_FILES([Makefile
		nexus.spec
		include/Makefile
		src/Makefile 
		applications/Makefile
		test/Makefile 
		bindings/Makefile 
		bindings/f77/Makefile
		bindings/f90/Makefile
		scripts/Makefile
		scripts/nxbuild])
AC_OUTPUT

echo "

Configuration (NeXus):

        Source code location:  ${srcdir}
        Version:               ${VERSION}
        Compiler:              ${CC},${CXX},${F77},${F90}

build:
        NeXus with:
                HDF4 support:  `if test -n "$H4ROOT" ; then echo yes ; else echo no ; fi`
                HDF5 support:  `if test -n "$H5ROOT" ; then echo yes ; else echo no ; fi`

        bindings:
                F77 :          `if test -n "$F77" ; then echo yes ; else echo no ; fi`
                F90 :          `if test -n "$F90" ; then echo yes ; else echo no ; fi`

Please check whether the configuration I detected matches what you
would like to have.
"
