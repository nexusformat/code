#====================================================================
#  NeXus - Neutron & X-ray Common Data Format
#  
#  Makefile for building the C and Fortran API libraries
#  
#  Copyright (C) 2002 Mark Koennecke
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2 of the License, or (at your option) any later version.
# 
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
# 
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free 
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
#  MA  02111-1307  USA
#             
#  For further information, see <http://www.neutron.anl.gov/NeXus/>
#
# $Id$
#
#====================================================================

default :
	@ echo "Possible targets are: lib4 lib5 lib45 libf90"


lib4: 
	make -f makefile_hdf4 

lib5: 
	make -f makefile_hdf5	

lib45: 
	make -f makefile_hdf45

libf90:
	make -f makefile_f90

clean:
	- rm *.o
