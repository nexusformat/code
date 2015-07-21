## Process this file with cmake
#=============================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  CMakeLists for building the NeXus library and applications.
#
# Copyright (C) 2008-2012 NeXus International Advisory Committee (NIAC)
#  
#  This library is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by the
#  Free Software Foundation; either version 2 of the License, or (at your
#  option) any later version.
# 
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
#  for more details.
# 
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation,
#  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#             
#  For further information, see <http://www.nexusformat.org>
#
#
#==============================================================================

cmake_minimum_required(VERSION 2.8.8)

#The name of our project
project (NeXus)

message(${PROJECT_SOURCE_DIR})
#set the module path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_include")
message(${CMAKE_MODULE_PATH})

#load module for GNU/Linux default installation directories
include(GNUInstallDirs)


#-----------------------------------------------------------------------------
# define package version
#-----------------------------------------------------------------------------
set (NeXus_VERSION_MAJOR 1)
set (NeXus_VERSION_MINOR 0)
set (NXLTVERSINFO '4:3:4')

#-----------------------------------------------------------------------------
# define command line options by which the user can configure the build 
#-----------------------------------------------------------------------------
option(WITH_HDF5 "Build with HDF5 support" OFF)
option(WITH_HDF4 "Build with HDF4 support" OFF)
option(WITH_MXML "Build with XML support" OFF)
option(WITHOUT_HDF5 "Build without HDF5 support" OFF)
option(WITHOUT_HDF4 "Build without HDF4 support" OFF)
option(WITHOUT_MXML "Build without MXML support" OFF)
option(ENABLE_CONTRIB "Build the user contributions" OFF)
option(ENABLE_FORTRAN90 "Build the Fortran 90 bindings" OFF)
option(ENABLE_FORTRAN77 "Build the Fortran 77 bindings" OFF)
option(ENABLE_JAVA      "Build Java bindings" OFF)
option(ENABLE_CXX       "Build C++ bindings" OFF)
option(MINGW_MSYS   "We are building under MINGW." OFF)
option(HAVE_MS_LIB  "We are building under MINGW." OFF)

#-----------------------------------------------------------------------------
# some chached variables 
#-----------------------------------------------------------------------------
set(HDF5_INCLUDE_DIRS "" CACHE PATH "Where to search for HDF5 header files")
set(HDF5_LIBRARY_DIRS "" CACHE PATH "Where to search for HDF5 library files")

set(HDF4_INCLUDE_DIRS "" CACHE PATH "Where to search for HDF4 header files")
set(HDF4_LIBRARY_DIRS "" CACHE PATH "Where to search for HDF4 library files")

set(MXML_INCLUDE_DIRS "" CACHE PATH "Where to search for MXML header files")
set(MXML_LIBRARY_DIRS "" CACHE PATH "Where to search for MXML library files")

set(SZIP_INCLUDE_DIRS "" CACHE PATH "Where to search for SZIP header files")
set(SZIP_LIBRARY_DIRS "" CACHE PATH "Where to search for SZIP library files")

set(ZLIB_INCLUDE_DIRS "" CACHE PATH "Where to search for ZLIB header files")
set(ZLIB_INCLUDE_DIRS "" CACHE PATH "Where to search for ZLIB library files")

set(CBF_INCLUDE_DIRS "" CACHE PATH "Where to search for CBF header files")
set(CBF_INCLUDE_DIRS "" CACHE PATH "Where to search for CBF library files")

#-----------------------------------------------------------------------------
# include some general purpose modules 
#-----------------------------------------------------------------------------
include(Utilities)
include(CompilerChecks)


#-----------------------------------------------------------------------------
# check for package config
#-----------------------------------------------------------------------------
include(FindPkgConfig)

#include(cmake_include/FindPython.cmake)
#include(cmake_include/FindCBFLib.cmake)
#include(cmake_include/FindJava.cmake)
#include(cmake_include/FindJNI.cmake)
#include(cmake_include/FindGuile.cmake)
#include(cmake_include/FindMZScheme.cmake)
#include(cmake_include/FindIDL.cmake)

include(FindMXML)
include(FindHDF4)
include(FindHDF5)
#include(cmake_include/FindOpenGenie.cmake)
#include(cmake_include/FindDocbookUtils.cmake)
#include(cmake_include/FindSZIP.cmake)
#include(cmake_include/FindAnt.cmake)


if(UNIX)
    CHECK_LIBRARY_EXISTS(dl dlopen "" HAVE_LIBDL)
    CHECK_LIBRARY_EXISTS(pthread pthread_open "" HAVE_LIBPTHREAD)
endif(UNIX)

#find_package(HDF5 REQUIRED)
#find_package(HDF5)
#find_package(JPEG)
#find_package(XMLRPC)
#find_package(ZLIB)
#find_package(LibXml2)
#find_package(Doxygen)
#find_package(SWIG)
#find_package(TCL)
#find_package(LATEX)
#find_package(PythonInterp)

#Find the java runtime and sdk
#if(ENABLE_JAVA_BINDINGS)
#    find_package(Java 1.6)
#    find_package(JNI)
#endif()

# set ZLIB_ROOT as hint to find_package
#if (ZLIB_SEARCH)
#    set(ZLIB_ROOT ${ZLIB_SEARCH})
#elseif($ENV{ZLIB_ROOT})
#    set(ZLIB_ROOT $ENV{ZLIB_ROOT})
#else()
#    set(ZLIB_ROOT ${HDF5_ROOT_DIR} ${HDF4_ROOT_DIR} ${SZIP_ROOT_DIR})
#endif()
#find_package(ZLIB)

#Find the libraries for the build.

#find_library(M m)
#find_library(RPC rpc)
#find_library(DL dl)
#find_library(PTHREAD pthread)
#find_library(TERMCAP termcap)
#find_library(READLINE readline)
#find_library(HISTORY history)

#if(NOT WIN32)
#CHECK_LIBRARY_EXISTS(${READLINE} readline "" HAVE_LIBREADLINE)
#endif()
#
#if(M)
#   set(M_LINK "-lm")
#endif(M)
#
#if(DL)
#   set(DL_LINK "-ldl")
#endif(DL)
#
#if(PTHREAD)
#   set(PTHREAD_LINK "-lpthread")
#endif(PTHREAD)
#
#if(SZIPLIB_FOUND)
#   set(SZIP_LINK "-lsz") 
#else(SZIPLIB_FOUND)
#   set(SZIP_LIB "")
#endif(SZIPLIB_FOUND)
#
#if(TERMCAP)
#   set(TERMCAP_LINK "")
#endif(TERMCAP)
#
#if(READLINE)
#   set(READLINE_LINK "${READLINE}")
#endif(READLINE)
#
#if(HISTORY)
#   set(HISTORY_LINK "")
#endif(HISTORY)

#
##Include source files.
#include_directories(${PROJECT_SOURCE_DIR}/include ${HDF5_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})



#if (BUILD_FORTRAN_BINDINGS)
#    enable_language(Fortran)
#endif (BUILD_FORTRAN_BINDINGS)
#
#We need a NXDOCDIR as a place to install the docs for various packages.
set(NXDOCDIR "share/nexus/doc")

#We need a NXEXAMPLEDIR as a place to install the examples.
set(NXEXAMPLEDIR  "share/nexus/examples")

#We need a NXINCLUDE and NX_INCLUDE as a place to install the include files.
set(NXINCLUDE  "include")
set(NX_INCLUDE "include/nexus")

configure_file("${PROJECT_SOURCE_DIR}/include/nxconfig_h_cmake.in" "${PROJECT_BINARY_DIR}/include/nxconfig.h" @ONLY)
include_directories("${PROJECT_BINARY_DIR}/include")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${NX_CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NX_CFLAGS}")

#if (CMAKE_SIZEOF_VOID_P MATCHES "8")
#    set(ARCH64 1)
#	set(ARCHSUFFIX "64")
#else()
#    set(ARCH64 0)
#	set(ARCHSUFFIX "")
#endif()


# Recurse into the subdirectories.
add_subdirectory (include)
add_subdirectory (src)
#add_subdirectory (bindings)
#add_subdirectory (applications)
#add_subdirectory (config)
#add_subdirectory (contrib)
#add_subdirectory (definitions)
#add_subdirectory (doc)
#add_subdirectory (examples)
#add_subdirectory (InstallerBits)
#add_subdirectory (macosx_install_kit)
#add_subdirectory (scripts)
add_subdirectory (test)
#add_subdirectory (third_party)
#add_subdirectory (vms)
#add_subdirectory (windows)
#add_subdirectory (Windows_extra)
