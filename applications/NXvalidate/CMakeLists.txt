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

configure_file(nxvalidate.in nxvalidate)

execute_process(COMMAND ${ANT})

install (TARGETS nxvalidate DESTINATION bin)
install (DIRECTORY dist/javadoc DESTINATION ${NXDOCDIR}/java/nxvalidate
         FILE_PERMISSIONS
            OWNER_READ OWNER_EXECUTE OWNER_WRITE
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE
         DIRECTORY_PERMISSIONS
            OWNER_READ OWNER_EXECUTE OWNER_WRITE
            GROUP_READ GROUP_EXECUTE 
            WORLD_READ WORLD_EXECUTE)

install (LIBRARY dist/NXvalidate.jar lib/jhall.jar lib/saxon9he.jar DESTINATION lib/lib COMPONENT Runtime)
install (LIBRARY dist/NXvalidate.jar DESTINATION lib COMPONENT Development)


