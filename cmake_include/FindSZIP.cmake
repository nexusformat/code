## Process this file with cmake
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
#  For further information, see <http://www.nexusformat.org>
#
#
#====================================================================

find_library(SZIP_LIB NAMES sz szip PATHS $ENV{HDF5_ROOT}/bin $ENV{HDF5_ROOT}/lib $ENV{HDF5_ROOT}/dll)

find_path(SZIP_INCLUDE NAMES sz.h szlib.h PATHS $ENV{HDF5_ROOT}/include)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_library(ZIP_LIB NAMES z zlib zdll zlib1 zlibd zlibd1 PATHS $ENV{HDF5_ROOT}/bin $ENV{HDF5_ROOT}/lib)
    find_path(ZIP_INCLUDE zlib.h PATHS $ENV{HDF5_ROOT}/include)
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

if(SZIP_INCLUDE)
    include_directories(${SZIP_INCLUDE})
endif(SZIP_INCLUDE)

if(ZIP_INCLUDE)
    include_directories(${ZIP_INCLUDE})
endif(ZIP_INCLUDE)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SZIPLIB DEFAULT_MSG SZIP_LIB ZIP_LIB)

MARK_AS_ADVANCED(SZIP_LIB)
