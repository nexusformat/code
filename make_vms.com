$!
$! $Id$
$!
$! Build command file for VMS
$!
$! Choose either the Fortran 77 or Fortran 90 versions 
$! by commenting out the alternative (i.e. start the line with $!)
$!
$ SET VERIFY
$ DEFINE HDF_ROOT AXPLIB$DISK:[HDF.]   ! where you installed HDF, trailing "." is needed
$!=======================================================================
$! Create library NEXUS.OLB (comment out either Fortran 77 or Fortran 90)
$!
$ CC/INCLUDE=HDF_ROOT:[INCLUDE]/DEFINE=(NEXUS_LIBRARY=1) NAPI.C
$!=======================================================================
$!Fortran 77
$ FORTRAN NAPIF.F
$ LIBRARY/CREATE/OBJECT NEXUS NAPI,NAPIF
$!=======================================================================
$!Fortran 90
$! F90 NXmodule, NXUmodule
$! LIBRARY/CREATE/OBJECT NEXUS NAPI,NXmodule,NXUmodule
$!=======================================================================
$! Create C test programs
$!
$ CC/INCLUDE=HDF_ROOT:[INCLUDE] NAPI_TEST.C
$ LINK NAPI_TEST,NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$ CC/INCLUDE=HDF_ROOT:[INCLUDE] NAPI_TEST_READ.C
$ LINK NAPI_TEST_READ,NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$!
$! Create FORTRAN test program
$!
$ FORTRAN NAPIF_TEST.F
$ LINK NAPIF_TEST,NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$!=======================================================================
$! Create Fortran 90 test program
$!
$! F90 NXtest, NXbrowse
$! LINK NXtest,NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
$! LINK NXbrowse,NEXUS/LIB,HDF_ROOT:[LIB]MFHDF/LIB,DF/LIB,LIBZ/LIB,LIBJPEG/LIB
