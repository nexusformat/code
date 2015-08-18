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

cmake_minimum_required(VERSION 2.8.12)

#The name of our project
project (NeXus)
enable_testing()

message(STATUS ${PROJECT_SOURCE_DIR})
#set the module path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_include")
message(STATUS ${CMAKE_MODULE_PATH})

#------------------------------------------------------------------------------
# set the default installation prefix
#------------------------------------------------------------------------------
if(WIN32)
    #don't know yet what to put here
else()
    if(NOT CMAKE_INSTALL_PREFIX)
        set(CMAKE_INSTALL_PREFIX "/usr/local")
    endif()
    #load module for GNU/Linux default installation directories
    include(GNUInstallDirs)
endif()


#-----------------------------------------------------------------------------
# define package version
#-----------------------------------------------------------------------------
set (NeXus_VERSION_MAJOR 1)
set (NeXus_VERSION_MINOR 0)
set (NXLTVERSINFO '4:3:4')

#-----------------------------------------------------------------------------
# define command line options by which the user can configure the build
#-----------------------------------------------------------------------------
option(ENABLE_HDF5 "Build with HDF5 support" ON)
option(ENABLE_HDF4 "Build with HDF4 support" OFF)
option(ENABLE_MXML "Build with XML support" OFF)
option(ENABLE_CONTRIB "Build the user contributions" OFF)
option(ENABLE_FORTRAN90 "Build the Fortran 90 bindings" OFF)
option(ENABLE_FORTRAN77 "Build the Fortran 77 bindings" OFF)
option(ENABLE_JAVA      "Build Java bindings" OFF)
option(ENABLE_CXX       "Build C++ bindings" OFF)
option(MINGW_MSYS   "We are building under MINGW." OFF)
option(HAVE_MS_LIB  "We are building under MINGW." OFF)

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

#------------------------------------------------------------------------------
# a list of libraries we have to link NAPI against
# we need this in the global scope as all other programs have to be
# linked against this libraries as well - in this early stage there is
# no pkg-config we could use!
#------------------------------------------------------------------------------
set(NAPI_LINK_LIBS)

if(ENABLE_MXML)
    include(FindMXML)
    # the required libraries are appended to
    # NAPI_LINK_LIBS by this module
    set(WITH_MXML TRUE)
else()
    #in all cases we build the libary without MXML
    message(STATUS "Build without MXML support!")
endif()

if(ENABLE_HDF4)
    include(FindHDF4)

    message(STATUS "HDF4 library found in: ${HDF4_LIBRARY_DIR}")
    message(STATUS "HDF4 headers found in: ${HDF4_INCLUDE_DIR}")

    # the required libraries are appended to
    # NAPI_LINK_LIBS by this module
    set(WITH_HDF4 TRUE)
else()
    message(STATUS "Build without HDF4 support!")
endif()

if(ENABLE_HDF5)

    message ("${HDF5_COMPONENTS}")
    if (ENABLE_CXX)
        find_package ( HDF5 COMPONENTS CXX HL REQUIRED )
    else()
        find_package ( HDF5 REQUIRED )
    endif()
    set(HAVE_HDF5 1)
    list(APPEND NAPI_LINK_LIBS ${HDF5_LIBRARIES})
    set(WITH_HDF5 TRUE)

    message(STATUS "HDF5 library found in: ${HDF5_LIBRARY_DIRS}")
    message(STATUS "HDF5 headers found in: ${HDF5_INCLUDE_DIRS}")
else()
    message(STATUS "Build without HDF5 support!")
endif()

if(ENABLE_FORTRAN90 OR ENABLE_FORTRAN77)
    enable_language(Fortran)
endif()

#include(cmake_include/FindOpenGenie.cmake)
#include(cmake_include/FindDocbookUtils.cmake)
#include(cmake_include/FindSZIP.cmake)
#include(cmake_include/FindAnt.cmake)

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

find_library(M m)
if(M)
   set(M_LINK "-lm")
endif(M)

find_library(READLINE readline)
if(READLINE)
   set(HAVE_LIBREADLINE 1)
   set(READLINE_LINK "readline")
endif(READLINE)

find_library(DL dl)
if(DL)
   set(DL_LINK "-ldl")
endif(DL)

find_library(PTHREAD pthread)
if(PTHREAD)
   set(PTHREAD_LINK "-lpthread")
endif(PTHREAD)

find_library(TERMCAP termcap)
if(TERMCAP)
   set(TERMCAP_LINK "")
endif(TERMCAP)

find_library(HISTORY history)
if(HISTORY)
   set(HISTORY_LINK "")
endif(HISTORY)

#find_library(RPC rpc)
#if(NOT WIN32)
#CHECK_LIBRARY_EXISTS(${READLINE} readline "" HAVE_LIBREADLINE)
#endif()
#
#
#
#
#if(SZIPLIB_FOUND)
#   set(SZIP_LINK "-lsz")
#else(SZIPLIB_FOUND)
#   set(SZIP_LIB "")
#endif(SZIPLIB_FOUND)
#
#
#

include_directories("./include")
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
add_subdirectory (test)
add_subdirectory (bindings)
add_subdirectory (applications)
#add_subdirectory (config)
#add_subdirectory (contrib)
#add_subdirectory (definitions)
#add_subdirectory (doc)
#add_subdirectory (examples)
#add_subdirectory (InstallerBits)
#add_subdirectory (macosx_install_kit)
#add_subdirectory (scripts)
#add_subdirectory (third_party)
#add_subdirectory (vms)
#add_subdirectory (windows)
#add_subdirectory (Windows_extra)
