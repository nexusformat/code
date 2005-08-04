#!/usr/bin/python
import os
from distutils.core import setup, Extension

def_hdf4 = [('HDF4','1')]
def_hdf5 = [('HDF5','1')]
def_xml  = [('NXXML','1')]
def_all  = def_hdf4 + def_hdf5 + def_xml

hdf4_incdir = '/usr/include'
hdf5_incdir = '/usr/include'
xml_incdir  = '/usr/local/include'

nexus_incdir = '../../../include'
napi_incdir  = '.' 

hdf4_libdir = '/usr/lib'
hdf5_libdir = '/usr/lib'
xml_libdir  = '/usr/local/lib'

nexus_libdir = '/usr/lib/'

incdir_list = [hdf4_incdir, hdf5_incdir, xml_incdir, nexus_incdir, napi_incdir]
libdir_list = [hdf4_libdir, hdf5_libdir, xml_libdir, nexus_libdir]

nexus_lib = "NeXus"

lib_list_hdf5 = [nexus_lib, "hdf5", "jpeg", "z"]   
lib_list_hdf4 = [nexus_lib, "mfhdf", "df", "z", "jpeg"]
lib_list_xml  = [nexus_lib, "mxml", "xml2", "jpeg", "z"]

lib_list_all = lib_list_hdf5 + lib_list_hdf4 + lib_list_xml


if os.uname()[0] == 'Linux':
    lib_list_all.append('stdc++')

setup( name = "nxpython",
       version = "0.1",
       description = "Nexus extension module for Python",
       author = "Hartmut Gilde",
       author_email = "hartmut.gilde@frm2.tum.de",
       url = "http://www.frm2.tum.de",
       ext_modules = [Extension( "_nexus", ["napi_swig.i"] ,
                                 define_macros = def_all,
                                 include_dirs = incdir_list,
                                 library_dirs = libdir_list,
                                 libraries = lib_list_all)],
       ) 

setup( name = "nxpython",
       version = "0.1",
       description = "Nexus extension module for Python",
       author = "Hartmut Gilde",
       author_email = "hartmut.gilde@frm2.tum.de",
       url = "http://www.frm2.tum.de",
		 py_modules = ['nexus']
       ) 

