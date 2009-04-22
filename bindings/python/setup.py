#!/usr/bin/env python
#
# This file is for a manual install of python on windows
# If python is already present it should be detected by
# the windows installer and the nxs directory copied to
# the python site-packages directory
#
# On Linux the Makefile will also install python, but not 
# using this file - instead it uses the automake python install bits
#
# To use this file type:
#
#        python setup.py install
#
from distutils.core import setup 
 
setup(name='NeXus', 
       version='4.2', 
       description='Python Bindings to libNeXus', 
       author='Paul Kienzle', 
       packages = ['nxs'], 
       ) 
