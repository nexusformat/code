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
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
#
#====================================================================

add_definitions(-DIN_NEXUS_LIBRARY ${HDF5_DEFINITIONS} ${HDF5_CPP} ${HDF4_CPP} ${MXML_CPP})

if (HDF4_FOUND)
  set (H4SRC napi4.c)
endif (HDF4_FOUND)

if (HDF5_FOUND)
  set (H5SRC napi5.c)
endif (HDF5_FOUND)

set (XMLSRC nxxml.c nxio.c nxio.h)

file(STRINGS ${PROJECT_SOURCE_DIR}/src/nexus_symbols.txt NEXUS_SYMBOLS)

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

# need napi_exports2.c  on win32 but not win64
if (WIN32)
    set(WINSRC napi_exports.c napi_exports.h)
endif (WIN32)

#Make NeXus Static Library
add_library (NeXus_Static_Library STATIC napi.c napiu.c nxstack.c nxstack.h
             stptok.c  nxdataset.c nxdataset.h nx_stptok.h ${WINSRC} ${H4SRC} ${H5SRC}
             ${XMLSRC})

set_target_properties(NeXus_Static_Library PROPERTIES OUTPUT_NAME NeXus)

target_link_libraries(NeXus_Static_Library ${HDF5_LIBRARIES}
                      ${HDF4_LINK} ${READLINE_LINK} ${M_LINK} ${DL_LINK} ${PTHREAD_LINK} ${DF_LINK}
                      ${TERMCAP_LINK} ${HISTORY_LINK} ${JPEG_LIBRARIES} ${ZIP_LIB} ${SZIP_LIB})

#Make NeXus Shared Library

add_library (NeXus_Shared_Library SHARED napi.c napiu.c nxstack.c nxstack.h
             stptok.c  nxdataset.c nxdataset.h nx_stptok.h ${WINSRC} ${H4SRC} ${H5SRC}
             ${XMLSRC})

#Note - library version needs to be got from somewhere?
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_target_properties(NeXus_Shared_Library PROPERTIES OUTPUT_NAME NeXus
                      VERSION 1.0 SOVERSION 4 LINK_FLAGS /def:${PROJECT_SOURCE_DIR}/Windows_extra/libNeXus-0-x64.def)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set_target_properties(NeXus_Shared_Library PROPERTIES OUTPUT_NAME NeXus
                      VERSION 1.0 SOVERSION 4)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

target_link_libraries(NeXus_Shared_Library ${HDF5_LIBRARIES}
                      ${HDF4_LINK} ${READLINE_LINK} ${M_LINK} ${DL_LINK} ${PTHREAD_LINK} ${DF_LINK}
                      ${TERMCAP_LINK} ${HISTORY_LINK} ${JPEG_LIBRARIES} ${ZIP_LIB} ${SZIP_LIB})

if(MXMLLIB_FOUND)
    target_link_libraries(NeXus_Static_Library ${MXML})
    target_link_libraries(NeXus_Shared_Library ${MXML})
else()
    target_link_libraries(NeXus_Static_Library MXML_Static_Library)
    target_link_libraries(NeXus_Shared_Library MXML_Static_Library)
endif(MXMLLIB_FOUND)

install (TARGETS NeXus_Static_Library NeXus_Shared_Library
         RUNTIME DESTINATION bin COMPONENT Runtime
         LIBRARY DESTINATION lib COMPONENT Runtime
         ARCHIVE DESTINATION lib/nexus COMPONENT Development)
