#
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
# AC_CHECK_C_OPTION tests for a given compiler option; we need to egrep
# the output as well as check the status as sometimes the compiler 
# will return success for invalid options
#
AC_DEFUN(
 [AC_CHECK_C_OPTION],
 [AC_MSG_CHECKING([for C compiler option $1])
  AC_LANG_PUSH(C)
  CFLAGS_SAVE=$CFLAGS
  ac_compile="$ac_compile 2>check_c_option.$$"
  CFLAGS="$CFLAGS $1"
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM],
    [C_OPTION=yes],
    [C_OPTION=no]) 
  if test $C_OPTION = "yes"; then
    $EGREP "unrecognized|unrecognised|unknown|invalid" check_c_option.$$ >/dev/null 2>&1 && C_OPTION="no" 
  fi
  if test $C_OPTION = "yes"; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    CFLAGS=$CFLAGS_SAVE
  fi
  rm -f check_c_option.$$
  AC_LANG_POP(C)]
)
#
