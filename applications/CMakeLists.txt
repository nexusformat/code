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

#The LibXML 2 Libraries
find_package(LibXml2)

# Recurse into the subdirectories.
include_directories("../include")

add_subdirectory (NXbrowse)
add_subdirectory(c-nxvalidate)

if (ENABLE_CXX)
  add_subdirectory (NXdir)
  add_subdirectory (NXconvert)
  add_subdirectory (NXtraverse)
endif()



#if (BUILD_FORTRAN_BINDINGS AND CMAKE_Fortran_COMPILER_WORKS)
#    add_subdirectory (NXdump)
#endif()
#
#if (HAVE_MXML)
#add_subdirectory (nxingest)
#endif(HAVE_MXML)

#
#if(Java_JAVAC_EXECUTABLE AND ANT_FOUND)
#    add_subdirectory (NXvalidate)
#endif(Java_JAVAC_EXECUTABLE AND ANT_FOUND)
#
#if(LIBXML2_FOUND)
#    add_subdirectory (NXsummary)
#    add_subdirectory (NXtranslate)
#endif(LIBXML2_FOUND)
#
#install (PROGRAMS nxdiff DESTINATION bin COMPONENT Runtime)
