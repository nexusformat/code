C----------------------------------------------------------------------------
C NeXus - Neutron & X-ray Common Data Format
C  
C API Fortran Interface Definitions
C  
C $Id$
C
C Copyright (C) 1997, Freddie Akeroyd
C                     ISIS Facility, Rutherford Appleton Laboratory, UK
C  
C See NAPI.C for more details
C
C 97/7/30 - Initial Release
C 97/7/31 - Correct NXPUTATTR/NXGETATTR and make 'implicit none' clean
C 97/8/7  - Update interface
C----------------------------------------------------------------------------

C *** Version of NeXus interface - should be consistent with napi.h!
      CHARACTER*5 NEXUS_VERSION
      PARAMETER(NEXUS_VERSION='1.1.0')
C *** NXaccess enum - access modes for NXopen
      INTEGER NXACC_READ,NXACC_RDWR,NXACC_CREATE
      PARAMETER(NXACC_READ=1,NXACC_RDWR=3,NXACC_CREATE=4)
C *** NXHANDLESIZE should be the size of an INTEGER*4 array that is (at least)
C *** large enough to hold an NXhandle structure
      INTEGER NXHANDLESIZE
      PARAMETER(NXHANDLESIZE=600)
C *** NXLINKSIZE is (at least) the size of an INTEGER*4 array that can hold
C *** an NXlink structure: we'll assume 64bit alignment of structure members for safety
      INTEGER NXLINKSIZE
      PARAMETER(NXLINKSIZE=4)
C *** Possible NXstatus values - these are returned by all NX routines
      INTEGER NX_OK,NX_ERROR,NX_EOD
      PARAMETER(NX_OK=1,NX_ERROR=0,NX_EOD=-1)
C *** Maximum values defined in HDF standard
      INTEGER NX_MAXRANK,NX_MAXNAMELEN
      PARAMETER(NX_MAXRANK=32,NX_MAXNAMELEN=64)
C *** HDF datatypes used by Nexus - see hntdefs.h in HDF distribution
      INTEGER DFNT_FLOAT32,DFNT_FLOAT64,DFNT_INT8,DFNT_UINT8,DFNT_INT16,
     +        DFNT_UINT16,DFNT_INT32,DFNT_UINT32,DFNT_UCHAR8,DFNT_CHAR8
      PARAMETER(DFNT_FLOAT32=5,DFNT_FLOAT64=6,DFNT_INT8=20,
     +          DFNT_UINT8=21,DFNT_INT16=22,DFNT_UINT16=23,
     +          DFNT_INT32=24,DFNT_UINT32=25,DFNT_UCHAR8=3,
     +          DFNT_CHAR8=4)
C *** NeXus names for HDF parameters
      INTEGER NX_FLOAT32,NX_FLOAT64,NX_INT8,NX_UINT8,NX_INT16,
     +        NX_UINT16,NX_INT32,NX_UINT32,NX_CHAR
      PARAMETER(NX_FLOAT32=5,NX_FLOAT64=6,NX_INT8=20,
     +          NX_UINT8=21,NX_INT16=22,NX_UINT16=23,
     +          NX_INT32=24,NX_UINT32=25,NX_CHAR=4)
      INTEGER NXOPEN, NXCLOSE, NXMAKEGROUP, NXOPENGROUP, NXCLOSEGROUP,
     +       NXMAKEDATA, NXOPENDATA, NXCLOSEDATA, NXGETDATA, 
     +       NXGETCHARDATA, NXGETSLAB, NXGETATTR, NXGETCHARATTR, 
     +       NXGETDIM, NXPUTDATA, NXPUTCHARDATA, NXPUTSLAB, 
     +       NXPUTATTR, NXPUTCHARATTR, NXPUTDIM, NXGETINFO, 
     +       NXGETNEXTENTRY, NXGETNEXTATTR, NXGETGROUPID, NXMAKELINK 
      EXTERNAL NXOPEN, NXCLOSE, NXMAKEGROUP, NXOPENGROUP, NXCLOSEGROUP,
     +       NXMAKEDATA, NXOPENDATA, NXCLOSEDATA, NXGETDATA, 
     +       NXGETCHARDATA, NXGETSLAB, NXGETATTR, NXGETCHARATTR, 
     +       NXGETDIM, NXPUTDATA, NXPUTCHARDATA, NXPUTSLAB, 
     +       NXPUTATTR, NXPUTCHARATTR, NXPUTDIM, NXGETINFO, 
     +       NXGETNEXTENTRY, NXGETNEXTATTR, NXGETGROUPID, NXMAKELINK 
