#!/bin/sh

for automake in automake-1.10 automake-1.9 automake-1.8 automake-1.7 automake-1.6 automake-1.5 automake-1.4 automake ; do
	AUTOMAKE=`which $automake 2>/dev/null |  grep -v '^no'`
	if test "$AUTOMAKE" ; then
		break;
	fi
done
AUTOMAKE="$automake -a -c"
case $automake in
	automake-*)
		version=`echo $automake | cut -f2 -d'-'`
		ACLOCAL="aclocal-$version";;
	*)
		ACLOCAL="aclocal";
esac
	
for autoconf in autoconf-2.59 autoconf-2.57 autoconf-2.53 autoconf-2.52 autoconf ; do 
	AUTOCONF=`which $autoconf 2>/dev/null | grep -v '^no'`
	if test "$AUTOCONF" ; then
		break;
	fi
done
AUTOCONF="$autoconf"
case $autoconf in
	autoconf-*)
		version=`echo $autoconf | cut -f2 -d'-'`
		AUTOHEADER="autoheader-$version";;
	*)
		AUTOHEADER="autoheader";;
esac

export AUTOMAKE ACLOCAL AUTOCONF AUTOHEADER

