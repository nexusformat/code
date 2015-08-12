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

add_definitions(-DIN_NEXUS_LIBRARY=1 ${NX_CPP})

#-----------------------------------------------------------------------------
# generate list of common source files
#-----------------------------------------------------------------------------
set (NAPISRC napi.c napiu.c nxstack.c nxstack.h stptok.c  nxdataset.c 
             nxdataset.h nx_stptok.h)

set (NAPILINK)

#-----------------------------------------------------------------------------
# add HDF4 specific source files
#-----------------------------------------------------------------------------
if (WITH_HDF4)
  set (NAPISRC ${NAPISRC} napi4.c)
  add_definitions(-DHDF4)
  include_directories(${HDF4_INCLUDE_DIRS})
  list(APPEND NAPILINK ${HDF4_LIBRARIES})
endif(WITH_HDF4)

#-----------------------------------------------------------------------------
# add HDF5 specific source files
#-----------------------------------------------------------------------------
if (WITH_HDF5)
  set (NAPISRC ${NAPISRC} napi5.c)
  add_definitions(-DHDF5 ${HDF5_API_DEFS})
  include_directories(${HDF5_INCLUDE_DIRS})
  link_directories(${HDF5_LIBRARY_DIR})
  list(APPEND NAPILINK ${HDF5_LIBRARIES})
endif (WITH_HDF5)

#-----------------------------------------------------------------------------
# add XML specific source files
#-----------------------------------------------------------------------------
if(WITH_MXML)
    set (NAPISRC ${NAPISRC} nxxml.c nxio.c nxio.h)
    add_definitions(-DNXXML)
	include_directories(${MXML_INCLUDE_DIRS})
    list(APPEND NAPILINK ${MXML_LIBRARIES})
endif(WITH_MXML)

set_property(SOURCE ${NAPISRC} APPEND PROPERTY COMPILE_FLAGS ${NX_CFLAGS})

file(STRINGS ${PROJECT_SOURCE_DIR}/src/nexus_symbols.txt NEXUS_SYMBOLS)

#if(SZIP_LIBRARIES)
#    list(APPEND NX_LIBS ${SZIP_LIBRARIES})
#endif()

#------------------------------------------------------------------------------
# building the static library
#------------------------------------------------------------------------------
message(STATUS "Add include directory: ${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include")
add_library (NeXus_Static_Library STATIC ${NAPISRC})

set_target_properties(NeXus_Static_Library PROPERTIES 
                      OUTPUT_NAME NeXusStatic)

set_property(TARGET NeXus_Static_Library APPEND PROPERTY 
             COMPILE_DEFINITIONS H4_BUILT_AS_STATIC_LIB)

target_link_libraries(NeXus_Static_Library ${NAPILINK})
#------------------------------------------------------------------------------
# building the shared library
#------------------------------------------------------------------------------
add_library (NeXus_Shared_Library SHARED ${NAPISRC})
set_property(TARGET NeXus_Shared_Library APPEND PROPERTY 
             COMPILE_DEFINITIONS H4_BUILT_AS_STATIC_LIB)

set_target_properties(NeXus_Shared_Library PROPERTIES 
                      OUTPUT_NAME NeXus
                      VERSION 1.0 
                      SOVERSION 4)

target_link_libraries(NeXus_Shared_Library ${NAPILINK})


#------------------------------------------------------------------------------
# install the targets
#-----------------------------------------------------------------------------
install (TARGETS NeXus_Static_Library NeXus_Shared_Library
         RUNTIME DESTINATION bin COMPONENT Runtime
         LIBRARY DESTINATION lib COMPONENT Runtime
         ARCHIVE DESTINATION lib/nexus COMPONENT Development)

