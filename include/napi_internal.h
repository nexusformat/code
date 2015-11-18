/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface Header File
  
  Copyright (C) 2015 NeXus International Advisory Committee
  
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
 
  For further information, see <http://www.nexusformat.org>
  
 ----------------------------------------------------------------------------*/

#ifndef NEXUS_INTERNAL_API
#define NEXUS_INTERNAL_API

#include <napi.h>

/*-----------------------------------------------------------------------
    NAPI internals 
------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  /**
   * Retrieve information about the currently open dataset. In contrast to the main function below, 
   * this function does not try to find out about the size of strings properly. 
   * \param handle A NeXus file handle as initialized by NXopen.
   * \param rank A pointer to an integer which will be filled with the rank of 
   * the dataset.
   * \param dimension An array which will be initialized with the size of the dataset in any of its 
   * dimensions. The array must have at least the size of rank.
   * \param datatype A pointer to an integer which be set to the NeXus data type code for this dataset.
   * \return NX_OK on success, NX_ERROR in the case of an error.   
   * \ingroup c_metadata
   */
extern  NXstatus  NXgetrawinfo(NXhandle handle, int* rank, int dimension[], int* datatype);

 /**
  * @copydoc NXgetrawinfo
  */
extern  NXstatus  NXgetrawinfo64(NXhandle handle, int* rank, int64_t dimension[], int* datatype);

/** \typedef void (*ErrFunc)(void *data, char *text)
 * All NeXus error reporting happens through this special function, the 
 * ErrFunc. The NeXus-API allows to replace this error reporting function
 * through a user defined implementation. The default error function prints to stderr. User 
 * defined ones may pop up dialog boxes or whatever.
 * \param data A pointer to some user defined data structure
 * \param text The text of the error message to display. 
 */
typedef void (*ErrFunc)(void *data, char *text);

  /**
   * Set a global error function.
   * Not threadsafe.
   * \param pData A pointer to a user defined data structure which be passed to 
   * the error display function.
   * \param newErr The new error display function.
   */
extern  void  NXMSetError(void *pData, ErrFunc newErr);

  /**
   * Set an error function for the current thread.
   * When used this overrides anything set in NXMSetError (for the current thread).
   * Use this method in threaded applications.
   * \param pData A pointer to a user defined data structure which be passed to 
   * the error display function.
   * \param newErr The new error display function.
   */
extern  void  NXMSetTError(void *pData, ErrFunc newErr);

  /**
   * Retrieve the current error display function
   * \return The current error display function.
   */
extern ErrFunc NXMGetError();

  /**
   * Suppress error reports from the NeXus-API
   */
extern  void  NXMDisableErrorReporting();

  /**
   * Enable error reports from the NeXus-API
   */
extern  void  NXMEnableErrorReporting();

 /**
  * Dispatches the error message to the error function defined by NXMSetTError
  */
extern void NXReportError(char *text);

 /**
  * Do not use, first parameter should be set by NXMSetTError
  */
extern void NXIReportError(void *pData,char *text);
/* extern void *NXpData; */
extern char *NXIformatNeXusTime();

/**
 * A function for setting the default cache size for HDF-5
 * \ingroup c_init
 */
extern  NXstatus  NXsetcache(long newVal);

  typedef struct {
        NXhandle pNexusData;   
        NXstatus ( *nxreopen)(NXhandle pOrigHandle, NXhandle* pNewHandle);
        NXstatus ( *nxclose)(NXhandle* pHandle);
        NXstatus ( *nxflush)(NXhandle* pHandle);
        NXstatus ( *nxmakegroup) (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
        NXstatus ( *nxopengroup) (NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass);
        NXstatus ( *nxclosegroup)(NXhandle handle);
        NXstatus ( *nxmakedata64) (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int64_t dim[]);
        NXstatus ( *nxcompmakedata64) (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int64_t dim[], int comp_typ, int64_t bufsize[]);
        NXstatus ( *nxcompress) (NXhandle handle, int compr_type);
        NXstatus ( *nxopendata) (NXhandle handle, CONSTCHAR* label);
        NXstatus ( *nxclosedata)(NXhandle handle);
        NXstatus ( *nxputdata)(NXhandle handle, const void* data);
        NXstatus ( *nxputattr)(NXhandle handle, CONSTCHAR* name, const void* data, int iDataLen, int iType);
        NXstatus ( *nxputattra)(NXhandle handle, CONSTCHAR* name, const void* data, const int rank, const int dim[], const int iType);
        NXstatus ( *nxputslab64)(NXhandle handle, const void* data, const int64_t start[], const int64_t size[]);    
        NXstatus ( *nxgetdataID)(NXhandle handle, NXlink* pLink);
        NXstatus ( *nxmakelink)(NXhandle handle, NXlink* pLink);
        NXstatus ( *nxmakenamedlink)(NXhandle handle, CONSTCHAR *newname, NXlink* pLink);
        NXstatus ( *nxgetdata)(NXhandle handle, void* data);
        NXstatus ( *nxgetinfo64)(NXhandle handle, int* rank, int64_t dimension[], int* datatype);
        NXstatus ( *nxgetnextentry)(NXhandle handle, NXname name, NXname nxclass, int* datatype);
        NXstatus ( *nxgetslab64)(NXhandle handle, void* data, const int64_t start[], const int64_t size[]);
        NXstatus ( *nxgetnextattr)(NXhandle handle, NXname pName, int *iLength, int *iType);
        NXstatus ( *nxgetnextattra)(NXhandle handle, NXname pName, int *rank, int dim[], int *iType);
        NXstatus ( *nxgetattr)(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
        NXstatus ( *nxgetattra)(NXhandle handle, char* name, void* data);
        NXstatus ( *nxgetattrainfo)(NXhandle handle, NXname pName, int *rank, int dim[], int *iType);
        NXstatus ( *nxgetattrinfo)(NXhandle handle, int* no_items);
        NXstatus ( *nxgetgroupID)(NXhandle handle, NXlink* pLink);
        NXstatus ( *nxgetgroupinfo)(NXhandle handle, int* no_items, NXname name, NXname nxclass);
        NXstatus ( *nxsameID)(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID);
        NXstatus ( *nxinitgroupdir)(NXhandle handle);
        NXstatus ( *nxinitattrdir)(NXhandle handle);
        NXstatus ( *nxsetnumberformat)(NXhandle handle, int type, char *format);
        NXstatus ( *nxprintlink)(NXhandle handle, NXlink* link);
        NXstatus ( *nxnativeexternallink)(NXhandle handle, CONSTCHAR* name, CONSTCHAR* externalfile, CONSTCHAR* remotetarget);
        NXstatus ( *nxnativeinquirefile)(NXhandle handle, char* externalfile, const int filenamelength);
        NXstatus ( *nxnativeisexternallink)(NXhandle handle, CONSTCHAR* name, char* url, int urllen);
        int stripFlag;
        int checkNameSyntax;
  } NexusFunction, *pNexusFunction;
  /*---------------------*/
  extern long nx_cacheSize;

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* NEXUS_INTERNAL_API */
