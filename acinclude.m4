#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  $Id$
#
#  Local autoconf m4 macros - these used by configure.ac
#  and automatically added to aclocal.m4 by running the "aclocal"
#  command (a separate file is needed becausle automake needs to
#  add extra stuff to aclocal.m4 as well)
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
# AC_CHECK_COMPILER_OPTION tests for a given compiler option; we need to egrep
# the output as well as check the status as sometimes the compiler 
# will return success for invalid options
#
# Argument 1 is printed and is the real name of the language
# Argument 2 must be "Fortran 77" or "C" (language to "Push")
# Argument 3 must be FFLAGS or CFLAGS as appropriate for argument 2
# Argument 4 is the compiler option to check
# Argumnet 5 is any extra program body for the test program
#
AC_DEFUN(
 [AC_CHECK_COMPILER_OPTION],
 [AC_MSG_CHECKING([for $1 compiler option $4])
  AC_LANG_PUSH($2)
  COMPFLAGS_SAVE=[$]$3
  ac_compile="$ac_compile >check_compiler_option.$$ 2>&1"
  $3="[$]$3 $4"
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(,[$5])],
    [COMPILER_OPTION=yes],
    [COMPILER_OPTION=no]) 
  if test $COMPILER_OPTION = "yes"; then
    if test `$EGREP "[Uu]nrecogni[sz]ed|[Uu]nknown|[Ii]nvalid|[Ee]rror" check_compiler_option.$$ | wc -l` -gt 0; then COMPILER_OPTION="no"; fi
  fi
  if test $COMPILER_OPTION = "yes"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    $3=$COMPFLAGS_SAVE
  fi
  rm -f check_compiler_option.$$
  AC_LANG_POP($2)]
)
#
# AC_CHECK_C_OPTION
#
AC_DEFUN(
 [AC_CHECK_C_OPTION],
 [AC_CHECK_COMPILER_OPTION(C,C,CFLAGS,[$1],[$2])]
)
#
# AC_CHECK_CPP_OPTION
#
AC_DEFUN(
 [AC_CHECK_CPP_OPTION],
 [AC_CHECK_COMPILER_OPTION(C,C,CPPFLAGS,[$1],[$2])]
)
#
# AC_CHECK_F77_OPTION
#
AC_DEFUN(
 [AC_CHECK_F77_OPTION],
 [AC_CHECK_COMPILER_OPTION(Fortran 77,Fortran 77,FFLAGS,[$1],[$2])]
)
#
# AC_CHECK_F90_OPTION
# We use the F77 test, but switch the name of the compiler
#
AC_DEFUN(
 [AC_CHECK_F90_OPTION],
 [F77_SAVE=[$]F77
  FFLAGS_SAVE=[$]FFLAGS
  F77=[$]F90
  FFLAGS=[$]F90FLAGS
  AC_CHECK_COMPILER_OPTION(Fortran 90,Fortran 77,FFLAGS,[$1],[$2])
  F77=[$]F77_SAVE
  FFLAGS=[$]FFLAGS_SAVE
 ]
)
AC_DEFUN([LINUX_DISTRIBUTION],
[
	AC_REQUIRE([AC_CANONICAL_TARGET])
	DISTRIBUTION=""
	case "$target" in
		i[[3456]]86-*-linux-* | i[[3456]]86-*-linux)
			if test -f /etc/lsb-release ; then
				DISTRIBUTION=`(. /etc/lsb-release; echo $DISTRIB_DESCRIPTION)`
			fi
			if test -z "$DISTRIBUTION"; then
				for i in /etc/*-release; do
				    if test "$i" != lsb-release; then DISTRIBUTION=`cat $i | head -1`; fi
				done
			fi
			;;
	esac
	if test -z "$DISTRIBUTION"; then DISTRIBUTION="Unknown"; fi
	AC_SUBST([DISTRIBUTION])
])

# AC_CHECK_ROOT
#
# $1 = name of arg
# $2 = root variable name to set
# $3 = list of root paths to try
# $4 = file in path to locate
#
# e.g. AC_CHECK_ROOT([tcl],[TCLROOT],[/usr /usr/local],[include/tcl.h])
#
AC_DEFUN(
  [AC_CHECK_ROOT],
  [ $2=""
    AC_SUBST([$2])
    AC_ARG_WITH([$1],
	AC_HELP_STRING([--with-$1=/path/to/$1/directory],
                       [Specify location of $1 install directory]),
	[if test x$withval != xno; then $2=$withval; fi], 
        [])
    if test x$withval != xno -a x[$]$2 = x; then
        AC_MSG_CHECKING(for $1 root installation directory)
        for i in $3; do
	    if test x[$]$2 = x -a -r $i/$4; then $2=$i; fi
        done
        if test x[$]$2 = x; then 
	    AC_MSG_RESULT(unknown)
        else 
	    AC_MSG_RESULT([$]$2)
        fi
    fi
  ])
