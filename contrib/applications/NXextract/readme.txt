NXEXTRACT
---------

'src' directory contains the source code for nxextract.

Files named base.cpp/.h,  membuf.cpp/.h, file.cpp/.h, date.cpp/.h are comming from a generic toolkit
developped at Soleil and used for various projects, so you may find some useless code.

nxfile.cpp/.h is a C++ layer other napi

Makefile is a rather simple make file without jpeg support. 
For adding jpeg support add __JPEG_SUPPORT__ definition add link with libjpeg

For linking you just need the common nexus dependencies (hdf, mxml,...) and libjpeg if __JPEG_SUPPORT__
is set.

!! I'm sorry There is no documentation yet. So please refer to the following examples.

'example' directory contains some extracting templates.

For testing purposes you can download the corresponding Nexus files from the Soleil web-site.
The rule is simple: use template_{i}.nxe to extract data from file_{i}.nxs

The nxextract syntax to use is:

 nxextract -t template_{i}.nxe -D dir={output directory} file_{i}.nxs

example: 

 nxextract -t template_1.nxe -D dir=/tmp file_1.nxs

- template_1.nxe is used to extract images recorded in NeXus in several NXdata groups. The output
is written is EDF format (ESRF Data Format). Using the NeXus file given as example (file_1.nxs) will
produce many warning because some metadata are missing. The result is 3 EDF files for this example.

- template_2.nxe is a template used to extract arbitrary scan data. It's not complete but match all
  our needs up today.

- template_3.nxe is used to extract images from 1D scan. The image are stacked in a single NXdata group
as a 3D dataset. For each image 2 files are written: 1 bmp file and 1 'raw data' file.
With file_3.nxs the result is 238 bmp and 238 raw data files.
