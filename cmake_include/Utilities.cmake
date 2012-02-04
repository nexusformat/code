## Process this file with cmake
#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#
#  CMakeLists for building the NeXus library and applications.
#
#  Copyright (C) 2011 Freddie Akeroyd
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

# checks a list of possible compiler flags and adds allowed ones to the current list
function(check_add_c_compiler_flags FLAGS)
    foreach(FLAG ${FLAGS})
	    message("c hello ${FLAG}")
        check_c_compiler_flag(${FLAG} RES)
	    if (RES)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}" PARENT_SCOPE)
	    endif()
	endforeach()
endfunction()

# checks a list of possible compiler flags and adds allowed ones to the current list
function(check_add_cxx_compiler_flags FLAGS)
    foreach(FLAG ${FLAGS})
	    message("cxx hello ${FLAG}")
        check_cxx_compiler_flag(${FLAG} RES)
	    if (RES)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}" PARENT_SCOPE)
	    endif()
	endforeach()
endfunction()

# define a HAVE_  if both  BUILD_  and  _FOUND  are defined
# e.g. creates HAVE_HDF5 if both BUILD_HDF5 and HDF5_FOUND are ture
function(create_have_vars NAMES)
    foreach(NAME ${NAMES})
        if(${BUILD_${NAME}} AND ${${NAME}_FOUND})
	        set(HAVE_${NAME} ON PARENT_SCOPE)
	    else()
	        set(HAVE_${NAME} OFF PARENT_SCOPE)
	    endif()
	endforeach()
endfunction()

function(install_pdb target)
#	set (OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR})
	set (OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Release)
	get_target_property (OUT_NAME ${target} OUTPUT_NAME)
	get_filename_component (OUT_BASE_NAME ${OUT_NAME} NAME_WE)
#	set(PDB_FILE ${OUT_DIR}/${OUT_BASE_NAME}${CMAKE_DEBUG_POSTFIX}.pdb)
	set(PDB_FILE ${OUT_DIR}/${OUT_BASE_NAME}.pdb)
	install (FILES ${PDB_FILE} DESTINATION bin COMPONENT Runtime)  
endfunction()
