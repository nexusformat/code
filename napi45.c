#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "napi45.h"
#ifdef HDF5
#include "napi5.h"
#endif
#ifdef HDF4
#include "napi4.h"
#endif

  typedef struct {
        NXhandle *pNexusData;   
        NXstatus (*nxclose)(NXhandle* pHandle);
        NXstatus (*nxflush)(NXhandle* pHandle);
        NXstatus (*nxmakegroup) (NXhandle handle, CONSTCHAR *name, char* NXclass);
        NXstatus (*nxopengroup) (NXhandle handle, CONSTCHAR *name, char* NXclass);
        NXstatus (*nxclosegroup)(NXhandle handle);
        NXstatus (*nxmakedata) (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
        NXstatus (*nxcompmakedata) (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize);
        NXstatus (*nxcompress) (NXhandle handle, int compr_type);
        NXstatus (*nxopendata) (NXhandle handle, CONSTCHAR* label);
        NXstatus (*nxclosedata)(NXhandle handle);
        NXstatus (*nxputdata)(NXhandle handle, void* data);
        NXstatus (*nxputattr)(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
        NXstatus (*nxputslab)(NXhandle handle, void* data, int start[], int size[]);    
        NXstatus (*nxgetdataID)(NXhandle handle, NXlink* pLink);
        NXstatus (*nxmakelink)(NXhandle handle, NXlink* pLink);
        NXstatus (*nxgetdata)(NXhandle handle, void* data);
        NXstatus (*nxgetinfo)(NXhandle handle, int* rank, int dimension[], int* datatype);
        NXstatus (*nxgetnextentry)(NXhandle handle, NXname name, NXname nxclass, int* datatype);
        NXstatus (*nxgetslab)(NXhandle handle, void* data, int start[], int size[]);
        NXstatus (*nxgetnextattr)(NXhandle handle, NXname pName, int *iLength, int *iType);
        NXstatus (*nxgetattr)(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
        NXstatus (*nxgetattrinfo)(NXhandle handle, int* no_items);
        NXstatus (*nxgetgroupID)(NXhandle handle, NXlink* pLink);
        NXstatus (*nxgetgroupinfo)(NXhandle handle, int* no_items, NXname name, NXname nxclass);
        NXstatus (*nxinitgroupdir)(NXhandle handle);
        NXstatus (*nxinitattrdir)(NXhandle handle);
  } NexusFunction, *pNexusFunction;
  
   /*---------------------------------------------------------------------*/

  static void NXNXNXReportError45(void *pData, char *string)
  {
    printf("%s \n",string);
  }
     
  /*---------------------------------------------------------------------*/

  void *NXpData45 = NULL;
  void (*NXIReportError45)(void *pData, char *string) = NXNXNXReportError45;

  /*---------------------------------------------------------------------*/

  void CALLING_STYLE NXMSetError45(void *pData, void (*NewError)(void *pD, char *text))
  {
    NXpData45 = pData;
    NXIReportError45 = NewError;
  }
  
  
  /* ---------------------------------------------------------------------- 
  
                          Definition of NeXus API

   ---------------------------------------------------------------------*/


  NXstatus CALLING_STYLE NXopen(CONSTCHAR *filename, NXaccess am, NXhandle *gHandle)
  {
    int hdf_type=0;
    int iRet=0;
    NXhandle hdf5_handle;
    NXhandle hdf4_handle;
    pNexusFunction fHandle;
        
    fHandle = (pNexusFunction)malloc(sizeof(NexusFunction));
    if (fHandle == NULL) {
      NXIReportError45 (NXpData45,"ERROR: no memory to create Function structure");
      return NX_ERROR;
    }
    if (am==NXACC_CREATE) {
      /* HDF4 will be used ! */
      hdf_type=1;
    } else if (am==NXACC_CREATE5) {
      /* HDF5 will be used ! */
      hdf_type=2;   
    } else {
      /* check file type hdf4/hdf5 for reading */
#ifdef HDF5
      iRet=H5Fis_hdf5((const char*)filename);
#endif
      if (iRet>0) {
        hdf_type=2; 
      } else {
#ifdef HDF4
        iRet=Hishdf(filename);
#endif
        if (iRet>0) {
          hdf_type=1; 
        } 
      }
    }
    if (hdf_type==1) {
      /* HDF4 type */
#ifdef HDF4
      NX4open(filename,am,&hdf4_handle);
      fHandle->pNexusData=hdf4_handle;
      fHandle->nxclose=NX4close;
      fHandle->nxflush=NX4flush;
      fHandle->nxmakegroup=NX4makegroup;
      fHandle->nxopengroup=NX4opengroup;
      fHandle->nxclosegroup=NX4closegroup;
      fHandle->nxmakedata=NX4makedata;
      fHandle->nxcompmakedata=NX4compmakedata;
      fHandle->nxcompress=NX4compress;
      fHandle->nxopendata=NX4opendata;
      fHandle->nxclosedata=NX4closedata;
      fHandle->nxputdata=NX4putdata;
      fHandle->nxputattr=NX4putattr;
      fHandle->nxputslab=NX4putslab;    
      fHandle->nxgetdataID=NX4getdataID;
      fHandle->nxmakelink=NX4makelink;
      fHandle->nxgetdata=NX4getdata;
      fHandle->nxgetinfo=NX4getinfo;
      fHandle->nxgetnextentry=NX4getnextentry;
      fHandle->nxgetslab=NX4getslab;
      fHandle->nxgetnextattr=NX4getnextattr;
      fHandle->nxgetattr=NX4getattr;
      fHandle->nxgetattrinfo=NX4getattrinfo;
      fHandle->nxgetgroupID=NX4getgroupID;
      fHandle->nxgetgroupinfo=NX4getgroupinfo;
      fHandle->nxinitgroupdir=NX4initgroupdir;
      fHandle->nxinitattrdir=NX4initattrdir;
#endif
      *gHandle = fHandle;
      return NX_OK; 
    } else if (hdf_type==2) {
      /* HDF5 type */
#ifdef HDF5
      NX5open(filename,am,&hdf5_handle);
      fHandle->pNexusData=hdf5_handle;
      fHandle->nxclose=NX5close;
      fHandle->nxflush=NX5flush;
      fHandle->nxmakegroup=NX5makegroup;
      fHandle->nxopengroup=NX5opengroup;
      fHandle->nxclosegroup=NX5closegroup;
      fHandle->nxmakedata=NX5makedata;
      fHandle->nxcompmakedata=NX5compmakedata;
      fHandle->nxopendata=NX5opendata;
      fHandle->nxclosedata=NX5closedata;
      fHandle->nxputdata=NX5putdata;
      fHandle->nxputattr=NX5putattr;
      fHandle->nxputslab=NX5putslab;    
      fHandle->nxgetdataID=NX5getdataID;
      fHandle->nxmakelink=NX5makelink;
      fHandle->nxgetdata=NX5getdata;
      fHandle->nxgetinfo=NX5getinfo;
      fHandle->nxgetnextentry=NX5getnextentry;
      fHandle->nxgetslab=NX5getslab;
      fHandle->nxgetnextattr=NX5getnextattr;
      fHandle->nxgetattr=NX5getattr;
      fHandle->nxgetattrinfo=NX5getattrinfo;
      fHandle->nxgetgroupID=NX5getgroupID;
      fHandle->nxgetgroupinfo=NX5getgroupinfo;
      fHandle->nxinitgroupdir=NX5initgroupdir;
      fHandle->nxinitattrdir=NX5initattrdir;
#endif      
      *gHandle = fHandle;
      return NX_OK;
    } else {
      NXIReportError45 (NXpData45,"ERROR: Format couldn't be handled!");
      *gHandle = NULL;
      return NX_ERROR;
    }
  }

/* ------------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXclose (NXhandle *fid)
  { 
    NXhandle hfil; 
    int status;
   
    pNexusFunction pFunc=NULL;
    pFunc = (pNexusFunction)*fid;
    hfil = pFunc->pNexusData;
    status =  pFunc->nxclose(&hfil);
    pFunc->pNexusData = hfil;
    free(pFunc); 
    return status;   
  }

  /*-----------------------------------------------------------------------*/   

  NXstatus CALLING_STYLE NXmakegroup (NXhandle fid, CONSTCHAR *name, char *nxclass) 
  {
     pNexusFunction pFunc = (pNexusFunction)fid;
     return pFunc->nxmakegroup(pFunc->pNexusData, name, nxclass);   
  }

  /*------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXopengroup (NXhandle fid, CONSTCHAR *name, char *nxclass)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxopengroup(pFunc->pNexusData, name, nxclass);  
  } 

  /* ------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXclosegroup (NXhandle fid)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxclosegroup(pFunc->pNexusData);  
  }   

  /* --------------------------------------------------------------------- */
  
  NXstatus CALLING_STYLE NXmakedata (NXhandle fid, CONSTCHAR *name, int datatype, 
                                  int rank, int dimensions[])
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxmakedata(pFunc->pNexusData, name, datatype, rank, dimensions); 
  }


 /* --------------------------------------------------------------------- */
  
  NXstatus CALLING_STYLE NXcompmakedata (NXhandle fid, CONSTCHAR *name, int datatype, 
                           int rank, int dimensions[],int compress_type, int chunk_size)
  {
    pNexusFunction pFunc = (pNexusFunction)fid; 
    return pFunc->nxcompmakedata (pFunc->pNexusData, name, datatype, rank, dimensions, compress_type, chunk_size); 
  } 
  
 
  /* --------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXcompress (NXhandle fid, int compress_type)
  {
    pNexusFunction pFunc = (pNexusFunction)fid; 
    return pFunc->nxcompress (pFunc->pNexusData, compress_type); 
  }
  
 
  /* --------------------------------------------------------------------- */
  
  NXstatus CALLING_STYLE NXopendata (NXhandle fid, CONSTCHAR *name)
  {
    pNexusFunction pFunc = (pNexusFunction)fid; 
    return pFunc->nxopendata(pFunc->pNexusData, name); 
  } 


  /* ----------------------------------------------------------------- */
    
  NXstatus CALLING_STYLE NXclosedata (NXhandle fid)
  { 
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxclosedata(pFunc->pNexusData);
  }

  /* ------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXputdata (NXhandle fid, void *data)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxputdata(pFunc->pNexusData, data);
  }

  /* ------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXputattr (NXhandle fid, CONSTCHAR *name, void *data, 
                                  int datalen, int iType)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxputattr(pFunc->pNexusData, name, data, datalen, iType);
  }

  /* ------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXputslab (NXhandle fid, void *data, int iStart[], int iSize[])
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxputslab(pFunc->pNexusData, data, iStart, iSize);
  }

  /* ------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXgetdataID (NXhandle fid, NXlink* sRes)
  {  
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetdataID(pFunc->pNexusData, sRes);
  }


  /* ------------------------------------------------------------------- */

  NXstatus CALLING_STYLE NXmakelink (NXhandle fid, NXlink* sLink)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxmakelink(pFunc->pNexusData, sLink);
  }

  
  /*----------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXflush(NXhandle *pHandle)
  {
    NXhandle hfil; 
    int status;
   
    pNexusFunction pFunc=NULL;
    pFunc = (pNexusFunction)*pHandle;
    hfil = pFunc->pNexusData;
    status =  pFunc->nxflush(&hfil);
    pFunc->pNexusData = hfil;
    return status; 
  }


  /*-------------------------------------------------------------------------*/
  
  NXstatus CALLING_STYLE NXmalloc (void** data, int rank, int dimensions[], int datatype)
  {
    int i;
    size_t size = 1;
    *data = NULL;
    for(i=0; i<rank; i++)
    size *= dimensions[i];
    if ((datatype == NX_CHAR) || (datatype == NX_INT8) || (datatype == NX_UINT8)) {
        /* size is correct already */
      }
      else if ((datatype == NX_INT16) || (datatype == NX_UINT16)) {
      size *= 2;
      }    
      else if ((datatype == NX_INT32) || (datatype == NX_UINT32) || (datatype == NX_FLOAT32)) {
        size *= 4;
      }    
      else if (datatype == NX_FLOAT64) {
        size *= 8;
      }
      else {
        NXIReportError45 (NXpData45, "ERROR: NXmalloc - unknown data type in array");
        return NX_ERROR;
    }
    *data = (void*)malloc(size);
    return NX_OK;
  }
    
  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXfree (void** data)
  {
    if (data == NULL) {
       NXIReportError45 (NXpData45, "ERROR: passing NULL to NXfree");
       return NX_ERROR;
    }
    if (*data == NULL) {
       NXIReportError45 (NXpData45,"ERROR: passing already freed pointer to NXfree");
       return NX_ERROR;
    }
    free(*data);
    *data = NULL;
    return NX_OK;
  }

  /* --------------------------------------------------------------------- */
           
 
  NXstatus CALLING_STYLE NXgetnextentry (NXhandle fid, NXname name, NXname nxclass, int *datatype)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetnextentry(pFunc->pNexusData, name, nxclass, datatype);  
  }


  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetdata (NXhandle fid, void *data)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetdata(pFunc->pNexusData, data); 
  }
  
    
  /*-------------------------------------------------------------------------*/
 
  NXstatus CALLING_STYLE NXgetinfo (NXhandle fid, int *rank, int dimension[], int *iType)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetinfo(pFunc->pNexusData, rank, dimension, iType);
  }
  
  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetslab (NXhandle fid, void *data, int iStart[], int iSize[])
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetslab(pFunc->pNexusData, data, iStart, iSize);
  }
  
  
  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetnextattr (NXhandle fileid, NXname pName,
                                     int *iLength, int *iType)
  {
    pNexusFunction pFunc = (pNexusFunction)fileid;
    return pFunc->nxgetnextattr(pFunc->pNexusData, pName, iLength, iType);  
  }
 

  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetattr (NXhandle fid, char *name, void *data, int* datalen, int* iType)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetattr(pFunc->pNexusData, name, data, datalen, iType);  
  }


  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetattrinfo (NXhandle fid, int *iN)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetattrinfo(pFunc->pNexusData, iN);  
  }


  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetgroupID (NXhandle fileid, NXlink* sRes)
  {
    pNexusFunction pFunc = (pNexusFunction)fileid;
    return pFunc->nxgetgroupID(pFunc->pNexusData, sRes);  
  }

  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetgroupinfo (NXhandle fid, int *iN, NXname pName, NXname pClass)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxgetgroupinfo(pFunc->pNexusData, iN, pName, pClass);  
  }

  
  /*-------------------------------------------------------------------------*/
  
  NXstatus CALLING_STYLE NXinitattrdir (NXhandle fid)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxinitattrdir(pFunc->pNexusData);
  }
  
  
  /*-------------------------------------------------------------------------*/
 
  NXstatus CALLING_STYLE NXinitgroupdir (NXhandle fid)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxinitgroupdir(pFunc->pNexusData);
  }
  
  