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

if(DOCBOOKUTILS_FOUND)

    SET(DOC_SRC NeXus_definitions.docbook terminology.docbook methods.docbook base_classes.docbook tofnpd.docbook tofnref.docbook monoref.docbook tofdgs.docbook monotas.docbook)
    SET(IMAGES tofnpd.jpg tofnpd.gif)

    ADD_CUSTOM_COMMAND( 
        OUTPUT    NeXus_definitions.pdf
        COMMAND   ${DOCBOOK2PDF_EXECUTABLE}
        ARGS      ${CMAKE_SOURCE_DIR}/doc/tech_ref/NeXus_definitions.docbook
        DEPENDS   ${DOC_SRC} ${IMAGES}
        COMMENT   "PDF NeXus Definitions"
    )

    ADD_CUSTOM_COMMAND( 
        OUTPUT    NeXus_definitions.txt
        COMMAND   ${DOCBOOK2TXT_EXECUTABLE}
        ARGS      ${CMAKE_SOURCE_DIR}/doc/tech_ref/NeXus_definitions.docbook
        DEPENDS   NeXus_definitions.pdf
        COMMENT   "TEXT NeXus Definitions"
    )

    ADD_CUSTOM_TARGET(NexusDocbookBuild ALL echo
        DEPENDS   NeXus_definitions.txt
    )

	install (FILES ${CMAKE_BINARY_DIR}/doc/tech_ref/NeXus_definitions.pdf
                       ${CMAKE_BINARY_DIR}/doc/tech_ref/NeXus_definitions.txt
                       DESTINATION ${NXDOCDIR}/tech_ref)

endif(DOCBOOKUTILS_FOUND)



