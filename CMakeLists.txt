## Process this file with cmake
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  CMakeLists for building the NeXus library and applications.
#
#  Copyright (C) 2010 Stephen Rankin
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

# The name of our project is "NeXus". CMakeLists files in this project can
# refer to the root source directory of the project as ${NeXus_SOURCE_DIR} and
# to the root binary directory of the project as ${NeXus_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)

#The name of our project
project (NeXus)

set (NeXus_VERSION_MAJOR 1)
set (NeXus_VERSION_MINOR 0)
set (NXLTVERSINFO '4:3:4')

#Packages to find that are required for the build

find_package(HDF5 REQUIRED)
find_package(JPEG)
find_package(Java)
find_package(JNI)
find_package(XMLRPC)
find_package(ZLIB)
find_package(LibXml2)
find_package(Doxygen)

#CMake includes 

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(cmake_include/FindPython.cmake)
include(cmake_include/FindCBFLib.cmake)
include(cmake_include/FindExtraJava.cmake)
include(cmake_include/FindGuile.cmake)
include(cmake_include/FindHDF4.cmake)
include(cmake_include/FindIDL.cmake)
include(cmake_include/FindMXML.cmake)
include(cmake_include/FindOpenGenie.cmake)
include(cmake_include/FindDocbookUtils.cmake)
include(cmake_include/FindSystemConfig.cmake)

#Find the libraries for the build.

find_library(M m)
#find_library(RPC rpc)
find_library(DL dl)
find_library(PTHREAD pthread)
#find_library(ZIP z)
find_library(SZIP sz)
find_library(TERMCAP termcap)
find_library(READLINE readline)
find_library(HISTORY history)

#Why do we need this?
SET(CPACK_SET_DESTDIR "ON")

if(M)
set(M_LINK "-lm")
endif(M)

if(DL)
set(DL_LINK "-ldl")
endif(DL)

if(PTHREAD)
set(PTHREAD_LINK "-lpthread")
endif(PTHREAD)

if(SZIP)
set(SZIP_LINK "-lsz")
endif(SZIP)

if(TERMCAP)
set(TERMCAP_LINK "")
endif(TERMCAP)

if(READLINE)
set(READLINE_LINK "")
endif(READLINE)

if(HISTORY)
set(HISTORY_LINK "")
endif(HISTORY)

if (HDF5_FOUND)
  set (HDF5_CPP "-DHDF5")
endif (HDF5_FOUND)

#Options

#Build user contributions
option (BUILD_CONTRIB 
        "Should I build the user contributions?" OFF)

#Use HDF4 in build
option (HAVE_HDF4 
        "We have the HDF4 libraries installed and would like to use them." OFF)

#Use HDF5 in build
if (HDF5_FOUND)
option (HAVE_HDF5 
        "We have the HDF5 libraries installed and would like to use them." ON)
endif(HDF5_FOUND)

#Use XML in build
option (HAVE_XML 
        "We have the XML libraries installed and would like to use them." ON)

#This is a MINGW build
option (MINGW_MSYS 
        "We are building under MINGW." OFF)

#This if we have the MS Libraries
option (HAVE_MS_LIB 
        "We are building under MINGW." OFF)

if (HAVE_MS_LIB)
    set (MS_LIB LIB)
endif (HAVE_MS_LIB)

#Include source files.
include_directories(${PROJECT_SOURCE_DIR}/include ${HDF5_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})

check_c_compiler_flag(-Wall C_HAS_ALL)
check_c_compiler_flag(-Wno-unused-variable C_HAS_NO_UNUSED_VARIABLE)
check_c_compiler_flag(-Wno-sign-compare C_HAS_NO_SIGN_COMPARE)
check_c_compiler_flag(-Wno-comment C_HAS_NO_COMMENT)

if (C_HAS_ALL)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif(C_HAS_ALL)

if (C_HAS_NO_UNUSED_VARIABLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable")
endif(C_HAS_NO_UNUSED_VARIABLE)

if (C_HAS_NO_SIGN_COMPARE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-compare")
endif(C_HAS_NO_SIGN_COMPARE)

if (C_HAS_NO_COMMENT)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-comment")
endif(C_HAS_NO_COMMENT)

check_cxx_compiler_flag(-Wall CXX_HAS_ALL)
check_cxx_compiler_flag(-Wno-unused-variable CXX_HAS_NO_UNUSED_VARIABLE)
check_cxx_compiler_flag(-Wno-sign-compare CXX_HAS_NO_SIGN_COMPARE)
check_cxx_compiler_flag(-Wno-comment CXX_HAS_NO_COMMENT)

if (CXX_HAS_ALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif(CXX_HAS_ALL)

if (CXX_HAS_NO_UNUSED_VARIABLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
endif(CXX_HAS_NO_UNUSED_VARIABLE)

if (CXX_HAS_NO_SIGN_COMPARE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
endif(CXX_HAS_NO_SIGN_COMPARE)

if (CXX_HAS_NO_COMMENT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-comment")
endif(CXX_HAS_NO_COMMENT)

#We need to be able to check for Fortran F77/90 - but how do we do this?
enable_language(Fortran)

#We need a NXDOCDIR as a place to install the docs for various packages.
set(NXDOCDIR "share/nexus/doc")

#We need a NXEXAMPLEDIR as a place to install the examples.
set(NXEXAMPLEDIR  "share/nexus/examples")

#We need a NXINCLUDE and NX_INCLUDE as a place to install the include files.
set(NXINCLUDE  "include")
set(NX_INCLUDE  "include/nexus")

file(WRITE ${CMAKE_SOURCE_DIR}/include/nxconfig.h "/*A dummy config*/")

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
