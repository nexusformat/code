C----------------------------------------------------------------------------
C NeXus - Neutron & X-ray Common Data Format
C  
C API Fortran Interface Definitions
C  
C Copyright (C) 1997, Freddie Akeroyd
C                     ISIS Facility, Rutherford Appleton Laboratory
C  
C See NAPI.C for details
C
C 97/7/30 - Initial Release
C 97/7/31 - Correct NXPUTATTR/NXGETATTR and make 'implicit none' clean
C----------------------------------------------------------------------------

C *** NXaccess enum - access modes for NXopen
      INTEGER NXACC_READ,NXACC_RDWR,NXACC_CREATE
      PARAMETER(NXACC_READ=1,NXACC_RDWR=3,NXACC_CREATE=7)
C *** NMAXSTACK definition
      INTEGER NMAXSTACK
      PARAMETER(NMAXSTACK=50)
C *** NXFILESIZE should be the size of an INTEGER*4 array that is (at least)
C *** large enough to hold a NexusFile structure
C *** To be safe, we will assume all structure elements are padded 
C *** to be 64bit aligned
      INTEGER NXFILESIZE
      PARAMETER(NXFILESIZE=2*(7+5*(NMAXSTACK+1)))
C *** NXLINKSIZE is (at least) the size of an INTEGER*4 array that can hold
C *** an NXlink structure: we'll again assume 64bit alignment for safety
      INTEGER NXLINKSIZE
      PARAMETER(NXLINKSIZE=4)
C *** Possible NXstatus values - these are returned by all NX routines
      INTEGER NX_OK,NX_ERROR,NX_EOD
      PARAMETER(NX_OK=1,NX_ERROR=0,NX_EOD=-1)
C *** HDF datatypes used by Nexus - see hntdefs.h in HDF distribution
      INTEGER DFNT_FLOAT32,DFNT_FLOAT64,DFNT_INT8,DFNT_UINT8,DFNT_INT16,
     +        DFNT_UINT16,DFNT_INT32,DFNT_UINT32
      PARAMETER(DFNT_FLOAT32=5,DFNT_FLOAT64=6,DFNT_INT8=20,
     +          DFNT_UINT8=21,DFNT_INT16=22,DFNT_UINT16=23,
     +          DFNT_INT32=24,DFNT_UINT32=25)
      INTEGER NXOPEN, NXCLOSE, NXMAKEGROUP, NXOPENGROUP, NXCLOSEGROUP,
     +       NXMAKEDATA, NXOPENDATA, NXCLOSEDATA, NXGETDATA, NXGETSLAB,
     +       NXGETATTR, NXGETDIM, NXPUTDATA, NXPUTSLAB, NXPUTATTR,
     +       NXPUTDIM, NXGETINFO, NXGETNEXTENTRY, NXGETNEXTATTR,
     +       NXGETGROUPID, NXMAKELINK 
      EXTERNAL NXOPEN, NXCLOSE, NXMAKEGROUP, NXOPENGROUP, NXCLOSEGROUP,
     +       NXMAKEDATA, NXOPENDATA, NXCLOSEDATA, NXGETDATA, NXGETSLAB,
     +       NXGETATTR, NXGETDIM, NXPUTDATA, NXPUTSLAB, NXPUTATTR,
     +       NXPUTDIM, NXGETINFO, NXGETNEXTENTRY, NXGETNEXTATTR,
     +       NXGETGROUPID, NXMAKELINK 
