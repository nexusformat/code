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
#  For further information, see <http://www.nexusformat.org/>
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
# $3 = list of root paths to try. If the path is a file, assume it is 
#                                 path/bin/files and then work out path
# $4 = file in path to locate
# $5 = default withval
#
# e.g. AC_CHECK_ROOT([tcl],[TCLROOT],[/usr /usr/local],[include/tcl.h])
#
AC_DEFUN(
  [AC_CHECK_ROOT],
  [ $2=""
    AC_SUBST([$2])
    AC_ARG_WITH([$1],
	AC_HELP_STRING([--with-$1=/path/to/$1_install_directory],
                       [Specify absolute path to root of $1 install directory.]),
	[if test x$withval != xno -a x$withval != xyes; then $2=$withval; fi], 
        [with_$1=$5])
    if test x$with_$1 != xno; then
        AC_MSG_CHECKING(for $1 root installation directory)
# if --with secified a file, assume it is ROOT/bin/file
	if test x[$]$2 != x; then
	    if test -f [$]$2; then
		$2=`dirname [$]$2`/..
	    fi
	fi
# $2 should now either be empty or a valid possible root
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

AC_DEFUN([AC_CHECK_PYTHON_MODULE],
[
    if test -z "$PYTHON"; then PYTHON="python"; fi
    AC_MSG_CHECKING(for python module $1)
    $PYTHON -c "import $1" 2>/dev/null
    if test $? -eq 0; then
        eval PYTHON_$1=1
        eval PYTHON_$1_found="yes"
        AC_MSG_RESULT(found)
    else
        eval PYTHON_$1=0
        eval PYTHON_$1_found="no"
        AC_MSG_RESULT(not found)
    fi
    AC_SUBST(PYTHON_$1)
])

# http://ac-archive.sourceforge.net/ac-archive/ac_prog_java_works.html

AC_DEFUN([AC_PROG_JAVA_WORKS], [
AC_CHECK_PROG(uudecode, uudecode$EXEEXT, yes)
if test x$uudecode = xyes; then
AC_CACHE_CHECK([if uudecode can decode base 64 file], ac_cv_prog_uudecode_base64, [
dnl /**
dnl  * Test.java: used to test if java compiler works.
dnl  */
dnl public class Test
dnl {
dnl
dnl public static void
dnl main( String[] argv )
dnl {
dnl     System.exit (0);
dnl }
dnl
dnl }
cat << \EOF > Test.uue
begin-base64 644 Test.class
yv66vgADAC0AFQcAAgEABFRlc3QHAAQBABBqYXZhL2xhbmcvT2JqZWN0AQAE
bWFpbgEAFihbTGphdmEvbGFuZy9TdHJpbmc7KVYBAARDb2RlAQAPTGluZU51
bWJlclRhYmxlDAAKAAsBAARleGl0AQAEKEkpVgoADQAJBwAOAQAQamF2YS9s
YW5nL1N5c3RlbQEABjxpbml0PgEAAygpVgwADwAQCgADABEBAApTb3VyY2VG
aWxlAQAJVGVzdC5qYXZhACEAAQADAAAAAAACAAkABQAGAAEABwAAACEAAQAB
AAAABQO4AAyxAAAAAQAIAAAACgACAAAACgAEAAsAAQAPABAAAQAHAAAAIQAB
AAEAAAAFKrcAErEAAAABAAgAAAAKAAIAAAAEAAQABAABABMAAAACABQ=
====
EOF
if uudecode$EXEEXT Test.uue; then
        ac_cv_prog_uudecode_base64=yes
else
        echo "configure: __oline__: uudecode had trouble decoding base 64 file 'Test.uue'" >&AC_FD_CC
        echo "configure: failed file was:" >&AC_FD_CC
        cat Test.uue >&AC_FD_CC
        ac_cv_prog_uudecode_base64=no
fi
rm -f Test.uue])
fi
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        rm -f Test.class
        AC_MSG_WARN([I have to compile Test.class from scratch])
        if test x$ac_cv_prog_javac_works = xno; then
                AC_MSG_ERROR([Cannot compile java source. $JAVAC does not work properly])
        fi
        if test x$ac_cv_prog_javac_works = x; then
                AC_PROG_JAVAC
        fi
fi
AC_CACHE_CHECK(if $JAVA works, ac_cv_prog_java_works, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
TEST=Test
changequote(, )dnl
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
public static void main (String args[]) {
        System.exit (0);
} }
EOF
changequote([, ])dnl
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) && test -s $CLASS_TEST; then
                :
        else
          echo "configure: failed program was:" >&AC_FD_CC
          cat $JAVA_TEST >&AC_FD_CC
          AC_MSG_ERROR(The Java compiler $JAVAC failed (see config.log, check the CLASSPATH?))
        fi
fi
if AC_TRY_COMMAND($JAVA $JAVAFLAGS $TEST) >/dev/null 2>&1; then
  ac_cv_prog_java_works=yes
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat $JAVA_TEST >&AC_FD_CC
  AC_MSG_ERROR(The Java VM $JAVA failed (see config.log, check the CLASSPATH?))
fi
rm -fr $JAVA_TEST $CLASS_TEST Test.uue
])
AC_PROVIDE([$0])dnl
]
)

# http://ac-archive.sourceforge.net/ac-archive/ac_prog_java.html

AC_DEFUN([AC_PROG_JAVA],[
AC_REQUIRE([AC_EXEEXT])dnl
if test x$JAVAPREFIX = x; then
        test x$JAVA = x && AC_CHECK_PROGS(JAVA, kaffe$EXEEXT java$EXEEXT)
else
        test x$JAVA = x && AC_CHECK_PROGS(JAVA, kaffe$EXEEXT java$EXEEXT, $JAVAPREFIX)
fi
test x$JAVA = x && AC_MSG_ERROR([no acceptable Java virtual machine found in \$PATH])
AC_PROG_JAVA_WORKS
AC_PROVIDE([$0])dnl
])

# http://ac-archive.sourceforge.net/ac-archive/ac_prog_javac_works.html

AC_DEFUN([AC_PROG_JAVAC_WORKS],[
AC_CACHE_CHECK([if $JAVAC works], ac_cv_prog_javac_works, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
}
EOF
if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) >/dev/null 2>&1; then
  ac_cv_prog_javac_works=yes
else
  AC_MSG_ERROR([The Java compiler $JAVAC failed (see config.log, check the CLASSPATH?)])
  echo "configure: failed program was:" >&AC_FD_CC
  cat $JAVA_TEST >&AC_FD_CC
fi
rm -f $JAVA_TEST $CLASS_TEST
])
AC_PROVIDE([$0])dnl
])


# http://ac-archive.sourceforge.net/ac-archive/ac_prog_javac.html

AC_DEFUN([AC_PROG_JAVAC],[
AC_REQUIRE([AC_EXEEXT])dnl
if test "x$JAVAPREFIX" = x; then
        test "x$JAVAC" = x && AC_CHECK_PROGS(JAVAC, "gcj$EXEEXT -C" guavac$EXEEXT jikes$EXEEXT javac$EXEEXT)
else
        test "x$JAVAC" = x && AC_CHECK_PROGS(JAVAC, "gcj$EXEEXT -C" guavac$EXEEXT jikes$EXEEXT javac$EXEEXT, $JAVAPREFIX)
fi
test "x$JAVAC" = x && AC_MSG_ERROR([no acceptable Java compiler found in \$PATH])
AC_PROG_JAVAC_WORKS
AC_PROVIDE([$0])dnl
])

# http://ac-archive.sourceforge.net/ac-archive/ac_try_compile_java.html

AC_DEFUN([AC_TRY_COMPILE_JAVA],[
AC_REQUIRE([AC_PROG_JAVAC])dnl
cat << \EOF > Test.java
/* [#]line __oline__ "configure" */
ifelse([$1], , , [import $1;])
public class Test {
[$2]
}
EOF
if AC_TRY_COMMAND($JAVAC $JAVACFLAGS Test.java) && test -s Test.class
then
dnl Don't remove the temporary files here, so they can be examined.
  ifelse([$3], , :, [$3])
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat Test.java >&AC_FD_CC
ifelse([$4], , , [  rm -fr Test*
  $4
])dnl
fi
rm -fr Test*])

# http://ac-archive.sourceforge.net/ac-archive/ac_check_class.html

AC_DEFUN([AC_CHECK_CLASS],[
AC_REQUIRE([AC_PROG_JAVA])
ac_var_name=`echo $1 | sed 's/\./_/g'`
dnl Normaly I'd use a AC_CACHE_CHECK here but since the variable name is
dnl dynamic I need an extra level of extraction
AC_MSG_CHECKING([for $1 class])
AC_CACHE_VAL(ac_cv_class_$ac_var_name, [
if test x$ac_cv_prog_uudecode_base64 = xyes; then
dnl /**
dnl  * Test.java: used to test dynamicaly if a class exists.
dnl  */
dnl public class Test
dnl {
dnl
dnl public static void
dnl main( String[] argv )
dnl {
dnl     Class lib;
dnl     if (argv.length < 1)
dnl      {
dnl             System.err.println ("Missing argument");
dnl             System.exit (77);
dnl      }
dnl     try
dnl      {
dnl             lib = Class.forName (argv[0]);
dnl      }
dnl     catch (ClassNotFoundException e)
dnl      {
dnl             System.exit (1);
dnl      }
dnl     lib = null;
dnl     System.exit (0);
dnl }
dnl
dnl }
cat << \EOF > Test.uue
begin-base64 644 Test.class
yv66vgADAC0AKQcAAgEABFRlc3QHAAQBABBqYXZhL2xhbmcvT2JqZWN0AQAE
bWFpbgEAFihbTGphdmEvbGFuZy9TdHJpbmc7KVYBAARDb2RlAQAPTGluZU51
bWJlclRhYmxlDAAKAAsBAANlcnIBABVMamF2YS9pby9QcmludFN0cmVhbTsJ
AA0ACQcADgEAEGphdmEvbGFuZy9TeXN0ZW0IABABABBNaXNzaW5nIGFyZ3Vt
ZW50DAASABMBAAdwcmludGxuAQAVKExqYXZhL2xhbmcvU3RyaW5nOylWCgAV
ABEHABYBABNqYXZhL2lvL1ByaW50U3RyZWFtDAAYABkBAARleGl0AQAEKEkp
VgoADQAXDAAcAB0BAAdmb3JOYW1lAQAlKExqYXZhL2xhbmcvU3RyaW5nOylM
amF2YS9sYW5nL0NsYXNzOwoAHwAbBwAgAQAPamF2YS9sYW5nL0NsYXNzBwAi
AQAgamF2YS9sYW5nL0NsYXNzTm90Rm91bmRFeGNlcHRpb24BAAY8aW5pdD4B
AAMoKVYMACMAJAoAAwAlAQAKU291cmNlRmlsZQEACVRlc3QuamF2YQAhAAEA
AwAAAAAAAgAJAAUABgABAAcAAABtAAMAAwAAACkqvgSiABCyAAwSD7YAFBBN
uAAaKgMyuAAeTKcACE0EuAAaAUwDuAAasQABABMAGgAdACEAAQAIAAAAKgAK
AAAACgAAAAsABgANAA4ADgATABAAEwASAB4AFgAiABgAJAAZACgAGgABACMA
JAABAAcAAAAhAAEAAQAAAAUqtwAmsQAAAAEACAAAAAoAAgAAAAQABAAEAAEA
JwAAAAIAKA==
====
EOF
                if uudecode$EXEEXT Test.uue; then
                        :
                else
                        echo "configure: __oline__: uudecode had trouble decoding base 64 file 'Test.uue'" >&AC_FD_CC
                        echo "configure: failed file was:" >&AC_FD_CC
                        cat Test.uue >&AC_FD_CC
                        ac_cv_prog_uudecode_base64=no
                fi
        rm -f Test.uue
        if AC_TRY_COMMAND($JAVA $JAVAFLAGS Test $1) >/dev/null 2>&1; then
                eval "ac_cv_class_$ac_var_name=yes"
        else
                eval "ac_cv_class_$ac_var_name=no"
        fi
        rm -f Test.class
else
        AC_TRY_COMPILE_JAVA([$1], , [eval "ac_cv_class_$ac_var_name=yes"],
                [eval "ac_cv_class_$ac_var_name=no"])
fi
eval "ac_var_val=$`eval echo ac_cv_class_$ac_var_name`"
eval "HAVE_$ac_var_name=$`echo ac_cv_class_$ac_var_val`"
HAVE_LAST_CLASS=$ac_var_val
if test x$ac_var_val = xyes; then
        ifelse([$2], , :, [$2])
else
        ifelse([$3], , :, [$3])
fi
])
dnl for some reason the above statment didn't fall though here?
dnl do scripts have variable scoping?
eval "ac_var_val=$`eval echo ac_cv_class_$ac_var_name`"
AC_MSG_RESULT($ac_var_val)
])

# ===========================================================================
#      http://www.gnu.org/software/autoconf-archive/ax_check_junit.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_JUNIT
#
# DESCRIPTION
#
#   AX_CHECK_JUNIT tests the availability of the Junit testing framework,
#   and set some variables for conditional compilation of the test suite by
#   automake.
#
#   If available, JUNIT is set to a command launching the text based user
#   interface of Junit, @JAVA_JUNIT@ is set to $JAVA_JUNIT and @TESTS_JUNIT@
#   is set to $TESTS_JUNIT, otherwise they are set to empty values.
#
#   You can use these variables in your Makefile.am file like this :
#
#    # Some of the following classes are built only if junit is available
#    JAVA_JUNIT  = Class1Test.java Class2Test.java AllJunitTests.java
#
#    noinst_JAVA = Example1.java Example2.java @JAVA_JUNIT@
#
#    EXTRA_JAVA  = $(JAVA_JUNIT)
#
#    TESTS_JUNIT = AllJunitTests
#
#    TESTS       = StandaloneTest1 StandaloneTest2 @TESTS_JUNIT@
#
#    EXTRA_TESTS = $(TESTS_JUNIT)
#
#    AllJunitTests :
#       echo "#! /bin/sh" > $@
#       echo "exec @JUNIT@ my.package.name.AllJunitTests" >> $@
#       chmod +x $@
#
# LICENSE
#
#   Copyright (c) 2008 Luc Maisonobe <luc@spaceroots.org>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

AU_ALIAS([AC_CHECK_JUNIT], [AX_CHECK_JUNIT])
AC_DEFUN([AX_CHECK_JUNIT],[
AC_CACHE_VAL(ac_cv_prog_JUNIT,[
AC_CHECK_CLASS(junit.textui.TestRunner)
if test x"`eval 'echo $ac_cv_class_junit_textui_TestRunner'`" != xno ; then
  ac_cv_prog_JUNIT='$(CLASSPATH_ENV) $(JAVA) $(JAVAFLAGS) junit.textui.TestRunner'
fi])
AC_MSG_CHECKING([for junit])
if test x"`eval 'echo $ac_cv_prog_JUNIT'`" != x ; then
  JUNIT="$ac_cv_prog_JUNIT"
  JAVA_JUNIT='$(JAVA_JUNIT)'
  TESTS_JUNIT='$(TESTS_JUNIT)'
else
  JUNIT=
  JAVA_JUNIT=
  TESTS_JUNIT=
fi
AC_MSG_RESULT($JAVA_JUNIT)
AC_SUBST(JUNIT)
AC_SUBST(JAVA_JUNIT)
AC_SUBST(TESTS_JUNIT)])
