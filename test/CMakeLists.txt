## Process this file with cmake
#==============================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  CMakeLists for building the NeXus library and applications.
#
#  Copyright (C) 2011 Stephen Rankin
#  
#  This library is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by the
#  Free Software Foundation; either version 2 of the License, or (at your
#  option) any later version.
# 
#  This library is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
#  for more details.
# 
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; if not, write to the Free Software Foundation,
#  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#             
#  For further information, see <http://www.nexusformat.org>
#
#
#==============================================================================

set(NAPITESTSRC napi_test.c)

#------------------------------------------------------------------------------
# Need to copy the test data files 
#------------------------------------------------------------------------------
add_subdirectory(data)

include_directories(${PROJECT_SOURCE_DIR}/include )
link_directories(${HDF5_LIBRARY_DIRS} 
                 ${HDF4_LIBRARY_DIRS}
                 ${MXML_LIBRARY_DIRS})

#------------------------------------------------------------------------------
# In all cases we have to build the C-tests
#------------------------------------------------------------------------------
add_executable(napi_c_test napi_test.c)
target_link_libraries(napi_c_test NeXus_Shared_Library ${NAPI_LINK_LIBS})
add_test(NAME "NAPI-C-test" COMMAND napi_t_test)
         

if(ENABLE_CXX)
    list(APPEND NAPITESTSRC napi_test_cpp.cxx)

    include_directories(../bindings/cpp)
    add_executable(napi_cpp_test napi_test_cpp.cxx)
    target_link_libraries(napi_cpp_test NeXus_CPP_Shared_Library
                          ${NAPI_LINK_LIBS})
endif()

if(ENABLE_FORTRAN77)
    #sources napif4_test.f  napif5_test.f  napif_test.f)
endif()

if(ENABLE_FORTRAN90)
    #need to add the directory with the .mod files to the include path
    include_directories(${PROJECT_BINARY_DIR}/bindings/f90)
    add_executable(NXtestf90 NXtest.f90)
    target_link_libraries(NXtestf90 NeXus_F90_Shared_Library 
                          ${NAPI_LINK_LIBS})
    add_test(NAME "Fortran-90-test" COMMAND NXtestf90)
endif()


