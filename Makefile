#
# $Id: Makefile,v 2.00 2001/27/07 11:00:02
# Makefile for building all NEXUS libraries
#


default :
	@ echo "Possible targets are: lib4 lib5 lib45"


lib4: 
	make -f makefile_hdf4 

lib5: 
	make -f makefile_hdf5	

lib45: 
	make -f makefile_hdf45	


