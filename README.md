NeXus - a common format for neutron and X-ray scattering data
=============================================================
<http://www.nexusformat.org/>

See COPYING file for licence information.

Installation Instructions
-------------------------

###Unix (including Mac OS X) source distribution###

If you have downloaded a source distribution (*e.g.* nexus-4.3.0.tar.gz) then you 
will already have a file called "configure" and the basic sequence to follow 
is:

        ./configure  # use sh ./configure    if this does not work
        make
        make check
        make install

This may not, however, build all the nexus utilities and binding you require, 
and may also locate the wrong HDF libraries and final installation directory. 
Thus you will probably need to pass some options to "configure" to control this.

General information about "configure" is located in the file called INSTALL.

To see a list of possible options type:

        ./configure --help

HDF libraries used by nexus are available via your system software package 
manager or can be downloaded from <http://www.hdfgroup.org/>.

A typical run of configure might be:

        ./configure --prefix=/usr/local/nexus --with-hdf5=/usr/local/hdf5

###Unix (including Mac OS X) git checkout###

As the "configure" file mentioned is a generated file, it is not included in the 
version control checkout. To create it run:

        sh autogen.sh

and then follow the same procedure as above. You may also want to look at the 
README.developers file.
