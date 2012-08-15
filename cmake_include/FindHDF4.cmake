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


# looks in HDF4_ROOT environment variable for hint
# set HDF4_FOUND HDF4_DEFINITIONS HDF4_INCLUDE_DIRS  HDF4_LIBRARIES  HDF4_ROOT_DIR

if (WIN32)
#    set(HDF4_SEARCH_DEFAULT "C:/InstallKits/HDF4.2.6_win_x64")
	set(HDF4_SEARCH_DEFAULT "C:/Program Files/HDF Group/HDF4/4.2.7")
	set(HD_NAMES hdfdll  hd427m hd426m hd425m )
	set(HM_NAMES mfhdfdll hm427m hm426m hm425m )
	find_library(HDF4_HD_LIBRARY NAMES ${HD_NAMES} HINTS ${HDF4_SEARCH} ENV HDF4_ROOT PATHS ${HDF4_SEARCH_DEFAULT} PATH_SUFFIXES dll bin lib DOC "location of hd dll" NO_SYSTEM_ENVIRONMENT_PATH)
	find_library(HDF4_HM_LIBRARY NAMES ${HM_NAMES} HINTS ${HDF4_SEARCH} ENV HDF4_ROOT PATHS ${HDF4_SEARCH_DEFAULT} PATH_SUFFIXES dll bin lib DOC "location of hm dll" NO_SYSTEM_ENVIRONMENT_PATH)
else(WIN32)
    set(HDF4_SEARCH_DEFAULT "/usr" "/usr/local" "/usr/local/hdf4" "/sw")
	set(HD_NAMES df)
	set(HM_NAMES mfhdf)
	find_library(HDF4_HD_LIBRARY NAMES ${HD_NAMES} HINTS ${HDF4_SEARCH} ENV HDF4_ROOT PATHS ${HDF4_SEARCH_DEFAULT} PATH_SUFFIXES lib DOC "location of hd dll")
	find_library(HDF4_HM_LIBRARY NAMES ${HM_NAMES} HINTS ${HDF4_SEARCH} ENV HDF4_ROOT PATHS ${HDF4_SEARCH_DEFAULT} PATH_SUFFIXES lib DOC "location of hm dll")
endif(WIN32)

mark_as_advanced(HDF4_HD_LIBRARY HDF4_HM_LIBRARY)

if (HDF4_HM_LIBRARY AND HDF4_HD_LIBRARY)
    set(HDF4_SHARED_LIBRARIES ${HDF4_HM_LIBRARY} ${HDF4_HD_LIBRARY})
    set(HDF4_STATIC_LIBRARIES ${HDF4_HM_LIBRARY} ${HDF4_HD_LIBRARY})
    set(HDF4_LIBRARIES ${HDF4_SHARED_LIBRARIES} CACHE FILEPATH "Location of HDF4 libraries")
    get_filename_component(_HDF4_LIBDIR ${HDF4_HM_LIBRARY} PATH)
    get_filename_component(HDF4_ROOT_DIR "${_HDF4_LIBDIR}/.." ABSOLUTE)
	find_path(HDF4_INCLUDE_DIRS NAMES mfhdf.h HINTS ${HDF4_SEARCH} ENV HDF4_ROOT PATHS ${HDF4_SEARCH_DEFAULT} PATH_SUFFIXES include DOC "location of hdf4 includes" NO_SYSTEM_ENVIRONMENT_PATH)
endif()

mark_as_advanced(HDF4_LIBRARIES HDF4_INCLUDE_DIRS)

set(HDF4_DEFINITIONS "")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HDF4 DEFAULT_MSG HDF4_LIBRARIES HDF4_INCLUDE_DIRS)
