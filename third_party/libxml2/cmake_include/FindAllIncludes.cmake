# Process this file with cmake
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#
#  CMakeLists for building the NeXus library and applications.
#
#  Copyright (C) 2011 Stephen Rankin
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
#
#====================================================================

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckLibraryExists.cmake)

#CHECK_INCLUDE_FILE("stddef.h" HAVE_STDDEF_H)
#CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)
#CHECK_INCLUDE_FILE("stdlib.h" HAVE_STDLIB_H)
#CHECK_INCLUDE_FILE("setjmp.h" HAVE_SETJMP_H)
#CHECK_INCLUDE_FILE("string.h" HAVE_STRING_H)
#CHECK_INCLUDE_FILE("stdio.h" HAVE_STDIO_H)
#CHECK_INCLUDE_FILE("math.h" HAVE_MATH_H)
#CHECK_INCLUDE_FILE("time.h" HAVE_TIME_H)
#CHECK_INCLUDE_FILE("sys/time.h" HAVE_SYS_TIME_H)
#CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)
#CHECK_INCLUDE_FILE("sys/socket.h" HAVE_SYS_SOCKET_H)
#CHECK_INCLUDE_FILE("sys/stat.h" HAVE_SYS_STAT_H)
#CHECK_INCLUDE_FILE("netinet/in.h" HAVE_NETINET_IN_H)
#CHECK_INCLUDE_FILE("arpa/inet.h" HAVE_ARPA_INET_H)
#CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
#CHECK_INCLUDE_FILE("fcntl.h" HAVE_FCNTL_H)
#CHECK_INCLUDE_FILE("errno.h" HAVE_ERRNO_H)

find_library(M m)
#find_library(RPC rpc)
find_library(DL dl)
find_library(PTHREAD pthread)
find_library(TERMCAP termcap)
find_library(READLINE readline)
find_library(HISTORY history)
find_package(ZLIB)
if(M)
   set(M_LINK "-lm")
endif(M)

if(PTHREAD)
   set(PTHREAD_LINK "-lpthread")
endif(PTHREAD)

if(SZIPLIB_FOUND)
   set(SZIP_LINK "-lsz")
endif(SZIPLIB_FOUND)


#check_function_exists(va_copy HAVE_VA_COPY)

CHECK_INCLUDE_FILE("ansidecl.h" HAVE_ANSIDECL_H)

CHECK_INCLUDE_FILE("arpa/inet.h" HAVE_ARPA_INET_H)

CHECK_INCLUDE_FILE("arpa/nameser.h" HAVE_ARPA_NAMESER_H)

CHECK_INCLUDE_FILE("ctype.h" HAVE_CTYPE_H)

CHECK_INCLUDE_FILE("dirent.h" HAVE_DIRENT_H)

CHECK_INCLUDE_FILE("dlfcn.h" HAVE_DLFCN_H)

CHECK_INCLUDE_FILE("dl.h" HAVE_DL_H)

CHECK_INCLUDE_FILE("errno.h" HAVE_ERRNO_H)

CHECK_INCLUDE_FILE("fcntl.h" HAVE_FCNTL_H)

CHECK_INCLUDE_FILE("float.h" HAVE_FLOAT_H)

CHECK_INCLUDE_FILE("fp_class.h" HAVE_FP_CLASS_H)

CHECK_INCLUDE_FILE("ieeefp.h" HAVE_IEEEFP_H)

CHECK_INCLUDE_FILE("inttypes.h" HAVE_INTTYPES_H)

CHECK_INCLUDE_FILE("inttypes.h.h" HAVE_INTTYPES_H_H)

CHECK_INCLUDE_FILE("limits.h" HAVE_LIMITS_H)

CHECK_INCLUDE_FILE("malloc.h" HAVE_MALLOC_H)

CHECK_INCLUDE_FILE("math.h" HAVE_MATH_H)

CHECK_INCLUDE_FILE("memory.h" HAVE_MEMORY_H)

CHECK_INCLUDE_FILE("nan.h" HAVE_NAN_H)

CHECK_INCLUDE_FILE("ndir.h" HAVE_NDIR_H)

CHECK_INCLUDE_FILE("netdb.h" HAVE_NETDB_H)

CHECK_INCLUDE_FILE("netinet/in.h" HAVE_NETINET_IN_H)

CHECK_INCLUDE_FILE("poll.h" HAVE_POLL_H)

CHECK_INCLUDE_FILE("pthread.h" HAVE_PTHREAD_H)

CHECK_INCLUDE_FILE("resolv.h" HAVE_RESOLV_H)

CHECK_INCLUDE_FILE("signal.h" HAVE_SIGNAL_H)

CHECK_INCLUDE_FILE("stdarg.h" HAVE_STDARG_H)
IF(HAVE_STDARG_H)
    CHECK_SYMBOL_EXISTS(va_copy "stdarg.h" HAVE_VA_COPY)
ENDIF(HAVE_STDARG_H)

CHECK_INCLUDE_FILE("stdint.h" HAVE_STDINT_H)

CHECK_INCLUDE_FILE("stdlib.h" HAVE_STDLIB_H)

CHECK_INCLUDE_FILE("strings.h" HAVE_STRINGS_H)

CHECK_INCLUDE_FILE("string.h" HAVE_STRING_H)

CHECK_INCLUDE_FILE("sys/dir.h" HAVE_SYS_DIR_H)

CHECK_INCLUDE_FILE("sys/mman.h" HAVE_SYS_MMAN_H)

CHECK_INCLUDE_FILE("sys/ndir.h" HAVE_SYS_NDIR_H)

CHECK_INCLUDE_FILE("sys/select.h" HAVE_SYS_SELECT_H)

CHECK_INCLUDE_FILE("sys/socket.h" HAVE_SYS_SOCKET_H)

CHECK_INCLUDE_FILE("sys/stat.h" HAVE_SYS_STAT_H)

CHECK_INCLUDE_FILE("sys/timeb.h" HAVE_SYS_TIMEB_H)

CHECK_INCLUDE_FILE("sys/time.h" HAVE_SYS_TIME_H)

CHECK_INCLUDE_FILE("sys/types.h" HAVE_SYS_TYPES_H)

CHECK_INCLUDE_FILE("time.h" HAVE_TIME_H)

CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)

CHECK_INCLUDE_FILE("zlib.h" HAVE_ZLIB_H)

configure_file (config.h.cmake ${CMAKE_SOURCE_DIR}/config.h @ONLY)

if(ICONV_FOUND)
   add_definitions(-DLIBXML_ICONV_ENABLED)
endif(ICONV_FOUND)

if(ICONV_SECOND_ARGUMENT_IS_CONST)
   file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define ICONV_CONST const")
else(ICONV_SECOND_ARGUMENT_IS_CONST)
   file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define ICONV_CONST")
endif(ICONV_SECOND_ARGUMENT_IS_CONST)

if(HAVE_PTHREAD_H)
   file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n")
   file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define LIBXML_THREAD_ENABLED")
   file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n")
endif(HAVE_PTHREAD_H)

check_function_exists("dlopen" CMAKE_HAVE_DLOPEN)
check_library_exists("dl" "dlopen" "" CMAKE_LIB_DL)
check_symbol_exists(dlopen "dl.h" CMAKE_SYMBOL_DL)

if (CMAKE_HAVE_DLOPEN OR CMAKE_LIB_DL OR CMAKE_SYMBOL_DL)
    
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define HAVE_DLOPEN")
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n")
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "#define HAVE_DL_H")
    file(APPEND ${CMAKE_SOURCE_DIR}/config.h "\n")

endif (CMAKE_HAVE_DLOPEN OR CMAKE_LIB_DL OR CMAKE_SYMBOL_DL)

