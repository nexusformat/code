=========================
Installation Instructions
=========================

Requirements
============

In order to build the Nexus C API the following software has to be installed on
the build system 

* C compiler 
* CMake >= 2.8.8 
* HDF5 libraries and header files 
* Optional: HDF4 libraries and header files
* Optional: MXML libraries and header files
* doxygen: for building the API documentation


Building the code on Linux
==========================


Enabling a physical file formats
--------------------------------

By default the NAPI is only built with HDF5 support. You can explicit switch on
a particular file format by defining the appropriate CMake variable during
configuration

* ``-DENABLE_HDF5=1`` for HDF5
* ``-DENABLE_HDF4=1`` for HDF4
* and ``-DENABLE_MXML=1`` for MXML

CMake tries then to figure out the locations of the required library binaries
and header files. This should work if the library provides a ``pkg-config`` file
and/or is installed in one of the systems default locations. 

If your library is not in a default location you have basically two options. 

1. if the library is installed with a ``pkg-config`` file you can add the path 
   to this file to your ``PKG_CONFIG_PATH`` environment variable. 
2. define CMake variables during configuration that point to the 
   appropriate location. 

For the second option the following CMake variables are available 

=================  ========================================
CMake variable     Content
=================  ========================================
HDF5_INCLUDE_DIRS  location of HDF5 header files
HDF5_LIBRARY_DIRS  location of HDF5 runtime libraries
HDF4_INCLUDE_DIRS  location of the HDF4 header files
HDF4_LIBRARY_DIRS  location of the HDF4 runtime libraries
MXML_INCLUDE_DIRS  location of the MXML header files
MXML_LIBRARY_DIRS  location of the MXML runtime libraries
=================  =======================================


Enable language bindings
------------------------

The library provides bindings for C++, Fortran 77, and Fortran 90. To enable
them set the following variables to one during code configuration

================ ===============================
CMAKE variable   language bindings
================ ===============================
ENABLE_CXX       build with C++ bindings
ENABLE_FORTRAN77 build with Fortran 77 bindings
ENABLE_FORTRAN90 build with Fortran 90 bindings
================ ===============================

Enable applications
-------------------

Aside with the C-library the NAPI source distribution ships a couple of command
line programs to work with NeXus files. These programs are not built by
default. In order to include them in the build the ``ENABLE_APPS`` variable
must be set to ``ON``. 

===============  =======================================================
Program          Description
===============  =======================================================
``nxbrowse``     browse a NeXus file
``nxdir``        list the contents of a NeXus file
``nxconvert``    convert a NeXus file to whatever?
``nxtraverse``   no idea what this is good for
``nxdump``      
``nxingest``
``nxsummary``
``nxtranslate``
===============  =======================================================

Building the distribution with these utility applications pulls in some
additional build requirements. These are

* ``libreadline``
* ``libtermcap``
* ``libhistory`` (most probably provided by the ``libreadline`` package)
* ``libxml2``

As one cannot select an individual program to be included in the build, all
these build dependencies must be satisified when ``ENABLE_APPS`` is set to
``ON`` in order for the build to succeed.
