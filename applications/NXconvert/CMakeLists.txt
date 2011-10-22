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

include_directories(../../third_party ../../bindings/cpp)

add_executable (nxconvert nxconvert.cpp nxconvert_common.h )

set(NXCONVERT_LIB_SRC nxconvert_common.cpp nxconvert_common.h )

add_library (NXconvert_Static_Library STATIC ${NXCONVERT_LIB_SRC} )

add_library (NXconvert_Shared_Library SHARED ${NXCONVERT_LIB_SRC} )

#set_target_properties(NXconvert_Static_Library PROPERTIES OUTPUT_NAME NXconvert)

target_link_libraries(NXconvert_Static_Library NeXus_CPP_Static_Library
                       ${READLINE_LINK} ${M_LINK} ${DL_LINK}
                      ${PTHREAD_LINK} ${DF_LINK} ${TERMCAP_LINK} ${HISTORY_LINK})

target_link_libraries(NXconvert_Shared_Library NeXus_CPP_Shared_Library
                       ${READLINE_LINK} ${M_LINK} ${DL_LINK}
                      ${PTHREAD_LINK} ${DF_LINK} ${TERMCAP_LINK} ${HISTORY_LINK})

target_link_libraries(nxconvert NXconvert_Shared_Library
                      ${READLINE_LINK} ${M_LINK} ${DL_LINK} ${PTHREAD_LINK}
                      ${DF_LINK} ${TERMCAP_LINK} ${HISTORY_LINK})

set_property(TARGET NXconvert_Shared_Library PROPERTY ENABLE_EXPORTS YES) 

install (TARGETS NXconvert_Static_Library NXconvert_Shared_Library nxconvert
         RUNTIME DESTINATION bin COMPONENT Runtime
         LIBRARY DESTINATION lib COMPONENT Runtime
         ARCHIVE DESTINATION lib/nexus COMPONENT Development)

install (FILES nxconvert.1 DESTINATION man/man1)

