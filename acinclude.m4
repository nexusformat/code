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
  ac_compile="$ac_compile 2>check_compiler_option.$$"
  $3="[$]$3 $4"
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(,[$5])],
    [COMPILER_OPTION=yes],
    [COMPILER_OPTION=no]) 
  if test $COMPILER_OPTION = "yes"; then
    $EGREP "unrecognized|unrecognised|unknown|invalid|error" check_compiler_option.$$ >/dev/null 2>&1 && COMPILER_OPTION="no" 
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
	case "$target" in
		i[[3456]]86-*-linux-* | i[[3456]]86-*-linux)
			if test -f /etc/lsb-release ; then
				DISTRIBUTION=`(. /etc/lsb-release; echo $DISTRIB_DESCRIPTION)`
			else
				DISTRIBUTION=`cat /etc/*-release | head -1`
			fi
			;;
	esac
	AC_SUBST([DISTRIBUTION])
])
