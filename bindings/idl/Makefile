#	$Id: //depot/idl/IDL_63_RELEASE/idldir/external/dlm/Makefile#1 $
#
#
#  Copyright (c) 1988-2006, Research Systems Inc.  All rights reserved.
#  This software includes information which is proprietary to and a
#  trade secret of Research Systems, Inc.  It is not to be disclosed
#  to anyone outside of this organization. Reproduction by any means
#  whatsoever is  prohibited without express written permission.
#
#
# Unix makefile for NeXusIDL-API DLM.
#
# This makefile is used to build NeXusIDL-API --- an example DLM.
# It works by determining what platform you are on and then
# recursively invoking make with the proper options to do the build.
#


# 	The following macro might need to be edited if the location
#	of IDL is not the standard location on your system.
# NeXusIDL-API

#Configure these directories:
IDL_DIR = /usr/pack/idl-6.3-vm/idl/
#HDFROOT=/afs/psi.ch/project/sinq/sl5
HDFROOT=/afs/psi.ch/project/sinq/sl-linux

LIBNEXUS = ../../src/.libs/libNeXus.a

# The following macros establish general defaults. They are overridden
# in the "all" rule as needed to satisfy a given platform's requirements.

CC		= cc
C_FLAGS		= -I$(IDL_DIR)/external/include -g -c $(CFLAGS)
LD 		= ld
SHELL		= /bin/sh
X_CFLAGS	=
X_LD_FLAGS	=
X_LD_POST	=
SO_EXT		=so


.c.o :
	$(CC) $(C_FLAGS) $(X_CFLAGS) $*.c


# The following is the default entry point. This section will determine 
# what system we are on and call make again with the correct flags set.

all : 
	@echo "OS type detected: "`uname`
	@case `uname` in \
	   "SunOS") if [ `/bin/uname -p` != i386 ]; then \
			echo "" ;\
			echo "We can build either a 32-bit or a 64-bit" ;\
			echo "version of this DLM. Please use the command" ;\
			echo "          make sun32" ;\
			echo "for a 32-bit version, or" ;\
			echo "          make sun64" ;\
			echo "for the 64-bit version. It is important that" ;\
			echo "you build the correct version for the IDL" ;\
			echo "binary you are using. Otherwise the DLM will" ;\
			echo "fail to load." ;\
		    else \
			make NeXusIDL-API \
			"X_CFLAGS=-K pic" \
			"X_LD_FLAGS=-B symbolic -z nodefs -G" ; \
		    fi \
		;; \
	   "AIX") make NeXusIDL-API \
			"LD=cc" \
			"SO_EXT=a" \
			"X_CFLAGS=-qro -qroconst" \
			"X_LD_FLAGS= -bM:SRE -bnoentry -btextro -bE:NeXusIDL-API.export -bI:$(IDL_DIR)/external/idl.export" ;;\
	   "Darwin" ) make NeXusIDL-API \
			"X_CFLAGS= -fPIC" \
			"X_LD_FLAGS= -flat_namespace -undefined suppress -bundle" ;; \
	   "HP-UX") make NeXusIDL-API  \
			"SO_EXT= sl" \
			"X_CFLAGS=+z +e -Aa -D_HPUX_SOURCE" \
			"X_LD_FLAGS= -b -Brestricted +s" ;; \
	   "IRIX" )  make NeXusIDL-API \
			"X_CFLAGS=-n32 -mips3" \
		       "X_LD_FLAGS=-Bsymbolic -n32 -mips3 -shared -all -rdata_shared" ;;\
	   "IRIX64" )  make NeXusIDL-API \
		       "X_CFLAGS=-n32 -mips3" \
		       "X_LD_FLAGS=-Bsymbolic -n32 -mips3 -shared -all -rdata_shared" ;;\
	   "OSF1" )  make NeXusIDL-API \
			"X_CFLAGS=-float" \
			"X_LD_FLAGS=-expect_unresolved '*' -shared -all" ;;\
	   "Linux" ) make NeXusIDL-API \
			"X_CFLAGS= -fPIC" \
			"X_LD_FLAGS= -shared -Bsymbolic --warn-once -noinhibit-exec " ; \
		;; \
	   *) echo "This system is not supported" ;; \
       esac



# We can't tell if you want a 32-bit or a 64-bit version of the DLM
# on a Sparc/Solaris system because we could build either. Hence, we
# force you to specify the target on the make command line, leading you to
# here
sun32 :
	make NeXusIDL-API "X_CFLAGS=-K pic" "X_LD_FLAGS=-B symbolic -z nodefs -G"

sun64 :
	make NeXusIDL-API "X_CFLAGS=-K pic -xtarget=ultra -xarch=v9" \
		"X_LD_FLAGS=-B symbolic -z nodefs -G"



# The following is the entry point for the recursive build

NeXusIDL-API : NeXusIDL-API.$(SO_EXT) handle.o 
	@date


#Original:
#NeXusIDL-API.$(SO_EXT) : NeXusIDL-API.o
#	$(LD) $(X_LD_FLAGS) -o NeXusIDL-API.$(SO_EXT) NeXusIDL-API.o $(X_LD_POST)

NeXusIDL-API.$(SO_EXT) : NeXusIDL-API.o
	$(LD) $(X_LD_FLAGS) -g -o NeXusIDL-API.$(SO_EXT) NeXusIDL-API.o handle.o $(X_LD_POST) \
	$(LIBNEXUS)  -L$(HDFROOT)/lib  $(HDFROOT)/lib/libhdf5.a -lmfhdf \
	-ldf -lmxml -lsz /usr/lib/libz.a  -ljpeg -lm


clean : 
	rm -f NeXusIDL-API.o NeXusIDL-API.so NeXusIDL-API.sl NeXusIDL-API.a \
		so_locations
