# This program is public domain
"""
Python NeXus interface.

NeXus_ is a common data format for neutron, Xray and muon science.
The files contain multidimensional data elements grouped into a
hierarchical structure.  The data sets are self-describing, with
a description of the instrument configuration including the units
used as well as the data measured.

The NeXus file interface requires binary libraries to read the
underlying HDF_ files, and 

Please be sure that the NeXus libraries are installed.  The
NeXus format site contains binaries for some platforms.  Details
of where the nxs package searches for the libraries are in `nxs.napi`.

Example
=======

First we need to load the file structure::

    import nxs
    f = nxs.read('file.nxs')

We can examine the file structure using a number of commands::

    f.nxattrs             # Shows file name, date, user, and NeXus version
    f.NXentry             # Shows the list of datasets in the file
    f.NXentry[0].nxdir()  # Lists the fields in the first entry

Some files can even be plotted automatically::

    f.NXentry[0].data.nxplot()

We can create a copy of the file using write::

    nxs.write('copy.nxs', tree)

For a complete description of the features available in this tree view
of the NeXus data file, see `nxs.tree`.

NeXus API
=========

When converting code to python from other languages you do not
necessarily want to redo the file handling code.  The `nxs.napi`
package provides an interface which more closely follows the
NeXus application programming interface (NAPI_).

.. _Nexus: http://www.nexusformat.org/Introduction
.. _NAPI:  http://www.nexusformat.org/Application_Program_Interface
.. _HDF:   http://www.hdfgroup.org
"""

from nxs.napi import *
from nxs.tree import *

