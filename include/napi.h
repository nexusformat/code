/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  API Header File
  
  Copyright (C) 1997 Mark Koennecke, Przemek Klosowski 
  
  See NAPI.C for details
  
----------------------------------------------------------------------------*/
#ifndef NEXUSAPI
#define NEXUSAPI

#include <mfhdf.h>

typedef enum {NXACC_READ = DFACC_READ, 
              NXACC_RDWR = DFACC_RDWR, 
              NXACC_CREATE = DFACC_CREATE } NXaccess;

#define NXMAXSTACK 50
#define NXSIGNATURE 959697

  typedef struct __NexusFile {
    int iNXID;
    int32 iVID;
    int32 iSID;
    int32 iCurrentVG;
    int32 iCurrentSDS;
    struct iStack {
      int32 iVref;
      int iNDir;
      int iCurDir;
      int32 *iRefDir;
      int32 *iTagDir;
    } iStack[NXMAXSTACK];
    int iStackPtr;
    struct iStack iAtt;
    char iAccess[2];
  } NexusFile, *pNexusFile;

typedef pNexusFile NXhandle;
typedef int  NXstatus;
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

  This list is an edited version of the one found in the HDF header file
  htndefs.h. That source will always be the real reference, this is
  documented here for your convenience.
--------------------------------------------------------------------------*/ 

  
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 *  Now, we have the interface visible from FORTRAN and C. On UNIX system
 *  FORTRAN routines get an extra training 
 *  Do not mangle using "_" and f2c mangles _ containing and non _
 *  containing names differently
 *  We must also lowercase anything that is called from fortran
 */

#define CONCAT(__a,__b)	__a##__b	/* token concatenation */

/* 
 * Define a macro for FORTRAN name mangling _ often we have to add an "_"
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

  
  NXstatus  NXopen(char * filename, NXaccess access_method, NXhandle fileid);
  NXstatus  NXclose(NXhandle fileid);
  
  NXstatus  NXmakegroup (NXhandle fileid, char* Vgroup, char* NXclass);
  NXstatus  NXopengroup (NXhandle fileid, char* Vgroup, char* NXclass);
  NXstatus  NXclosegroup(NXhandle fileid);
  
  NXstatus  NXmakedata (NXhandle fileid, char* label, int datatype, int rank, int dim[]);
  NXstatus  NXopendata (NXhandle fileid, char* label);
  NXstatus  NXclosedata(NXhandle fileid);
  
  NXstatus  NXgetdata(NXhandle fileid, void* data);
  NXstatus  NXgetslab(NXhandle fileid, void* data, int start[], int size[]);
  NXstatus  NXgetattr(NXhandle fileid, char* name, char* data, int datalen);
  NXstatus  NXgetdim (NXhandle fileid, int dimension, void * data);
  
  NXstatus  NXputdata(NXhandle fileid, void* data);
  NXstatus  NXputslab(NXhandle fileid, void* data, int start[], int size[]);
  NXstatus  NXputattr(NXhandle fileid, char* name, char* data, int datalen);
  NXstatus  NXputdim (NXhandle fileid, int dimension, void * data);
  
  NXstatus  NXgetinfo     (NXhandle fileid, int* rank, int dimension[], int* datatype);
  NXstatus  NXgetnextentry(NXhandle fileid, NXname name, NXname nxclass, int* datatype);
  NXstatus  NXgetnextattr(NXhandle fileid, NXname pName, int *iLength, int *iType);
  
  NXstatus  NXgetgroupID(NXhandle fileid, NXlink* link);
  NXstatus  NXgetdataID(NXhandle fileid, NXlink* link);
  NXstatus  NXmakelink(NXhandle fileid, NXlink* link);
  
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
