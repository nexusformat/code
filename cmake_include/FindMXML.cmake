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

find_library(MXML NAMES mxml mxml1 PATHS $ENV{MXML_ROOT})

find_path(MXML_INCLUDE NAMES mxml.h mxml1.h PATHS $ENV{MXML_ROOT})

if(MXML AND MXML_INCLUDE)
    set(MXML_LINK "-lmxml")
    set(MXML_CPP "-DNXXML")
    include_directories($ENV{MXML_INCLUDE})
else(MXML AND MXML_INCLUDE)
    include_directories(${CMAKE_SOURCE_DIR}/third_party/mxml-2.6)
#    add_dependencies(NeXus_Static_Library MXML_Static_Library)
#    add_dependencies(NeXus_Shared_Library MXML_Static_Library)
#    set(MXML_LINK "-lmxml")
#    set(MXML_CPP "-DNXXML")
endif(MXML AND MXML_INCLUDE)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MXMLLIB DEFAULT_MSG MXML MXML_INCLUDE)

MARK_AS_ADVANCED(MXML)