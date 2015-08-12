## Process this file with cmake
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  CMakeLists for building the NeXus library and applications.
#
# Copyright (C) 2008-2012 NeXus International Advisory Committee (NIAC)
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
#  For further information, see <http://www.nexusformat.org>
#
#
#====================================================================

# we need 2.8.8 for correct component install with CPACK for archives
if(WIN32)
    cmake_minimum_required (VERSION 2.8.8)
else()
    cmake_minimum_required (VERSION 2.8.8)
    if (${CMAKE_VERSION} VERSION_LESS 2.8.4)
        set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove this line when CMake >= 2.8.4 is required
    endif()
endif()

#
# convert a native windows path to its equivalent cygwin path
#
function(to_cygwin_path PATH RESULT)
    file(TO_CMAKE_PATH ${PATH} CP)								# changes \ to / on windows
    string(REGEX REPLACE "^([a-zA-Z]):/" "/\\1/" CP2 ${CP})   	# c:/ -> /c/
	set(${RESULT} "/cygdrive${CP2}" PARENT_SCOPE) 				# /c/ -> /cygdrive/c/
endfunction()

# set a default install prefix for windows - we will adjust for 64bit later if this is unchanged
set(DEFAULT_WIN_INSTALL_PREFIX "c:/nxinstall")
if(WIN32)
    set(CMAKE_INSTALL_PREFIX ${DEFAULT_WIN_INSTALL_PREFIX} CACHE PATH "")
endif()

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")


#The name of our project
project (NeXus)

set (NeXus_VERSION_MAJOR 1)
set (NeXus_VERSION_MINOR 0)
set (NXLTVERSINFO '4:3:4')

# needed for windows NSIS installer
file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR} PROJECT_SOURCE_DIR_NATIVE) 
file(TO_NATIVE_PATH ${PROJECT_BINARY_DIR} PROJECT_BINARY_DIR_NATIVE) 
string(REPLACE "\\" "\\\\" PROJECT_SOURCE_DIR_NATIVE_D ${PROJECT_SOURCE_DIR_NATIVE})
string(REPLACE "\\" "\\\\" PROJECT_BINARY_DIR_NATIVE_D ${PROJECT_BINARY_DIR_NATIVE})

find_program(SVNVERSION svnversion)

#Packages to find that are required for the build

set(HDF5_SEARCH "" CACHE PATH "Where to search for HDF5")

set(HDF4_SEARCH "" CACHE PATH "Where to search for HDF4")

set(MXML_SEARCH "" CACHE PATH "Where to search for MXML")

set(SZIP_SEARCH "" CACHE PATH "Where to search for SZIP")

set(ZLIB_SEARCH "" CACHE PATH "Where to search for ZLIB")

#CMake includes 

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckFunctionExists)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)

if(WIN32)
include(InstallRequiredSystemLibraries)
endif()

include(cmake_include/Utilities.cmake)
#include(cmake_include/FindPython.cmake)
include(cmake_include/FindCBFLib.cmake)
#include(cmake_include/FindJava.cmake)
#include(cmake_include/FindJNI.cmake)
include(cmake_include/FindGuile.cmake)
include(cmake_include/FindMZScheme.cmake)
include(cmake_include/FindHDF4.cmake)
include(cmake_include/FindHDF5.cmake)
include(cmake_include/FindIDL.cmake)
include(cmake_include/FindMXML.cmake)
include(cmake_include/FindOpenGenie.cmake)
include(cmake_include/FindDocbookUtils.cmake)
include(cmake_include/FindSZIP.cmake)
include(cmake_include/FindAnt.cmake)

CHECK_TYPE_SIZE("int" SIZEOF_INT)
set(HAVE_INT ${HAVE_SIZEOF_INT})
CHECK_TYPE_SIZE("long int" SIZEOF_LONG_INT)
set(HAVE_LONG_INT ${HAVE_SIZEOF_LONG_INT})
CHECK_TYPE_SIZE("long long int" SIZEOF_LONG_LONG_INT)
set(HAVE_LONG_LONG_INT ${HAVE_SIZEOF_LONG_LONG_INT})
CHECK_TYPE_SIZE("unsigned long long int" SIZEOF_UNSIGNED_LONG_LONG_INT)
set(HAVE_UNSIGNED_LONG_LONG_INT ${HAVE_SIZEOF_UNSIGNED_LONG_LONG_INT})
CHECK_TYPE_SIZE("size_t" SIZEOF_SIZE_T)
set(HAVE_SIZE_T ${HAVE_SIZEOF_SIZE_T})

CHECK_FUNCTION_EXISTS(ftime HAVE_FTIME)
CHECK_FUNCTION_EXISTS(tzset HAVE_TZSET)
CHECK_FUNCTION_EXISTS(strdup HAVE_STRDUP)

CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H)

if (SIZEOF_LONG_LONG_INT EQUAL 8)
	set(PRINTF_INT64 "lld")
	set(PRINTF_UINT64 "llu")
elseif (WIN32)
	set(PRINTF_INT64 "I64d")
	set(PRINTF_UINT64 "I64u")
else()
	message(SEND_ERROR "Cannot printf int64 and uint64")
endif()

if(UNIX)
    CHECK_LIBRARY_EXISTS(dl dlopen "" HAVE_LIBDL)
    CHECK_LIBRARY_EXISTS(pthread pthread_open "" HAVE_LIBPTHREAD)
endif(UNIX)

#find_package(HDF5 REQUIRED)
#find_package(HDF5)
#find_package(JPEG)
find_package(XMLRPC)
#find_package(ZLIB)
find_package(LibXml2)
find_package(Doxygen)
find_package(SWIG)
find_package(TCL)
find_package(LATEX)
find_package(PythonInterp)

#Find the java runtime and sdk
if(ENABLE_JAVA_BINDINGS)
    find_package(Java 1.6)
    find_package(JNI)
endif()

# set ZLIB_ROOT as hint to find_package
if (ZLIB_SEARCH)
    set(ZLIB_ROOT ${ZLIB_SEARCH})
elseif($ENV{ZLIB_ROOT})
    set(ZLIB_ROOT $ENV{ZLIB_ROOT})
else()
    set(ZLIB_ROOT ${HDF5_ROOT_DIR} ${HDF4_ROOT_DIR} ${SZIP_ROOT_DIR})
endif()
find_package(ZLIB)

#Find the libraries for the build.

find_library(M m)
#find_library(RPC rpc)
find_library(DL dl)
find_library(PTHREAD pthread)
find_library(TERMCAP termcap)
find_library(READLINE readline)
find_library(HISTORY history)

if(NOT WIN32)
CHECK_LIBRARY_EXISTS(${READLINE} readline "" HAVE_LIBREADLINE)
endif()

if(M)
   set(M_LINK "-lm")
endif(M)

if(DL)
   set(DL_LINK "-ldl")
endif(DL)

if(PTHREAD)
   set(PTHREAD_LINK "-lpthread")
endif(PTHREAD)

if(SZIPLIB_FOUND)
   set(SZIP_LINK "-lsz") 
else(SZIPLIB_FOUND)
   set(SZIP_LIB "")
endif(SZIPLIB_FOUND)

if(TERMCAP)
   set(TERMCAP_LINK "")
endif(TERMCAP)

if(READLINE)
   set(READLINE_LINK "${READLINE}")
endif(READLINE)

if(HISTORY)
   set(HISTORY_LINK "")
endif(HISTORY)

if (HDF5_FOUND)
#   set (HDF5_CPP "-DHDF5 -DH5_USE_16_API")
#   these are needed if HDF5 1.8 has been instaleld with H5_USE_16_API_DEFAULT 
  set(HDF5_API_DEFS "-DH5Acreate_vers=2 -DH5Aiterate_vers=2 -DH5Dcreate_vers=2 -DH5Dopen_vers=2 -DH5Eclear_vers=2 -DH5Eprint_vers=2 -DH5Epush_vers=2 -DH5Eset_auto_vers=2 -DH5Eget_auto_vers=2 -DH5Ewalk_vers=2 -DH5Gcreate_vers=2 -DH5Gopen_vers=2 -DH5Pget_filter_vers=2 -DH5Pget_filter_by_id_vers=2 -DH5Pinsert_vers=2 -DH5Pregister_vers=2 -DH5Rget_obj_type_vers=2 -DH5Tarray_create_vers=2 -DH5Tcommit_vers=2 -DH5Tget_array_dims_vers=2 -DH5Topen_vers=2")
  set (HDF5_CPP "-DHDF5 -DH5_NO_DEPRECATED_SYMBOLS ${HDF5_API_DEFS}")
endif (HDF5_FOUND)

#Options

#Build user contributions
option (BUILD_CONTRIB 
        "Should I build the user contributions?" OFF)

#Build user contributions
option (BUILD_FORTRAN_BINDINGS 
        "Should I build the fortran bindings?" OFF)

#Use HDF4 in build
option (BUILD_HDF4 
        "Should I build HDF4 bindings if libraries available?" ON)

option (BUILD_HDF5 
        "Should I build HDF4 bindings if libraries available?" ON)

#Use XML in build
option (BUILD_XML 
        "Should I build XML bindings if libraries available?" ON)
set(BUILD_MXML ${BUILD_XML})

#This is a MINGW build
option (MINGW_MSYS 
        "We are building under MINGW." OFF)

#This if we have the MS Libraries
option (HAVE_MS_LIB 
        "We are building under MINGW." OFF)

option(ENABLE_JAVA_BINDINGS "Build Java bindings" ON)

if (HAVE_MS_LIB)
    set (MS_LIB LIB)
endif (HAVE_MS_LIB)

create_have_vars(HDF4 HDF5 MXML)

#Include source files.
include_directories(${PROJECT_SOURCE_DIR}/include ${HDF5_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})


if (UNIX)
    check_add_c_compiler_flags("-Wall -Wno-unused-variable -Wno-sign-compare -Wno-comment")
    check_add_cxx_compiler_flags("-Wall -Wno-unused-variable -Wno-sign-compare -Wno-comment")
endif(UNIX)

if (BUILD_FORTRAN_BINDINGS)
    enable_language(Fortran)
endif (BUILD_FORTRAN_BINDINGS)

#We need a NXDOCDIR as a place to install the docs for various packages.
set(NXDOCDIR "share/nexus/doc")

#We need a NXEXAMPLEDIR as a place to install the examples.
set(NXEXAMPLEDIR  "share/nexus/examples")

#We need a NXINCLUDE and NX_INCLUDE as a place to install the include files.
set(NXINCLUDE  "include")
set(NX_INCLUDE "include/nexus")

configure_file("${PROJECT_SOURCE_DIR}/include/nxconfig_h_cmake.in" "${PROJECT_BINARY_DIR}/include/nxconfig.h" @ONLY)
include_directories("${PROJECT_BINARY_DIR}/include")

if(MSVC)
# warning 4820 is byte padding in structures
# warning 4996 is about using _strdup ratherthan strdup etc
# /W4 rather than /Wall
# /Zi to get pdb file in all configurations
    set(NX_CFLAGS "/W4 /wd4820 /wd4996 /Zi")
    set(NX_CPP -D_CRT_SECURE_NO_WARNINGS)
	set(CMAKE_DEBUG_POSTFIX "D")
#	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL" CACHE STRING "" FORCE)   # workaround for problems
endif(MSVC)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${NX_CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NX_CFLAGS}")

if (CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(ARCH64 1)
	set(ARCHSUFFIX "64")
else()
    set(ARCH64 0)
	set(ARCHSUFFIX "")
endif()

# if we haven't changed the default windows install prefix, adjust for 64 bit
if(${CMAKE_INSTALL_PREFIX} STREQUAL ${DEFAULT_WIN_INSTALL_PREFIX})
    set(CMAKE_INSTALL_PREFIX "${DEFAULT_WIN_INSTALL_PREFIX}${ARCHSUFFIX}" CACHE PATH "" FORCE)
endif()

if(WIN32)
	if(ARCH64)
		include_directories("${NEXUS_THIRD_PARTY}/win64/include")
		file(GLOB EXTRA_DLLS "${NEXUS_THIRD_PARTY}/win64/bin/*.dll")
	else()
		include_directories("${NEXUS_THIRD_PARTY}/win32/include")
		file(GLOB EXTRA_DLLS "${NEXUS_THIRD_PARTY}/win32/bin/*.dll")
	endif()
	install(FILES ${EXTRA_DLLS} DESTINATION bin COMPONENT Runtime)
endif()

# Recurse into the subdirectories.
add_subdirectory (src)
add_subdirectory (bindings)
add_subdirectory (applications)
#add_subdirectory (config)
#add_subdirectory (contrib)
#add_subdirectory (definitions)
add_subdirectory (doc)
add_subdirectory (examples)
add_subdirectory (include)
#add_subdirectory (InstallerBits)
#add_subdirectory (macosx_install_kit)
#add_subdirectory (scripts)
add_subdirectory (test)
#add_subdirectory (third_party)
#add_subdirectory (vms)
#add_subdirectory (windows)
#add_subdirectory (Windows_extra)

#
# set CPack packaging options
#
configure_file("${PROJECT_SOURCE_DIR}/CPackOptions.cmake.in" "${PROJECT_BINARY_DIR}/CPackOptions.cmake"  @ONLY)
set(CPACK_PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/CPackOptions.cmake")
set (CPACK_GENERATOR TGZ) # not use ZIP on UNIX as problem with symlinks
set (CPACK_SOURCE_GENERATOR TGZ) # not use ZIP on UNIX as problem with symlinks
if(WIN32)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};ZIP;NSIS)
    set (CPACK_SOURCE_GENERATOR ${CPACK_SOURCE_GENERATOR};ZIP) 
elseif(APPLE)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};PackageMaker)
elseif(CYGWIN)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};CygwinBinary)
    set (CPACK_SOURCE_GENERATOR ${CPACK_SOURCE_GENERATOR};CygwinSource) 
elseif(UNIX)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};DEB;RPM)
endif()
# Include of CPack must always be last
include(CPack)
