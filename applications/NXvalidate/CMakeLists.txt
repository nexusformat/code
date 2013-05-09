## Process this file with cmake
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

add_subdirectory(cmdline)

if (WIN32)
    configure_file(nxvalidate.bat.in nxvalidate.bat @ONLY)
	install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/nxvalidate.bat DESTINATION bin)
else(WIN32)
	configure_file(nxvalidate.in nxvalidate @ONLY)
	install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/nxvalidate DESTINATION bin)
endif(WIN32)

if ( NOT (${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR}) )
    add_custom_command(OUTPUT build.xml COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/build.xml . COMMENT "Creating build.xml")
    add_custom_command(OUTPUT nbproject COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/nbproject nbproject COMMENT "Creating nbproject")
    set_property(DIRECTORY . APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES build.xml nbproject)
endif()

add_custom_target(NXvalidate ALL COMMAND ${ANT_EXEC} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build.xml ${CMAKE_CURRENT_BINARY_DIR}/nbproject)

if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/dist/javadoc)
install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/javadoc DESTINATION ${NXDOCDIR}/java/nxvalidate
         FILE_PERMISSIONS
            OWNER_READ OWNER_EXECUTE OWNER_WRITE
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE
         DIRECTORY_PERMISSIONS
            OWNER_READ OWNER_EXECUTE OWNER_WRITE
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE)
endif()

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/dist/NXvalidate.jar lib/jhall.jar lib/saxon9he.jar DESTINATION share/java/lib COMPONENT Runtime)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/dist/NXvalidate.jar DESTINATION share/java COMPONENT Development)


