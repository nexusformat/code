/*---------------------------------------------------------------------------
                            Nexus API header file

   $Id$

   copyleft: Mark Koennecke, March 1997 at LNS,PSI, Switzerland
             Przemek Klosowski, U. of Maryland & NIST, USA       

   No warranties of any kind taken.
----------------------------------------------------------------------------*/
#ifndef NEXUSAPI
#define NEXUSAPI

#define NEXUS_VERSION	"1.0"		/* major.minor */

#include <mfhdf.h>

typedef enum {NXACC_READ = DFACC_READ, 
              NXACC_RDWR = DFACC_RDWR, 
              NXACC_CREATE = DFACC_CREATE } NXaccess;

typedef void* NXhandle;		/* really a pointer to a NexusFile structure */
typedef int NXstatus;
typedef char NXname[VGNAMELENMAX];

    typedef struct {
                    int32 iTag;
                    int32 iRef;
                   } NXlink;

#define NX_OK 1
#define NX_ERROR 0
#define NX_EOD -1

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

  This list is a edited version of the one found in the HDF header file
  htndefs.h. That source will always be the real reference, this is
  documented here for your convenience.
--------------------------------------------------------------------------*/ 

  
/* Map NeXus to HDF types */
#define NX_FLOAT32	DFNT_FLOAT32
#define NX_FLOAT64	DFNT_FLOAT64
#define NX_INT8		DFNT_INT8
#define NX_UINT8	DFNT_UINT8
#define NX_INT16	DFNT_INT16
#define NX_UINT16	DFNT_UINT16
#define NX_INT32	DFNT_INT32
#define NX_UINT32	DFNT_UINT32
#define NX_CHAR		DFNT_CHAR8

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 *  Now, we have the interface visible from FORTRAN and C. On UNIX system
 *  FORTRAN routines usually get an extra training 
 *  Do not mangle using "_" as f2c mangles _ containing and non _
 *  containing names differently
 *  We must also lowercase anything that is called from FORTRAN
 *  else we can't link
 */

#define CONCAT(__a,__b)	__a##__b	/* token concatenation */

/* 
 * Define a macro for FORTRAN name mangling _ pften we have to add an "_"
 */
#if defined(__VMS) || defined(__unix__) || defined(__MWERKS__)
#    if defined(__VMS) || defined(__MWERKS__)
#        define MANGLE(__arg)	__arg
#    else /* unix */
#        define MANGLE(__arg)	CONCAT(__arg,_)
#    endif

#    define NXopen 		MANGLE(nxiopen)
#    define NXclose 		MANGLE(nxiclose)
#    define NXmakegroup 	MANGLE(nximakegroup)
#    define NXopengroup 	MANGLE(nxiopengroup)
#    define NXclosegroup 	MANGLE(nxiclosegroup)
#    define NXmakedata 		MANGLE(nximakedata)
#    define NXopendata 		MANGLE(nxiopendata)
#    define NXclosedata 	MANGLE(nxiclosedata)
#    define NXgetdata 		MANGLE(nxigetdata)
#    define NXgetslab 		MANGLE(nxigetslab)
#    define NXgetattr 		MANGLE(nxigetattr)
#    define NXgetdim 		MANGLE(nxigetdim)
#    define NXputdata 		MANGLE(nxiputdata)
#    define NXputslab 		MANGLE(nxiputslab)
#    define NXputattr 		MANGLE(nxiputattr)
#    define NXputdim 		MANGLE(nxiputdim)
#    define NXgetinfo 		MANGLE(nxigetinfo)
#    define NXgetnextentry 	MANGLE(nxigetnextentry)
#    define NXgetnextattr 	MANGLE(nxigetnextattr)
#    define NXgetgroupID 	MANGLE(nxigetgroupid)
#    define NXgetdataID 	MANGLE(nxigetdataid)
#    define NXmakelink 		MANGLE(nximakelink)
#    define NXmalloc 		MANGLE(nximalloc)
#    define NXfree 		MANGLE(nxifree)
/* FORTRAN helpers - for NeXus internal use only */
#    define NXfopen		MANGLE(nxifopen)
#    define NXfclose		MANGLE(nxifclose)
#    define NXfmakedata		MANGLE(nxifmakedata)
#    define NXfputattr		MANGLE(nxifputattr)
#elif defined(_WIN32)
#	define NXopen 			NXIOPEN_
#       define NXclose 			NXICLOSE_
#       define NXmakegroup 		NXIMAKEGROUP_
#       define NXopengroup 		NXIOPENGROUP_
#       define NXclosegroup 		NXICLOSEGROUP_
#       define NXmakedata 		NXIMAKEDATA_
#       define NXopendata 		NXIOPENDATA_
#       define NXclosedata 		NXICLOSEDATA_
#       define NXgetdata 		NXIGETDATA_
#       define NXgetslab 		NXIGETSLAB_
#       define NXgetattr 		NXIGETATTR_
#       define NXgetdim 		NXIGETDIM_
#       define NXputdata 		NXIPUTDATA_
#       define NXputslab 		NXIPUTSLAB_
#       define NXputattr 		NXIPUTATTR_
#       define NXputdim 		NXIPUTDIM_
#       define NXgetinfo 		NXIGETINFO_
#       define NXgetnextentry 		NXIGETNEXTENTRY_
#       define NXgetnextattr 		NXIGETNEXTATTR_
#       define NXgetgroupID 		NXIGETGROUPID_
#       define NXgetdataID 		NXIGETDATAID_
#       define NXmakelink 		NXIMAKELINK_
#       define NXmalloc 		NXIMALLOC_
#       define NXfree 			NXIFREE_
/* FORTRAN helpers - for NeXus internal use only */
#	define NXfopen 			NXIFOPEN_
#	define NXfclose			NXIFCLOSE_
#    	define NXfmakedata		NXIFMAKEDATA_
#    	define NXfputattr		NXIFPUTATTR_
#else
#   error Cannot compile - unknown operating system
#endif

/* 
 * Standard interface 
 */
  NXstatus  NXopen(char * filename, NXaccess access_method, NXhandle* pHandle);
  NXstatus  NXclose(NXhandle* pHandle);
  
  NXstatus  NXmakegroup (NXhandle handle, char* Vgroup, char* NXclass);
  NXstatus  NXopengroup (NXhandle handle, char* Vgroup, char* NXclass);
  NXstatus  NXclosegroup(NXhandle handle);
  
  NXstatus  NXmakedata (NXhandle handle, char* label, int datatype, int rank, int dim[]);
  NXstatus  NXopendata (NXhandle handle, char* label);
  NXstatus  NXclosedata(NXhandle handle);
  
  NXstatus  NXgetdata(NXhandle handle, void* data);
  NXstatus  NXgetslab(NXhandle handle, void* data, int start[], int size[]);
  NXstatus  NXgetattr(NXhandle handle, char* name, char* data, int* iDataLen, int* iType);
  
  NXstatus  NXputdata(NXhandle handle, void* data);
  NXstatus  NXputslab(NXhandle handle, void* data, int start[], int size[]);
  NXstatus  NXputattr(NXhandle handle, char* name, void* data, int iDataLen, int iType);
  
  NXstatus  NXgetinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
  NXstatus  NXgetnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);
  NXstatus  NXgetnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
  
  NXstatus  NXgetgroupID(NXhandle handle, NXlink* pLink);
  NXstatus  NXgetdataID(NXhandle handle, NXlink* pLink);
  NXstatus  NXmakelink(NXhandle handle, NXlink* pLink);
  
/* 
 * Helper interface 
 */

  NXstatus NXmalloc(void** data, int rank, int dimensions[], int datatype);
  NXstatus NXfree(void** data);

/*-----------------------------------------------------------------------
    A non Nexus standard function to set an error handler 
*/
  void NXMSetError(void *pData, void (*ErrFunc)(void *pD, char *text));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NEXUSAPI */
