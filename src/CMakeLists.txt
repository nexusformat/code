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

add_definitions(-DIN_NEXUS_LIBRARY=1 ${NX_CPP})

set (NAPISRC napi.c napiu.c nxstack.c nxstack.h stptok.c  nxdataset.c nxdataset.h nx_stptok.h)

if (HAVE_HDF4)
  set (NAPISRC ${NAPISRC} napi4.c)
  add_definitions(-DHDF4 ${HDF4_DEFINITIONS} ${HDF4_CPP})
  include_directories(${HDF4_INCLUDE_DIRS})
  file(GLOB HDF4DLLS "${HDF4_ROOT_DIR}/bin/*.dll") 
  install (FILES ${HDF4DLLS} DESTINATION bin COMPONENT Runtime)
endif (HAVE_HDF4)

if (HAVE_HDF5)
  set (NAPISRC ${NAPISRC} napi5.c)
  add_definitions(-DHDF5 ${HDF5_DEFINITIONS} ${HDF5_CPP})
  include_directories(${HDF5_INCLUDE_DIRS})
  file(GLOB HDF5DLLS "${HDF5_ROOT_DIR}/bin/*.dll") 
  install (FILES ${HDF5DLLS} DESTINATION bin COMPONENT Runtime)
endif (HAVE_HDF5)

if(HAVE_MXML)
    set (NAPISRC ${NAPISRC} nxxml.c nxio.c nxio.h)
    add_definitions(-DNXXML ${MXML_DEFINITIONS} ${MXML_CPP})
	include_directories(${MXML_INCLUDE_DIRS})
endif(HAVE_MXML)

# need napi_exports2.c  on win32 but not win64
if (WIN32)
    if (CMAKE_CL_64)
        set(NAPISRC ${NAPISRC} napi_exports2.c napi_exports.h)
    else()
        set(NAPISRC ${NAPISRC} napi_exports.c napi_exports2.c napi_exports.h)
	endif()
endif (WIN32)

set_property(SOURCE ${NAPISRC} APPEND PROPERTY COMPILE_FLAGS ${NX_CFLAGS})

file(STRINGS ${PROJECT_SOURCE_DIR}/src/nexus_symbols.txt NEXUS_SYMBOLS)

set (NX_LIBS  ${HDF4_LINK} ${READLINE_LINK} ${M_LINK} ${DL_LINK} ${PTHREAD_LINK} ${DF_LINK}
                      ${TERMCAP_LINK} ${HISTORY_LINK} ${JPEG_LIBRARIES} ${SZIP_LIBRARIES} ${ZLIB_LIBRARIES}  )
#if (MINGW_MSYS)
#  set (MINGW_SRC napi_exports.c napi_exports2.c napi_exports.h)
#  file(STRINGS ${PROJECT_SOURCE_DIR}/src/nexus_symbols_win.txt
#       NEXUS_SYMBOLS_WIN)
#  file(WRITE ${PROJECT_SOURCE_DIR}/src/nexus_symbols.sym ${NEXUS_SYMBOLS}
#       ${NEXUS_SYMBOLS_WIN})
#
#  if (HAVE_MS_LIB)
#    file(REMOVE libNeXus.dll.lib libNeXus.dll.exp)
#    execute_process(
#      COMMAND ${MS_LIB} /MACHINE:I386 /DEF:libNeXus.def /OUT:libNeXus.dll.lib)
#  endif (HAVE_MS_LIB)
#
#else (MINGW_MSYS)
#  file(WRITE ${PROJECT_SOURCE_DIR}/src/nexus_symbols.sym ${NEXUS_SYMBOLS})
#endif (MINGW_MSYS)

#Make NeXus Static Library
add_library (NeXus_Static_Library STATIC ${NAPISRC})

set_target_properties(NeXus_Static_Library PROPERTIES OUTPUT_NAME NeXusStatic)

target_link_libraries(NeXus_Static_Library ${HDF5_STATIC_LIBRARIES} ${HDF4_STATIC_LIBRARIES} ${MXML_STATIC_LIBRARIES}  ${NX_LIBS})

#Make NeXus Shared Library

add_library (NeXus_Shared_Library SHARED ${NAPISRC})

#set_property(TARGET NeXus_Shared_Library APPEND PROPERTY COMPILE_DEFINITIONS _HDF5USEDLL_ _HDFDLL_)
set_property(TARGET NeXus_Shared_Library APPEND PROPERTY COMPILE_DEFINITIONS H4_BUILT_AS_STATIC_LIB)
set_property(TARGET NeXus_Static_Library APPEND PROPERTY COMPILE_DEFINITIONS H4_BUILT_AS_STATIC_LIB)

if (CMAKE_CL_64)
   set(DEF_FILE ${PROJECT_SOURCE_DIR}/Windows_extra/libNeXus-0-x64.def)
else()
   set(DEF_FILE ${PROJECT_SOURCE_DIR}/Windows_extra/libNeXus-0-Win32.def)
endif()

#Note - library version needs to be got from somewhere?
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_target_properties(NeXus_Shared_Library PROPERTIES OUTPUT_NAME libNeXus-0
                      VERSION 1.0 SOVERSION 4 )
	if (MSVC)
	    set_property(TARGET NeXus_Shared_Library APPEND PROPERTY LINK_FLAGS /def:${DEF_FILE})
	endif(MSVC)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_target_properties(NeXus_Shared_Library PROPERTIES OUTPUT_NAME NeXus
                      VERSION 1.0 SOVERSION 4)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

target_link_libraries(NeXus_Shared_Library ${HDF5_STATIC_LIBRARIES} ${HDF4_STATIC_LIBRARIES} ${MXML_STATIC_LIBRARIES} ${NX_LIBS})

#if(HAVE_MXML)
#if(MXMLLIB_FOUND)
#    target_link_libraries(NeXus_Static_Library ${MXML})
#    target_link_libraries(NeXus_Shared_Library ${MXML})
#else()
#    target_link_libraries(NeXus_Static_Library MXML_Static_Library)
#    target_link_libraries(NeXus_Shared_Library MXML_Shared_Library)
#endif(MXMLLIB_FOUND)
#endif(HAVE_MXML)

install (TARGETS NeXus_Static_Library NeXus_Shared_Library
         RUNTIME DESTINATION bin COMPONENT Runtime
         LIBRARY DESTINATION lib COMPONENT Runtime
         ARCHIVE DESTINATION lib/nexus COMPONENT Development)

#if(WIN32)
#install_pdb (NeXus_Shared_Library)
#endif()
