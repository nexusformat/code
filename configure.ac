#                       -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.57)
AC_REVISION($Revision$)
AC_INIT(NeXus Library, 2.0.0, nexus-developers@anl.gov, nexus)
AC_CONFIG_SRCDIR([src/napi.c])
AC_CONFIG_HEADER([include/nxconfig.h])
AC_CONFIG_AUX_DIR(.)
AC_CANONICAL_HOST
# Checks for programs.
AC_ARG_WITH([cc],
	AC_HELP_STRING([--with-cc=c_compile],
                       [Specify name of C compiler]),
	[if test $withval != no; then CC=$withval; fi])
AC_ARG_WITH([f90],
	AC_HELP_STRING([--with-f90=f90_compile],
                       [Specify name of FORTRAN 90 compiler]),
	[if test $withval != no; then F90=$withval; fi])
AC_ARG_WITH([f77],
	AC_HELP_STRING([--with-f77=f77_compile],
                       [Specify name of FORTRAN 77 compiler]),
	[if test $withval != no; then F77=$withval; fi])
AC_PROG_CC
AC_PROG_F77
AC_CHECK_PROGS(F90,f90 f95)
AC_SUBST(F90)
AC_SUBST(F90FLAGS)
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_SUBST(NXTARGET)
AC_SUBST(BINDINGS)
NXTARGET=""
BINDINGS=""
#
# First any host specific bits
#
case $host in
    alpha*-dec-osf*)
	LDFLAGS="$LDFLAGS -non_shared"
	;;
    *darwin*)
	CFLAGS="$CFLAGS -g -no-cpp-precomp -D__unix"
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
HDF4SEARCH="/usr/local/hdf /usr/local/hdf4"
if test "$with_hdf4" != "no" -a -z "$H4ROOT"; then
    AC_MSG_CHECKING(for location of HDF4 libraries)
    for i in $HDF4SEARCH; do
	if test -d $i; then H4ROOT=$i; fi
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
    NXTARGET="$NXTARGET nexus4"
fi
# HDF5
HDF5SEARCH="/usr/local/hdf5"
if test "$with_hdf5" != "no" -a -z "$H5ROOT"; then
    AC_MSG_CHECKING(for location of HDF5 libraries)
    for i in $HDF5SEARCH; do
	if test -d $i; then H5ROOT=$i; fi
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
    NXTARGET="$NXTARGET nexus5"
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

# compiler specifi targets
if test ! -z "$F77"; then 
    NXTARGET="$NXTARGET nexus77"
    BINDINGS="$BINDINGS bindings/f77"
fi
if test ! -z "$F90"; then 
    NXTARGET="$NXTARGET nexus90"
    BINDINGS="$BINDINGS bindings/f90"
fi

AC_CONFIG_FILES([Makefile src/Makefile applications/Makefile
		test/Makefile bindings/f77/Makefile
		bindings/f90/Makefile])
AC_OUTPUT
