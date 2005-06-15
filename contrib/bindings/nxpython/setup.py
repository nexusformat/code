#!/usr/bin/python
import os
from distutils.core import setup, Extension

hdf4macros = [('HDF4','1')]
hdf5macros = [('HDF5','1')]

hdf4_incdir = '/usr/include'
hdf5_incdir = '/usr/include'
nexus_incdir = '../../../include'
napi_incdir = '.' 
incdir_list = [hdf4_incdir, hdf5_incdir, nexus_incdir, napi_incdir]

hdf4_libdir = '/usr/lib'
hdf5_libdir = '/usr/lib'
nexus_libdir = '/usr/lib/'
libdir_list = [ hdf4_libdir, hdf5_libdir, nexus_libdir]

nexus_lib = "NeXus"
lib_list = [nexus_lib, 
            "z","jpeg",  
            "hdf5"]   

lib_list_hdf4 = [nexus_lib,
            "mfhdf","df","z","jpeg",
            "hdf4"]


if os.uname()[0] == 'Linux':
    lib_list.append('stdc++')

setup( name = "nxpython",
       version = "0.1",
       description = "Nexus extension module for Python",
       author = "Hartmut Gilde",
       author_email = "hartmut.gilde@frm2.tum.de",
       url = "http://www.frm2.tum.de",
       ext_modules = [Extension( "_nexus", ["napi_swig.i"] ,
                                 define_macros = hdf5macros,
                                 include_dirs = incdir_list,
                                 library_dirs = libdir_list,
                                 libraries = lib_list )],
       ) 

setup( name = "nxpython",
       version = "0.1",
       description = "Nexus extension module for Python",
       author = "Hartmut Gilde",
       author_email = "hartmut.gilde@frm2.tum.de",
       url = "http://www.frm2.tum.de",
		 py_modules = ['nexus']
       ) 

