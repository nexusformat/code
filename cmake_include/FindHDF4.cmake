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

set(HDF4_LIBRARIES df mfhdf)

find_library(HDF4_LIBRARIES NAMES df mfhdf PATH_SUFFIXES hdf)

find_library(_HDF4_DF_LIBRARY NAMES df PATH_SUFFIXES hdf)
get_filename_component(HDF4_LIBRARY_DIRS ${_HDF4_DF_LIBRARY} PATH)

find_path ( HDF4_INCLUDE_DIR mfhdf.h PATH /usr/include /usr/include/hdf)

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
list(APPEND NAPI_LINK_LIBS  df mfhdf jpeg)
include_directories ( SYSTEM ${HDF4_INCLUDE_DIR} )
link_directories(${HDF4_LIBRARY_DIRS})
