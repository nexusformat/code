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
$! Create library NEXUS.OLB (comment out either Fortran 77 or Fortran 90)
$!
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("NEXUS_LIBRARY=1","HDF4=1") NAPI.C
$!=======================================================================
$!
$!Fortran 77
$! FORTRAN NAPIF.F
$! LIBRARY/CREATE/OBJECT SYS$DISK:[]NEXUS.OLB NAPI,NAPIF
$!=======================================================================
$!Fortran 90
$ F90 NXmodule, NXUmodule
$ LIBRARY/CREATE/OBJECT SYS$DISK:[]NEXUS.OLB NAPI,NXmodule,NXUmodule
$!=======================================================================
$! Create C test programs
$!
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=("HDF4=1") NAPI4_TEST.C
$ LINK NAPI4_TEST,SYS$DISK:[]NEXUS.OLB/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$!
$! Create FORTRAN 77 test program
$!
$! FORTRAN NAPIF_TEST.F
$! LINK NAPIF_TEST,SYS$DISK:[]NEXUS.OLB/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$! Create Fortran 90 test program
$!
$ F90 NXtest, NXbrowse
$ LINK NXtest,SYS$DISK:[]NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$ LINK NXbrowse,SYS$DISK:[]NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
