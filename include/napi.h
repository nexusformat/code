/*---------------------------------------------------------------------------
                            Nexus API header file

   copyleft: Mark Koennecke, March 1997 at LNS,PSI, Switzerland
             Przemek Klosowski, U. of Maryland & NIST, USA       

   No warranties of any kind taken.
----------------------------------------------------------------------------*/
#ifndef NEXUSAPI
#define NEXUSAPI

#include <mfhdf.h>

typedef enum {NXACC_READ = DFACC_READ, 
              NXACC_RDWR = DFACC_RDWR, 
              NXACC_CREATE = DFACC_CREATE } NXaccess;
typedef void *  NXhandle;
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

  This list is a edited version of the one found in the HDF header file
  htndefs.h. That source will always be the real reference, this is
  documented here for your convenience.
--------------------------------------------------------------------------*/ 

  
  NXhandle  NXopen(char * filename, NXaccess access_method);
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
  NXstatus  NXgetnextentry(NXhandle fileid, NXname name, NXname class, int* datatype);
  NXstatus  NXgetnextattr(NXhandle fileid, NXname pName, int *iLength, int *iType);
  
  
  NXlink      NXgetgroupID(NXhandle fileid);
  NXlink      NXgetdataID(NXhandle fileid);
  NXstatus    NXmakelink(NXhandle fileid, NXlink link);
  
/*-----------------------------------------------------------------------
    A non Nexus standard function to set an error handler 
*/
  void NXMSetError(void *pData, void (*ErrFunc)(void *pD, char *text));

#endif


