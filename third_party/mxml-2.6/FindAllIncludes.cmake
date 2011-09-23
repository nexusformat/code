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
INCLUDE(CheckTypeSize)
find_library(PTHREAD pthread)

if(PTHREAD)
   set(PTHREAD_LINK "-lpthread")
endif(PTHREAD)

CHECK_FUNCTION_EXISTS (snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS (vsnprintf HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS (strdup HAVE_STRDUP)
CHECK_TYPE_SIZE("long long" HAVE_LONG_LONG)

CHECK_INCLUDE_FILE("pthread.h" HAVE_PTHREAD_H)
CHECK_INCLUDE_FILE("ctype.h" HAVE_CTYPE_H)
CHECK_INCLUDE_FILE("stdarg.h" HAVE_STDARG_H)
CHECK_INCLUDE_FILE("stdlib.h" HAVE_STDLIB_H)
CHECK_INCLUDE_FILE("stdio.h" HAVE_STDIO_H)
CHECK_INCLUDE_FILE("string.h" HAVE_STRING_H)

configure_file (config.h.cmake ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h @ONLY)

file(READ ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h MXML_CONFIG)

if(HAVE_STDIO_H)
   file(WRITE ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "\n")
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "#include <stdio.h>")
endif(HAVE_STDIO_H)

if(HAVE_STDLIB_H)
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "\n")
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "#include <stdlib.h>")
endif(HAVE_STDLIB_H)

if(HAVE_STRING_H)
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "\n")
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "#include <string.h>")

endif(HAVE_STRING_H)

if(HAVE_STDARG_H)
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "\n")
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "#include <stdarg.h>")
endif(HAVE_STDARG_H)

if(HAVE_CTYPE_H)
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "\n")
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "#include <ctype.h>")
   file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp "\n")
endif(HAVE_CTYPE_H)

file(APPEND ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp ${MXML_CONFIG})

file(RENAME ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h.tmp ${CMAKE_SOURCE_DIR}/third_party/mxml-2.6/config.h)

