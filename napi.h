/*---------------------------------------------------------------------------
                            Nexus API header file

   copyleft: Mark Koennecke, March 1997 at LNS,PSI, Switzerland
             Przemek Klosowski, U. of Maryland & NIST, USA       

   No warranties of any kind taken.
----------------------------------------------------------------------------*/
#ifndef NEXUSAPI
#define NEXUSAPI

#define NEXUS_VERSION	"0.9"		/* major.minor */

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

  DFNT_FLOAT32     32 bit float
  DFNT_FLOAT64     64 nit float == double
  DFNT_INT8        8 bit integer ==char, byte
  DFNT_UINT8       8 bit unsigned integer
  DFNT_INT16       16 bit integer
  DFNT_UINT16      16 bit unsigned integer
  DFNT_INT32       32 bit integer
  DFNT_UINT32      32 bit unsigned integer

  This list is a edited version of the one found in the HDF header file
  htndefs.h. That source will always be the real reference, this is
  documented here for your convenience.
--------------------------------------------------------------------------*/ 

  
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
#if defined(__VMS)
#define MANGLE(__arg)	__arg
#elif defined(__unix__)
#define MANGLE(__arg)	CONCAT(__arg,_)
#else
#error Cannot compile - unknown operating system
#endif

#define NXopen 			MANGLE(nxiopen)
#define NXclose 		MANGLE(nxiclose)
#define NXmakegroup 		MANGLE(nximakegroup)
#define NXopengroup 		MANGLE(nxiopengroup)
#define NXclosegroup 		MANGLE(nxiclosegroup)
#define NXmakedata 		MANGLE(nximakedata)
#define NXopendata 		MANGLE(nxiopendata)
#define NXclosedata 		MANGLE(nxiclosedata)
#define NXgetdata 		MANGLE(nxigetdata)
#define NXgetslab 		MANGLE(nxigetslab)
#define NXgetattr 		MANGLE(nxigetattr)
#define NXgetdim 		MANGLE(nxigetdim)
#define NXputdata 		MANGLE(nxiputdata)
#define NXputslab 		MANGLE(nxiputslab)
#define NXputattr 		MANGLE(nxiputattr)
#define NXputdim 		MANGLE(nxiputdim)
#define NXgetinfo 		MANGLE(nxigetinfo)
#define NXgetnextentry 		MANGLE(nxigetnextentry)
#define NXgetnextattr 		MANGLE(nxigetnextattr)
#define NXgetgroupID 		MANGLE(nxigetgroupid)
#define NXgetdataID 		MANGLE(nxigetdataid)
#define NXmakelink 		MANGLE(nximakelink)
#define NXmalloc 		MANGLE(nximalloc)
#define NXfree 			MANGLE(nxifree)

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
