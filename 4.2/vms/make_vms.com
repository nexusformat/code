$!
$! $Id$
$!
$! Build command file for VMS
$!
$! Freddie Akeroyd, ISIS facility, CCLRC Rutherford Appleton Laboratory
$!
$! Choose either the Fortran 77 or Fortran 90 versions 
$! by commenting out the alternative (i.e. start the line with $!)
$!
$ SET VERIFY
$ DEFINE HDF_ROOT AXPLIB$DISK:[4_1R3_ALPHAVMS7_1.]   ! where you installed HDF, trailing "." is needed
$!=======================================================================
$! Create library NEXUS.OLB and/or NEXUS90.OLB 
$! (comment out Fortran 77 or Fortran 90 versions if necessary)
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("NEXUS_LIBRARY=1","HDF4=1") NAPI.C
$!=======================================================================
$!Fortran 77 Library
$ FORTRAN NAPIF.F
$ LIBRARY/CREATE/OBJECT SYS$DISK:[]NEXUS.OLB NAPI,NAPIF
$!=======================================================================
$!Fortran 90 Library
$ F90 NXmodule, NXUmodule
$ LIBRARY/CREATE/OBJECT SYS$DISK:[]NEXUS90.OLB NAPI,NXmodule,NXUmodule
$!=======================================================================
$! Compile and link C test programs
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("HDF4=1") NAPI4_TEST.C
$ LINK NAPI4_TEST,SYS$DISK:[]NEXUS.OLB/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$! Compile and link FORTRAN 77 test program
$ FORTRAN NAPIF_TEST.F
$ LINK NAPIF_TEST,SYS$DISK:[]NEXUS.OLB/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$! Compile and link Fortran 90 test program
$ F90 NXtest
$ LINK NXtest,SYS$DISK:[]NEXUS90/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$! Compile and link NeXus utilities
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("HDF4=1") NXbrowse
$ LINK NXbrowse,SYS$DISK:[]NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("HDF4=1") NXtoXML
$ LINK NXtoXML,SYS$DISK:[]NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("HDF4=1") NXtoDTD
$ LINK NXtoDTD,SYS$DISK:[]NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
