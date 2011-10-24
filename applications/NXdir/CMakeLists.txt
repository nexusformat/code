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

add_executable (nxdir main.cpp string_util.cpp tree.cpp data.cpp
                data_writer.cpp nxdir.h nxdir_help.h)

target_link_libraries(nxdir NeXus_Shared_Library 
                      ${MXML_LINK} ${READLINE_LINK} ${M_LINK}
                      ${DL_LINK} ${PTHREAD_LINK} ${DF_LINK} ${TERMCAP_LINK}
                      ${HISTORY_LINK} )

install (TARGETS nxdir DESTINATION bin COMPONENT Runtime)
install (FILES nxdir.1 DESTINATION man/man1 COMPONENT Documentation)


