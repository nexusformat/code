# Process this file with cmake
#=============================================================================
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
#=============================================================================

add_subdirectory (text_collist)
add_subdirectory (text_plain)
add_subdirectory (text_xml)
add_subdirectory (sns_histogram docs)
add_subdirectory (FRM2)
add_subdirectory (loopy)
add_subdirectory (binary)
add_subdirectory (spec)
add_subdirectory (esrf_edf)

add_definitions(-DFRM2_RETRIEVER 
                -DTEXT_COLLIST_RETRIEVER 
                -DTEXT_PLAIN_RETRIEVER 
                -DTEXT_XML_RETRIEVER 
                -DDYNAMIC_RETRIEVER 
                -DSNS_HISTOGRAM_RETRIEVER 
                -DLOOPY_RETRIEVER 
                -DBINARY_RETRIEVER 
                -DEDF_RETRIEVER 
                -DSPEC_RETRIEVER)


include_directories(. ${LIBXML2_INCLUDE_DIR} text_collist text_plain 
                    text_xml sns_histogram FRM2 loopy)

set(SOURCES attr.cpp 
            main.cpp 
            nexus_retriever.cpp  
            nexus_util.cpp 
            node.cpp
	        node_util.cpp 
            retriever.cpp 
            string_util.cpp 
            xml_parser.cpp
            xml_util.h 
            xml_util.cpp 
            attr.h 
            nexus_retriever.h 
            nexus_util.h node.h 
            node_util.h 
            nxtranslate_debug.h 
            Ptr.h 
            retriever.h 
            string_util.h 
            xml_parser.h 
            tree.hh 
            dynamic_retriever.cpp 
            dynamic_retriever.h)

add_executable(nxtranslate ${SOURCES} 
               $<TARGET_OBJECTS:BinaryRetriever>
               $<TARGET_OBJECTS:TextCollist>
               $<TARGET_OBJECTS:TextPlain>
               $<TARGET_OBJECTS:TextXML>
               $<TARGET_OBJECTS:SNShistogram>
               $<TARGET_OBJECTS:FRM2>
               $<TARGET_OBJECTS:Loopy>
               $<TARGET_OBJECTS:Spec>
               $<TARGET_OBJECTS:Edf>
              )

target_link_libraries(nxtranslate NeXus_Static_Library ${LIBXML2_LIBRARIES})


install (TARGETS nxtranslate DESTINATION bin COMPONENT Runtime)

