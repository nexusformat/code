## Process this file with cmake
#=============================================================================
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
#=============================================================================

# looks in HDF5_ROOT environment variable for hint
# set HDF5_FOUND HDF5_DEFINITIONS HDF5_INCLUDE_DIRS  \ 
# HDF5_C_LIBRARIES HDF5_LIBRARIES  HDF5_LIBRARY_DIRS HDF5_ROOT_DIR

if (WIN32)
    #-------------------------------------------------------------------------
    # find HDF5 libraries on Windows
    #-------------------------------------------------------------------------
	set(HDF5_SEARCH_DEFAULT "C:/Program Files/HDF Group/HDF5/1.8.9")
	find_library(HDF5_SHARED_LIBRARIES NAMES hdf5dll 
                 HINTS ${HDF5_SEARCH} 
                 ENV HDF5_ROOT 
                 PATHS ${HDF5_SEARCH_DEFAULT} 
                 PATH_SUFFIXES dll bin lib 
                 DOC "location of hdf5 dll" NO_SYSTEM_ENVIRONMENT_PATH)
	find_library(HDF5_STATIC_LIBRARIES NAMES hdf5 
                 HINTS ${HDF5_SEARCH} 
                 ENV HDF5_ROOT 
                 PATHS ${HDF5_SEARCH_DEFAULT} 
                 PATH_SUFFIXES lib 
                 DOC "location of hdf5 lib" NO_SYSTEM_ENVIRONMENT_PATH)
else(WIN32)
    #-------------------------------------------------------------------------
    # find HDF5 libraries on Linux/Unix
    #
    # We first try to use pkg-config 
    #-------------------------------------------------------------------------
    if(PKG_CONFIG_FOUND)
        pkg_search_module(HDF5 REQUIRED hdf5)
        set(HDF5_SHARED_LIBRARIES ${HDF5_LIBRARIES})
        message(${HDF5_SHARED_LIBRARIES})
        message(${CMAKE_LIBRARY_ARCHITECTURE})
    else()
        set(HDF5_SEARCH_DEFAULT "/usr" "/usr/local" "/usr/local/hdf5" "/sw")
        find_library(HDF5_SHARED_LIBRARIES NAMES hdf5
                     HINTS ${HDF5_SEARCH} 
                     ENV HDF5_ROOT 
                     PATHS ${HDF5_SEARCH_DEFAULT} 
                     PATH_SUFFIXES lib 
                     DOC "location of hdf5 dll")

        find_library(HDF5_STATIC_LIBRARIES NAMES hdf5
                     HINTS ${HDF5_SEARCH} 
                     ENV HDF5_ROOT 
                     PATHS ${HDF5_SEARCH_DEFAULT} 
                     PATH_SUFFIXES lib 
                     DOC "location of hdf5 lib")
    endif()
endif(WIN32)

mark_as_advanced(HDF5_SHARED_LIBRARIES HDF5_STATIC_LIBRARIES)

if (HDF5_SHARED_LIBRARIES)
    set(HDF5_LIBRARIES ${HDF5_SHARED_LIBRARIES} 
        CACHE FILEPATH "Location of HDF5 libraries")
	get_filename_component(_HDF5_LIBDIR ${HDF5_SHARED_LIBRARIES} PATH)
elseif (HDF5_STATIC_LIBRARIES)
    set(HDF5_LIBRARIES ${HDF5_STATIC_LIBRARIES} 
        CACHE FILEPATH "Location of HDF5 libraries")
	get_filename_component(_HDF5_LIBDIR ${HDF5_STATIC_LIBRARIES} PATH)
endif()
set(HDF5_DEFINITIONS "")
get_filename_component(HDF5_ROOT_DIR "${_HDF5_LIBDIR}/.." ABSOLUTE)
find_path(HDF5_INCLUDE_DIRS NAMES hdf5.h HINTS ${HDF5_SEARCH} ENV HDF5_ROOT 
          PATHS ${HDF5_SEARCH_DEFAULT} PATH_SUFFIXES include 
          DOC "location of hdf5 includes" NO_SYSTEM_ENVIRONMENT_PATH)

mark_as_advanced(HDF5_LIBRARIES HDF5_INCLUDE_DIRS)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HDF5 DEFAULT_MSG HDF5_LIBRARIES HDF5_INCLUDE_DIRS)
