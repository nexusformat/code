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

if(DEFINED PYTHONINTERP_FOUND)

    SET (PYDOC_OUTPUT napi.html)
    SET (NXSPYTHON_DOC nxs.napi.html)
    SET (NXSPYTHON_SOURCE ${CMAKE_SOURCE_DIR}/bindings/python/nxs/napi.py)

    GET_TARGET_PROPERTY(LIB_NAME NeXus_Shared_Library LOCATION)
    SET (ENV{NEXUSLIB} ${LIB_NAME})

    FILE(READ ${NXSPYTHON_SOURCE} TEMP_SOURCE_IN)

    STRING(REGEX REPLACE "nxprefix = .*" "nxprefix = ${CMAKE_INSTALL_PREFIX}" TEMP_SOURCE_OUT ${TEMP_SOURCE_IN})

    FILE(WRITE nxs/napi.py.out ${TEMP_SOURCE_OUT}) 

    #We need to run py_compile somehow now on napi.py.out
    #according to the Makefile.am. py_compile is not portable
    #and DESTDIR is not available as a variable in CMAKE
    #(more googleing needed).

    ADD_CUSTOM_COMMAND( 
        OUTPUT    ${PYDOC_OUTPUT}
        COMMAND   ${CMAKE_COMMAND} 
        ARGS      -D NEXUSLIB="${LIB_NAME}" -D NXSPYTHON_SOURCE="${NXSPYTHON_SOURCE}" -D PYTHON_DOC="${PYTHON_DOC}" -P "${CMAKE_SOURCE_DIR}/bindings/python/pythondoc.cmake"
        COMMENT   "Build NXS Python Docs"
    )

 #   ADD_CUSTOM_COMMAND( 
 #       OUTPUT    ${PYDOC_OUTPUT}
 #       COMMAND   ${CMAKE_COMMAND} -E env "NEXUSLIB=${LIB_NAME}" ${PYTHON_DOC}
 #       ARGS      -w ${NXSPYTHON_SOURCE}
 #       COMMENT   "Build NXS Python Docs"
 #   )

    ADD_CUSTOM_COMMAND( 
        OUTPUT    ${NXSPYTHON_DOC}
        COMMAND   ${CMAKE_COMMAND}
        ARGS      -E copy ${PYDOC_OUTPUT} ${NXSPYTHON_DOC}
        DEPENDS   ${PYDOC_OUTPUT}
        COMMENT   "Rename NXS Python Docs"
    )

    ADD_CUSTOM_TARGET(NXSPythonBuild ALL echo
        DEPENDS   ${NXSPYTHON_DOC}
    )

    install (FILES ${CMAKE_BINARY_DIR}/bindings/python/${NXSPYTHON_DOC} DESTINATION ${NXDOCDIR}/python)
    
endif(DEFINED PYTHONINTERP_FOUND)


