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
#  For further information, see <http://www.nexusformat.org>
#
#
#====================================================================

if(DEFINED JAVA_COMPILE)
    if(DEFINED JNI_INCLUDE_DIRS)

        INCLUDE_DIRECTORIES(. ../ native ${JNI_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/bindings/java/native)

        SET(CMAKE_INCLUDE_CURRENT_DIR ON)

        MESSAGE(STATUS ${CMAKE_SOURCE_DIR}/bindings/java/)

        SET(JAVA_TEST ${CMAKE_SOURCE_DIR}/bindings/java/test/TestJapi.java)

        SET(JAVA_SOURCES ${CMAKE_SOURCE_DIR}/bindings/java/ncsa/hdf/hdflib/HDFException.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/ncsa/hdf/hdflib/HDFJavaException.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/ncsa/hdf/hdflib/HDFNotImplementedException.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/ncsa/hdf/hdflib/HDFConstants.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/ncsa/hdf/hdflib/HDFArray.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/ncsa/hdf/hdflib/HDFNativeData.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/org/nexusformat/NexusException.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/org/nexusformat/NXlink.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/org/nexusformat/NeXusFileInterface.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/org/nexusformat/AttributeEntry.java 
             ${CMAKE_SOURCE_DIR}/bindings/java/org/nexusformat/NexusFile.java 
             ${JAVA_TEST})

        SET(JAVA_CLASSES ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/HDFException.class 
             ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/HDFJavaException.class 
             ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/HDFNotImplementedException.class 
             ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/HDFConstants.class 
             ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/HDFArray.class 
             ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/HDFNativeData.class 
             ${CMAKE_BINARY_DIR}/bindings/java/org/nexusformat/NexusException.class 
             ${CMAKE_BINARY_DIR}/bindings/java/org/nexusformat/NXlink.class 
             ${CMAKE_BINARY_DIR}/bindings/java/org/nexusformat/NeXusFileInterface.class 
             ${CMAKE_BINARY_DIR}/bindings/java/org/nexusformat/AttributeEntry.class 
             ${CMAKE_BINARY_DIR}/bindings/java/org/nexusformat/NexusFile.class 
             ${CMAKE_BINARY_DIR}/bindings/java/TestJapi.class)

        SET(EXTRA_CLASSES ${CMAKE_BINARY_DIR}/bindings/java/ncsa/hdf/hdflib/ArrayDescriptor.class)

        SET(JNI_HEADER native/org_nexusformat_NexusFile.h)

        SET(JAR_ARCHIVE ${CMAKE_BINARY_DIR}/bindings/java/jnexus.jar)

        SET(JAVA_DOCS ${CMAKE_BINARY_DIR}/bindings/java/apidoc)

        FIND_PROGRAM(JAVA_NATIVE
           NAMES javah
           PATHS ${JAVA_BIN_PATH}
        )

        STRING(REPLACE ".java" ".class" javaclass "${JAVA_SOURCES}")

        ADD_CUSTOM_COMMAND( 
            OUTPUT    ${JAVA_CLASSES}
            COMMAND   ${JAVA_COMPILE}
            ARGS      -d . ${JAVA_SOURCES}
            COMMENT   "Compile Java"
        )

        ADD_CUSTOM_COMMAND( 
            OUTPUT    ${EXTRA_CLASSES}
            COMMAND   
            DEPENDS   ${JAVA_CLASSES}
            COMMENT   "Include Extra Built Classes"
        )

        ADD_CUSTOM_COMMAND( 
            OUTPUT    ${JAR_ARCHIVE}
            COMMAND   ${JAVA_ARCHIVE}
            ARGS      cvf ${JAR_ARCHIVE} ${JAVA_CLASSES} ${EXTRA_CLASSES}
            DEPENDS   ${EXTRA_CLASSES}
            COMMENT   "Build JAR File"
        )

        ADD_CUSTOM_COMMAND( 
            OUTPUT    ${JNI_HEADER}
            COMMAND   ${Java_JNI_EXECUTABLE}
            ARGS      -jni -d native -classpath . org.nexusformat.NexusFile
            DEPENDS   ${JAR_ARCHIVE}
            COMMENT   "Build JNI Header"
        )

        ADD_CUSTOM_COMMAND( 
            OUTPUT    ${JAVA_DOCS}
            COMMAND   ${Java_DOC_EXECUTABLE}
            ARGS      -d ${JAVA_DOCS} -windowtitle jnexus -doctitle jnexus -classpath . 
                      -sourcepath ${CMAKE_SOURCE_DIR}/bindings/java org.nexusformat ncsa.hdf.hdflib
            DEPENDS   ${JNI_HEADER}
            COMMENT   "Build Javadocs"
        )

        ADD_CUSTOM_TARGET(NexusJavaBuild ALL echo
           DEPENDS   ${JAR_ARCHIVE}
        )
        
        ADD_CUSTOM_TARGET(NexusJavadocBuild ALL echo
           DEPENDS   ${JAVA_DOCS}
        )

        ADD_LIBRARY(jnexus SHARED native/hdfnativeImp.c 
                     native/hdfexceptionImp.c 
                     native/handle.c 
                     native/NexusFile.c 
                     native/hdfexceptionImp.h)

        SET_TARGET_PROPERTIES(jnexus PROPERTIES VERSION 1.0 SOVERSION 4)

        TARGET_LINK_LIBRARIES(jnexus NeXus_Shared_Library ${HDF5_LIBRARIES}
                      ${READLINE_LINK} ${M_LINK} ${DF_LINK}
                      ${DL_LINK} ${PTHREAD_LINK} ${TERMCAP_LINK} ${HDF4_LINK}
                      ${HISTORY_LINK} ${JPEG_LIBRARIES} ${ZIP_LIB} ${SZIP_LIB}
                      )

        INSTALL(TARGETS jnexus
            LIBRARY DESTINATION lib COMPONENT Runtime ARCHIVE DESTINATION lib COMPONENT Runtime
        )

        INSTALL(FILES ${JAR_ARCHIVE} DESTINATION lib)

        INSTALL(DIRECTORY ${JAVA_DOCS} DESTINATION ${NXDOCDIR}/java)

        INSTALL(FILES ${JAVA_TEST} DESTINATION ${NXEXAMPLEDIR}/)

     endif(DEFINED JNI_INCLUDE_DIRS)
endif(DEFINED JAVA_COMPILE)


