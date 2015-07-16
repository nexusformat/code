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

if(MXML_INCLUDE_DIRS OR MXML_LIBRARY_DIRS)
    #if the user has provided the search path we have to do nothing but check 
    #wether or not the library files exist 
    if(MXML_LIBRARY_DIRS)
        #if the user has provided a path we use this one 
        find_library(MXML_LIBFILE NAME mxml PATHS ${MXML_LIBRARY_DIRS} NO_DEFAULT_PATH)
    else(MXML_LIBRARY_DIRS)
        #if the user has not provided a path we look in the 
        #system defaults
        find_library(MXML_LIBFILE NAME mxml PATHS)
        get_filename_component(MXML_LIBRARY_DIRS ${MXML_LIBFILE} PATH)
    endif(MXML_LIBRARY_DIRS)

    if(${MXML_LIBFILE} MATCHES "MXML_LIBFILE-NOTFOUND")
        set(HAVE_MXML FALSE)
    endif()

    #-------------------------------------------------------------------------
    # search for the header file
    #-------------------------------------------------------------------------
    if(MXML_INCLUDE_DIRS)
        #just check if the user provided path contains the header file
        find_file(MXML_HDRFILE NAME mxml.h PATHS ${MXML_INCLUDE_DIRS} NO_DEFAULT_PATH)
    else()
        find_file(MXML_HDRFILE NAME mxml.h PATHS)
        get_filename_component(MXML_INCLUDE_DIRS ${MXML_HDRFILE} PATH)
    endif()
    
    if(${MXML_HDRFILE} MATCHES "MXML_HDRFILE-NOTFOUND")
        set(HAVE_MXML FALSE)
    endif()

else()
    #if the user has not provided any configuration we have to do this manually
    if(PKG_CONFIG_FOUND)
        #the easy way  - we use package config
        pkg_search_module(MXML mxml)
    endif()

    #if pkg-config was not successful we have to do this the hard way
    if(NOT MXML-FOUND)
        find_library(MXML_LIBFILE NAME mxml PATHS)
        if(${MXML_LIBFILE} MATCHES "MXML_LIBFILE-NOTFOUND")
            set(HAVE_MXML FALSE)
        endif()

        find_file(MXML_HDRFILE NAME mxml.h PATHS)
        if(${MXML_HDRFILE} MATCHES "MXML_HDRFILE-NOTFOUND")
            set(HAVE_MXML FALSE)
        endif()
        
    endif()

endif()

if(WITH_MXML AND NOT HAVE_MXML)
    message(FATAL_ERROR "User requested MXML not found!")
else(NOT WITH_MXML AND HAVE_MXML) 
    set(WITH_MXML TRUE)
    message("-- Build with MXML support!")
endif()


