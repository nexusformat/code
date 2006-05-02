/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface Routines
  
  Copyright (C) 1997-2006 Mark Koennecke, Przemek Klosowski
  
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

static const char* rscid = "$Id$";	/* Revision inserted by CVS */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include "napi.h"
#include "nxstack.h"

/*---------------------------------------------------------------------
 Recognized and handled napimount URLS
 -----------------------------------------------------------------------*/
#define NXBADURL 0
#define NXFILE 1

/*--------------------------------------------------------------------*/
static int iFortifyScope;
/*----------------------------------------------------------------------
  This is a section with code for searching the NX_LOAD_PATH
  -----------------------------------------------------------------------*/
#ifdef _WIN32
#define LIBSEP ";"
#define PATHSEP "\\"
#else
#define LIBSEP ":"
#define PATHSEP "/"
#endif
extern char *stptok(char *s, char *tok, size_t toklen, char *brk);
/*---------------------------------------------------------------------
 wrapper for getenv. This is a future proofing thing for porting to OS
 which have different ways of accessing environment variables
 --------------------------------------------------------------------*/ 
static char *nxgetenv(const char *name){
  return getenv(name);
}
/*----------------------------------------------------------------------*/
static int canOpen(char *filename){
  FILE *fd = NULL;

  fd = fopen(filename,"r");
  if(fd != NULL){
    fclose(fd);
    return 1;
  } else {
    return 0;
  }
} 
/*--------------------------------------------------------------------*/
static char *locateNexusFileInPath(char *startName){
  char *loadPath = NULL, *testPath = NULL, *pPtr = NULL;
  char pathPrefix[256];
  int length;

  if(canOpen(startName)){
    return strdup(startName);
  }

  loadPath = nxgetenv("NX_LOAD_PATH");
  if(loadPath == NULL){
    /* file not found will be issued by upper level code */
    return strdup(startName);
  }

  pPtr = stptok(loadPath,pathPrefix,255,LIBSEP);
  while(pPtr != NULL){
    length = strlen(pathPrefix) + strlen(startName) + strlen(PATHSEP) + 2;
    testPath = malloc(length*sizeof(char));
    if(testPath == NULL){
      return strdup(startName);
    }
    memset(testPath,0,length*sizeof(char));
    strcpy(testPath, pathPrefix);
    strcat(testPath,PATHSEP);
    strcat(testPath,startName);
    if(canOpen(testPath)){
      return(testPath);
    }
    free(testPath);
    pPtr = stptok(pPtr,pathPrefix,255,LIBSEP);
  }
  return  strdup(startName);
}
/*------------------------------------------------------------------------
  HDF-5 cache size special stuff
  -------------------------------------------------------------------------*/
long nx_cacheSize =  1024000; /* 1MB, HDF-5 default */

NXstatus NXsetcache(long newVal)
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
  void NXNXNXReportError(void *pData, char *string)
  {
    printf("%s \n",string);
  }
  /*---------------------------------------------------------------------*/
  void *NXpData = NULL;
  void (*NXIReportError)(void *pData, char *string) = NXNXNXReportError;
  /*---------------------------------------------------------------------*/
  extern void NXMSetError(void *pData, 
			      void (*NewError)(void *pD, char *text))
  {
    NXpData = pData;
    NXIReportError = NewError;
  }
/*----------------------------------------------------------------------*/
extern ErrFunc NXMGetError(){
  return NXIReportError;
}
/*----------------------------------------------------------------------*/
void NXNXNoReport(void *pData, char *string){
  /* do nothing */
}  
/*----------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------*/
static pNexusFunction handleToNexusFunc(NXhandle fid){
  pFileStack fileStack = NULL;
  fileStack = (pFileStack)fid;
  if(fileStack != NULL){
    return peekFileOnStack(fileStack);
  } else {
    return NULL;
  }
}
/*----------------------------------------------------------------------*/
NXstatus   NXopen(CONSTCHAR *userfilename, NXaccess am, NXhandle *gHandle){
  int status;
  pFileStack fileStack = NULL;
  NXhandle hfile;

  fileStack = makeFileStack();
  if(fileStack == NULL){
    NXIReportError (NXpData,"ERROR: no memory to create filestack");
      return NX_ERROR;
  }
  status = NXinternalopen(userfilename,am,fileStack);
  if(status == NX_OK){
    *gHandle = fileStack;
  }

  return status;
}
/*-----------------------------------------------------------------------*/
NXstatus   NXinternalopen(CONSTCHAR *userfilename, NXaccess am, pFileStack fileStack)
  {
    int hdf_type=0;
    int iRet=0;
    NXhandle hdf5_handle;
    NXhandle hdf4_handle;
    NXhandle xmlHandle;
    pNexusFunction fHandle;
    NXstatus retstat;
    char error[1024];
    char *filename = NULL;
        
    /* configure fortify 
    iFortifyScope = Fortify_EnterScope();
    Fortify_CheckAllMemory();
    */
    
    /*
      allocate data
    */
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
      filename = strdup(userfilename);
    } else if (am==NXACC_CREATE4) {
      /* HDF4 will be used ! */
      hdf_type=1;   
      filename = strdup(userfilename);
    } else if (am==NXACC_CREATE5) {
      /* HDF5 will be used ! */
      hdf_type=2;   
      filename = strdup(userfilename);
    } else if (am==NXACC_CREATEXML) {
      /* XML will be used ! */
      hdf_type=3;   
      filename = strdup(userfilename);
    } else {
      filename = locateNexusFileInPath((char *)userfilename);
      if(filename == NULL){
	NXIReportError(NXpData,"Out of memory in NeXus-API");
	free(fHandle);
	return NX_ERROR;
      }
      /* check file type hdf4/hdf5/XML for reading */
      iRet = determineFileType(filename);
      if(iRet < 0) {
	snprintf(error,1023,"failed to open %s for reading",
		 filename);
	NXIReportError(NXpData,error);
	return NX_ERROR;
      }
      if(iRet == 0){
	snprintf(error,1023,"failed to determine filetype for %s ",
		 filename);
	NXIReportError(NXpData,error);
	free(fHandle);
	return NX_ERROR;
      }
      hdf_type = iRet;
    }
    if(filename == NULL){
	NXIReportError(NXpData,"Out of memory in NeXus-API");
	return NX_ERROR;
    }

    if (hdf_type==1) {
      /* HDF4 type */
#ifdef HDF4
      retstat = NX4open((const char *)filename,am,&hdf4_handle);
      if(retstat != NX_OK){
	free(fHandle);
	free(filename);
	return retstat;
      }
      fHandle->pNexusData=hdf4_handle;
      NX4assignFunctions(fHandle);
      pushFileStack(fileStack,fHandle,filename);
#else
      NXIReportError (NXpData,
         "ERROR: Attempt to create HDF4 file when not linked with HDF4");
      free(filename);
      retstat = NX_ERROR;
#endif /* HDF4 */
      return retstat; 
    } else if (hdf_type==2) {
      /* HDF5 type */
#ifdef HDF5
      retstat = NX5open(filename,am,&hdf5_handle);
      if(retstat != NX_OK){
	free(fHandle);
	free(filename);
	return retstat;
      }
      fHandle->pNexusData=hdf5_handle;
      NX5assignFunctions(fHandle);
      pushFileStack(fileStack,fHandle, filename);
#else
      NXIReportError (NXpData,
	 "ERROR: Attempt to create HDF5 file when not linked with HDF5");
      free(filename);
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
	free(filename);
	return retstat;
      }
      fHandle->pNexusData=xmlHandle;
      NXXassignFunctions(fHandle);
      pushFileStack(fileStack,fHandle, filename);
#else
      NXIReportError (NXpData,
	 "ERROR: Attempt to create XML file when not linked with XML");
      retstat = NX_ERROR;
#endif
    } else {
      NXIReportError (NXpData,
          "ERROR: Format not readable by this NeXus library");
      free(filename);
      return NX_ERROR;
    }
    free(filename); 
    return NX_OK;
  }

/* ------------------------------------------------------------------------- */

  NXstatus  NXclose (NXhandle *fid)
  { 
    NXhandle hfil; 
    int status;
    pFileStack fileStack = NULL;
   
    pNexusFunction pFunc=NULL;
    fileStack = (pFileStack)*fid;
    pFunc = peekFileOnStack(fileStack);
    hfil = pFunc->pNexusData;
    status =  pFunc->nxclose(&hfil);
    pFunc->pNexusData = hfil;
    free(pFunc);
    popFileStack(fileStack);
    if(fileStackDepth(fileStack) < 0){
      killFileStack(fileStack);
    }
    /* 
    Fortify_CheckAllMemory();
    */
    return status;   
  }

  /*-----------------------------------------------------------------------*/   

  NXstatus  NXmakegroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass) 
  {
     pNexusFunction pFunc = handleToNexusFunc(fid);
     return pFunc->nxmakegroup(pFunc->pNexusData, name, nxclass);   
  }
  /*------------------------------------------------------------------------*/
static int analyzeNapimount(char *napiMount, char *extFile, int extFileLen, 
			    char *extPath, int extPathLen){
  char *pPtr = NULL, *path = NULL;
  int length;

  memset(extFile,0,extFileLen);
  memset(extPath,0,extPathLen);
  pPtr = strstr(napiMount,"nxfile://");
  if(pPtr == NULL){
    return NXBADURL;
  }
  path = strrchr(napiMount,'#');
  if(path == NULL){
    length = strlen(napiMount) - 9;
    if(length > extFileLen){
      NXIReportError(NXpData,"ERROR: internal errro with external linking");
      return NXBADURL;
    }
    memcpy(extFile,pPtr+9,length);
    strcpy(extPath,"/");
    return NXFILE;
  } else {
    pPtr += 9;
    length = path - pPtr;
    if(length > extFileLen){
      NXIReportError(NXpData,"ERROR: internal errro with external linking");
      return NXBADURL;
    }
    memcpy(extFile,pPtr,length);
    length = strlen(path-1);
    if(length > extPathLen){
      NXIReportError(NXpData,"ERROR: internal errro with external linking");
      return NXBADURL;
    }
    strcpy(extPath,path+1);
    return NXFILE;
  }
  return NXBADURL;
}
  /*------------------------------------------------------------------------*/

  NXstatus  NXopengroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass)
  {
    int status, attStatus, type = NX_CHAR, length = 1023;
    NXaccess access = NXACC_RDWR;
    NXlink breakID;
    pFileStack fileStack;    
    char nxurl[1024], exfile[512], expath[512];
    ErrFunc oldError;

    fileStack = (pFileStack)fid;
    pNexusFunction pFunc = handleToNexusFunc(fid);

    status = pFunc->nxopengroup(pFunc->pNexusData, name, nxclass);  
    oldError = NXMGetError();
    NXIReportError = NXNXNoReport;
    attStatus = NXgetattr(fid,"napimount",nxurl,&length, &type);
    NXIReportError = oldError;
    if(attStatus == NX_OK){
      /*
	this is an external linking group
      */
      status = analyzeNapimount(nxurl,exfile,511,expath,511);
      if(status == NXBADURL){
	return NX_ERROR;
      }
      status = NXinternalopen(exfile,access, fileStack);
      if(status == NX_ERROR){
	return status;
      }
      status = NXopenpath(fid,expath);
      NXgetgroupID(fid,&breakID);
      setCloseID(fileStack,breakID);
    }
    
    return status;
  } 

  /* ------------------------------------------------------------------- */

  NXstatus  NXclosegroup (NXhandle fid)
  {
    int status;
    pFileStack fileStack = NULL;
    NXlink closeID, currentID;

    pNexusFunction pFunc = handleToNexusFunc(fid);
    fileStack = (pFileStack)fid;
    if(fileStackDepth(fileStack) == 0){
      return pFunc->nxclosegroup(pFunc->pNexusData);  
    } else {
      /* we have to check for leaving an external file */
      NXgetgroupID(fid,&currentID);
      peekIDOnStack(fileStack,&closeID);
      if(NXsameID(fid,&closeID,&currentID) == NX_OK){
	NXclose(&fid);
	status = NXclosegroup(fid);
      } else {
	status = pFunc->nxclosegroup(pFunc->pNexusData);
      }
      return status;
    }
  }   

  /* --------------------------------------------------------------------- */
  
  NXstatus  NXmakedata (NXhandle fid, CONSTCHAR *name, int datatype, 
                                  int rank, int dimensions[])
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    if ( (datatype == NX_CHAR) && (rank > 1) )
    {
        NXIReportError (NXpData,
          "ERROR: multi-dimensional NX_CHAR arrays are not supported by the NeXus library");
	return NX_ERROR;
    }
    else
    {
        return pFunc->nxmakedata(pFunc->pNexusData, name, datatype, rank, dimensions); 
    }
  }


 /* --------------------------------------------------------------------- */
  
  NXstatus  NXcompmakedata (NXhandle fid, CONSTCHAR *name, int datatype, 
                           int rank, int dimensions[],int compress_type, int chunk_size[])
  {
    pNexusFunction pFunc = handleToNexusFunc(fid); 
    if ( (datatype == NX_CHAR) && (rank > 1) )
    {
        NXIReportError (NXpData,
          "ERROR: multi-dimensional NX_CHAR arrays are not supported by the NeXus library");
	return NX_ERROR;
    }
    else
    {
        return pFunc->nxcompmakedata (pFunc->pNexusData, name, datatype, rank, dimensions, compress_type, chunk_size); 
    }
  } 
  
 
  /* --------------------------------------------------------------------- */

  NXstatus  NXcompress (NXhandle fid, int compress_type)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid); 
    return pFunc->nxcompress (pFunc->pNexusData, compress_type); 
  }
  
 
  /* --------------------------------------------------------------------- */
  
  NXstatus  NXopendata (NXhandle fid, CONSTCHAR *name)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid); 
    return pFunc->nxopendata(pFunc->pNexusData, name); 
  } 


  /* ----------------------------------------------------------------- */
    
  NXstatus  NXclosedata (NXhandle fid)
  { 
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxclosedata(pFunc->pNexusData);
  }

  /* ------------------------------------------------------------------- */

  NXstatus  NXputdata (NXhandle fid, void *data)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxputdata(pFunc->pNexusData, data);
  }

  /* ------------------------------------------------------------------- */

  NXstatus  NXputattr (NXhandle fid, CONSTCHAR *name, void *data, 
                                  int datalen, int iType)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxputattr(pFunc->pNexusData, name, data, datalen, iType);
  }

  /* ------------------------------------------------------------------- */

  NXstatus  NXputslab (NXhandle fid, void *data, int iStart[], int iSize[])
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxputslab(pFunc->pNexusData, data, iStart, iSize);
  }

  /* ------------------------------------------------------------------- */

  NXstatus  NXgetdataID (NXhandle fid, NXlink* sRes)
  {  
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxgetdataID(pFunc->pNexusData, sRes);
  }


  /* ------------------------------------------------------------------- */

  NXstatus  NXmakelink (NXhandle fid, NXlink* sLink)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxmakelink(pFunc->pNexusData, sLink);
  }
  /* --------------------------------------------------------------------*/
  NXstatus  NXopensourcegroup(NXhandle fid)
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

  NXstatus  NXflush(NXhandle *pHandle)
  {
    NXhandle hfil; 
    pFileStack fileStack = NULL;
    int status;
   
    pNexusFunction pFunc=NULL;
    fileStack = (pFileStack)*pHandle;
    pFunc = peekFileOnStack(fileStack);
    hfil = pFunc->pNexusData;
    status =  pFunc->nxflush(&hfil);
    pFunc->pNexusData = hfil;
    return status; 
  }


  /*-------------------------------------------------------------------------*/
  
  NXstatus  NXmalloc (void** data, int rank, 
				   int dimensions[], int datatype)
  {
    int i;
    size_t size = 1;
    *data = NULL;
    for(i=0; i<rank; i++)
    size *= dimensions[i];
    if ((datatype == NX_CHAR) || (datatype == NX_INT8) 
	|| (datatype == NX_UINT8)) {
        /* allow for terminating \0 */
      size += 1;
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

  NXstatus  NXfree (void** data)
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
           
 
  NXstatus  NXgetnextentry (NXhandle fid, NXname name, NXname nxclass, int *datatype)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
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
	    str = ibuf;

            /*
            **  Remove trailing spaces (from RMTRAIL.C)
            */
	    i = strlen(str);
            while (--i >= 0)
            {
                  if (!isspace(str[i]))
                        break;
            }
            str[++i] = NUL;
      }
      return str;
}
  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetdata (NXhandle fid, void *data)
  {
    int status, type, rank, iDim[NX_MAXRANK];
    char *pPtr, *pPtr2;

    pNexusFunction pFunc = handleToNexusFunc(fid);
    status = pFunc->nxgetinfo(pFunc->pNexusData, &rank, iDim, &type); /* unstripped size if string */
    if ( (type == NX_CHAR) && (pFunc->stripFlag == 1) )
    {
	pPtr = (char*)malloc(iDim[0]+1);
        memset(pPtr, 0, iDim[0]+1);
        status = pFunc->nxgetdata(pFunc->pNexusData, pPtr); 
	pPtr2 = nxitrim(pPtr);
	strncpy((char*)data, pPtr2, strlen(pPtr2)); /* not NULL terminated by default */
	free(pPtr);
    }
    else
    {
        status = pFunc->nxgetdata(pFunc->pNexusData, data); 
    }
    return status;
  }
    
  /*-------------------------------------------------------------------------*/
 
  NXstatus  NXgetinfo (NXhandle fid, int *rank, 
				    int dimension[], int *iType)
  {
    int status;
    char *pPtr = NULL;

    pNexusFunction pFunc = handleToNexusFunc(fid);
    status = pFunc->nxgetinfo(pFunc->pNexusData, rank, dimension, iType);
    /*
      the length of a string may be trimmed....
    */
    if(*iType == NX_CHAR && pFunc->stripFlag == 1){
      pPtr = (char *)malloc((dimension[0]+1)*sizeof(char));
      if(pPtr != NULL){
	memset(pPtr,0,(dimension[0]+1)*sizeof(char));
	pFunc->nxgetdata(pFunc->pNexusData, pPtr);
	dimension[0] = strlen(nxitrim(pPtr));
	free(pPtr);
      }
    } 
    if ( (*iType == NX_CHAR) && (*rank > 1) )
    {
	NXIReportError(NXpData,
      "WARNING: multi-dimensional character arrays are not really supported");
    }
    return status;
  }
  
  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetslab (NXhandle fid, void *data, 
				    int iStart[], int iSize[])
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxgetslab(pFunc->pNexusData, data, iStart, iSize);
  }
  
  
  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetnextattr (NXhandle fileid, NXname pName,
                                     int *iLength, int *iType)
  {
    pNexusFunction pFunc = handleToNexusFunc(fileid);
    return pFunc->nxgetnextattr(pFunc->pNexusData, pName, iLength, iType);  
  }
 

  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetattr (NXhandle fid, char *name, void *data, int* datalen, int* iType)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxgetattr(pFunc->pNexusData, name, data, datalen, iType);  
  }


  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetattrinfo (NXhandle fid, int *iN)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxgetattrinfo(pFunc->pNexusData, iN);  
  }


  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetgroupID (NXhandle fileid, NXlink* sRes)
  {
    pNexusFunction pFunc = handleToNexusFunc(fileid);
    return pFunc->nxgetgroupID(pFunc->pNexusData, sRes);  
  }

  /*-------------------------------------------------------------------------*/

  NXstatus  NXgetgroupinfo (NXhandle fid, int *iN, NXname pName, NXname pClass)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxgetgroupinfo(pFunc->pNexusData, iN, pName, pClass);  
  }

  
  /*-------------------------------------------------------------------------*/

  NXstatus  NXsameID (NXhandle fileid, NXlink* pFirstID, NXlink* pSecondID)
  {
    pNexusFunction pFunc = handleToNexusFunc(fileid);
    return pFunc->nxsameID(pFunc->pNexusData, pFirstID, pSecondID);  
  }

  /*-------------------------------------------------------------------------*/
  
  NXstatus  NXinitattrdir (NXhandle fid)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxinitattrdir(pFunc->pNexusData);
  }
  /*-------------------------------------------------------------------------*/
  
  NXstatus  NXsetnumberformat (NXhandle fid, 
					    int type, char *format)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
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
 
  NXstatus  NXinitgroupdir (NXhandle fid)
  {
    pNexusFunction pFunc = handleToNexusFunc(fid);
    return pFunc->nxinitgroupdir(pFunc->pNexusData);
  }
/*----------------------------------------------------------------------*/
 NXstatus  NXinquirefile(NXhandle handle, char *filename, 
				int filenameBufferLength){
  pFileStack fileStack;
  char *pPtr = NULL;
  int length;

  fileStack = (pFileStack)handle;
  pPtr = peekFilenameOnStack(fileStack);
  if(pPtr != NULL){
    length = strlen(pPtr);
    if(length > filenameBufferLength){
      length = filenameBufferLength -1;
    }
    memset(filename,0,filenameBufferLength);
    memcpy(filename,pPtr, length);
    return NX_OK;
  } else {
    return NX_ERROR;
  }
}
/*------------------------------------------------------------------------*/
NXstatus  NXisexternalgroup(NXhandle fid, CONSTCHAR *name, CONSTCHAR *class, 
			    char *url, int urlLen){
  int status, attStatus, length = 1023, type = NX_CHAR;
  ErrFunc oldError;
  char nxurl[1024];

  pNexusFunction pFunc = handleToNexusFunc(fid);

  status = pFunc->nxopengroup(pFunc->pNexusData, name,class);
  if(status != NX_OK){
    return status;
  }
  oldError = NXMGetError();
  NXIReportError = NXNXNoReport;
  attStatus = NXgetattr(fid,"napimount",nxurl,&length, &type);
  NXIReportError = oldError;
  pFunc->nxclosegroup(pFunc->pNexusData);
  if(attStatus == NX_OK){
    length = strlen(nxurl);
    if(length > urlLen){
      length = urlLen - 1;
    }
    memset(url,0,urlLen);
    memcpy(url,nxurl,length);
    return attStatus;
  } else {
    return NX_ERROR;
  }
}
/*------------------------------------------------------------------------*/
NXstatus  NXlinkexternal(NXhandle fid, CONSTCHAR *name, CONSTCHAR *class, 
			 CONSTCHAR *url){
  int status, type = NX_CHAR, length;
  pNexusFunction pFunc = handleToNexusFunc(fid);

  status = pFunc->nxopengroup(pFunc->pNexusData,name,class);
  if(status != NX_OK){
    return status;
  }
  length = strlen(url);
  status = NXputattr(fid, "napimount",(void *)url,length, type);
  if(status != NX_OK){
    return status;
  }
  pFunc->nxclosegroup(pFunc->pNexusData);
  return NX_OK;
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
NXstatus  NXopenpath(NXhandle hfil, CONSTCHAR *path)
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
NXstatus  NXopengrouppath(NXhandle hfil, CONSTCHAR *path)
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

NXstatus NXIprintlink(NXhandle fid, NXlink* link)
{
     pNexusFunction pFunc = handleToNexusFunc(fid);
     return pFunc->nxprintlink(pFunc->pNexusData, link);   
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
  NXstatus  NXfopen(char * filename, NXaccess* am, 
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
  NXstatus  NXfclose (NexusFunction* pHandle)
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
  NXstatus  NXfflush(NexusFunction* pHandle)
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
  NXstatus  NXfmakedata(NXhandle fid, char *name, int *pDatatype,
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


  NXstatus  NXfcompmakedata(NXhandle fid, char *name, 
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
  NXstatus  NXfcompress(NXhandle fid, int *compr_type)
  { 
      return NXcompress(fid,*compr_type);
  }
/*-----------------------------------------------------------------------*/
  NXstatus  NXfputattr(NXhandle fid, char *name, void *data, 
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

/*
 * windows stdcall aliases
 */
#ifdef _WIN32

NXstatus __stdcall NXISETCACHE(long newVal)
{
    return NXsetcache(newVal);
}

void __stdcall NXNXNXREPORTERROR(void *pData, char *string)
{
    NXNXNXReportError(pData, string);
}
     
NXstatus __stdcall NXIOPEN(CONSTCHAR *filename, NXaccess am, NXhandle *gHandle)
{
    return NXopen(filename, am, gHandle);
}

NXstatus __stdcall NXICLOSE(NXhandle *fid)
{
    return NXclose(fid);
}

NXstatus __stdcall NXIFLUSH(NXhandle* pHandle)
{
    return NXflush(pHandle);
}

NXstatus __stdcall NXIMAKEGROUP(NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass)
{
    return NXmakegroup(handle, name, NXclass);
}

NXstatus __stdcall NXIOPENGROUP(NXhandle handle, CONSTCHAR *name, CONSTCHAR* NXclass)
{
    return NXopengroup(handle, name, NXclass);
}

NXstatus __stdcall NXIOPENPATH(NXhandle handle, CONSTCHAR *path)
{
    return NXopenpath(handle, path);
}

NXstatus __stdcall NXIOPENGROUPPATH (NXhandle handle, CONSTCHAR *path)
{
    return NXopengrouppath(handle, path);
}

NXstatus __stdcall NXICLOSEGROUP(NXhandle handle)
{
    return NXclosegroup(handle);
}
  
NXstatus __stdcall NXIMAKEDATA (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[])
{
    return NXmakedata (handle, label, datatype, rank, dim);
}

NXstatus __stdcall NXICOMPMAKEDATA (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[])
{
    return NXcompmakedata (handle, label, datatype, rank, dim, comp_typ, bufsize);
}

NXstatus __stdcall NXICOMPRESS (NXhandle handle, int compr_type)
{
    return NXcompress (handle, compr_type);
}

NXstatus __stdcall NXIOPENDATA (NXhandle handle, CONSTCHAR* label)
{
    return NXopendata (handle, label);
}

NXstatus __stdcall NXICLOSEDATA(NXhandle handle)
{
    return NXclosedata(handle);
}

NXstatus __stdcall NXIPUTDATA(NXhandle handle, void* data)
{
    return NXputdata(handle, data);
}

NXstatus __stdcall NXIPUTATTR(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType)
{
    return NXputattr(handle, name, data, iDataLen, iType);
}

NXstatus __stdcall NXIPUTSLAB(NXhandle handle, void* data, int start[], int size[])
{
    return NXputslab(handle, data, start, size);
}

NXstatus __stdcall NXIGETDATAID(NXhandle handle, NXlink* pLink)
{
    return NXgetdataID(handle, pLink);
}

NXstatus __stdcall NXIMAKELINK(NXhandle handle, NXlink* pLink)
{
    return NXmakelink(handle, pLink);
}

NXstatus __stdcall NXIOPENSOURCEGROUP(NXhandle handle)
{
    return NXopensourcegroup(handle);
}

NXstatus __stdcall NXIGETDATA(NXhandle handle, void* data)
{
    return NXgetdata(handle, data);
}

NXstatus __stdcall NXIGETINFO(NXhandle handle, int* rank, int dimension[], int* datatype)
{
    return NXgetinfo(handle, rank, dimension, datatype);
}

NXstatus __stdcall NXIGETNEXTENTRY(NXhandle handle, NXname name, NXname nxclass, int* datatype)
{
    return NXgetnextentry(handle, name, nxclass, datatype);
}

NXstatus __stdcall NXIGETSLAB(NXhandle handle, void* data, int start[], int size[])
{
    return NXgetslab(handle, data, start, size);
}

NXstatus __stdcall NXIGETNEXTATTR(NXhandle handle, NXname pName, int *iLength, int *iType)
{
    return NXgetnextattr(handle, pName, iLength, iType);
}

NXstatus __stdcall NXIGETATTR(NXhandle handle, char* name, void* data, int* iDataLen, int* iType)
{
    return NXgetattr(handle, name, data, iDataLen, iType);
}

NXstatus __stdcall NXIGETATTRINFO(NXhandle handle, int* no_items)
{
    return NXgetattrinfo(handle, no_items);
}

NXstatus __stdcall NXIGETGROUPID(NXhandle handle, NXlink* pLink)
{
    return NXgetgroupID(handle, pLink);
}

NXstatus __stdcall NXIGETGROUPINFO(NXhandle handle, int* no_items, NXname name, NXname nxclass)
{
    return NXgetgroupinfo(handle, no_items, name, nxclass);
}

NXstatus __stdcall NXISAMEID(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID)
{
    return NXsameID(handle, pFirstID, pSecondID);
}

NXstatus __stdcall NXIINITGROUPDIR(NXhandle handle)
{
    return  NXinitgroupdir(handle);
}
NXstatus __stdcall NXIINITATTRDIR(NXhandle handle)
{
    return  NXinitattrdir(handle);
}
NXstatus __stdcall NXISETNUMBERFORMAT(NXhandle handle, int type, char *format)
{
    return  NXsetnumberformat(handle,type, format);
}

NXstatus __stdcall NXIMALLOC(void** data, int rank, int dimensions[], int datatype)
{
    return NXmalloc(data, rank, dimensions, datatype);
}

NXstatus __stdcall NXIFREE(void** data)
{
    return NXfree(data);
}

#if 0
/*-----------------------------------------------------------------------
    NAPI internals 
------------------------------------------------------------------------*/
extern  void  NXMSetError(void *pData, void (*ErrFunc)(void *pD, char *text));
extern void (*NXIReportError)(void *pData,char *text);
extern void *NXpData;
extern char *NXIformatNeXusTime();
#endif

/* FORTRAN internals */

NXstatus __stdcall NXIFOPEN(char * filename, NXaccess* am, 
					NexusFunction* pHandle)
{
    return NXfopen(filename, am, pHandle);
}

NXstatus __stdcall NXIFCLOSE (NexusFunction* pHandle)
{
  return  NXfclose (pHandle);
}

NXstatus __stdcall NXIFPUTATTR(NXhandle fid, char *name, void *data, 
                                   int *pDatalen, int *pIType)
{
  return  NXfputattr(fid, name, data, pDatalen, pIType);
}

NXstatus __stdcall NXIFCOMPRESS(NXhandle fid, int *compr_type)
{
  return  NXfcompress(fid, compr_type);
}

NXstatus __stdcall NXIFCOMPMAKEDATA(NXhandle fid, char *name, 
                int *pDatatype,
		int *pRank, int dimensions[],
                int *compression_type, int chunk[])
{
  return  NXfcompmakedata(fid, name, pDatatype, pRank, dimensions,
                compression_type, chunk);
}

NXstatus __stdcall NXIFMAKEDATA(NXhandle fid, char *name, int *pDatatype,
		int *pRank, int dimensions[])
{
  return  NXfmakedata(fid, name, pDatatype, pRank, dimensions);
}

NXstatus __stdcall NXIFFLUSH(NexusFunction* pHandle)
{
  return NXfflush(pHandle);
}

#endif /* _WIN32 */
