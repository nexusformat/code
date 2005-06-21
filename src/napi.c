/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface Routines
  
  Copyright (C) 1997-2000 Mark Koennecke, Przemek Klosowski
  
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

----------------------------------------------------------------------------*/

static const char* rscid = "$Id$";	/* Revision interted by CVS */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "napi.h"

  
  static int iFortifyScope;
/*------------------------------------------------------------------------
  HDF-5 cache size special stuff
  -------------------------------------------------------------------------*/
long nx_cacheSize =  1024000; /* 1MB, HDF-5 default */

NXstatus CALLING_STYLE NXsetcache(long newVal)
{
  if(newVal > 0)
  {
    nx_cacheSize = newVal;
    return NX_OK;
  }
  return NX_ERROR;
}
/*-----------------------------------------------------------------------*/
static NXstatus NXisXML(CONSTCHAR *filename)
{
  FILE *fd = NULL;
  char line[132];

  fd = fopen(filename,"r");
  if(fd) {
    fgets(line,131,fd);
    fclose(fd);
    if(strstr(line,"?xml") != NULL){
      return NX_OK;
    }
  }
  return NX_ERROR;
}
/*-------------------------------------------------------------------------*/
 

   /*---------------------------------------------------------------------*/

  void NXNXNXReportError(void *pData, char *string)
  {
    printf("%s \n",string);
  }
     
  /*---------------------------------------------------------------------*/

  void *NXpData = NULL;
  void (*NXIReportError)(void *pData, char *string) = NXNXNXReportError;

  /*---------------------------------------------------------------------*/

  NX_EXTERNAL void CALLING_STYLE NXMSetError(void *pData, 
			      void (*NewError)(void *pD, char *text))
  {
    NXpData = pData;
    NXIReportError = NewError;
  }
  
#ifdef HDF5
#include "napi5.h"
#endif
#ifdef HDF4
#include "napi4.h"
#endif
#ifdef NXXML
#include "nxxml.h"
#endif  

  /* ---------------------------------------------------------------------- 
  
                          Definition of NeXus API

   ---------------------------------------------------------------------*/
static int determineFileType(CONSTCHAR *filename)
{
  FILE *fd = NULL;
  int iRet, fapl;
  NXhandle handle;
  
  /*
    this is for reading, check for existence first
  */
  fd = fopen(filename,"r");
  if(fd == NULL){
    return -1;
  }
  fclose(fd);
#ifdef HDF5
  iRet=H5Fis_hdf5((const char*)filename);
  if( iRet > 0){
    return 2;
  }
#endif  
#ifdef HDF4
  iRet=Hishdf((const char*)filename);
  if( iRet > 0){
    return 1;
  }
#endif
#ifdef NXXML
  iRet = NXisXML(filename);
  if(iRet == NX_OK){
    return 3;
  }
#endif
  /*
    file type not recognized
  */
  return 0;
}
/*-----------------------------------------------------------------------*/
  NXstatus CALLING_STYLE  NXopen(CONSTCHAR *filename, NXaccess am, NXhandle *gHandle)
  {
    int hdf_type=0;
    int iRet=0;
    NXhandle hdf5_handle;
    NXhandle hdf4_handle;
    NXhandle xmlHandle;
    pNexusFunction fHandle;
    NXstatus retstat;
    char error[1024];
        
    /* configure fortify 
    iFortifyScope = Fortify_EnterScope();
    Fortify_CheckAllMemory();
    */
    
    /*
      allocate data
    */
    *gHandle = NULL;
    fHandle = (pNexusFunction)malloc(sizeof(NexusFunction));
    if (fHandle == NULL) {
      NXIReportError (NXpData,"ERROR: no memory to create Function structure");
      return NX_ERROR;
    }
	memset(fHandle, 0, sizeof(NexusFunction)); /* so any functions we miss are NULL */
       
    /*
      test the strip flag. Elimnate it for the rest of the tests to work
    */
    fHandle->stripFlag = 1;
    if(am & NXACC_NOSTRIP){
      fHandle->stripFlag = 0;
      am -= NXACC_NOSTRIP;
    }

    if (am==NXACC_CREATE) {
      /* HDF4 will be used ! */
      hdf_type=1;
    } else if (am==NXACC_CREATE4) {
      /* HDF4 will be used ! */
      hdf_type=1;   
    } else if (am==NXACC_CREATE5) {
      /* HDF5 will be used ! */
      hdf_type=2;   
    } else if (am==NXACC_CREATEXML) {
      /* XML will be used ! */
      hdf_type=3;   
    } else {
      /* check file type hdf4/hdf5/XML for reading */
      iRet = determineFileType(filename);
      if(iRet < 0) {
	snprintf(error,1023,"failed to open %s for reading",
		 filename);
	NXIReportError(NXpData,error);
	return NX_ERROR;
      }
      if(iRet == 0){
	snprintf(error,1023,"failed to detrmine filetype for %s ",
		 filename);
	NXIReportError(NXpData,error);
	return NX_ERROR;
      }
      hdf_type = iRet;
    }
    if (hdf_type==1) {
      /* HDF4 type */
#ifdef HDF4
      retstat = NX4open((const char *)filename,am,&hdf4_handle);
      if(retstat != NX_OK){
	free(fHandle);
	return retstat;
      }
      fHandle->pNexusData=hdf4_handle;
      NX4assignFunctions(fHandle);
      *gHandle = fHandle;
#else
      NXIReportError (NXpData,
         "ERROR: Attempt to create HDF4 file when not linked with HDF4");
      *gHandle = NULL;
      retstat = NX_ERROR;
#endif /* HDF4 */
      return retstat; 
    } else if (hdf_type==2) {
      /* HDF5 type */
#ifdef HDF5
      retstat = NX5open(filename,am,&hdf5_handle);
      if(retstat != NX_OK){
	free(fHandle);
	return retstat;
      }
      fHandle->pNexusData=hdf5_handle;
      NX5assignFunctions(fHandle);
      *gHandle = fHandle;
#else
      NXIReportError (NXpData,
	 "ERROR: Attempt to create HDF5 file when not linked with HDF5");
      *gHandle = NULL;
      retstat = NX_ERROR;
#endif /* HDF5 */
      return retstat;
    } else if(hdf_type == 3){
      /*
	XML type
      */
#ifdef NXXML
      retstat = NXXopen(filename,am,&xmlHandle);
      if(retstat != NX_OK){
	free(fHandle);
	return retstat;
      }
      fHandle->pNexusData=xmlHandle;
      NXXassignFunctions(fHandle);
      *gHandle = fHandle;
#else
      NXIReportError (NXpData,
	 "ERROR: Attempt to create XML file when not linked with XML");
      *gHandle = NULL;
      retstat = NX_ERROR;
#endif
    } else {
      NXIReportError (NXpData,
          "ERROR: Format not readable by this NeXus library");
      *gHandle = NULL;
      return NX_ERROR;
    }
    return NX_OK;
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
    /* 
    Fortify_CheckAllMemory();
    */
    return status;   
  }

  /*-----------------------------------------------------------------------*/   

  NXstatus CALLING_STYLE NXmakegroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass) 
  {
     pNexusFunction pFunc = (pNexusFunction)fid;
     return pFunc->nxmakegroup(pFunc->pNexusData, name, nxclass);   
  }

  /*------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXopengroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass)
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
                           int rank, int dimensions[],int compress_type, int chunk_size[])
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
  /* --------------------------------------------------------------------*/
  NXstatus CALLING_STYLE NXopensourcegroup(NXhandle fid)
  {
    char target_path[512];
    int status, type = NX_CHAR, length = 511;

    status = NXgetattr(fid,"target",target_path,&length,&type);
    if(status != NX_OK)
    {
      NXIReportError(NXpData,"ERROR: item not linked");
      return NX_ERROR;
    }
    return NXopengrouppath(fid,target_path);
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
  
  NXstatus CALLING_STYLE NXmalloc (void** data, int rank, 
				   int dimensions[], int datatype)
  {
    int i;
    size_t size = 1;
    *data = NULL;
    for(i=0; i<rank; i++)
    size *= dimensions[i];
    if ((datatype == NX_CHAR) || (datatype == NX_INT8) 
	|| (datatype == NX_UINT8)) {
        /* size is correct already */
      }
      else if ((datatype == NX_INT16) || (datatype == NX_UINT16)) {
      size *= 2;
      }    
      else if ((datatype == NX_INT32) || (datatype == NX_UINT32) 
	       || (datatype == NX_FLOAT32)) {
        size *= 4;
      }    
      else if (datatype == NX_FLOAT64) {
        size *= 8;
      }
      else {
        NXIReportError (NXpData,
			"ERROR: NXmalloc - unknown data type in array");
        return NX_ERROR;
    }
    *data = (void*)malloc(size);
     memset(*data,0,size);
    return NX_OK;
  }
    
  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXfree (void** data)
  {
    if (data == NULL) {
       NXIReportError (NXpData, "ERROR: passing NULL to NXfree");
       return NX_ERROR;
    }
    if (*data == NULL) {
       NXIReportError (NXpData,"ERROR: passing already freed pointer to NXfree");
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
/*----------------------------------------------------------------------*/
/*
**  TRIM.C - Remove leading, trailing, & excess embedded spaces
**
**  public domain by Bob Stout
*/
#define NUL '\0'

static char *nxitrim(char *str)
{
      char *ibuf = str, *obuf = str;
      int i = 0, cnt = 0;

      /*
      **  Trap NULL
      */

      if (str)
      {
            /*
            **  Remove leading spaces (from RMLEAD.C)
            */

            for (ibuf = str; *ibuf && isspace(*ibuf); ++ibuf)
                  ;
            if (str != ibuf)
                  memmove(str, ibuf, ibuf - str);

            /*
            **  Collapse embedded spaces (from LV1WS.C)
            */

            while (*ibuf)
            {
                  if (isspace(*ibuf) && cnt)
                        ibuf++;
                  else
                  {
                        if (!isspace(*ibuf))
                              cnt = 0;
                        else
                        {
                              *ibuf = ' ';
                              cnt = 1;
                        }
                        obuf[i++] = *ibuf++;
                  }
            }
            obuf[i] = NUL;

            /*
            **  Remove trailing spaces (from RMTRAIL.C)
            */

            while (--i >= 0)
            {
                  if (!isspace(obuf[i]))
                        break;
            }
            obuf[++i] = NUL;
      }
      return str;
}
  /*-------------------------------------------------------------------------*/

  NXstatus CALLING_STYLE NXgetdata (NXhandle fid, void *data)
  {
    int status, type, rank, iDim[NX_MAXRANK];
    char *pPtr;

    pNexusFunction pFunc = (pNexusFunction)fid;
    status = pFunc->nxgetdata(pFunc->pNexusData, data); 
    if(status != NX_ERROR && pFunc->stripFlag == 1){
      NXgetinfo(fid,&rank,iDim,&type);
      if(type == NX_CHAR){
	pPtr = (char *)data;
	data = (void *)nxitrim(pPtr);
      }
    }
    return status;
  }
    
  /*-------------------------------------------------------------------------*/
 
  NXstatus CALLING_STYLE NXgetinfo (NXhandle fid, int *rank, 
				    int dimension[], int *iType)
  {
    int status;
    char *pPtr = NULL;

    pNexusFunction pFunc = (pNexusFunction)fid;
    status = pFunc->nxgetinfo(pFunc->pNexusData, rank, dimension, iType);
    /*
      allow for a \0 at the end of the data
    */
    if(*iType == NX_CHAR){
      dimension[0] += 1;
    }
    /*
      the length of a string may be trimmed....
    */
    if(*iType == NX_CHAR && pFunc->stripFlag == 1){
      pPtr = (char *)malloc(dimension[0]*sizeof(char));
      if(pPtr != NULL){
	memset(pPtr,0,dimension[0]*sizeof(char));
	pFunc->nxgetdata(pFunc->pNexusData, pPtr);
	dimension[0] = strlen(nxitrim(pPtr)) + 1;
	free(pPtr);
      }
    } 
    return status;
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

  NXstatus CALLING_STYLE NXsameID (NXhandle fileid, NXlink* pFirstID, NXlink* pSecondID)
  {
    pNexusFunction pFunc = (pNexusFunction)fileid;
    return pFunc->nxsameID(pFunc->pNexusData, pFirstID, pSecondID);  
  }

  /*-------------------------------------------------------------------------*/
  
  NXstatus CALLING_STYLE NXinitattrdir (NXhandle fid)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxinitattrdir(pFunc->pNexusData);
  }
  /*-------------------------------------------------------------------------*/
  
  NXstatus CALLING_STYLE NXsetnumberformat (NXhandle fid, 
					    int type, char *format)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    if(pFunc->nxsetnumberformat != NULL)
    {
      return pFunc->nxsetnumberformat(pFunc->pNexusData,type,format);
    }
    else
    {
      /*
	silently ignore this. Most NeXus file formats do not require
        this
      */
      return NX_OK;
    }
  }
  
  
  /*-------------------------------------------------------------------------*/
 
  NXstatus CALLING_STYLE NXinitgroupdir (NXhandle fid)
  {
    pNexusFunction pFunc = (pNexusFunction)fid;
    return pFunc->nxinitgroupdir(pFunc->pNexusData);
  }

/*------------------------------------------------------------------------
  Implementation of NXopenpath. 
  --------------------------------------------------------------------------*/
static int isDataSetOpen(NXhandle hfil)
{
  NXlink id;
  
  /*
    This uses the (sensible) feauture that NXgetdataID returns NX_ERROR
    when no dataset is open
  */
  if(NXgetdataID(hfil,&id) == NX_ERROR)
  {
    return 0;
  }
  else 
  {
    return 1;
  }
}
/*----------------------------------------------------------------------*/
static int isRoot(NXhandle hfil)
{
  NXlink id;
  
  /*
    This uses the feauture that NXgetgroupID returns NX_ERROR
    when we are at root level
  */
  if(NXgetgroupID(hfil,&id) == NX_ERROR)
  {
    return 1;
  }
  else 
  {
    return 0;
  }
}
/*--------------------------------------------------------------------
  copies the next path element into element.
  returns a pointer into path beyond the extracted path
  ---------------------------------------------------------------------*/
static char *extractNextPath(char *path, NXname element)
{
  char *pPtr, *pStart;
  int length;

  pPtr = path;
  /*
    skip over leading /
  */
  if(*pPtr == '/')
  {
    pPtr++;
  }
  pStart = pPtr;
  
  /*
    find next /
  */
  pPtr = strchr(pStart,'/');
  if(pPtr == NULL)
  {
    /*
      this is the last path element
    */
    strcpy(element,pStart);
    return NULL;
  } else {
    length = pPtr - pStart;
    strncpy(element,pStart,length);
    element[length] = '\0';
  }
  return pPtr + 1;
}
/*-------------------------------------------------------------------*/
static NXstatus gotoRoot(NXhandle hfil)
{
    int status;

    if(isDataSetOpen(hfil))
    {
      status = NXclosedata(hfil);
      if(status == NX_ERROR)
      {
	return status;
      }
    }
    while(!isRoot(hfil))
    {
      status = NXclosegroup(hfil);
      if(status == NX_ERROR)
      {
	return status;
      }
    }
    return NX_OK;
}
/*--------------------------------------------------------------------*/
static int isRelative(char *path)
{
  if(path[0] == '.' && path[1] == '.')
    return 1;
  else
    return 0;
}
/*------------------------------------------------------------------*/
static NXstatus moveOneDown(NXhandle hfil)
{
  if(isDataSetOpen(hfil))
  {
    return NXclosedata(hfil);
  } 
  else
  {
    return NXclosegroup(hfil);
  }
}
/*-------------------------------------------------------------------
  returns a pointer to the remaining path string to move up
  --------------------------------------------------------------------*/
static char *moveDown(NXhandle hfil, char *path, int *code)
{
  int status;
  NXname pathElem;
  char *pPtr;

  *code = NX_OK;

  if(path[0] == '/')
  {
    *code = gotoRoot(hfil);
    return path;
  } 
  else 
  {
    pPtr = path;
    while(isRelative(pPtr))
    {
      status = moveOneDown(hfil);
      if(status == NX_ERROR)
      {
	*code = status;
	return pPtr;
      }
      pPtr += 3;
    }
    return pPtr;
  }
} 
/*--------------------------------------------------------------------*/
static NXstatus stepOneUp(NXhandle hfil, char *name)
{
  int status, datatype;
  NXname name2, xclass;
  char pBueffel[256];  

  /*
    catch the case when we are there: i.e. no further stepping
    necessary. This can happen with paths like ../
  */
  if(strlen(name) < 1)
  {
      return NX_OK;
  }
  
  NXinitgroupdir(hfil);
  while(NXgetnextentry(hfil,name2,xclass,&datatype) != NX_EOD)
  {
    
    if(strcmp(name2,name) == 0)
    {
      if(strcmp(xclass,"SDS") == 0)
      {
	return NXopendata(hfil,name);
      } 
      else
      {
	return NXopengroup(hfil,name,xclass);
      }
    }
  }
  snprintf(pBueffel,255,"ERROR: NXopenpath cannot step into %s",name);
  NXIReportError (NXpData, pBueffel);
  return NX_ERROR;              
}
/*--------------------------------------------------------------------*/
static NXstatus stepOneGroupUp(NXhandle hfil, char *name)
{
  int status, datatype;
  NXname name2, xclass;
  char pBueffel[256];  

  /*
    catch the case when we are there: i.e. no further stepping
    necessary. This can happen with paths like ../
  */
  if(strlen(name) < 1)
  {
      return NX_OK;
  }
  
  NXinitgroupdir(hfil);
  while(NXgetnextentry(hfil,name2,xclass,&datatype) != NX_EOD)
  {
    
    if(strcmp(name2,name) == 0)
    {
      if(strcmp(xclass,"SDS") == 0)
      {
	return NX_EOD;
      } 
      else
      {
	return NXopengroup(hfil,name,xclass);
      }
    }
  }
  snprintf(pBueffel,255,"ERROR: NXopenpath cannot step into %s",name);
  NXIReportError (NXpData, pBueffel);
  return NX_ERROR;              
}
/*---------------------------------------------------------------------*/
NXstatus CALLING_STYLE NXopenpath(NXhandle hfil, CONSTCHAR *path)
{
  int status, run = 1;
  NXname pathElement;
  char *pPtr;

  if(hfil == NULL || path == NULL)
  {
    NXIReportError(NXpData,
     "ERROR: NXopendata needs both a file handle and a path string");
    return NX_ERROR;
  }

  pPtr = moveDown(hfil,(char *)path,&status);
  if(status != NX_OK)
  {
    NXIReportError (NXpData, 
		    "ERROR: NXopendata failed to move down in hierarchy");
    return status;
  }

  while(run == 1)
  {
    pPtr = extractNextPath(pPtr, pathElement);
    status = stepOneUp(hfil,pathElement);
    if(status != NX_OK)
    {
      return status;
    }
    if(pPtr == NULL)
    {
      run = 0;
    }
  }
  return NX_OK;
}
/*---------------------------------------------------------------------*/
NXstatus CALLING_STYLE NXopengrouppath(NXhandle hfil, CONSTCHAR *path)
{
  int status, run = 1;
  NXname pathElement;
  char *pPtr;

  if(hfil == NULL || path == NULL)
  {
    NXIReportError(NXpData,
     "ERROR: NXopendata needs both a file handle and a path string");
    return NX_ERROR;
  }

  pPtr = moveDown(hfil,(char *)path,&status);
  if(status != NX_OK)
  {
    NXIReportError (NXpData, 
		    "ERROR: NXopendata failed to move down in hierarchy");
    return status;
  }

  while(run == 1)
  {
    pPtr = extractNextPath(pPtr, pathElement);
    status = stepOneGroupUp(hfil,pathElement);
    if(status == NX_ERROR)
    {
      return status;
    }
    if(pPtr == NULL || status == NX_EOD)
    {
      run = 0;
    }
  }
  return NX_OK;
}
/*--------------------------------------------------------------------
  format NeXus time. Code needed in every NeXus file driver
  ---------------------------------------------------------------------*/
char *NXIformatNeXusTime(){
    char *timeData;
    time_t timer;
    char* time_buffer = NULL;
    struct tm *time_info;
    const char* time_format;
    long gmt_offset;
#ifdef USE_FTIME
    struct timeb timeb_struct;
#endif 

    time_buffer = (char *)malloc(64*sizeof(char));
    if(!time_buffer){
      NXIReportError(NXpData,"Failed to allocate buffer for time data");
      return NULL;
    }

#ifdef NEED_TZSET
    tzset();
#endif 
    time(&timer);
#ifdef USE_FTIME
    ftime(&timeb_struct);
    gmt_offset = -timeb_struct.timezone * 60;
    if (timeb_struct.dstflag != 0)
    {
        gmt_offset += 3600;
    }
#else
    time_info = gmtime(&timer);
    if (time_info != NULL)
    {
        gmt_offset = (long)difftime(timer, mktime(time_info));
    }
    else
    {
        NXIReportError (NXpData, 
        "Your gmtime() function does not work ... timezone information will be incorrect\n");
        gmt_offset = 0;
    }
#endif 
    time_info = localtime(&timer);
    if (time_info != NULL)
    {
        if (gmt_offset < 0)
        {
            time_format = "%04d-%02d-%02d %02d:%02d:%02d-%02d%02d";
        }
        else
        {
            time_format = "%04d-%02d-%02d %02d:%02d:%02d+%02d%02d";
        }
        sprintf(time_buffer, time_format,
            1900 + time_info->tm_year,
            1 + time_info->tm_mon,
            time_info->tm_mday,
            time_info->tm_hour,
            time_info->tm_min,
            time_info->tm_sec,
            abs(gmt_offset / 3600),
            abs((gmt_offset % 3600) / 60)
        );
    }
    else
    {
        strcpy(time_buffer, "1970-01-01 00:00:00+0000");
    }
    return time_buffer;
}
/*----------------------------------------------------------------------
                 F77 - API - Support - Routines
  ----------------------------------------------------------------------*/
  /*
   * We store the whole of the NeXus file in the array - that way
   * we can just pass the array name to C as it will be a valid
   * NXhandle. We could store the NXhandle value in the FORTRAN array
   * instead, but that would mean writing far more wrappers
   */
  NXstatus CALLING_STYLE NXfopen(char * filename, NXaccess* am, 
                                 NexusFunction* pHandle)
  {
	NXstatus ret;
 	NXhandle fileid = NULL;
	ret = NXopen(filename, *am, &fileid);
	if (ret == NX_OK)
	{
	    memcpy(pHandle, fileid, sizeof(NexusFunction));
	}
	else
	{
	    memset(pHandle, 0, sizeof(NexusFunction));
	}
	if (fileid != NULL)
	{
	    free(fileid);
	}
	return ret;
  }
/* 
 * The pHandle from FORTRAN is a pointer to a static FORTRAN
 * array holding the NexusFunction structure. We need to malloc()
 * a temporary copy as NXclose will try to free() this
 */
  NXstatus CALLING_STYLE NXfclose (NexusFunction* pHandle)
  {
    NXhandle h;
    NXstatus ret;
    h = (NXhandle)malloc(sizeof(NexusFunction));
    memcpy(h, pHandle, sizeof(NexusFunction));
    ret = NXclose(&h);		/* does free(h) */
    memset(pHandle, 0, sizeof(NexusFunction));
    return ret;
  }
  
/*---------------------------------------------------------------------*/  
  NXstatus CALLING_STYLE NXfflush(NexusFunction* pHandle)
  {
    NXhandle h;
    NXstatus ret;
    h = (NXhandle)malloc(sizeof(NexusFunction));
    memcpy(h, pHandle, sizeof(NexusFunction));
    ret = NXflush(&h);		/* modifies and reallocates h */
    memcpy(pHandle, h, sizeof(NexusFunction));
    return ret;
  }
/*----------------------------------------------------------------------*/
  NXstatus CALLING_STYLE NXfmakedata(NXhandle fid, char *name, int *pDatatype,
		int *pRank, int dimensions[])
  {
    NXstatus ret;
    static char buffer[256];
    int i, *reversed_dimensions;
    reversed_dimensions = (int*)malloc(*pRank * sizeof(int));
    if (reversed_dimensions == NULL)
    {
        sprintf (buffer, 
        "ERROR: Cannot allocate space for array rank of %d in NXfmakedata", 
                *pRank);
        NXIReportError (NXpData, buffer);
	return NX_ERROR;
    }
/*
 * Reverse dimensions array as FORTRAN is column major, C row major
 */
    for(i=0; i < *pRank; i++)
    {
	reversed_dimensions[i] = dimensions[*pRank - i - 1];
    }
    ret = NXmakedata(fid, name, *pDatatype, *pRank, reversed_dimensions);
    free(reversed_dimensions);
    return ret;
  }


  NXstatus CALLING_STYLE NXfcompmakedata(NXhandle fid, char *name, 
                int *pDatatype,
		int *pRank, int dimensions[],
                int *compression_type, int chunk[])
  {
    NXstatus ret;
    static char buffer[256];
    int i, *reversed_dimensions, *reversed_chunk;
    reversed_dimensions = (int*)malloc(*pRank * sizeof(int));
    reversed_chunk = (int*)malloc(*pRank * sizeof(int));
    if (reversed_dimensions == NULL || reversed_chunk == NULL)
    {
        sprintf (buffer, 
      "ERROR: Cannot allocate space for array rank of %d in NXfcompmakedata", 
         *pRank);
        NXIReportError (NXpData, buffer);
	return NX_ERROR;
    }
/*
 * Reverse dimensions array as FORTRAN is column major, C row major
 */
    for(i=0; i < *pRank; i++)
    {
	reversed_dimensions[i] = dimensions[*pRank - i - 1];
	reversed_chunk[i] = chunk[*pRank - i - 1];
    }
    ret = NXcompmakedata(fid, name, *pDatatype, *pRank, 
        reversed_dimensions,*compression_type, reversed_chunk);
    free(reversed_dimensions);
    free(reversed_chunk);
    return ret;
  }
/*-----------------------------------------------------------------------*/
  NXstatus CALLING_STYLE NXfcompress(NXhandle fid, int *compr_type)
  { 
      return NXcompress(fid,*compr_type);
  }
/*-----------------------------------------------------------------------*/
  NXstatus CALLING_STYLE NXfputattr(NXhandle fid, char *name, void *data, 
                                   int *pDatalen, int *pIType)
  {
    return NXputattr(fid, name, data, *pDatalen, *pIType);
  }


  /*
   * implement snprintf when it is not available 
   */
  int nxisnprintf(char* buffer, int len, const char* format, ... )
  {
	  int ret;
	  va_list valist;
	  va_start(valist,format);
	  ret = vsprintf(buffer, format, valist);
	  va_end(valist);
	  return ret;
  }
