
set (CPACK_PACKAGE_NAME "NeXus")
set (CPACK_PACKAGE_VENDOR "Science and Technology Facilities Council")
set (CPACK_PACKAGE_VERSION_MAJOR "4")
set (CPACK_PACKAGE_VERSION_MINOR "2")
set (CPACK_PACKAGE_VERSION_PATCH "1")
set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR/cmake_include}/nexus_description.txt")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "NeXus - a common format for neutron and X-ray scattering data http://www.nexusformat.org/")
set (CPACK_PACKAGE_FILE_NAME "nexus-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "nexus")
#set (CPACK_PROJECT_CONFIG_FILE)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
#set (CPACK_RESOURCE_FILE_README)
set (CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR/cmake_include}/WELCOME")
#set (CPACK_MONOLITHIC_INSTALL)
if(UNIX)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};STGZ;TGZ)
endif(UNIX)
if(LINUX)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};DEB;RPM)
    set (CPACK_RPM_PACKAGE_RELEASE "1")
    set (CPACK_RPM_PACKAGE_REQUIRES "python >= 2.5.0, cmake >= 2.8")
    #set (CPACK_RPM_PACKAGE_PROVIDES "")
endif(LINUX)
if(MSVC)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};NSIS)
endif(MSVC)
if(APPLE)
    set (CPACK_GENERATOR ${CPACK_GENERATOR};PackageMaker)
endif(APPLE)
#set (CPACK_OUTPUT_CONFIG_FILE)
#set (CPACK_PACKAGE_EXECUTABLES)
#set (CPACK_STRIP_FILES)
set (CPACK_SOURCE_PACKAGE_FILE_NAME "nexus-source-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}")
#set (CPACK_SOURCE_STRIP_FILES)
#set (CPACK_SOURCE_GENERATOR)
#set (CPACK_SOURCE_OUTPUT_CONFIG_FILE)
#set (CPACK_SOURCE_IGNORE_FILES)
set(CPACK_RPM_COMPONENT_INSTALL ON)


cpack_add_component(Runtime
                    DISPLAY_NAME "Binary Applications"
                    DESCRIPTION "Binary applications such as nxconvert, nxbrows etc..."
                    )

cpack_add_component(Documentation
                    DISPLAY_NAME "Documentation"
                    DESCRIPTION "Application Documentation, API and help files."
                    )

cpack_add_component(Development
                    DISPLAY_NAME "Development"
                    DESCRIPTION "Development libraies and headers."
                    )

cpack_add_component(Examples
                    DISPLAY_NAME "Examples"
                    DESCRIPTION "Code example files."
                    )