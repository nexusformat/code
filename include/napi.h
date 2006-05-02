/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface Header File
  
  Copyright (C) 2000-2005 Mark Koennecke, Uwe Filges
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
  For further information, see <http://www.neutron.anl.gov/NeXus/>
  
  $Id$

 ----------------------------------------------------------------------------*/
  
#ifndef NEXUSAPI
#define NEXUSAPI

/* NeXus HDF45 */
#define NEXUS_VERSION   "3.9.0"                /* major.minor.patch */

#define CONSTCHAR       const char

#ifdef _WIN32
//#define snprintf nxisnprintf
extern int nxisnprintf(char* buffer, int len, const char* format, ... );
#endif /* _WIN32 */

typedef void* NXhandle;         /* really a pointer to a NexusFile structure */
typedef int NXstatus;
typedef char NXname[128];

/* 
 * Any new NXaccess options should be numbered in 2^n format 
 * (8, 16, 32, etc) so that they can be bit masked and tested easily.
 *
 * To test older non bit masked options (values below 8) use e.g.
 *
 *       if ( (mode & NXACCMASK_REMOVEFLAGS) == NXACC_CREATE )
 *
 * To test new (>=8) options just use normal bit masking e.g.
 * 
 *       if ( mode & NXACC_NOSTRIP )
 *
 */
#define NXACCMASK_REMOVEFLAGS (0x7) /* bit mask to remove higher flag options */

typedef enum {NXACC_READ=1, NXACC_RDWR=2, NXACC_CREATE=3, NXACC_CREATE4=4, 
	      NXACC_CREATE5=5, NXACC_CREATEXML=6, NXACC_NOSTRIP=128} NXaccess;

typedef struct {
                char *iname;
                int   type;
               }info_type, *pinfo;  
 
#define NX_OK 1
#define NX_ERROR 0
#define NX_EOD -1

#define NX_UNLIMITED -1

#define NX_MAXRANK 32
#define NX_MAXNAMELEN 64

/*-------------------------------------------------------------------------
                HDF Datatype values for datatype parameters 
                       in the Nexus API

  NX_FLOAT32     32 bit float
  NX_FLOAT64     64 nit float == double
  NX_INT8        8 bit integer == byte
  NX_UINT8       8 bit unsigned integer
  NX_INT16       16 bit integer
  NX_UINT16      16 bit unsigned integer
  NX_INT32       32 bit integer
  NX_UINT32      32 bit unsigned integer
  NX_CHAR        8 bit character
  NX_BINARY      lump of binary data == NX_UINT8

--------------------------------------------------------------------------*/ 

/* Map NeXus to HDF types */
#define NX_FLOAT32   5
#define NX_FLOAT64   6
#define NX_INT8     20  
#define NX_UINT8    21
#define NX_BOOLEAN NX_UINT
#define NX_INT16    22  
#define NX_UINT16   23
#define NX_INT32    24
#define NX_UINT32   25
#define NX_CHAR      4
#define NX_BINARY   21

/* Map NeXus compression methods to HDF compression methods */
#define NX_COMP_NONE 100
#define NX_COMP_LZW 200
#define NX_COMP_RLE 300
#define NX_COMP_HUF 400  

typedef struct {
                long iTag;          /* HDF4 variable */
                long iRef;          /* HDF4 variable */
                char iTag5[1024];     /* HDF5 variable */ 
                char iRef5[1024];     /* HDF5 variable */
                char iRefd[1024];     /* HDF5 variable */
                char targetPath[1024]; /* XML path */
               } NXlink;

#define NXMAXSTACK 50

#define CONCAT(__a,__b) __a##__b        /* token concatenation */

#    ifdef __VMS
#        define MANGLE(__arg)	__arg 
#    else
#        define MANGLE(__arg)   CONCAT(__arg,_)
#    endif

#    define NXopen              MANGLE(nxiopen)
#    define NXclose             MANGLE(nxiclose)
#    define NXmakegroup         MANGLE(nximakegroup)
#    define NXopengroup         MANGLE(nxiopengroup)
#    define NXopenpath          MANGLE(nxiopenpath)
#    define NXopengrouppath     MANGLE(nxiopengrouppath)
#    define NXclosegroup        MANGLE(nxiclosegroup)
#    define NXmakedata          MANGLE(nximakedata)
#    define NXcompmakedata      MANGLE(nxicompmakedata)
#    define NXcompress          MANGLE(nxicompress)
#    define NXopendata          MANGLE(nxiopendata)
#    define NXclosedata         MANGLE(nxiclosedata)
#    define NXputdata           MANGLE(nxiputdata)
#    define NXputslab           MANGLE(nxiputslab)
#    define NXputattr           MANGLE(nxiputattr)
#    define NXgetdataID         MANGLE(nxigetdataid)
#    define NXmakelink          MANGLE(nximakelink)
#    define NXopensourcegroup   MANGLE(nxiopensourcegroup)
#    define NXmalloc            MANGLE(nximalloc)
#    define NXfree              MANGLE(nxifree)
#    define NXflush             MANGLE(nxiflush)

#    define NXgetinfo           MANGLE(nxigetinfo)
#    define NXgetnextentry      MANGLE(nxigetnextentry)
#    define NXgetdata           MANGLE(nxigetdata)

#    define NXgetslab           MANGLE(nxigetslab)
#    define NXgetnextattr       MANGLE(nxigetnextattr)
#    define NXgetattr           MANGLE(nxigetattr)
#    define NXgetattrinfo       MANGLE(nxigetattrinfo)
#    define NXgetgroupID        MANGLE(nxigetgroupid)
#    define NXgetgroupinfo      MANGLE(nxigetgroupinfo)
#    define NXsameID            MANGLE(nxisameid)
#    define NXinitgroupdir      MANGLE(nxiinitgroupdir)
#    define NXinitattrdir       MANGLE(nxiinitattrdir)
#    define NXsetnumberformat   MANGLE(nxisetnumberformat)
#    define NXsetcache          MANGLE(nxisetcache)
#    define NXinquirefile       MANGLE(nxiinquirefile)
#    define NXisexternalgroup   MANGLE(nxiisexternalgroup)
#    define NXlinkexternal      MANGLE(nxilinkexternal)

/* 
 * FORTRAN helpers - for NeXus internal use only 
 */
#    define NXfopen             MANGLE(nxifopen)
#    define NXfclose            MANGLE(nxifclose)
#    define NXfflush            MANGLE(nxifflush)
#    define NXfmakedata         MANGLE(nxifmakedata)
#    define NXfcompmakedata     MANGLE(nxifcompmakedata)
#    define NXfcompress         MANGLE(nxifcompress)
#    define NXfputattr          MANGLE(nxifputattr)


/* 
 * Standard interface 
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
extern  NXstatus  NXopen(CONSTCHAR * filename, NXaccess access_method, NXhandle* pHandle);
extern  NXstatus  NXclose(NXhandle* pHandle);
extern  NXstatus  NXflush(NXhandle* pHandle);
  
extern  NXstatus  NXmakegroup (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
extern  NXstatus  NXopengroup (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
extern  NXstatus  NXopenpath (NXhandle handle, CONSTCHAR *path);
extern  NXstatus  NXopengrouppath (NXhandle handle, CONSTCHAR *path);

extern  NXstatus  NXclosegroup(NXhandle handle);
  
extern  NXstatus  NXmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
extern  NXstatus  NXcompmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
extern  NXstatus  NXcompress (NXhandle handle, int compr_type);
extern  NXstatus  NXopendata (NXhandle handle, CONSTCHAR* label);
extern  NXstatus  NXclosedata(NXhandle handle);
extern  NXstatus  NXputdata(NXhandle handle, void* data);

extern  NXstatus  NXputattr(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
extern  NXstatus  NXputslab(NXhandle handle, void* data, int start[], int size[]);    

extern  NXstatus  NXgetdataID(NXhandle handle, NXlink* pLink);
extern  NXstatus  NXmakelink(NXhandle handle, NXlink* pLink);
extern  NXstatus  NXopensourcegroup(NXhandle handle);

extern  NXstatus  NXgetdata(NXhandle handle, void* data);
extern  NXstatus  NXgetinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
extern  NXstatus  NXgetnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);

extern  NXstatus  NXgetslab(NXhandle handle, void* data, int start[], int size[]);
extern  NXstatus  NXgetnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
extern  NXstatus  NXgetattr(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
extern  NXstatus  NXgetattrinfo(NXhandle handle, int* no_items);
extern  NXstatus  NXgetgroupID(NXhandle handle, NXlink* pLink);
extern  NXstatus  NXgetgroupinfo(NXhandle handle, int* no_items, NXname name, NXname nxclass);
extern  NXstatus  NXsameID(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID);

extern  NXstatus  NXinitgroupdir(NXhandle handle);
extern  NXstatus  NXinitattrdir(NXhandle handle);
extern  NXstatus  NXsetnumberformat(NXhandle handle,
 						      int type, char *format);

extern  NXstatus  NXinquirefile(NXhandle handle, char *filename, int filenameBufferLength);
extern  NXstatus  NXisexternalgroup(NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass, char *url, int urlLen); 
extern  NXstatus  NXlinkexternal(NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass, CONSTCHAR *url);

extern  NXstatus  NXmalloc(void** data, int rank, int dimensions[], int datatype);
extern  NXstatus  NXfree(void** data);


/*-----------------------------------------------------------------------
    NAPI internals 
------------------------------------------------------------------------*/
typedef void (*ErrFunc)(void *data, char *text);
extern  void  NXMSetError(void *pData, ErrFunc);
extern ErrFunc NXMGetError();
extern void (*NXIReportError)(void *pData,char *text);
extern void *NXpData;
extern char *NXIformatNeXusTime();
extern  NXstatus  NXIprintlink(NXhandle fid, NXlink* link);

/*
  another special function for setting the default cache size for HDF-5
*/
extern  NXstatus  NXsetcache(long newVal);

  typedef struct {
        NXhandle *pNexusData;   
        int stripFlag;
        NXstatus ( *nxclose)(NXhandle* pHandle);
        NXstatus ( *nxflush)(NXhandle* pHandle);
        NXstatus ( *nxmakegroup) (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
        NXstatus ( *nxopengroup) (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
        NXstatus ( *nxclosegroup)(NXhandle handle);
        NXstatus ( *nxmakedata) (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
        NXstatus ( *nxcompmakedata) (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
        NXstatus ( *nxcompress) (NXhandle handle, int compr_type);
        NXstatus ( *nxopendata) (NXhandle handle, CONSTCHAR* label);
        NXstatus ( *nxclosedata)(NXhandle handle);
        NXstatus ( *nxputdata)(NXhandle handle, void* data);
        NXstatus ( *nxputattr)(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
        NXstatus ( *nxputslab)(NXhandle handle, void* data, int start[], int size[]);    
        NXstatus ( *nxgetdataID)(NXhandle handle, NXlink* pLink);
        NXstatus ( *nxmakelink)(NXhandle handle, NXlink* pLink);
        NXstatus ( *nxgetdata)(NXhandle handle, void* data);
        NXstatus ( *nxgetinfo)(NXhandle handle, int* rank, int dimension[], int* datatype);
        NXstatus ( *nxgetnextentry)(NXhandle handle, NXname name, NXname nxclass, int* datatype);
        NXstatus ( *nxgetslab)(NXhandle handle, void* data, int start[], int size[]);
        NXstatus ( *nxgetnextattr)(NXhandle handle, NXname pName, int *iLength, int *iType);
        NXstatus ( *nxgetattr)(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
        NXstatus ( *nxgetattrinfo)(NXhandle handle, int* no_items);
        NXstatus ( *nxgetgroupID)(NXhandle handle, NXlink* pLink);
        NXstatus ( *nxgetgroupinfo)(NXhandle handle, int* no_items, NXname name, NXname nxclass);
        NXstatus ( *nxsameID)(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID);
        NXstatus ( *nxinitgroupdir)(NXhandle handle);
        NXstatus ( *nxinitattrdir)(NXhandle handle);
        NXstatus ( *nxsetnumberformat)(NXhandle handle, int type, char *format);
        NXstatus ( *nxprintlink)(NXhandle handle, NXlink* link);
  } NexusFunction, *pNexusFunction;
  /*---------------------*/
  extern long nx_cacheSize;

/* FORTRAN internals */

  extern NXstatus  NXfopen(char * filename, NXaccess* am, 
					NexusFunction* pHandle);
  extern NXstatus  NXfclose (NexusFunction* pHandle);
  extern NXstatus  NXfputattr(NXhandle fid, char *name, void *data, 
                                   int *pDatalen, int *pIType);
  extern NXstatus  NXfcompress(NXhandle fid, int *compr_type);
  extern NXstatus  NXfcompmakedata(NXhandle fid, char *name, 
                int *pDatatype,
		int *pRank, int dimensions[],
                int *compression_type, int chunk[]);
  extern NXstatus  NXfmakedata(NXhandle fid, char *name, int *pDatatype,
		int *pRank, int dimensions[]);
  extern NXstatus  NXfflush(NexusFunction* pHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*NEXUSAPI*/

