Building NeXus with Visual Studio
---------------------------------

Setup
-----

You need to define two Windows environment variables
HDF4ROOT and HDF5ROOT to point to top of unpacked HDF distributions
e.g. 

    HDF4ROOT=c:\program files\hdf41r5 
    HDF5ROOT=C:\Program Files\HDF5-144

the HDF4 includes wilL then be in %HDFROOT%\include and
the HDF5 includes in %HDF5ROOT%\c\debug\include and %HDF5ROOT%\c\release\include 

All the visual studio project files make use of these variables and so should not
depend on the location HDF files are installed in.

Building
--------

Open nexus.dsw, and then select a project to build

nexus4, nexus5 and nexus45 - NeXus libraries with HDF4, HDF5, and both HDF4/HDF5 support
napi*_test                 - the various test programs 
nxbrowse                   - the nexus file browser

Freddie Akeroyd (Freddie.Akeroyd@rl.ac.uk)
ISIS Facility
Rutherford Appleton Laboratory
GB

$Id$