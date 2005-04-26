#define NX5SIGNATURE 959695

#include <hdf5.h>

/* HDF5 interface */

NX_EXTERNAL  NXstatus CALLING_STYLE NX5open(CONSTCHAR *filename, NXaccess access_method, NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5close(NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5flush(NXhandle* pHandle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX5makegroup (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5opengroup (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5closegroup(NXhandle handle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NX5makedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5compmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5compress (NXhandle handle, int compr_type);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5opendata (NXhandle handle, CONSTCHAR* label);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5closedata(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5putdata(NXhandle handle, void* data);

NX_EXTERNAL  NXstatus CALLING_STYLE NX5putattr(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5putslab(NXhandle handle, void* data, int start[], int size[]);    

NX_EXTERNAL  NXstatus CALLING_STYLE NX5getdataID(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5makelink(NXhandle handle, NXlink* pLink);

NX_EXTERNAL  NXstatus CALLING_STYLE NX5getdata(NXhandle handle, void* data);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);

NX_EXTERNAL  NXstatus CALLING_STYLE NX5getslab(NXhandle handle, void* data, int start[], int size[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getattr(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getattrinfo(NXhandle handle, int* no_items);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getgroupID(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5getgroupinfo(NXhandle handle, int* no_items, NXname name, NXname nxclass);

NX_EXTERNAL  NXstatus CALLING_STYLE NX5initgroupdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NX5initattrdir(NXhandle handle);

void NX5assignFunctions(pNexusFunction fHandle);

herr_t nxgroup_info(hid_t loc_id, const char *name, void *op_data);
herr_t attr_info(hid_t loc_id, const char *name, void *opdata);
herr_t group_info(hid_t loc_id, const char *name, void *opdata);

