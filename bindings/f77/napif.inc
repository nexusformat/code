C------------------------------------------------------------------------------
C NeXus - Neutron & X-ray Common Data Format
C  
C Application Program Interface (Fortran 77) Header File
C
C Copyright (C) 1997-2004 Freddie Akeroyd, Mark Koennecke
C
C This library is free software; you can redistribute it and/or
C modify it under the terms of the GNU Lesser General Public
C License as published by the Free Software Foundation; either
C version 2 of the License, or (at your option) any later version.
C
C This library is distributed in the hope that it will be useful,
C but WITHOUT ANY WARRANTY; without even the implied warranty of
C MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
C Lesser General Public License for more details.
C
C You should have received a copy of the GNU Lesser General Public
C License along with this library; if not, write to the Free Software
C Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
C
C For further information, see <http://www.neutron.anl.gov/NeXus/>
C
C $Id$
C------------------------------------------------------------------------------

C *** Version of NeXus interface - should be consistent with napi.h!
      CHARACTER*5 NEXUS_VERSION
      PARAMETER(NEXUS_VERSION='3.0.0')
C *** NXaccess enum - access modes for NXopen
      INTEGER NXACC_READ,NXACC_RDWR,NXACC_CREATE, NXACC_CREATE5
      PARAMETER(NXACC_READ=1,NXACC_RDWR=2,NXACC_CREATE=3,
     +    NXACC_CREATE4=4,NXACC_CREATE5=5,NXACC_CREATEXML=6)
C *** NXHANDLESIZE should be the size of an INTEGER*4 array that is (at least)
C *** large enough to hold an NXhandle structure
      INTEGER NXHANDLESIZE
      PARAMETER(NXHANDLESIZE=600)
C *** NXLINKSIZE is (at least) the size of an INTEGER*4 array that can hold
C *** an NXlink structure: we'll assume 64bit alignment of structure members for safety
      INTEGER NXLINKSIZE
      PARAMETER(NXLINKSIZE=1028)
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
C**** NeXus compression schemes
      INTEGER NX_COMP_NONE, NX_COMP_LZW, NX_COMP_HUF, NX_COMP_RLE
      PARAMETER(NX_COMP_NONE=100,NX_COMP_LZW=200,NX_COMP_RLE=300,
     +          NX_COMP_HUF=400)
C**** NeXus Unlimited Dimension
      INTEGER NX_UNLIMITED
      PARAMETER (NX_UNLIMITED=-1)
      INTEGER NXOPEN, NXCLOSE, NXMAKEGROUP, NXOPENGROUP, NXCLOSEGROUP,
     +       NXMAKEDATA, NXOPENDATA, NXCLOSEDATA, NXGETDATA, 
     +       NXGETCHARDATA, NXGETSLAB, NXGETATTR, NXGETCHARATTR, 
     +       NXGETDIM, NXPUTDATA, NXPUTCHARDATA, NXPUTSLAB, 
     +       NXPUTATTR, NXPUTCHARATTR, NXPUTDIM, NXGETINFO, 
     +       NXGETNEXTENTRY, NXGETNEXTATTR, NXGETGROUPID, NXMAKELINK,
     +       NXGETGROUPINFO, NXINITGROUPDIR, NXGETATTRINFO, 
     +       NXINITATTRDIR, NXFLUSH, NXCOMPMAKEDATA, NXOPENPATH,
     +       NXSETNUMBERFORMAT
      LOGICAL NXSAMEID
      EXTERNAL NXOPEN, NXCLOSE, NXMAKEGROUP, NXOPENGROUP, NXCLOSEGROUP,
     +       NXMAKEDATA, NXOPENDATA, NXCLOSEDATA, NXGETDATA, 
     +       NXGETCHARDATA, NXGETSLAB, NXGETATTR, NXGETCHARATTR, 
     +       NXGETDIM, NXPUTDATA, NXPUTCHARDATA, NXPUTSLAB, 
     +       NXPUTATTR, NXPUTCHARATTR, NXPUTDIM, NXGETINFO, 
     +       NXGETNEXTENTRY, NXGETNEXTATTR, NXGETGROUPID, NXMAKELINK,
     +       NXGETGROUPINFO, NXINITGROUPDIR, NXGETATTRINFO, 
     +       NXINITATTRDIR, NXFLUSH, NXCOMPMAKEDATA, NXSAMEID, 
     +       NXOPENPATH,NXSETNUMBERFORMAT





