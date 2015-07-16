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
#==============================================================================

message(${CMAKE_LIBRARY_ARCHITECTURE})

set(HAVE_MXML TRUE)
find_module(MXML 
            LIB_NAMES mxml
            HEADER_NAMES mxml.h
            MOD_NAME mxml)

if(WITH_MXML AND NOT HAVE_MXML)
    message(FATAL_ERROR "User requested MXML not found!")
else(NOT WITH_MXML AND HAVE_MXML) 
    set(WITH_MXML ON)
    message(STATUS "Build with MXML support!")
    message(STATUS "MXML header dir: ${MXML_INCLUDE_DIRS}")
    message(STATUS "MXML library dir: ${MXML_LIBRARY_DIRS}")
endif()

