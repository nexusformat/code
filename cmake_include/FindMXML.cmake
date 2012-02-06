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


# looks in MXML_ROOT environment variable for hint
# set MXML_FOUND MXML_DEFINITIONS MXML_INCLUDE_DIRS  MXML_LIBRARIES  MXML_ROOT_DIR

if (WIN32)
    set(MXML_SEARCH_DEFAULT "C:/InstallKits/HDF5-1.8.6-win64")
	find_library(MXML_SHARED_LIBRARIES NAMES mxml mxml1 HINTS ${MXML_SEARCH} ENV MXML_ROOT PATHS ${MXML_SEARCH_DEFAULT} PATH_SUFFIXES dll lib DOC "location of mxml dll")
	find_library(MXML_STATIC_LIBRARIES NAMES mxml mxml1 HINTS ${MXML_SEARCH} ENV MXML_ROOT PATHS ${MXML_SEARCH_DEFAULT} PATH_SUFFIXES lib DOC "location of mxml lib")
else(WIN32)
    set(MXML_SEARCH_DEFAULT "/usr" "/usr/local" "/sw")
	find_library(MXML_SHARED_LIBRARIES NAMES mxml HINTS ${MXML_SEARCH} ENV MXML_ROOT PATHS ${MXML_SEARCH_DEFAULT} PATH_SUFFIXES lib DOC "location of mxml dll")
	find_library(MXML_STATIC_LIBRARIES NAMES mxml HINTS ${MXML_SEARCH} ENV MXML_ROOT PATHS ${MXML_SEARCH_DEFAULT} PATH_SUFFIXES lib DOC "location of mxml lib")
endif(WIN32)

mark_as_advanced(MXML_SHARED_LIBRARIES MXML_STATIC_LIBRARIES)

if (MXML_SHARED_LIBRARIES)
    set(MXML_LIBRARIES ${MXML_SHARED_LIBRARIES} CACHE FILEPATH "Location of MXML libraries")
	get_filename_component(_MXML_LIBDIR ${MXML_SHARED_LIBRARIES} PATH)
elseif (MXML_STATIC_LIBRARIES)
    set(MXML_LIBRARIES ${MXML_STATIC_LIBRARIES}  CACHE FILEPATH "Location of MXML libraries")
	get_filename_component(_MXML_LIBDIR ${MXML_STATIC_LIBRARIES} PATH)
endif()
set(MXML_DEFINITIONS "")
get_filename_component(MXML_ROOT_DIR "${_MXML_LIBDIR}/.." ABSOLUTE)

find_path(MXML_INCLUDE_DIRS NAMES mxml.h HINTS ${MXML_SEARCH} ENV MXML_ROOT PATHS ${MXML_SEARCH_DEFAULT} PATH_SUFFIXES include DOC "location of mxml includes")

mark_as_advanced(MXML_LIBRARIES MXML_INCLUDE_DIRS)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MXML DEFAULT_MSG MXML_LIBRARIES MXML_INCLUDE_DIRS)
