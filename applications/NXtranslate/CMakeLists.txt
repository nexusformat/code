# Process this file with cmake
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

#add_subdirectory (IPNS_CPP)
add_subdirectory (text_collist)
add_subdirectory (text_plain)
add_subdirectory (text_xml)
add_subdirectory (sns_histogram docs)
add_subdirectory (FRM2)
add_subdirectory (loopy)
add_subdirectory (binary)
add_subdirectory (spec)
add_subdirectory (esrf_edf)
#add_subdirectory ($(GENIESUBDIR)

add_definitions(-DIPNS_RETRIEVER -DFRM2_RETRIEVER -DTEXT_COLLIST_RETRIEVER -DTEXT_PLAIN_RETRIEVER -DTEXT_XML_RETRIEVER -DDYNAMIC_RETRIEVER -DSNS_HISTOGRAM_RETRIEVER -DLOOPY_RETRIEVER -DBINARY_RETRIEVER -DEDF_RETRIEVER -DSPEC_RETRIEVER)


include_directories(. ${LIBXML2_INCLUDE_DIR} IPNS_CPP text_collist text_plain text_xml sns_histogram FRM2 loopy)

add_executable (nxtranslate attr.cpp main.cpp nexus_retriever.cpp  nexus_util.cpp node.cpp
	                  node_util.cpp retriever.cpp string_util.cpp xml_parser.cpp
                          xml_util.h xml_util.cpp attr.h nexus_retriever.h nexus_util.h node.h 
                          node_util.h nxtranslate_debug.h Ptr.h retriever.h string_util.h xml_parser.h 
                          tree.hh dynamic_retriever.cpp dynamic_retriever.h)

target_link_libraries(nxtranslate NeXus_Static_Library ${HDF5_LIBRARIES}
                      ${MXML_LINK} ${READLINE_LINK} ${M_LINK} ${DF_LINK}
                      ${DL_LINK} ${PTHREAD_LINK} ${TERMCAP_LINK} ${HDF4_LINK}
                      ${HISTORY_LINK} ${JPEG_LIBRARIES} ${ZLIB_LIBRARIES} 
                      ${LIBXML2_LIBRARIES} BinaryRetriever Edf FRM2 IPNSRunfile
                      Loopy SNShistogram Spec TextCollist TextPlain TextXML
                      )

install (TARGETS nxtranslate DESTINATION bin COMPONENT Runtime)

