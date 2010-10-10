# This program is public domain
"""@package nxs
Python NeXus interface.

NeXus_ is a common data format for neutron, Xray and muon science.
The files contain multidimensional data elements grouped into a
hierarchical structure.  The data sets are self-describing, with
a description of the instrument configuration including the units
used as well as the data measured.

The NeXus file interface requires compiled libraries to read the
underlying HDF_ files.  Binary packages are available for some
platforms from the NeXus site.  Details of where the nxs package
searches for the libraries are recorded in `nxs.napi`.
"""

## @mainpage NeXus Python Documentation
## See nxs.napi on the Packages tab

from nxs.napi import *
from nxs.tree import *
