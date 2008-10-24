#!/usr/bin/env python

from distutils.core import setup

setup(name='NeXus',
      version='4.0',
      description='Python Bindings to libNeXus',
      author='Paul Kienzle',
      packages = ['nxs'],
      package_dir = {'nxs': ''},
      )
