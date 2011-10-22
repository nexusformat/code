/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface (HDF5) Routines
  
  Copyright (C) 1997-2011 Mark Koennecke, Przemek Klosowski
  
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

#ifdef HDF5 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "napi.h"
#include "napi5.h"

#define NX_UNKNOWN_GROUP "" /* for when no NX_class attr */

extern  void *NXpData;

  typedef struct __NexusFile5 {
        struct iStack5 {
          char irefn[1024];
          int iVref;
          hsize_t iCurrentIDX;
        } iStack5[NXMAXSTACK];
        struct iStack5 iAtt5;
        int iFID;
        int iCurrentG;
        int iCurrentD;
        int iCurrentS;
        int iCurrentT;
        int iCurrentA;
        int iNX;
        int iNXID;
        int iStackPtr;
        char *iCurrentLGG;
        char *iCurrentLD;
        char name_ref[1024];
        char name_tmp[1024];
        char iAccess[2];
  } NexusFile5, *pNexusFile5;

/* 
   forward declaration of NX5closegroup in order to get rid of a nasty
   warning
*/
NXstatus  NX5closegroup (NXhandle fid);
/*-------------------------------------------------------------------*/

  static pNexusFile5 NXI5assert(NXhandle fid)
  {
    pNexusFile5 pRes;
  
    assert(fid != NULL);
    pRes = (pNexusFile5)fid;
    assert(pRes->iNXID == NX5SIGNATURE);
    return pRes;
  }
  
  /*--------------------------------------------------------------------*/

   static void NXI5KillDir (pNexusFile5 self)
  {
    self->iStack5[self->iStackPtr].iCurrentIDX = 0;
  }
  
  /*--------------------------------------------------------------------*/

  static void NXI5KillAttDir (pNexusFile5 self)
  {
    self->iAtt5.iCurrentIDX = 0;
  }
/*---------------------------------------------------------------------*/
static void buildCurrentPath(pNexusFile5 self, char *pathBuffer, 
			     int pathBufferLen){

  memset(pathBuffer,0,pathBufferLen);
  if(self->iCurrentG != 0) {
    strcpy(pathBuffer,"/");
    if(strlen(self->name_ref) + 1 < pathBufferLen){
      strcat(pathBuffer, self->name_ref);
    }
  }
  if(self->iCurrentD != 0){
    strcat(pathBuffer,"/");
    if(strlen(self->iCurrentLD) + strlen(pathBuffer) < pathBufferLen){
      strcat(pathBuffer,self->iCurrentLD);
    }
  }
}
  /* ---------------------------------------------------------------------- 
  
                          Definition of NeXus API

   ---------------------------------------------------------------------*/

  NXstatus  NX5open(CONSTCHAR *filename, NXaccess am, 
				 NXhandle* pHandle)
  {
  hid_t attr1,aid1, aid2, iVID;
  pNexusFile5 pNew = NULL;
  char pBuffer[512];
  char *time_buffer = NULL;
  char version_nr[10];
  unsigned int vers_major, vers_minor, vers_release, am1 ;
  hid_t fapl = -1;     
  int mdc_nelmts;
  size_t rdcc_nelmts;
  size_t rdcc_nbytes;
  double rdcc_w0;

  /* mask of any options for now */
  am = (NXaccess)(am & NXACCMASK_REMOVEFLAGS);

  /* turn off the automatic HDF error handling */  
     H5Eset_auto(NULL,NULL); 
#ifdef USE_FTIME
    struct timeb timeb_struct;
#endif 

    *pHandle = NULL;

    pNew = (pNexusFile5) malloc (sizeof (NexusFile5));
    if (!pNew) {
      NXReportError("ERROR: not enough memory to create file structure");
      return NX_ERROR;
    }
    memset (pNew, 0, sizeof (NexusFile5));


    /* start HDF5 interface */
    if (am == NXACC_CREATE5) {  
       fapl = H5Pcreate(H5P_FILE_ACCESS);
       H5Pget_cache(fapl,&mdc_nelmts,&rdcc_nelmts,&rdcc_nbytes,&rdcc_w0);
       rdcc_nbytes=(size_t)nx_cacheSize;
       H5Pset_cache(fapl,mdc_nelmts,rdcc_nelmts,rdcc_nbytes,rdcc_w0);
       H5Pset_fclose_degree(fapl,H5F_CLOSE_STRONG);
       am1 = H5F_ACC_TRUNC;
       pNew->iFID = H5Fcreate (filename, am1, H5P_DEFAULT, fapl);
    } else {
       if (am == NXACC_READ) {
          am1 = H5F_ACC_RDONLY;
        } else {
          am1 = H5F_ACC_RDWR;
        }    
        fapl = H5Pcreate(H5P_FILE_ACCESS);
	H5Pset_fclose_degree(fapl,H5F_CLOSE_STRONG);
        pNew->iFID = H5Fopen (filename, am1, fapl);
    }  
    if(fapl != -1) {
      H5Pclose(fapl);
    }
    if (pNew->iFID <= 0) {
      sprintf (pBuffer, "ERROR: cannot open file: %s", filename);
      NXReportError( pBuffer);
      free (pNew);
      return NX_ERROR;
    }

/*
 * need to create global attributes         file_name file_time NeXus_version 
 * at some point for new files
 */
    if (am1 != H5F_ACC_RDONLY) 
    {
        iVID=H5Gopen(pNew->iFID,"/");
        aid2 = H5Screate(H5S_SCALAR);
        aid1 = H5Tcopy(H5T_C_S1);
        H5Tset_size(aid1, strlen(NEXUS_VERSION));
        if (am1 == H5F_ACC_RDWR)
        {
        H5Adelete(iVID, "NeXus_version"); 
        }
        attr1= H5Acreate(iVID, "NeXus_version", aid1, aid2, H5P_DEFAULT, H5P_DEFAULT);
        if (attr1<0)
        {
          NXReportError( 
                         "ERROR: failed to store NeXus_version attribute ");
          return NX_ERROR;
        } 
        if (H5Awrite(attr1, aid1,NEXUS_VERSION)<0)
        {
          NXReportError( 
                         "ERROR: failed to store NeXus_version attribute ");
          return NX_ERROR;
        }
       /* Close attribute dataspace  */
       H5Tclose(aid1);
       H5Sclose(aid2); 
       /* Close attribute */
       H5Aclose(attr1); 
       H5Gclose(iVID); 
    }
    if (am1 == H5F_ACC_TRUNC) 
    {
        iVID=H5Gopen(pNew->iFID,"/");
        aid2=H5Screate(H5S_SCALAR);
        aid1 = H5Tcopy(H5T_C_S1);
        H5Tset_size(aid1, strlen(filename));
        attr1= H5Acreate(iVID, "file_name", aid1, aid2, H5P_DEFAULT, H5P_DEFAULT);
        if (attr1 < 0)
        {
          NXReportError( 
			  "ERROR: failed to store file_name attribute ");
          return NX_ERROR;
        }
        if (H5Awrite(attr1, aid1, (char*)filename) < 0)
        {
          NXReportError( 
			  "ERROR: failed to store file_name attribute ");
          return NX_ERROR;
        }
	H5Tclose(aid1);
	H5Sclose(aid2); 
        H5Aclose(attr1);
	/*  ------- library version ------*/
        H5get_libversion(&vers_major, &vers_minor, &vers_release);
        sprintf (version_nr, "%d.%d.%d", vers_major,vers_minor,vers_release);
        aid2=H5Screate(H5S_SCALAR);
        aid1 = H5Tcopy(H5T_C_S1);
        H5Tset_size(aid1, strlen(version_nr));
        attr1= H5Acreate(iVID, "HDF5_Version", aid1, aid2, H5P_DEFAULT, H5P_DEFAULT);
        if (attr1 < 0)
        {
          NXReportError( 
			  "ERROR: failed to store file_name attribute ");
          return NX_ERROR;
        }
        if (H5Awrite(attr1, aid1, (char*)version_nr) < 0)
        {
          NXReportError( 
			  "ERROR: failed to store file_name attribute ");
          return NX_ERROR;
        }
	H5Tclose(aid1);
	H5Sclose(aid2); 
        H5Aclose(attr1);
	/*----------- file time */
	time_buffer = NXIformatNeXusTime();
	if(time_buffer != NULL){
	  aid2=H5Screate(H5S_SCALAR);
	  aid1 = H5Tcopy(H5T_C_S1);
	  H5Tset_size(aid1, strlen(time_buffer));
	  attr1=H5Acreate(iVID, "file_time", aid1, aid2, H5P_DEFAULT, H5P_DEFAULT);
	  if (attr1 < 0)
	  {
	      NXReportError( 
			  "ERROR: failed to store file_time attribute ");
	      free(time_buffer);
	      return NX_ERROR;
	  }
	  if (H5Awrite(attr1, aid1, time_buffer) < 0)
	  {
	      NXReportError( 
			  "ERROR: failed to store file_time attribute ");
	      free(time_buffer);
	      return NX_ERROR;
	  }
	  /* Close attribute dataspace */
	  H5Tclose(aid1);
	  H5Sclose(aid2); 
	  /* Close attribute */
	  H5Aclose(attr1); 
	  free(time_buffer);
	}
	H5Gclose(iVID); 
    }
    /* Set HDFgroup access mode */
    if (am1 == H5F_ACC_RDONLY) {
      strcpy(pNew->iAccess,"r");
    } else {
      strcpy(pNew->iAccess,"w");
    }
    pNew->iNXID = NX5SIGNATURE;
    pNew->iStack5[0].iVref = 0;    /* root! */
    *pHandle = (NXhandle)pNew;
     return NX_OK;
  }
 
  /* ------------------------------------------------------------------------- */

  NXstatus  NX5close (NXhandle* fid)
  {
    pNexusFile5 pFile = NULL;
    herr_t iRet;
 
    pFile=NXI5assert(*fid);

    iRet=0;
    /*
    printf("HDF5 object count before close: %d\n",
	   H5Fget_obj_count(pFile->iFID,H5F_OBJ_ALL));
    */
    iRet = H5Fclose(pFile->iFID);

    /* 
      leave this here: it helps in debugging leakage problems   
    printf("HDF5 object count after close: %d\n",
	   H5Fget_obj_count(H5F_OBJ_ALL,H5F_OBJ_ALL));
    printf("HDF5 dataset count after close: %d\n",
	   H5Fget_obj_count(H5F_OBJ_ALL,H5F_OBJ_DATASET));
    printf("HDF5 group count after close: %d\n",
	   H5Fget_obj_count(H5F_OBJ_ALL,H5F_OBJ_GROUP));
    printf("HDF5 datatype count after close: %d\n",
	   H5Fget_obj_count(H5F_OBJ_ALL,H5F_OBJ_DATATYPE));
    printf("HDF5 attribute count after close: %d\n",
	   H5Fget_obj_count(H5F_OBJ_ALL,H5F_OBJ_ATTR));
    */

    if (iRet < 0) {
      NXReportError( "ERROR: cannot close HDF file");
    }
    /* release memory */
    NXI5KillDir (pFile);
    if(pFile->iCurrentLGG != NULL){
      free(pFile->iCurrentLGG);
    }
    if(pFile->iCurrentLD != NULL){
      free(pFile->iCurrentLD);
    }
    free (pFile);
    *fid = NULL;
    H5garbage_collect();
    return NX_OK;
  }

 /*-----------------------------------------------------------------------*/   

  NXstatus  NX5makegroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass) 
  {
    pNexusFile5 pFile;
    herr_t iRet;
    hid_t iVID;
    hid_t attr1,aid1, aid2;
    char pBuffer[1024] = "";
    
    pFile = NXI5assert (fid);
    /* create and configure the group */
    if (pFile->iCurrentG==0)
    {
       iRet = H5Gcreate(pFile->iFID,(const char*)name, 0);
       snprintf(pBuffer,1023,"/%s",name);
    } else
    {
       snprintf(pBuffer,1023,"/%s/%s",pFile->name_ref,name);
       iRet = H5Gcreate(pFile->iFID,(const char*)pBuffer, 0);
    }   
    if (iRet < 0) {
      NXReportError( "ERROR: could not create Group");
      return NX_ERROR;
    }
    iVID = iRet;
    aid2 = H5Screate(H5S_SCALAR);
    aid1 = H5Tcopy(H5T_C_S1);
    H5Tset_size(aid1, strlen(nxclass));
    attr1= H5Acreate(iVID, "NX_class", aid1, aid2, H5P_DEFAULT, H5P_DEFAULT);
    if (attr1 < 0)
       {
       NXReportError( "ERROR: failed to store class name");
       return NX_ERROR;
       }
    if (H5Awrite(attr1, aid1, (char*)nxclass) < 0)
      {
      NXReportError( "ERROR: failed to store class name");
      return NX_ERROR;
      }
    /* close group */
    iRet=H5Sclose(aid2);
    iRet=H5Tclose(aid1);
    iRet=H5Aclose(attr1);
    iRet=H5Gclose(iVID);
    return NX_OK;
  }
  
  /*------------------------------------------------------------------------*/

  herr_t attr_check (hid_t loc_id, const char *member_name, const H5A_info_t *unused, void *opdata)
  {
    char attr_name[8+1];   /* need to leave space for \0 as well */
    
    strcpy(attr_name,"NX_class");
    return strstr(member_name, attr_name) ? 1 : 0;
  }
  /*------------------------------------------------------------------------*/
  NXstatus  NX5opengroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass)
  {

    pNexusFile5 pFile;
    hid_t attr1, atype;
    herr_t iRet;
    char pBuffer[1024];
    char data[128];
          
    pFile = NXI5assert (fid);
    if (pFile->iCurrentG == 0)
    {
      strcpy(pBuffer,name);
    } 
    else 
    {
       sprintf(pBuffer,"%s/%s",pFile->name_tmp,name);
    }
    iRet = H5Gopen (pFile->iFID,(const char *)pBuffer);
    if (iRet < 0) {
      sprintf (pBuffer, "ERROR: group %s does not exist", pFile->name_tmp);
      NXReportError( pBuffer);
      return NX_ERROR;  
    }
    pFile->iCurrentG = iRet;
    strcpy(pFile->name_tmp,pBuffer);
    strcpy(pFile->name_ref,pBuffer);

    if ((nxclass != NULL) && (strcmp(nxclass, NX_UNKNOWN_GROUP) != 0))
    {
        /* check group attribute */
        iRet=H5Aiterate(pFile->iCurrentG,H5_INDEX_CRT_ORDER,H5_ITER_INC,0,attr_check,NULL);
        if (iRet < 0) {
          NXReportError( "ERROR: iterating through attribute list");
          return NX_ERROR;  
        } else if (iRet == 1) {
         /* group attribute was found */
        } else {
         /* no group attribute available */
         NXReportError( "ERROR: no group attribute available");
         return NX_ERROR;
        }
        /* check contents of group attribute */
        attr1 = H5Aopen_name(pFile->iCurrentG, "NX_class");
        if (attr1 < 0)
        {
              NXReportError( "ERROR: opening NX_class group attribute");
              return NX_ERROR; 
        }
        atype=H5Tcopy(H5T_C_S1);
        H5Tset_size(atype,sizeof(data));  
        iRet = H5Aread(attr1, atype, data);
        if (strcmp(data, nxclass) == 0) {
              /* test OK */
        } else {
              NXReportError( "ERROR: group class is not identical");
              iRet = H5Tclose(atype);
              iRet = H5Aclose(attr1); 
              return NX_ERROR; 
        }          
        iRet = H5Tclose(atype);
        iRet = H5Aclose(attr1); 
    }

    /* maintain stack */
    pFile->iStackPtr++;
    pFile->iStack5[pFile->iStackPtr].iVref=pFile->iCurrentG;
    strcpy(pFile->iStack5[pFile->iStackPtr].irefn,name);
    pFile->iAtt5.iCurrentIDX=0;
    pFile->iCurrentD = 0;
    if(pFile->iCurrentLGG != NULL){
      free(pFile->iCurrentLGG);
    }
    pFile->iCurrentLGG = strdup(name);
    NXI5KillDir (pFile);
    return NX_OK;
  }

  /* ------------------------------------------------------------------- */

  NXstatus  NX5closegroup (NXhandle fid)
  {
    pNexusFile5 pFile;
    int i,ii;
    char *uname = NULL;
    char *u1name = NULL;
    
    pFile = NXI5assert (fid);
    /* first catch the trivial case: we are at root and cannot get 
       deeper into a negative directory hierarchy (anti-directory)
     */
    if (pFile->iCurrentG == 0) {
      NXI5KillDir (pFile);
      return NX_OK;
    } else {                      
      /* close the current group and decrement name_ref */
      H5Gclose (pFile->iCurrentG);
      i = 0;
      i = strlen(pFile->iStack5[pFile->iStackPtr].irefn);
      ii = strlen(pFile->name_ref);
      if (pFile->iStackPtr>1) {
         ii=ii-i-1;
      } else {
          ii=ii-i;
      }       
      if (ii>0) {
        uname = strdup(pFile->name_ref);  
        u1name = (char*) malloc((ii+1)*sizeof(char));
        memset(u1name,0,ii);
        for (i=0; i<ii; i++) {
           *(u1name + i) = *(uname + i);
        }
        *(u1name + i) = '\0';
        /*
        strncpy(u1name, uname, ii);
        */
        strcpy(pFile->name_ref,u1name);
        strcpy(pFile->name_tmp,u1name);
        free(uname);
        free(u1name);
      } else {
        strcpy(pFile->name_ref,"");
        strcpy(pFile->name_tmp,"");
      }
      NXI5KillDir (pFile);
      pFile->iStackPtr--;
      if (pFile->iStackPtr>0) {
         pFile->iCurrentG=pFile->iStack5[pFile->iStackPtr].iVref;
      } else {
         pFile->iCurrentG=0;
      }
    }
    return NX_OK;
  }
/*-----------------------------------------------------------------------*/
static hid_t nxToHDF5Type(int datatype)
{
  hid_t type;
    if (datatype == NX_CHAR)
    {
        type=H5T_C_S1;
    }
    else if (datatype == NX_INT8)
    {
        type=H5T_NATIVE_CHAR;
    }
    else if (datatype == NX_UINT8)
    {
        type=H5T_NATIVE_UCHAR;
    }
    else if (datatype == NX_INT16)
    {
        type=H5T_NATIVE_SHORT;
    }
    else if (datatype == NX_UINT16)
    {
        type=H5T_NATIVE_USHORT;
    }
    else if (datatype == NX_INT32)
    {
        type=H5T_NATIVE_INT;
    }
    else if (datatype == NX_UINT32)
    {
        type=H5T_NATIVE_UINT;
    }
    else if (datatype == NX_INT64)
    {
	type = H5T_NATIVE_INT64;
    }
    else if (datatype == NX_UINT64)
    {
	type = H5T_NATIVE_UINT64;
    }
    else if (datatype == NX_FLOAT32)
    {
        type=H5T_NATIVE_FLOAT;
    }
    else if (datatype == NX_FLOAT64)
    {
        type=H5T_NATIVE_DOUBLE;
    }
    else
    {
      NXReportError( "ERROR: nxToHDF5Type: unknown type");
      type = -1;
    }
    return type;
}
  
 /* --------------------------------------------------------------------- */

  NXstatus  NX5compmakedata64 (NXhandle fid, CONSTCHAR *name, 
					  int datatype, 
					  int rank, int64_t dimensions[],
					  int compress_type, int64_t chunk_size[])
  {
      hid_t datatype1, dataspace, iNew;
      herr_t iRet;
      hid_t type, cparms = -1;
      pNexusFile5 pFile;
      char pBuffer[256];
      int i, byte_zahl = 0;
      hsize_t chunkdims[H5S_MAX_RANK];
      hsize_t mydim[H5S_MAX_RANK], mydim1[H5S_MAX_RANK];  
      hsize_t size[H5S_MAX_RANK];
      hsize_t maxdims[H5S_MAX_RANK];
      int compress_level;
      int unlimiteddim = 0;

      pFile = NXI5assert (fid);
      if (pFile->iCurrentG <= 0)
      {
          sprintf (pBuffer, "ERROR: no group open for makedata on %s",
              name);
          NXReportError( pBuffer);
          return NX_ERROR;
      }

      if (rank <= 0) 
      {
          sprintf (pBuffer, "ERROR: invalid rank specified %s",
              name);
          NXReportError( pBuffer);
          return NX_ERROR;
      }

      type = nxToHDF5Type(datatype);

      /*
      Check dimensions for consistency. Dimension may be -1
      thus denoting an unlimited dimension.
      */
      for (i = 0; i < rank; i++)
      {
          chunkdims[i] = chunk_size[i];
          mydim[i] = dimensions[i];
          maxdims[i] = dimensions[i];
          size[i] = dimensions[i];
          if (dimensions[i] <= 0) 
          {
		mydim[i] = 1;
		maxdims[i] = H5S_UNLIMITED;
                size[i] = 1;
		unlimiteddim = 1;
          } else {
                mydim[i] = dimensions[i];
                maxdims[i] = dimensions[i];
                size[i] = dimensions[i];
          }
      }

      if (datatype == NX_CHAR)
      {
          /* 
          *  This assumes string lenght is in the last dimensions and
          *  the logic must be the same as used in NX5getslab and NX5getinfo
          *
          *  search for tests on H5T_STRING
          */
          byte_zahl=mydim[rank-1]; 
          for(i = 0; i < rank; i++)
          {
              mydim1[i] = mydim[i];
		if (dimensions[i] <= 0) 
		  {
	              mydim1[0] = 1;
       		       maxdims[0] = H5S_UNLIMITED;
		 }

          }
          mydim1[rank-1] = 1;
          if (mydim[rank-1] > 1)
          {
              mydim[rank-1] = maxdims[rank-1] = size[rank-1] = 1;
          }
          if (chunkdims[rank-1] > 1) 
          { 
              chunkdims[rank-1] = 1; 
          }
          dataspace=H5Screate_simple(rank,mydim1,maxdims);
      } 
      else 
      {
	  if (unlimiteddim)
	  {
              dataspace=H5Screate_simple(rank, mydim, maxdims);
          } 
          else 
          {
              /* dataset creation */
              dataspace=H5Screate_simple(rank, mydim, NULL);  
          }
      }  
      datatype1=H5Tcopy(type);
      if (datatype == NX_CHAR)
      {
          H5Tset_size(datatype1, byte_zahl);
          /*       H5Tset_strpad(H5T_STR_SPACEPAD); */
      }
      compress_level = 6;
      if ( (compress_type / 100) ==  NX_COMP_LZW )
      {
          compress_level = compress_type % 100;
          compress_type = NX_COMP_LZW;
      }
      if(compress_type == NX_COMP_LZW)
      {
          cparms = H5Pcreate(H5P_DATASET_CREATE);
          iNew = H5Pset_chunk(cparms,rank,chunkdims);
          if (iNew < 0) 
          {
              NXReportError( "ERROR: size of chunks could not be set");
              return NX_ERROR;
          }
          H5Pset_deflate(cparms,compress_level); 
          iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, dataspace, cparms);   
      } 
      else if (compress_type == NX_COMP_NONE) 
      {
          if (unlimiteddim)
          {
              cparms = H5Pcreate(H5P_DATASET_CREATE);
              iNew = H5Pset_chunk(cparms,rank,chunkdims);
              if (iNew < 0) 
              {
                  NXReportError( "ERROR: size of chunks could not be set");
                  return NX_ERROR;
              }
              iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, dataspace, cparms);   
          } 
          else 
          {
              iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, dataspace, H5P_DEFAULT);
          }               
      } 
      else if (compress_type == NX_CHUNK) 
      {
          cparms = H5Pcreate(H5P_DATASET_CREATE);
          iNew = H5Pset_chunk(cparms,rank,chunkdims);
          if (iNew < 0) 
          {
              NXReportError( "ERROR: size of chunks could not be set");
              return NX_ERROR;
          }
          iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, dataspace, cparms);   

      } 
      else 
      {
          NXReportError( 
              "HDF5 doesn't support selected compression method! Dataset was saved without compression");
          iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, dataspace, H5P_DEFAULT); 
      }
      if (iRet < 0) 
      {
          NXReportError( "ERROR: creating chunked dataset failed");
          return NX_ERROR;
      } 
      else 
      {
          pFile->iCurrentD = iRet;
      }
      if (unlimiteddim)
      {      
          iNew = H5Dset_extent  (pFile->iCurrentD, size);
          if (iNew < 0) 
          {
              sprintf (pBuffer, "ERROR: cannot create Dataset %s",
                  name);
              NXReportError( pBuffer);
              return NX_ERROR;
          }
      }
      if (cparms != -1)
      {
          iRet = H5Pclose(cparms);
      }
      iRet = H5Sclose(dataspace);
      iRet = H5Tclose(datatype1);
      iRet = H5Dclose(pFile->iCurrentD);
      pFile->iCurrentD = 0;
      if (iRet < 0)
      {
          NXReportError( "ERROR: HDF cannot close Dataset");
          return NX_ERROR;
      }
      return NX_OK;
  }


  /* --------------------------------------------------------------------- */

  NXstatus  NX5makedata64 (NXhandle fid, CONSTCHAR *name, int datatype, 
                                  int rank, int64_t dimensions[])
  {
  int64_t chunk_size[H5S_MAX_RANK];
  int i;
   
  NXI5assert(fid);
    
  memset(chunk_size,0,H5S_MAX_RANK*sizeof(int64_t));
  memcpy(chunk_size,dimensions,rank*sizeof(int64_t));
  for(i = 0; i< rank; i++) {
  if (dimensions[i] == NX_UNLIMITED){
         chunk_size[i]= 1;
  }    
  }
  return NX5compmakedata64 (fid, name, datatype, rank, dimensions, NX_COMP_NONE, chunk_size);
  }

  
  /* --------------------------------------------------------------------- */

  NXstatus  NX5compress (NXhandle fid, int compress_type)
  {
    printf(" NXcompress ERROR: NeXus API  based  on  HDF5  doesn't support\n");
    printf("                   NXcompress  function!  Using  HDF5 library,\n");
    printf("                   the NXcompmakedata function can be applied\n"); 
    printf("                   for compression of data!\n");
    return NX_ERROR;
  }  

  /* --------------------------------------------------------------------- */

  NXstatus  NX5opendata (NXhandle fid, CONSTCHAR *name)
  {
    pNexusFile5 pFile;
    char pBuffer[256];
      
    pFile = NXI5assert (fid);
    /* clear pending attribute directories first */
    NXI5KillAttDir (pFile);


    /* find the ID number and open the dataset */
    pFile->iCurrentD = H5Dopen(pFile->iCurrentG, name);
    if (pFile->iCurrentD < 0) {
      sprintf (pBuffer, "ERROR: Dataset \"%s\" not found at this level", name);
      NXReportError( pBuffer);
      return NX_ERROR;
    }
     /* find the ID number of datatype */
    pFile->iCurrentT = H5Dget_type(pFile->iCurrentD);
    if (pFile->iCurrentT < 0) {
      NXReportError( "ERROR: error opening Dataset");
      pFile->iCurrentT=0;
      return NX_ERROR;
    }
    /* find the ID number of dataspace */
    pFile->iCurrentS = H5Dget_space(pFile->iCurrentD);
    if (pFile->iCurrentS < 0) {
      NXReportError( "ERROR:HDF error opening Dataset");
      pFile->iCurrentS=0;
      return NX_ERROR;
    }
    if(pFile->iCurrentLD != NULL){
      free(pFile->iCurrentLD);
    }
    pFile->iCurrentLD = strdup(name); 
    return NX_OK;
  }
  
  /* ----------------------------------------------------------------- */

  NXstatus  NX5closedata (NXhandle fid)
  {
    pNexusFile5 pFile;
    herr_t iRet;
  
    pFile = NXI5assert (fid);
    iRet = H5Sclose(pFile->iCurrentS);
    iRet = H5Tclose(pFile->iCurrentT);
    iRet = H5Dclose(pFile->iCurrentD);
    if (iRet < 0) {
        NXReportError( "ERROR: cannot end access to Dataset");
        return NX_ERROR;
     }
    pFile->iCurrentD=0;
    return NX_OK;
  }
  
  /* ------------------------------------------------------------------- */
  


  NXstatus  NX5putdata (NXhandle fid, void *data)
  {
    pNexusFile5 pFile;
    herr_t iRet;
    
    char pError[512] = "";
      
    pFile = NXI5assert (fid);
    
    /* actually write */
    iRet = H5Dwrite (pFile->iCurrentD, pFile->iCurrentT, H5S_ALL, H5S_ALL, 
                     H5P_DEFAULT, data);
    if (iRet < 0) {
      snprintf (pError,511, "ERROR: failure to write data");
      NXReportError( pError);
      return NX_ERROR;
    }
    return NX_OK;
  }
/*------------------------------------------------------------------*/
static int getAttVID(pNexusFile5 pFile){
  int vid;
     if(pFile->iCurrentG == 0 && pFile->iCurrentD == 0){
       /* global attribute */
       vid = H5Gopen(pFile->iFID,"/");
     } else if(pFile->iCurrentD != 0) {
       /* dataset attribute */
       vid = pFile->iCurrentD;
     } else {
       /* group attribute */;
       vid = pFile->iCurrentG;
     }
     return vid;
}
/*---------------------------------------------------------------*/
static void killAttVID(pNexusFile5 pFile, int vid){
  if(pFile->iCurrentG == 0 && pFile->iCurrentD == 0){
    H5Gclose(vid);
  }
}
  /* ------------------------------------------------------------------- */

  NXstatus  NX5putattr (NXhandle fid, CONSTCHAR *name, void *data, 

			int datalen, int iType)
  {
    pNexusFile5 pFile;
    hid_t  attr1, aid1, aid2;
    hid_t type;
    herr_t iRet;
    int vid;  

    pFile = NXI5assert (fid);

    type = nxToHDF5Type(iType);
   
    /* determine vid */
    vid = getAttVID(pFile);
    aid2=H5Screate(H5S_SCALAR);
    aid1=H5Tcopy(type);
    if (iType == NX_CHAR){
      H5Tset_size(aid1,datalen); 
    }         
    iRet = H5Aopen_name(vid, name);
    if (iRet>0) {
      H5Aclose(iRet);
      iRet=H5Adelete(vid,name);
      if (iRet<0) {
	NXReportError( "ERROR: old attribute cannot be removed! ");
	killAttVID(pFile,vid);
	return NX_ERROR;
      }
    }    
    attr1 = H5Acreate(vid, name, aid1, aid2, H5P_DEFAULT, H5P_DEFAULT);
    if (attr1 < 0) {
      NXReportError( "ERROR: attribute cannot created! ");
      killAttVID(pFile,vid);
      return NX_ERROR;
    }   
    if (H5Awrite(attr1,aid1,data) < 0) {
      NXReportError( "ERROR: failed to store attribute ");
      killAttVID(pFile,vid);
      return NX_ERROR;
    }
    /* Close attribute dataspace */
    iRet=H5Tclose(aid1);
    iRet=H5Sclose(aid2); 
    /* Close attribute  */
    iRet=H5Aclose(attr1); 
    killAttVID(pFile,vid);
    return NX_OK;
  }
  
  /* ------------------------------------------------------------------- */
 
  NXstatus  NX5putslab64 (NXhandle fid, void *data, int64_t iStart[], int64_t iSize[])
  {
    pNexusFile5 pFile;
    int iRet, i;
    int rank;
    hsize_t myStart[H5S_MAX_RANK];
    hsize_t mySize[H5S_MAX_RANK];
    hsize_t size[H5S_MAX_RANK],thedims[H5S_MAX_RANK],maxdims[H5S_MAX_RANK];
    hid_t   filespace,dataspace; 
    int unlimiteddim = 0;
  
    pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
    if (pFile->iCurrentD == 0) {
      NXReportError( "ERROR: no dataset open");
      return NX_ERROR;
    }
    rank = H5Sget_simple_extent_ndims(pFile->iCurrentS);    
    iRet = H5Sget_simple_extent_dims(pFile->iCurrentS, thedims, maxdims);
    for(i = 0; i < rank; i++)
    {
       myStart[i] = iStart[i];
       mySize[i]  = iSize[i];
       size[i]    = iSize[i];
       if (maxdims[i] == NX_UNLIMITED) {
	unlimiteddim = 1;
	size[i] = iStart[i] + iSize[i];
       }
    }
    if (H5Tget_class(pFile->iCurrentT) == H5T_STRING)
    {
        mySize[rank - 1] = 1;
        myStart[rank - 1] = 0;
        size[rank - 1] = 1;
    }
    dataspace = H5Screate_simple(rank, mySize, NULL);
    if (unlimiteddim)
    {
       for(i = 0; i < rank; i++)
       {
	if (size[i] < thedims[i]) {
		size[i] = thedims[i];
	}
	} 
       iRet = H5Dset_extent(pFile->iCurrentD, size);
       if (iRet < 0) 
       {
           NXReportError( "ERROR: extend slab failed");
           return NX_ERROR;
       }

       filespace = H5Dget_space(pFile->iCurrentD);
       
       /* define slab */
       iRet = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, myStart,
                               NULL, mySize, NULL);
       /* deal with HDF errors */
       if (iRet < 0) 
       {
       NXReportError( "ERROR: selecting slab failed");
       return NX_ERROR;
       }
       /* write slab */ 
       iRet = H5Dwrite(pFile->iCurrentD, pFile->iCurrentT, dataspace, 
                    filespace, H5P_DEFAULT, data);
       if (iRet < 0)
       {
           NXReportError( "ERROR: writing slab failed");
       }
       iRet = H5Sclose(filespace);             
   } else {
       /* define slab */
       iRet = H5Sselect_hyperslab(pFile->iCurrentS, H5S_SELECT_SET, myStart,
                               NULL, mySize, NULL);
       /* deal with HDF errors */
       if (iRet < 0) 
       {
       NXReportError( "ERROR: selecting slab failed");
       return NX_ERROR;
       }
       /* write slab */ 
       iRet = H5Dwrite(pFile->iCurrentD, pFile->iCurrentT, dataspace, 
                    pFile->iCurrentS, H5P_DEFAULT, data);
       if (iRet < 0)
       {
           NXReportError( "ERROR: writing slab failed");
       }
   }
   /* deal with HDF errors */
   iRet = H5Sclose(dataspace);
   if (iRet < 0) 
   {
      NXReportError( "ERROR: closing slab failed");
      return NX_ERROR;
   }
    return NX_OK;
  }
 
  /* ------------------------------------------------------------------- */

  NXstatus  NX5getdataID (NXhandle fid, NXlink* sRes)
  {
    pNexusFile5 pFile;
    int datalen, type = NX_CHAR;
  
    pFile = NXI5assert (fid);

    /*
      we cannot return ID's when no datset is open
    */
    if(pFile->iCurrentD <= 0){
      return NX_ERROR;
    }

    /*
      this means: if the item is already linked: use the target attribute else, 
      the path to the current node
    */
    NXMDisableErrorReporting();
    datalen = 1024;
    memset(&sRes->targetPath,0,datalen*sizeof(char));
    if(NX5getattr(fid,"target",&sRes->targetPath,&datalen,&type) != NX_OK)
    {
      buildCurrentPath(pFile, sRes->targetPath, 1024);
    }
    NXMEnableErrorReporting();
    sRes->linkType = 1;
    return NX_OK;
  }

 /* ------------------------------------------------------------------- */
 
  NXstatus  NX5printlink (NXhandle fid, NXlink* sLink)
  {
    NXI5assert(fid);
    printf("HDF5 link: targetPath = \"%s\", linkType = \"%d\"\n", sLink->targetPath, sLink->linkType);
    return NX_OK;
  }
/*--------------------------------------------------------------------*/
static NXstatus NX5settargetattribute(pNexusFile5 pFile, NXlink *sLink)
{
  hid_t  dataID, aid2, aid1, attID;
  herr_t status;
  int type = NX_CHAR;
  char name[] = "target";

    /*
      set the target attribute
    */
    if(sLink->linkType > 0)
    {
      dataID = H5Dopen(pFile->iFID,sLink->targetPath);
    } else {
      dataID = H5Gopen(pFile->iFID,sLink->targetPath);
    }
    if(dataID < 0)
    {
      NXReportError("Internal error, path to link does not exist");
      return NX_ERROR;
    }
    status = H5Aopen_name(dataID,name);
    if(status > 0)
    {
      H5Aclose(status);
      status = H5Adelete(dataID,name);
      if(status < 0)
      {
	return NX_OK;
      }
    }
    aid2 = H5Screate(H5S_SCALAR);
    aid1 = H5Tcopy(H5T_C_S1);
    H5Tset_size(aid1,strlen(sLink->targetPath));
    attID = H5Acreate(dataID,name,aid1,aid2,H5P_DEFAULT, H5P_DEFAULT);
    if(attID < 0)
    {
	return NX_OK;
    }
    status = H5Awrite(attID,aid1,sLink->targetPath);
    H5Tclose(aid1);
    H5Sclose(aid2); 
    H5Aclose(attID); 
    if(sLink->linkType > 0){
      H5Dclose(dataID);
    } else {
      H5Gclose(dataID);
    }
    return NX_OK;
}
/*---------------------------------------------------------------------*/
NXstatus NX5makenamedlink(NXhandle fid, CONSTCHAR *name, NXlink *sLink)
{
    pNexusFile5 pFile;
    char        linkTarget[1024];
    int         type = NX_CHAR;

    pFile = NXI5assert (fid);
    if (pFile->iCurrentG == 0) { /* root level, can not link here */
      return NX_ERROR;
    }
    
    /*
      build pathname to link from our current group and the name 
      of the thing to link
    */
    if(strlen(pFile->name_ref) + strlen(name) + 2 < 1024)
    {
      strcpy(linkTarget,"/");
      strcat(linkTarget,pFile->name_ref);
      strcat(linkTarget,"/");
      strcat(linkTarget,name);
    }
    else 
    {
      NXReportError("ERROR: path string to long");
      return NX_ERROR;
    } 

    H5Glink(pFile->iFID, H5G_LINK_HARD, sLink->targetPath, linkTarget);

    return NX5settargetattribute(pFile,sLink);
}
 /* ------------------------------------------------------------------- */
  
  NXstatus  NX5makelink (NXhandle fid, NXlink* sLink)
  {
    pNexusFile5 pFile;
    char        linkTarget[1024];
    int         type = NX_CHAR;
    char        *itemName = NULL;

    pFile = NXI5assert (fid);
    if (pFile->iCurrentG == 0) { /* root level, can not link here */
      return NX_ERROR;
    }
    
    /*
      locate name of the element to link 
    */
    itemName = strrchr(sLink->targetPath,'/');
    if(itemName == NULL){
      NXReportError("ERROR: bad link structure");
      return NX_ERROR;
    }
    itemName++;

    /*
      build pathname to link from our current group and the name 
      of the thing to link
    */
    if(strlen(pFile->name_ref) + strlen(itemName) + 2 < 1024)
    {
      strcpy(linkTarget,"/");
      strcat(linkTarget,pFile->name_ref);
      strcat(linkTarget,"/");
      strcat(linkTarget,itemName);
    }
    else 
    {
      NXReportError("ERROR: path string to long");
      return NX_ERROR;
    } 

    H5Glink(pFile->iFID, H5G_LINK_HARD, sLink->targetPath, linkTarget);

    return NX5settargetattribute(pFile,sLink);
   }
 
  /*----------------------------------------------------------------------*/

  NXstatus  NX5flush(NXhandle *pHandle)
  {
    pNexusFile5 pFile = NULL;
    herr_t      iRet;
   
    pFile = NXI5assert (*pHandle);    
    if (pFile->iCurrentD != 0)
    {    
    iRet=H5Fflush(pFile->iCurrentD,H5F_SCOPE_LOCAL);
    }
    else if (pFile->iCurrentG != 0)
    {    
    iRet=H5Fflush(pFile->iCurrentG,H5F_SCOPE_LOCAL);
    }
    else
    { 
    iRet=H5Fflush(pFile->iFID,H5F_SCOPE_LOCAL);
    }
    if (iRet < 0){
      NXReportError( "ERROR: The object cannot be flushed");
      return NX_ERROR; 
    }
    return NX_OK;
  }   
  
  /*-------------------------------------------------------------------------*/
  
  /* Operator function. */
           
  herr_t nxgroup_info(hid_t loc_id, const char *name, void *op_data)
  {
    H5G_stat_t statbuf;
    pinfo self;
    
    self = (pinfo)op_data;
    H5Gget_objinfo(loc_id, name, 0, &statbuf);
    switch (statbuf.type) 
    {
      case H5G_GROUP: 
         self->iname = strdup(name);
         self->type = H5G_GROUP;
         break;
      case H5G_DATASET: 
         self->iname = strdup(name);
         self->type = H5G_DATASET;
         break;
      default:
         self->type=0;
         break;     
    }
    return 1; 
  }
  /* --------------------------------------------------------------------- */

  /* Operator function. */

  herr_t group_info1(hid_t loc_id, const char *name, void *opdata)
  {
    H5G_stat_t statbuf;
    int iNX = *((int*)opdata);
    H5Gget_objinfo(loc_id, name, 0, &statbuf);
    
    switch (statbuf.type) 
    {
      case H5G_GROUP: 
        iNX++;
        *((int*)opdata)=iNX;
        break;
      case H5G_DATASET:
        iNX++;
        *((int*)opdata)=iNX;
        break;
      default:
	break;
    }
    return 0; 
  }
  
  /*-------------------------------------------------------------------------*/

  NXstatus  NX5getgroupinfo_recurse (NXhandle fid, int *iN, NXname pName, NXname pClass)
  {
    pNexusFile5 pFile;
    hid_t       atype, attr_id;
    char        data[64];
        
    pFile = NXI5assert (fid);
    /* check if there is a group open */
    if (pFile->iCurrentG == 0) {
       strcpy (pName, "root");
       strcpy (pClass, "NXroot");
       pFile->iNX=0;
       H5Giterate(pFile->iFID,"/",0,group_info1,&pFile->iNX);
       *iN=pFile->iNX;
    }
    else {
      strcpy (pName,pFile->name_ref);
      attr_id = H5Aopen_name(pFile->iCurrentG,"NX_class");
      if (attr_id<0) {
         strcpy(pClass, NX_UNKNOWN_GROUP);
      } else {
        atype=H5Tcopy(H5T_C_S1);
        H5Tset_size(atype,sizeof(data));  
        H5Aread(attr_id, atype, data);
        strcpy(pClass,data);
        pFile->iNX=0;
        H5Giterate(pFile->iFID,pFile->name_ref,0,group_info1, &pFile->iNX);
        *iN=pFile->iNX;
        H5Aclose(attr_id);
      }
    }
    return NX_OK;
  }
/*---------------------------------------------------------------------------*/
static int countObjectsInGroup(hid_t loc_id)
{
  int count = 0, type;
  hsize_t numobj, i;
  
  herr_t status;

  status = H5Gget_num_objs(loc_id, &numobj);
  if(status < 0) {
    NXReportError("Internal error, failed to retrive no of objects");
    return 0;
  }

  for(i = 0; i < numobj; i++){
    type = H5Gget_objtype_by_idx(loc_id,i);
    if(type == H5G_GROUP || type == H5G_DATASET){
      count++;
    }
  }
  return count;
}
/*----------------------------------------------------------------------------*/
  NXstatus  NX5getgroupinfo (NXhandle fid, int *iN, NXname pName, NXname pClass)
  {
    pNexusFile5 pFile;
    hid_t atype, attr_id, gid;
    char data[64];
        
    pFile = NXI5assert (fid);
    /* check if there is a group open */
    if (pFile->iCurrentG == 0) {
       strcpy (pName, "root");
       strcpy (pClass, "NXroot");
       gid = H5Gopen(pFile->iFID,"/");
       *iN = countObjectsInGroup(gid);
       H5Gclose(gid);
    }
    else {
      strcpy (pName,pFile->name_ref);
      attr_id = H5Aopen_name(pFile->iCurrentG,"NX_class");
      if (attr_id<0) {
         strcpy(pClass, NX_UNKNOWN_GROUP);
      } else {
        atype=H5Tcopy(H5T_C_S1);
        H5Tset_size(atype,sizeof(data));  
        H5Aread(attr_id, atype, data);
        strcpy(pClass,data);
        pFile->iNX=0;
        *iN = countObjectsInGroup(pFile->iCurrentG);
        H5Aclose(attr_id);
      }
    }
    return NX_OK;
  }


/*-------------------------------------------------------------------------
 * Function: hdf5ToNXType
 *
 * Purpose:	Convert a HDF5 class to a NeXus type;  it handles the following HDF5 classes
 *  H5T_STRING
 *  H5T_INTEGER
 *  H5T_FLOAT
 *
 * Return: the NeXus type
 *
 *-------------------------------------------------------------------------
 */
  static int hdf5ToNXType(H5T_class_t tclass, hid_t atype)
  {
      int        iPtype = -1;
      size_t     size;
      H5T_sign_t sign;

      if (tclass==H5T_STRING)
      {
          iPtype=NX_CHAR;
      }
      else if (tclass==H5T_INTEGER)
      {
          size=H5Tget_size(atype);
          sign=H5Tget_sign(atype);
          if (size==1)
          {
              if (sign==H5T_SGN_2)
              {
                  iPtype=NX_INT8;
              } else {
                  iPtype=NX_UINT8;
              }
          } 
          else if (size==2) 
          {
              if (sign==H5T_SGN_2)
              {
                  iPtype=NX_INT16;
              } else {
                  iPtype=NX_UINT16;
              }
          }
          else if (size==4) 
          {
              if (sign==H5T_SGN_2)
              {
                  iPtype=NX_INT32;
              } else {
                  iPtype=NX_UINT32;
              }
          } 
          else if(size == 8)
          {
              if (sign==H5T_SGN_2)
              {
                  iPtype=NX_INT64;
              } else {
                  iPtype=NX_UINT64;
              }
          }
      } 
      else if (tclass==H5T_FLOAT)     
      {
          size=H5Tget_size(atype);
          if (size==4)
          {
              iPtype=NX_FLOAT32;
          } 
          else if (size==8) 
          {
              iPtype=NX_FLOAT64;
          }
      }
      if (iPtype == -1)
      {
          NXReportError( "ERROR: hdf5ToNXtype: invalid type");
      }

      return iPtype;
  }
/*--------------------------------------------------------------------------*/
  static hid_t h5MemType(hid_t atype)
  {
      hid_t       memtype_id = -1;
      size_t      size;
      H5T_sign_t  sign;
      H5T_class_t tclass;

      tclass = H5Tget_class(atype);

      if (tclass==H5T_INTEGER)
      {
          size=H5Tget_size(atype);
          sign=H5Tget_sign(atype);
          if (size==1)
          {
              if (sign==H5T_SGN_2)
              {
                  memtype_id = H5T_NATIVE_INT8;
              } else {
                  memtype_id = H5T_NATIVE_UINT8;
              }
          } 
          else if (size==2) 
          {
              if (sign==H5T_SGN_2)
              {
                  memtype_id = H5T_NATIVE_INT16;
              } else {
                  memtype_id = H5T_NATIVE_UINT16; 
              }
          }
          else if (size==4) 
          {
              if (sign==H5T_SGN_2)
              {
                  memtype_id = H5T_NATIVE_INT32;
              } else {
                  memtype_id = H5T_NATIVE_UINT32; 
              }
          }
          else if (size==8) 
          {
              if (sign==H5T_SGN_2)
              {
                  memtype_id = H5T_NATIVE_INT64;
              } else {
                  memtype_id = H5T_NATIVE_UINT64;
              }
          }
      } else if (tclass==H5T_FLOAT)     
      {
          size=H5Tget_size(atype);
          if (size==4)
          {
              memtype_id = H5T_NATIVE_FLOAT; 
          } else if (size==8) {
              memtype_id = H5T_NATIVE_DOUBLE;
          }
      }           
      if (memtype_id == -1)
      {
          NXReportError( "ERROR: h5MemType: invalid type");
      }
      return memtype_id;
  }
  /*-------------------------------------------------------------------------*/

  NXstatus  NX5getnextentry (NXhandle fid,NXname name, NXname nxclass, int *datatype)
  {
    pNexusFile5 pFile;
    hid_t       grp, attr1,type,atype;
    herr_t      iRet;
    int         iPtype, i;
    int         idx;
    H5T_class_t tclass;
    char        data[128];
    char        ph_name[1024];
    info_type   op_data;
    herr_t      iRet_iNX=-1;
    char        pBuffer[256];
     
    pFile = NXI5assert (fid);
    op_data.iname = NULL;

    /*
      iterate to next entry in group list
    */
    idx=pFile->iStack5[pFile->iStackPtr].iCurrentIDX;
    if (strlen(pFile->name_ref) == 0) {
       /* root group */
       strcpy(pFile->name_ref,"/");
    }  
    iRet=H5Giterate(pFile->iFID,pFile->name_ref,&idx,nxgroup_info,&op_data);
    strcpy(nxclass, NX_UNKNOWN_GROUP);
   
    /*
      figure out the number of items in the current group. We need this in order to
      find out if we are at the end of the search. 
    */
    if (pFile->iCurrentG == 0) {
       pFile->iNX=0;
       iRet_iNX = H5Giterate(pFile->iFID,"/",0,group_info1,&pFile->iNX);
    } else {
      pFile->iNX=0;
      iRet_iNX = H5Giterate(pFile->iFID,pFile->name_ref,0,group_info1, &pFile->iNX);
    }
    if (idx == pFile->iNX) {
       iRet_iNX = 2;   
    } 
         
    if (iRet > 0)
      {
        pFile->iStack5[pFile->iStackPtr].iCurrentIDX++;
        if (op_data.iname != NULL) {
	  strcpy(name,op_data.iname);
           free(op_data.iname);
        } else {
	  pFile->iStack5[pFile->iStackPtr].iCurrentIDX = 0;   
	  return NX_EOD;
	}	  
        if (op_data.type == H5G_GROUP)
        {
	  /*
	    open group and find class name attribute 
	  */
           strcpy(ph_name,"");
           for(i = 1; i < (pFile->iStackPtr + 1); i++)
           {
              strcat(ph_name,pFile->iStack5[i].irefn);
              strcat(ph_name,"/");
           }
           strcat(ph_name,name);
           grp=H5Gopen(pFile->iFID,ph_name);
           if (grp < 0) {
              sprintf (pBuffer, "ERROR: group %s does not exist", ph_name);
              NXReportError( pBuffer);
              return NX_ERROR;  
           }
           attr1 = H5Aopen_name(grp, "NX_class");
           if (attr1 < 0) {
	      strcpy(nxclass, NX_UNKNOWN_GROUP);
           } else {
               type=H5T_C_S1;
               atype=H5Tcopy(type);
               H5Tset_size(atype,sizeof(data));  
               iRet = H5Aread(attr1, atype, data);
               strcpy(nxclass,data);
               H5Tclose(atype);
	       H5Aclose(attr1);
           }
           H5Gclose(grp);
        } else if (op_data.type==H5G_DATASET)
        {
	  /*
	    open dataset and find type
	  */
           grp=H5Dopen(pFile->iCurrentG,name);
           type=H5Dget_type(grp);
           atype=H5Tcopy(type);
           tclass = H5Tget_class(atype);
           iPtype = hdf5ToNXType(tclass, atype);
           *datatype=iPtype;
           strcpy(nxclass, "SDS");
           H5Tclose(atype);
	    H5Tclose(type);
	    H5Dclose(grp);
	 }
	  return NX_OK;
       } 
       else
       { 
	 /*
	   we are at the end of the search: clear the data structure and reset 
	   iCurrentIDX to 0
	 */
	  if (iRet_iNX == 2) {
	    if (op_data.iname != NULL) {
	       free(op_data.iname);
	    }
	    pFile->iStack5[pFile->iStackPtr].iCurrentIDX = 0;   
	    return NX_EOD;
	 } 
	 if (op_data.iname != NULL) {
	    free(op_data.iname);
	 } 
	 NXReportError( 
			    "ERROR: Iteration (directory) was not successful");
	 return NX_ERROR;              
       }
   }

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getdata (NXhandle fid, void *data)
   {
     pNexusFile5 pFile;
     int iStart[H5S_MAX_RANK], status;
     hid_t memtype_id; 
     H5T_class_t tclass;
     int dims;    

     pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
     if (pFile->iCurrentD == 0) 
     {
	NXReportError( "ERROR: no Dataset open");
	return NX_ERROR;
     }
     memset (iStart, 0, H5S_MAX_RANK * sizeof(int));
     /* map datatypes of other plateforms */
     tclass = H5Tget_class(pFile->iCurrentT);
     if (tclass==H5T_STRING)
     {
	dims = H5Tget_size(pFile->iCurrentT);
	memtype_id = H5Tcopy(H5T_C_S1);
	H5Tset_size(memtype_id, dims);
     }
     else 
     {
       memtype_id = h5MemType(pFile->iCurrentT);
     }

     /* actually read */    
     status = H5Dread (pFile->iCurrentD, memtype_id, 
		       H5S_ALL, H5S_ALL,H5P_DEFAULT, data);
     if(tclass == H5T_STRING)
     {
       H5Tclose(memtype_id);
     }
     if(status < 0)
     {
	NXReportError( "ERROR: failed to transfer dataset");
	return NX_ERROR;

     }
     return NX_OK;
   }

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getinfo64 (NXhandle fid, int *rank, int64_t dimension[], int *iType)
   {
     pNexusFile5 pFile;
     int i, iRank, mType;
     hsize_t myDim[H5S_MAX_RANK]; 
     H5T_class_t tclass;

     pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
     if (pFile->iCurrentD == 0) {
       NXReportError( "ERROR: no dataset open");
       return NX_ERROR;
     }

     /* read information */
     tclass = H5Tget_class(pFile->iCurrentT);
     mType = hdf5ToNXType(tclass,pFile->iCurrentT);
     iRank = H5Sget_simple_extent_ndims(pFile->iCurrentS);
     H5Sget_simple_extent_dims(pFile->iCurrentS, myDim, NULL);   
     /* conversion to proper ints for the platform */ 
     *iType = (int)mType;
     if (tclass==H5T_STRING && myDim[iRank-1] == 1) {
        myDim[iRank-1] = H5Tget_size(pFile->iCurrentT);
     } 
     *rank = (int)iRank;
     for (i = 0; i < iRank; i++)
     {
	  dimension[i] = (int)myDim[i];
     }
     return NX_OK;
   }

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getslab64 (NXhandle fid, void *data, int64_t iStart[], int64_t iSize[])
   {
     pNexusFile5 pFile;
     hsize_t myStart[H5S_MAX_RANK];
     hsize_t mySize[H5S_MAX_RANK];
     hsize_t mStart[H5S_MAX_RANK];
     hid_t   memspace, iRet;
     H5T_class_t tclass;
     hid_t   memtype_id;
     char *tmp_data = NULL;
     char *data1;
     int i, dims, iRank, mtype = 0;

     pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
     if (pFile->iCurrentD == 0) 
       {
	 NXReportError( "ERROR: no dataset open");
	 return NX_ERROR;
       }
     iRank = H5Sget_simple_extent_ndims(pFile->iCurrentS); 
     for (i = 0; i < iRank; i++)
	{
	  myStart[i] = (hssize_t)iStart[i];
	  mySize[i]  = (hsize_t)iSize[i];
	  mStart[i] = (hsize_t)0;
	}
      tclass = H5Tget_class(pFile->iCurrentT);
      if (tclass == H5T_STRING) {
/* 
 * FAA 24/1/2007: I don't think this will work for multidimensional
 * string arrays. 
 * MK 23/7/2007: You are right Freddie. 
*/
	 mtype = NX_CHAR;
	 if (mySize[0] == 1) {
	     mySize[0]  = H5Tget_size(pFile->iCurrentT);
	 }    
	 tmp_data = (char*) malloc(mySize[0]);
	 memset(tmp_data,0,sizeof(mySize[0]));
	 iRet = H5Sselect_hyperslab(pFile->iCurrentS, H5S_SELECT_SET, mStart,
				NULL, mySize, NULL);
      } else {
	 iRet = H5Sselect_hyperslab(pFile->iCurrentS, H5S_SELECT_SET, myStart,
				NULL, mySize, NULL);
      }
      /* define slab */
      /* deal with HDF errors */
      if (iRet < 0) 
	{
	  NXReportError( "ERROR: selecting slab failed");
	  return NX_ERROR;
	}

      memspace=H5Screate_simple(iRank, mySize, NULL);
      iRet = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mStart,
				NULL, mySize, NULL);
      if (iRet < 0) 
	{
	  NXReportError( "ERROR: selecting memspace failed");
	  return NX_ERROR;
	}
       /* map datatypes of other plateforms */
       if (tclass == H5T_STRING)
       {
	 dims = H5Tget_size(pFile->iCurrentT);
	 memtype_id = H5Tcopy(H5T_C_S1);
	 H5Tset_size(memtype_id, dims);  
       }
       else 
       {
	 memtype_id = h5MemType(pFile->iCurrentT);
       }

      /* read slab */ 
      if (mtype == NX_CHAR) {
	 iRet = H5Dread(pFile->iCurrentD, memtype_id, H5S_ALL, 
		     H5S_ALL, H5P_DEFAULT,tmp_data);
	  data1 = tmp_data + myStart[0];
	  strncpy((char*)data,data1,(hsize_t)iSize[0]);
	  free(tmp_data);           
      } else {    
	 iRet = H5Dread(pFile->iCurrentD, memtype_id, memspace, 
		     pFile->iCurrentS, H5P_DEFAULT,data);
      }    
      /* cleanup */
      if (tclass == H5T_STRING) { /* we used H5Tcopy */
         H5Tclose(memtype_id);
      }
      H5Sclose(memspace);

      if (iRet < 0) 

	{
	  NXReportError( "ERROR: reading slab failed");
	  return NX_ERROR;
	}
      return NX_OK;
   }

   /*-------------------------------------------------------------------------*/

   /* Operator function. */

   herr_t attr_info(hid_t loc_id, const char *name, const H5A_info_t *unused, void *opdata)
   {
     *((char**)opdata)=strdup(name);
     return 1;
   }

   NXstatus  NX5getnextattr (NXhandle fileid, NXname pName, int *iLength, int *iType)
   {
     pNexusFile5 pFile;
     hid_t attr_id;
     hid_t atype, aspace;
     herr_t iRet;
     int iPType,rank;
     char *iname = NULL; 
     hsize_t idx, intern_idx=-1;
     int vid;

     pFile = NXI5assert (fileid);

     vid = getAttVID(pFile);

     pName[0] = '\0';
     idx=pFile->iAtt5.iCurrentIDX;
     iRet=0;
     // TODO replace
     intern_idx=H5Aget_num_attrs(vid);
     if(intern_idx == idx) {
       killAttVID(pFile,vid);
       return NX_EOD;
     }

     if (intern_idx > idx) {
       /* iRet=H5Aiterate(vid,&idx,attr_info,&iname); */
       iRet=H5Aiterate(vid,H5_INDEX_CRT_ORDER,H5_ITER_INC,&idx,attr_info,&iname);
     } else {
       iRet=0;
     } 
     intern_idx=-1;
     if (iRet < 0) {
	   NXReportError( "ERROR: iterating through attribute list");
	   killAttVID(pFile,vid);
	   return NX_ERROR;  
     } 
     pFile->iAtt5.iCurrentIDX++;
     if (iname != NULL) {
       if(strcmp(iname, "NX_class") == 0 && pFile->iCurrentG != 0) {
	 /*
	   skip NXclass attribute which is internal 
	 */
	 killAttVID(pFile, vid);
	 return NX5getnextattr(fileid, pName, iLength, iType);
       }
       strcpy(pName, iname);
       free(iname);
       iname = NULL;
     } else {
       strcpy(pName,"What is this?");
     }
     pFile->iCurrentA = H5Aopen_name(vid, pName);
     atype  = H5Aget_type(pFile->iCurrentA);
     aspace = H5Aget_space(pFile->iCurrentA);
     rank = H5Sget_simple_extent_ndims(aspace);
     attr_id = H5Tget_class(atype);
     if (attr_id==H5T_STRING) {
       iPType=NX_CHAR;
       rank = H5Tget_size(atype);
     }
     if (rank == 0) {
       rank++;
     }  
     iPType = hdf5ToNXType(attr_id,atype);
     *iType=iPType;
     *iLength=rank;
     H5Tclose(atype);
     H5Sclose(aspace);
     H5Aclose(pFile->iCurrentA);
  
     intern_idx=H5Aget_num_attrs(vid);

     killAttVID(pFile,vid);
     return NX_OK;
   }
 /*-------------------------------------------------------------------------*/

   NXstatus  NX5getattr (NXhandle fid, char *name, 
			 void *data, int* datalen, int* iType)
   {
     pNexusFile5 pFile;
     int iNew, vid;
     herr_t iRet;
     hid_t type, atype = -1;
     char pBuffer[256];

     pFile = NXI5assert (fid);

     type = nxToHDF5Type(*iType);

     vid = getAttVID(pFile);
     iNew = H5Aopen_name(vid, name);
     if (iNew < 0) {
       sprintf (pBuffer, "ERROR: attribute \"%s\" not found", name);
       killAttVID(pFile,vid);
       NXReportError( pBuffer);
       return NX_ERROR;
     }
     pFile->iCurrentA = iNew;
     /* finally read the data */
     if (type==H5T_C_S1)
     {
	atype = H5Aget_type(pFile->iCurrentA);
	H5Tclose(atype);
	atype=H5Tcopy(type);
	H5Tset_size(atype,*datalen);  
	iRet = H5Aread(pFile->iCurrentA, atype, data);
	*datalen = strlen((char*)data);
     } else {
       iRet = H5Aread(pFile->iCurrentA, type, data);
       *datalen=1;
     }

     if (iRet < 0) {
       sprintf (pBuffer, "ERROR: could not read attribute data");
       NXReportError( pBuffer);
       killAttVID(pFile,vid);
       return NX_ERROR;
     }

     iRet = H5Aclose(pFile->iCurrentA);

     killAttVID(pFile,vid);
     if (type==H5T_C_S1)
     {
       H5Tclose(atype);
     }
     return NX_OK;
   }  

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getattrinfo (NXhandle fid, int *iN)
   {
     pNexusFile5 pFile;
     char *iname = NULL; 
     unsigned int idx;
     int vid;
    
     pFile = NXI5assert (fid);
     idx=0;
     *iN = idx;

     vid = getAttVID(pFile);

     idx=H5Aget_num_attrs(vid);
     if (idx > 0) {
       if(pFile->iCurrentG > 0 && pFile->iCurrentD == 0){
	 *iN = idx -1; 
       } else {
	 *iN = idx;
       }
     } else {
       *iN = 0;   
     }
     killAttVID(pFile,vid);
     return NX_OK;
   }


   /*-------------------------------------------------------------------------*/
   NXstatus  NX5getgroupID (NXhandle fileid, NXlink* sRes)
  {
    pNexusFile5 pFile;
    int datalen, type = NX_CHAR;
  
    pFile = NXI5assert (fileid);
    if (pFile->iCurrentG == 0) {
      return NX_ERROR;
    } 
    else {
      /*
	this means: if the item is already linked: use the target attribute, else 
	the path to the current node
      */
      NXMDisableErrorReporting();
      datalen = 1024;
      memset(sRes->targetPath,0,datalen*sizeof(char));
      if(NX5getattr(fileid,"target",sRes->targetPath,&datalen,&type) != NX_OK){
	buildCurrentPath(pFile,sRes->targetPath,1024);
      }
      NXMEnableErrorReporting();
      sRes->linkType = 0;
      return NX_OK;
    }
    /* not reached */
    return NX_ERROR;
  }  
 
  /* ------------------------------------------------------------------- */

  NXstatus  NX5sameID (NXhandle fileid, NXlink* pFirstID, NXlink* pSecondID)
  {
    NXI5assert(fileid);
    if ((strcmp(pFirstID->targetPath,pSecondID->targetPath) == 0)){
       return NX_OK;
    } else {
       return NX_ERROR;
    }
  }
 
 /*-------------------------------------------------------------------------*/
 
  NXstatus  NX5initattrdir (NXhandle fid)
  {
    pNexusFile5 pFile;
        
    pFile = NXI5assert (fid);
    NXI5KillAttDir (pFile);
    return NX_OK;
  }

  /*-------------------------------------------------------------------------*/
 
  NXstatus  NX5initgroupdir (NXhandle fid)
  {
    pNexusFile5 pFile;
        
    pFile = NXI5assert (fid);
    NXI5KillDir (pFile);
    return NX_OK;
  }
/*------------------------------------------------------------------------*/
void NX5assignFunctions(pNexusFunction fHandle)
{

      fHandle->nxclose=NX5close;
      fHandle->nxflush=NX5flush;
      fHandle->nxmakegroup=NX5makegroup;
      fHandle->nxopengroup=NX5opengroup;
      fHandle->nxclosegroup=NX5closegroup;
      fHandle->nxmakedata64=NX5makedata64;
      fHandle->nxcompmakedata64=NX5compmakedata64;
      fHandle->nxcompress=NX5compress;
      fHandle->nxopendata=NX5opendata;
      fHandle->nxclosedata=NX5closedata;
      fHandle->nxputdata=NX5putdata;
      fHandle->nxputattr=NX5putattr;
      fHandle->nxputslab64=NX5putslab64;    
      fHandle->nxgetdataID=NX5getdataID;
      fHandle->nxmakelink=NX5makelink;
      fHandle->nxmakenamedlink=NX5makenamedlink;
      fHandle->nxgetdata=NX5getdata;
      fHandle->nxgetinfo64=NX5getinfo64;
      fHandle->nxgetnextentry=NX5getnextentry;
      fHandle->nxgetslab64=NX5getslab64;
      fHandle->nxgetnextattr=NX5getnextattr;
      fHandle->nxgetattr=NX5getattr;
      fHandle->nxgetattrinfo=NX5getattrinfo;
      fHandle->nxgetgroupID=NX5getgroupID;
      fHandle->nxgetgroupinfo=NX5getgroupinfo;
      fHandle->nxsameID=NX5sameID;
      fHandle->nxinitgroupdir=NX5initgroupdir;
      fHandle->nxinitattrdir=NX5initattrdir;
      fHandle->nxprintlink=NX5printlink;
}

#endif /* HDF5 */
