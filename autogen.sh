#!/bin/sh

rm -fr autom4te.cache

PACKAGE_RELEASE=""
NEXUS_VERSION=""
NEXUS_RELEASE=""
while getopts "p:r:v:" opt; do
  case $opt in
    p)
	PACKAGE_RELEASE="$OPTARG"
	;;
    v)
	NEXUS_VERSION="$OPTARG"
	;;
    r)
	NEXUS_RELEASE="$OPTARG"
	;;
    \?)
	echo "Invalid option: -$OPTARG" >&2
	;;
  esac
done
if test ! -z "$PACKAGE_RELEASE"; then
    echo "Setting package release to $PACKAGE_RELEASE"
    mv configure.ac configure.ac.$$
    sed -e "s/^m4_define.*PACKAGE_REL.*/m4_define\([PACKAGE_REL],[$PACKAGE_RELEASE]\)dnl/" < configure.ac.$$ > configure.ac
    rm -f configure.ac.$$
fi
if test ! -z "$NEXUS_VERSION"; then
    echo "Setting NeXus version to $NEXUS_VERSION"
    mv configure.ac configure.ac.$$
    sed -e "s/^m4_define.*NEXUS_VERSION.*/m4_define\([NEXUS_VERSION],[$NEXUS_VERSION]\)dnl/" < configure.ac.$$ > configure.ac
    rm -f configure.ac.$$
fi
if test ! -z "$NEXUS_RELEASE"; then
    echo "Setting NeXus release to $NEXUS_RELEASE"
    mv configure.ac configure.ac.$$
    sed -e "s/^m4_define.*NEXUS_REL.*/m4_define\([NEXUS_REL],[$NEXUS_RELEASE]\)dnl/" < configure.ac.$$ > configure.ac
    rm -f configure.ac.$$
fi

for libtoolize in glibtoolize libtoolize ; do 
	LIBTOOLIZE=`which $libtoolize 2>/dev/null`
	if test "$LIBTOOLIZE" ; then
		break;
	fi
done
LIBTOOLIZE="$libtoolize --force --copy --automake"

# set up environment variables with the correct
# version of autoconf and automake - this also needs
# to be done in configure.ac to maintain consistancy
. ./autoversion.sh

touch AUTHORS NEWS README
rm -f ChangeLog

# Discover what version of autoconf we are using.
autoconfversion=`$AUTOCONF --version | head -n 1`
automakeversion=`$AUTOMAKE --version | head -n 1`
libtoolversion=`$LIBTOOLIZE --version | head -n 1`

echo "Using $autoconfversion"
echo "Using $automakeversion"
echo "Using $libtoolversion"
case $autoconfversion in
    *2.6*)
	;;
    *2.7*)
	;;
    *)
	echo "This autoconf version is not supported by NeXus."
	echo "NeXus only supports autoconf 2.61 and above"
	echo "You may download it from ftp://ftp.gnu.org/gnu/autoconf"
	exit
	;;
esac

case $automakeversion in
    *1.[4-9]*)
	;;
    *1.1[0-9]*)
	;;
    *)
	echo "This automake version is not supported by NeXus."
	echo "NeXus only supports automake 1.4 and above"
	echo "You may download it from ftp://ftp.gnu.org/gnu/automake"
	exit
	;;
esac

case $libtoolversion in
    *1.[456]*)
	;;
    *[2-9].*)
	;;
    *)
	echo "This libtool version is not supported by NeXus."
	echo "NeXus only supports libtool 1.4 and above"
	echo "You may download it from ftp://ftp.gnu.org/gnu/libtool"
	exit
	;;
esac

echo -n "Locating GNU m4... "
GNUM4=
for prog in $M4 gm4 gnum4 m4; do
	# continue if $prog generates error (e.g. does not exist)
	( $prog --version ) < /dev/null > /dev/null 2>&1
	if test $? -ne 0 ; then continue; fi

	# /dev/null input prevents a hang of the script for some m4 compilers (e.g. on FreeBSD)
	case `$prog --version < /dev/null 2>&1` in
	*GNU*)	GNUM4=$prog
		break ;;
	esac
done
if test x$GNUM4 = x ; then
	echo "not found."
	exit
else
	echo `which $GNUM4`
fi

# 
if { test ! -d config ; } ; then 
	mkdir config ; 
else 
	rm -rf config/* ;  
fi

# Prepare the use of libtool
if ( $LIBTOOLIZE --version ) < /dev/null > /dev/null 2>&1 ; then
	echo "Preparing the use of libtool ..."
	$LIBTOOLIZE
	if { test -r ltmain.sh ; } ; then mv ltmain.sh config/ ; fi 
	echo "done."
else
	echo "libtoolize not found -- aborting"
	exit
fi

# Generate the Makefiles and configure files
if ( $ACLOCAL --version ) < /dev/null > /dev/null 2>&1; then
	echo "Building macros..."
	$ACLOCAL
	echo "done."
else
	echo "aclocal not found -- aborting"
	exit
fi

if ( $AUTOHEADER --version ) < /dev/null > /dev/null 2>&1; then
	echo "Building config header template..."
	$AUTOHEADER 
	echo "done."
else
	echo "autoheader not found -- aborting"
	exit
fi

if ( $AUTOMAKE --version ) < /dev/null > /dev/null 2>&1; then
	echo "Building Makefile templates..."
	$AUTOMAKE
	echo "done."
else
	echo "automake not found -- aborting"
	exit
fi

if ( $AUTOCONF --version ) < /dev/null > /dev/null 2>&1; then
	echo "Building configure..."
	$AUTOCONF
	echo "done."
else
	echo "autoconf not found -- aborting"
	exit
fi

