#define NXSIGNATURE 959697

#include "mfhdf.h"
/* #include "napi4.c" */

/* 
 * HDF4 interface 
 */

extern  NXstatus  NX4open(CONSTCHAR *filename, NXaccess access_method, NXhandle* pHandle);
extern  NXstatus  NX4close(NXhandle* pHandle);
extern  NXstatus  NX4flush(NXhandle* pHandle);
  
extern  NXstatus  NX4makegroup (NXhandle handle, CONSTCHAR* Vgroup, CONSTCHAR* NXclass);
extern  NXstatus  NX4opengroup (NXhandle handle, CONSTCHAR* Vgroup, CONSTCHAR* NXclass);
extern  NXstatus  NX4closegroup(NXhandle handle);
  
extern  NXstatus  NX4makedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
extern  NXstatus  NX4compmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
extern  NXstatus  NX4compress (NXhandle handle, int compr_type);
extern  NXstatus  NX4opendata (NXhandle handle, CONSTCHAR* label);

extern  NXstatus  NX4closedata(NXhandle handle);
  
extern  NXstatus  NX4getdata(NXhandle handle, void* data);
extern  NXstatus  NX4getslab(NXhandle handle, void* data, int start[], int size[]);
extern  NXstatus  NX4getattr(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
  
extern  NXstatus  NX4putdata(NXhandle handle, void* data);
extern  NXstatus  NX4putslab(NXhandle handle, void* data, int start[], int size[]);
extern  NXstatus  NX4putattr(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
  
extern  NXstatus  NX4getinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
extern  NXstatus  NX4getgroupinfo(NXhandle handle, int* no_items, NXname name, NXname nxclass);
extern  NXstatus  NX4initgroupdir(NXhandle handle);
extern  NXstatus  NX4getnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);
extern  NXstatus  NX4getattrinfo(NXhandle handle, int* no_items);
extern  NXstatus  NX4initattrdir(NXhandle handle);
extern  NXstatus  NX4getnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
  
extern  NXstatus  NX4getgroupID(NXhandle handle, NXlink* pLink);
extern  NXstatus  NX4getdataID(NXhandle handle, NXlink* pLink);
extern  NXstatus  NX4makelink(NXhandle handle, NXlink* pLink);
void NX4assignFunctions(pNexusFunction fHandle);

  
