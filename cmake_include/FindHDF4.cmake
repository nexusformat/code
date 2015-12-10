## Process this file with cmake
#=============================================================================
#  NeXus - Neutron & X-ray Common Data Format
#
#  CMakeLists for building the NeXus library and applications.
#
#  Copyright (C) 2011 Stephen Rankin
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
#=============================================================================

<<<<<<< HEAD
find_library(HDF4_LIBRARIES NAMES df mfhdf PATH_SUFFIXES hdf)
find_library(HDF4_DF_LIBRARY NAMES df PATH_SUFFIXES hdf)
find_library(HDF4_MFHDF_LIBRARY NAMES mfhdf PATH_SUFFIXES hdf)
message(STATUS "DF library found: ${HDF4_DF_LIBRARY}")
message(STATUS "MFHDF library found: ${HDF4_MFHDF_LIBRARY}")

message(STATUS "HDF4 libraries: ${HDF4_LIBRARIES}")
=======
set(HDF4_LIBRARIES df mfhdf)

#------------------------------------------------------------------------------
# find the runtime binaries of the HDF4 library
#------------------------------------------------------------------------------
find_library(HDF4_LIBRARIES NAMES df mfhdf 
             HINTS ENV HDF4_ROOT 
             PATH_SUFFIXES hdf)

find_library(_HDF4_DF_LIBRARY NAMES df hdf 
             HINTS ENV 
             HDF4_ROOT PATH_SUFFIXES hdf)
>>>>>>> 336bba8dd4b92aa97c0684b1285f94d575b23f8a

#if the binaries have been found their parent directory has to be 
#extracted from the total path
get_filename_component(HDF4_LIBRARY_DIRS ${_HDF4_DF_LIBRARY} PATH)

#------------------------------------------------------------------------------
# find the HDF4 header file
#------------------------------------------------------------------------------
find_path (HDF4_INCLUDE_DIR mfhdf.h HINTS ENV HDF4_ROOT PATH_SUFFIXES hdf)

include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args( HDF4 DEFAULT_MSG HDF4_LIBRARIES HDF4_INCLUDE_DIR )

if(NOT HDF4_FOUND)
    #the user has explicitely requested HDF4 support but the required libraries
    #could not be found
    message(FATAL_ERROR "User requested HDF4 not found!")
endif()

#------------------------------------------------------------------------------
# search for additional packages required to link against HDF4
#------------------------------------------------------------------------------
find_package(JPEG REQUIRED)

#------------------------------------------------------------------------------
# add libraries to the link list for NAPI
#------------------------------------------------------------------------------
list(APPEND NAPI_LINK_LIBS  ${_HDF4_DF_LIBRARY} mfhdf jpeg)
include_directories ( SYSTEM ${HDF4_INCLUDE_DIR} )
link_directories(${HDF4_LIBRARY_DIRS})
