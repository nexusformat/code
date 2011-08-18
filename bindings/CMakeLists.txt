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
# Recurse into the subdirectories.
add_subdirectory (cpp)
if (CMAKE_Fortran_COMPILER_WORKS)
    add_subdirectory (f77)
    add_subdirectory (f90)
endif (CMAKE_Fortran_COMPILER_WORKS)
#add_subdirectory (idl)
if (NOT(Java_JAVAC_EXECUTABLE MATCHES NOTFOUND))
    add_subdirectory (java)
endif ()
#add_subdirectory (matlab)
if (PYTHONINTERP_FOUND)
    add_subdirectory (python)
endif (PYTHONINTERP_FOUND)
#add_subdirectory (swig)
