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

set(HDF4_LIBRARIES df mfhdf)

find_module(HDF4
            LIB_NAMES ${HDF4_LIBRARIES}
            HEADER_NAMES mfhdf.h
            )

if(WITH_HDF4 AND NOT HAVE_HDF4)
    message(FATAL_ERROR "User requested HDF4 not found!")
elseif(NOT WITH_HDF4 AND HAVE_HDF4) 
    set(WITH_HDF4 ON)
    message(STATUS "Build with HDF4 support!")
    message(STATUS "HDF4 header dir: ${HDF4_INCLUDE_DIRS}")
    message(STATUS "HDF4 library dir: ${HDF4_LIBRARY_DIRS}")
else()
    message(STATUS "Build without HDF4 support!")
endif()

