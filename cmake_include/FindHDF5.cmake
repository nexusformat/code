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

set(HDF5_LIBRARIES hdf5)

find_module(HDF5
            LIB_NAMES hdf5
            HEADER_NAMES hdf5.h
            MOD_NAME hdf5
            )

if(WITH_HDF5 AND NOT HAVE_HDF5)
    message(FATAL_ERROR "User requested HDF5 not found!")
elseif(NOT WITH_HDF5 AND HAVE_HDF5) 
    set(WITH_HDF5 ON)
    message(STATUS "Build with HDF5 support!")
    message(STATUS "HDF5 header dir: ${HDF5_INCLUDE_DIRS}")
    message(STATUS "HDF5 library dir: ${HDF5_LIBRARY_DIRS}")
else()
    message(STATUS "Build without HDF5 support!")
endif()
