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

#-----------------------------------------------------------------------------
# The HDF5 search will set the following variables 
#
# HDF5_FOUND        - set to 1 if the libary exists
# HDF5_LIBRARIES    - the libraries one has to link
# HDF5_LIBRARY_DIRS - the directory where the library binaries are installed
# HDF5_INCLUDE_DIRS - the directory where the header files are installed
#
#-----------------------------------------------------------------------------


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
    # We first try to use pkg-config  (at least on Debian systems this should 
    # work and help avoiding problems with mulit-arch installations.
    #-------------------------------------------------------------------------
    if(PKG_CONFIG_FOUND)
        pkg_search_module(HDF5 REQUIRED hdf5)
        message(${HDF5_SHARED_LIBRARIES})
        message(${CMAKE_LIBRARY_ARCHITECTURE})
    else()
        #do a manual search 
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

        #construct the required variables from the seach results
        if(HDF5_SHARED_LIBRARIES)
            get_filename_component(HDF5_LIBRARY_DIRS ${HDF5_SHARED_LIBRARIES} PATH)
            get_filename_component(HDF5_LIBRARIES ${HDF5_SHARED_LIBRARIES} NAME_WE)
        elseif(HDF5_STATIC_LIBRARIES)
            get_filename_component(HDF5_STATIC_LIBRARY_DIRS
                                   ${HDF5_STATIC_LIBRARIES} PATH)
            get_filename_component(HDF5_STATIC_LIBRARY_DIRS
                                   ${HDF5_STATIC_LIBRARIES NAME_WE)
        endif()

    endif()
endif(WIN32)

