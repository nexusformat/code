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
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
#
#====================================================================

#The LibXML 2 Libraries
find_package(LibXml2)

#Find the java runtime and sdk
find_package(Java 1.6)

# Recurse into the subdirectories.

add_subdirectory (NXbrowse)
add_subdirectory (NXdir)
add_subdirectory (NXconvert)

if (CMAKE_Fortran_COMPILER_WORKS)
    add_subdirectory (NXdump)
endif(CMAKE_Fortran_COMPILER_WORKS)

if(MXML_FOUND)
    add_subdirectory (nxingest)
endif(MXML_FOUND)

if(Java_FOUND)
    add_subdirectory (NXvalidate)
endif(Java_FOUND)

if(LIBXML2_FOUND)
    add_subdirectory (NXsummary)
    add_subdirectory (NXtranslate)
endif(LIBXML2_FOUND)

install (PROGRAMS nxdiff DESTINATION bin)

