## Process this file with cmake
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#
#  CMakeLists for building the NeXus library and applications.
#
#  Copyright (C) 2010 Stephen Rankin
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
include(FindAllIncludes.cmake)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include_directories(vcnet)
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

set (SOURCE mxml-attr.c mxml-file.c mxml-private.c mxml-string.c 
mxml-index.c mxml-search.c mxml-entity.c mxml-node.c mxml-set.c)

set (HEADER mxml.h mxml-private.h)

add_executable (testmxml testmxml.c ${HEADER})

add_executable (mxmldoc mxmldoc.c ${HEADER})

add_library (MXML_Static_Library STATIC ${HEADER} ${SOURCE})

#add_library (MXML_Shared_Library SHARED ${HEADER} ${SOURCE})

set_target_properties(MXML_Static_Library PROPERTIES OUTPUT_NAME mxml1)

#set_target_properties(MXML_Shared_Library PROPERTIES OUTPUT_NAME mxml1)

target_link_libraries(testmxml MXML_Static_Library ${PTHREAD_LINK})

target_link_libraries(mxmldoc MXML_Static_Library ${PTHREAD_LINK})


