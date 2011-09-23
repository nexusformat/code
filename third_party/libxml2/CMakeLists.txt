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
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
#
#====================================================================
cmake_minimum_required(VERSION 2.8)

OPTION(WITH_TRIO "Enable TRIO support" OFF)

OPTION(LIBXML_AUTOMATA_ENABLED "" OFF)
OPTION(LIBXML_C14N_ENABLED "" ON)
OPTION(LIBXML_CATALOG_ENABLED "" ON)
OPTION(LIBXML_DEBUG_ENABLED "" ON)
OPTION(LIBXML_DOCB_ENABLED "" ON)
OPTION(LIBXML_EXPR_ENABLED "" OFF)
OPTION(LIBXML_FTP_ENABLED "" ON)
OPTION(LIBXML_HTML_ENABLED "" ON)
OPTION(LIBXML_HTTP_ENABLED "" ON)
OPTION(LIBXML_ICU_ENABLED "" ON)
OPTION(LIBXML_ISO8859X_ENABLED "" ON)
OPTION(LIBXML_LEGACY_ENABLED "" OFF )
OPTION(LIBXML_MODULES_ENABLED "" ON)
OPTION(LIBXML_OUTPUT_ENABLED "" ON)
OPTION(LIBXML_PATTERN_ENABLED "" ON)
OPTION(LIBXML_PUSH_ENABLED "" ON)
OPTION(LIBXML_READER_ENABLED "" ON)
OPTION(LIBXML_REGEXP_ENABLED "" ON)
OPTION(LIBXML_SAX1_ENABLED "" ON)
OPTION(LIBXML_SCHEMAS_ENABLED "" ON)
OPTION(LIBXML_SCHEMATRON_ENABLED "" ON)
OPTION(LIBXML_TEST_VERSION "" OFF)
OPTION(LIBXML_THREAD_ENABLED "" ON)
OPTION(LIBXML_TREE_ENABLED "" ON)
OPTION(LIBXML_UNICODE_ENABLED "" ON)
OPTION(LIBXML_VALID_ENABLED "" ON)
OPTION(LIBXML_WRITER_ENABLED "" ON)
OPTION(LIBXML_XINCLUDE_ENABLED "" ON)
OPTION(LIBXML_XPATH_ENABLED "" ON)
OPTION(LIBXML_XPTR_ENABLED "" ON)
OPTION(LIBXML_ZLIB_ENABLED "" ON)

include(cmake_include/Findlconv.cmake)
include(cmake_include/FindAllIncludes.cmake)

#add_subdirectory (doc)
#add_subdirectory (example)
#add_subdirectory (include)
#add_subdirectory (python)
#add_subdirectory (xstc)

#add_definitions(-DIPNS_RETRIEVER -DFRM2_RETRIEVER -DTEXT_COLLIST_RETRIEVER -DTEXT_PLAIN_RETRIEVER -DTEXT_XML_RETRIEVER -DDYNAMIC_RETRIEVER -DSNS_HISTOGRAM_RETRIEVER -DLOOPY_RETRIEVER -DBINARY_RETRIEVER -DEDF_RETRIEVER -DSPEC_RETRIEVER)

include_directories(. include include/libxml ${CMAKE_BINARY_DIR})

if(WITH_TRIO)
    set (libxml2_sources SAX.c entities.c encoding.c error.c parserInternals.c
		parser.c tree.c hash.c list.c xmlIO.c xmlmemory.c uri.c
		valid.c xlink.c HTMLparser.c HTMLtree.c debugXML.c xpath.c
		xpointer.c xinclude.c nanohttp.c nanoftp.c DOCBparser.c
		catalog.c globals.c threads.c c14n.c xmlstring.c
		xmlregexp.c xmlschemas.c xmlschemastypes.c xmlunicode.c
		triostr.c trio.c xmlreader.c relaxng.c dict.c SAX2.c
		xmlwriter.c legacy.c chvalid.c pattern.c xmlsave.c
		xmlmodule.c schematron.c)
else(WITH_TRIO)
    set (libxml2_sources SAX.c entities.c encoding.c error.c parserInternals.c
		parser.c tree.c hash.c list.c xmlIO.c xmlmemory.c uri.c
		valid.c xlink.c HTMLparser.c HTMLtree.c debugXML.c xpath.c
		xpointer.c xinclude.c nanohttp.c nanoftp.c DOCBparser.c
		catalog.c globals.c threads.c c14n.c xmlstring.c
		xmlregexp.c xmlschemas.c xmlschemastypes.c xmlunicode.c
		xmlreader.c relaxng.c dict.c SAX2.c
		xmlwriter.c legacy.c chvalid.c pattern.c xmlsave.c
		xmlmodule.c schematron.c)
endif(WITH_TRIO)

add_library (XML2_Static_Library STATIC ${libxml2_sources})

add_library (XML2_Shared_Library SHARED ${libxml2_sources})

set_target_properties(XML2_Static_Library PROPERTIES OUTPUT_NAME xml2)

set_target_properties(XML2_Shared_Library PROPERTIES OUTPUT_NAME xml2)

add_executable (runtest runtest.c)
target_link_libraries(runtest XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testrecurse testrecurse.c)
target_link_libraries(testrecurse XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testchar testchar.c)
target_link_libraries(testchar XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testdict testdict.c)
target_link_libraries(testdict XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (runsuite runsuite.c)
target_link_libraries(runsuite XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (xmllint xmllint.c)
target_link_libraries(xmllint XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testSAX testSAX.c)
target_link_libraries(testSAX XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testHTML testHTML.c)
target_link_libraries(testHTML XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (xmlcatalog xmlcatalog.c)
target_link_libraries(xmlcatalog XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testXPath testXPath.c)
target_link_libraries(testXPath XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testC14N testC14N.c)
target_link_libraries(testC14N XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testThreadsWin32 testThreadsWin32.c)
target_link_libraries(testThreadsWin32 XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testURI testURI.c)
target_link_libraries(testURI XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testRegexp testRegexp.c)
target_link_libraries(testRegexp XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testAutomata testAutomata.c)
target_link_libraries(testAutomata XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testSchemas testSchemas.c)
target_link_libraries(testSchemas XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testRelax testRelax.c)
target_link_libraries(testRelax XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testReader testReader.c)
target_link_libraries(testReader XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_executable (testModule testModule.c)
target_link_libraries(testModule XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})
add_library (Testdso_Library SHARED testdso.c)
target_link_libraries(Testdso_Library XML2_Static_Library ${M_LINK} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})

#target_link_libraries(nxtranslate NeXus_Static_Library ${HDF5_LIBRARIES}
#                      ${MXML_LINK} ${READLINE_LINK} ${M_LINK} ${DF_LINK}
#                      ${DL_LINK} ${PTHREAD_LINK} ${TERMCAP_LINK} ${HDF4_LINK}
#                      ${HISTORY_LINK} ${JPEG_LIBRARIES} ${ZLIB_LIBRARIES} 
#                      ${LIBXML2_LIBRARIES} BinaryRetriever Edf FRM2 IPNSRunfile
#                      Loopy SNShistogram Spec TextCollist TextPlain TextXML
#                      )

#install (TARGETS nxtranslate DESTINATION bin)