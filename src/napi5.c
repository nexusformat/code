/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface (HDF5) Routines
  
  Copyright (C) 1997-2002 Mark Koennecke, Przemek Klosowski
  
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
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "napi.h"
#include "napi5.h"

extern  void *NXpData;

  typedef struct __NexusFile5 {
        struct iStack5 {
          int *iRefDir;
          int *iTagDir;
          char irefn[1024];
          int iVref;
          int iCurrentIDX;
        } iStack5[NXMAXSTACK];
        struct iStack5 iAtt5;
        int iVID;
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
  
  /*--------------------------------------------------------------------*/

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
    if (self->iStack5[self->iStackPtr].iRefDir) {
      free (self->iStack5[self->iStackPtr].iRefDir);
      self->iStack5[self->iStackPtr].iRefDir = NULL;
    }
    if (self->iStack5[self->iStackPtr].iTagDir) {
      free (self->iStack5[self->iStackPtr].iTagDir);
      self->iStack5[self->iStackPtr].iTagDir = NULL;
    }
    self->iStack5[self->iStackPtr].iCurrentIDX = 0;
  }
  
  /*--------------------------------------------------------------------*/

  static void NXI5KillAttDir (pNexusFile5 self)
  {
    if (self->iAtt5.iRefDir) {
      free (self->iAtt5.iRefDir);
      self->iAtt5.iRefDir = NULL;
    }
    if (self->iAtt5.iTagDir) {
      free (self->iAtt5.iTagDir);
      self->iAtt5.iTagDir = NULL;
    }
    self->iAtt5.iCurrentIDX = 0;
  }

  /* ---------------------------------------------------------------------- 
  
                          Definition of NeXus API

   ---------------------------------------------------------------------*/

  NXstatus  NX5open(CONSTCHAR *filename, NXaccess am, 
				 NXhandle* pHandle)
  {
  hid_t attr1,aid1, aid2;
  pNexusFile5 pNew = NULL;
  char pBuffer[512];
  char *time_buffer;
  char version_nr[10];
  int iRet;
  unsigned int vers_major, vers_minor, vers_release, am1 ;
  hid_t fapl;     
  int mdc_nelmts;
  int rdcc_nelmts;
  size_t rdcc_nbytes;
  double rdcc_w0;

  /* turn off the automatic HDF error handling */  
    H5Eset_auto(NULL,NULL); 
#ifdef USE_FTIME
    struct timeb timeb_struct;
#endif 

    *pHandle = NULL;

    pNew = (pNexusFile5) malloc (sizeof (NexusFile5));
    if (!pNew) {
      NXIReportError (NXpData,"ERROR: no memory to create File datastructure");
      return NX_ERROR;
    }
    memset (pNew, 0, sizeof (NexusFile5));

    time_buffer = NXIformatNeXusTime();

    /* start HDF5 interface */
    if (am == NXACC_CREATE5) {  
       fapl = H5Pcreate(H5P_FILE_ACCESS);
       iRet=H5Pget_cache(fapl,&mdc_nelmts,&rdcc_nelmts,&rdcc_nbytes,&rdcc_w0);
       rdcc_nbytes=(size_t)nx_cacheSize;
       iRet = H5Pset_cache(fapl,mdc_nelmts,rdcc_nelmts,rdcc_nbytes,rdcc_w0);
       /*
	 setting the close degree is absolutely necessary in HDF5 
         versions > 1.6. If you use a lessere version and the compiler 
        complains, comment it out but keep this in mind.
       */
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
    if (pNew->iFID <= 0) {
      sprintf (pBuffer, "ERROR: cannot open file: %s", filename);
      NXIReportError (NXpData, pBuffer);
      free (pNew);
      return NX_ERROR;
    }

/*
 * need to create global attributes         file_name file_time NeXus_version 
 * at some point for new files
 */
    if (am1 != H5F_ACC_RDONLY) 
    {
        pNew->iVID=H5Gopen(pNew->iFID,"/");
        aid2 = H5Screate(H5S_SCALAR);
        aid1 = H5Tcopy(H5T_C_S1);
        H5Tset_size(aid1, strlen(NEXUS_VERSION));
        if (am1 == H5F_ACC_RDWR)
        {
        H5Adelete(pNew->iVID, "NeXus_version"); 
        }
        attr1= H5Acreate(pNew->iVID, "NeXus_version", aid1, aid2, H5P_DEFAULT);
        if (attr1<0)
        {
          NXIReportError (NXpData, 
                         "ERROR: HDF failed to store NeXus_version attribute ");
          return NX_ERROR;
        } 
        if (H5Awrite(attr1, aid1,NEXUS_VERSION)<0)
        {
          NXIReportError (NXpData, 
                         "ERROR: HDF failed to store NeXus_version attribute ");
          return NX_ERROR;
        }
       /* Close attribute dataspace  */
       iRet = H5Tclose(aid1);
       iRet = H5Sclose(aid2); 
       /* Close attribute */
       iRet = H5Aclose(attr1); 
       H5Gclose(pNew->iVID); 
    }
    if (am1 == H5F_ACC_TRUNC) 
    {
        pNew->iVID=H5Gopen(pNew->iFID,"/");
        aid2=H5Screate(H5S_SCALAR);
        aid1 = H5Tcopy(H5T_C_S1);
        H5Tset_size(aid1, strlen(filename));
        attr1= H5Acreate(pNew->iVID, "file_name", aid1, aid2, H5P_DEFAULT);
        if (attr1 < 0)
        {
          NXIReportError (NXpData, 
			  "ERROR: HDF failed to store file_name attribute ");
          return NX_ERROR;
        }
        if (H5Awrite(attr1, aid1, (char*)filename) < 0)
        {
          NXIReportError (NXpData, 
			  "ERROR: HDF failed to store file_name attribute ");
          return NX_ERROR;
        }
	iRet = H5Tclose(aid1);
	iRet = H5Sclose(aid2); 
        iRet = H5Aclose(attr1);
	/*  ------- library version ------*/
        H5get_libversion(&vers_major, &vers_minor, &vers_release);
        sprintf (version_nr, "%d.%d.%d", vers_major,vers_minor,vers_release);
        aid2=H5Screate(H5S_SCALAR);
        aid1 = H5Tcopy(H5T_C_S1);
        H5Tset_size(aid1, strlen(version_nr));
        attr1= H5Acreate(pNew->iVID, "HDF5_Version", aid1, aid2, H5P_DEFAULT);
        if (attr1 < 0)
        {
          NXIReportError (NXpData, 
			  "ERROR: HDF failed to store file_name attribute ");
          return NX_ERROR;
        }
        if (H5Awrite(attr1, aid1, (char*)version_nr) < 0)
        {
          NXIReportError (NXpData, 
			  "ERROR: HDF failed to store file_name attribute ");
          return NX_ERROR;
        }
	iRet = H5Tclose(aid1);
	iRet = H5Sclose(aid2); 
        iRet = H5Aclose(attr1);
	/*----------- file time */
	if(time_buffer != NULL){
	  aid2=H5Screate(H5S_SCALAR);
	  aid1 = H5Tcopy(H5T_C_S1);
	  H5Tset_size(aid1, strlen(time_buffer));
	  attr1=H5Acreate(pNew->iVID, "file_time", aid1, aid2, H5P_DEFAULT);
	  if (attr1 < 0)
	  {
	      NXIReportError (NXpData, 
			  "ERROR: HDF failed to store file_time attribute ");
	      free(time_buffer);
	      return NX_ERROR;
	  }
	  if (H5Awrite(attr1, aid1, time_buffer) < 0)
	  {
	      NXIReportError (NXpData, 
			  "ERROR: HDF failed to store file_time attribute ");
	      free(time_buffer);
	      return NX_ERROR;
	  }
	  /* Close attribute dataspace */
	  iRet = H5Tclose(aid1);
	  iRet = H5Sclose(aid2); 
	  /* Close attribute */
	  iRet = H5Aclose(attr1); 
	  free(time_buffer);
	}
	H5Gclose(pNew->iVID); 
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
    int iRet;
 
    pFile=NXI5assert(*fid);

    iRet=0;
    /*
    printf("HDF5 object count before close: %d\n",
	   H5Fget_obj_count(pFile->iFID,H5F_OBJ_ALL));
    */
    iRet = H5Fclose(pFile->iFID);
   
    /*
      Please leave this here, it helps debugging HDF5 resource leakages
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
      NXIReportError (NXpData, "ERROR: HDF cannot close HDF file");
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
    hid_t iRet;
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
      NXIReportError (NXpData, "ERROR: HDF could not create Group");
      return NX_ERROR;
    }
    pFile->iVID = iRet;
    strncpy(pFile->name_ref,pBuffer,1023);
    aid2 = H5Screate(H5S_SCALAR);
    aid1 = H5Tcopy(H5T_C_S1);
    H5Tset_size(aid1, strlen(nxclass));
    attr1= H5Acreate(pFile->iVID, "NX_class", aid1, aid2, H5P_DEFAULT);
    if (attr1 < 0)
       {
       NXIReportError (NXpData, "ERROR: HDF failed to store class name!");
       return NX_ERROR;
       }
    if (H5Awrite(attr1, aid1, (char*)nxclass) < 0)
      {
      NXIReportError (NXpData, "ERROR: HDF failed to store class name!");
      return NX_ERROR;
      }
    /* close group */
    iRet=H5Sclose(aid2);
    iRet=H5Tclose(aid1);
    iRet=H5Aclose(attr1);
    iRet=H5Gclose(pFile->iVID);
    return NX_OK;
  }
  
  /*------------------------------------------------------------------------*/

  herr_t attr_check (hid_t loc_id, const char *member_name, void *opdata)
  {
    char attr_name[8+1];   /* need to leave space for \0 as well */
    
    strcpy(attr_name,"NX_class");
    return strstr(member_name, attr_name) ? 1 : 0;
  }

  NXstatus  NX5opengroup (NXhandle fid, CONSTCHAR *name, CONSTCHAR *nxclass)
  {

    pNexusFile5 pFile;
    hid_t iRet, attr1, atype;
    char pBuffer[1024];
    char data[128];
          
    pFile = NXI5assert (fid);
    if (pFile->iCurrentG == 0)
    {
       iRet = H5Gopen (pFile->iFID, (const char *)name);
       if (iRet < 0) {
          sprintf (pBuffer, "ERROR: Group %s does not exist!", name);
          NXIReportError (NXpData, pBuffer);
          return NX_ERROR;  
       }
       pFile->iCurrentG = iRet;
       /* check group attribute */
       iRet = H5Aiterate(pFile->iCurrentG,NULL,attr_check,NULL);
       if (iRet < 0) {
          NXIReportError (NXpData, "ERROR iterating through group!");
          return NX_ERROR;  
       } else if (iRet == 1) {
         /* group attribute was found */
       } else {
         /* no group attribute available */
         NXIReportError (NXpData, "No group attribute available");
         return NX_ERROR;
       }
       /* check contents of group attribute */
       attr1 = H5Aopen_name(pFile->iCurrentG, "NX_class");
       if (attr1 < 0)
       {
          NXIReportError (NXpData, "Error opening group attribute!");
          return NX_ERROR; 
       }
       atype=H5Tcopy(H5T_C_S1);
       H5Tset_size(atype,128);  
       iRet = H5Aread(attr1, atype, data);
       if (strcmp(data, nxclass) == 0) {
          /* test OK */
       } else {
          NXIReportError (NXpData, "Group class is not identical!");
          iRet = H5Tclose(atype);
          iRet = H5Aclose(attr1); 
          return NX_ERROR; 
       }          
       iRet = H5Tclose(atype);
       iRet = H5Aclose(attr1); 
       pFile->iStack5[pFile->iStackPtr].iVref=0;
       strcpy(pFile->iStack5[pFile->iStackPtr].irefn,"");
       strcpy(pFile->name_ref,name);
       strcpy(pFile->name_tmp,name);
    } else {
       sprintf(pBuffer,"%s/%s",pFile->name_tmp,name);
       iRet = H5Gopen (pFile->iFID,(const char *)pBuffer);
       if (iRet < 0) {
          sprintf (pBuffer, "ERROR: Group %s does not exist!", pFile->name_tmp);
          NXIReportError (NXpData, pBuffer);
          return NX_ERROR;  
       }
       pFile->iCurrentG = iRet;
       strcpy(pFile->name_tmp,pBuffer);
       strcpy(pFile->name_ref,pBuffer);
       /* check group attribute */
       iRet=H5Aiterate(pFile->iCurrentG,NULL,attr_check,NULL);
       if (iRet < 0) {
          NXIReportError (NXpData, "ERROR iterating through group!");
          return NX_ERROR;  
       } else if (iRet == 1) {
         /* group attribute was found */
       } else {
         /* no group attribute available */
         NXIReportError (NXpData, "No group attribute available");
         return NX_ERROR;
       }
       /* check contains of group attribute */
       attr1 = H5Aopen_name(pFile->iCurrentG, "NX_class");
       if (attr1 < 0)
       {
          NXIReportError (NXpData, "Error opening group attribute!");
          return NX_ERROR; 
       }
       atype=H5Tcopy(H5T_C_S1);
       H5Tset_size(atype,128);  
       iRet = H5Aread(attr1, atype, data);
       if (strcmp(data, nxclass) == 0) {
          /* test OK */
       } else {
          NXIReportError (NXpData, "Group class is not identical!");
          iRet = H5Tclose(atype);
          iRet = H5Aclose(attr1); 
          return NX_ERROR; 
       }          
       iRet = H5Tclose(atype);
       iRet = H5Aclose(attr1); 
    }
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
  
 /* --------------------------------------------------------------------- */

  NXstatus  NX5compmakedata (NXhandle fid, CONSTCHAR *name, 
					  int datatype, 
					  int rank, int dimensions[],
					  int compress_type, int chunk_size[])
  {
    hid_t datatype1, dataspace, iNew, iRet;
    hid_t type,cparms;
    pNexusFile5 pFile;
    char pBuffer[256];
    int i, byte_zahl;
    hsize_t chunkdims[H5S_MAX_RANK];
    hsize_t mydim[H5S_MAX_RANK], mydim1[H5S_MAX_RANK];  
    hsize_t size[2];
    hsize_t maxdims[1] = {H5S_UNLIMITED};

    pFile = NXI5assert (fid);
  
    for (i = 0; i < rank; i++)
       {
         chunkdims[i]=chunk_size[i];
       }   
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
    else if (datatype == NX_FLOAT32)
    {
        type=H5T_NATIVE_FLOAT;
    }
    else if (datatype == NX_FLOAT64)
    {
        type=H5T_NATIVE_DOUBLE;
    }
    if (rank <= 0) {
      sprintf (pBuffer, "ERROR: invalid rank specified %s",
               name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    /*
      Check dimensions for consistency. The first dimension may be 0
      thus denoting an unlimited dimension.
    */
    for (i = 1; i < rank; i++) {
      if (dimensions[i] <= 0) {
        sprintf (pBuffer,
                 "ERROR: invalid dimension %d, value %d given for Dataset %s",
                 i, dimensions[i], name);
        NXIReportError (NXpData, pBuffer);
        return NX_ERROR;
      }
    }
    if (datatype == NX_CHAR)
    {
      byte_zahl=dimensions[0]; 
      dimensions[0]=rank;
      for(i = 0; i < rank; i++)
         {
         mydim1[i] = dimensions[i];
         }
      dimensions[0] = byte_zahl;
         dataspace=H5Screate_simple(rank,mydim1,NULL);
    } else {
      if (dimensions[0] == NX_UNLIMITED)
      {
        mydim[0]=0;
        dataspace=H5Screate_simple(rank, mydim, maxdims);
      } else {
        for(i = 0; i < rank; i++)
        {
        mydim[i] = dimensions[i];
        }
        /* dataset creation */
        dataspace=H5Screate_simple(rank, mydim, NULL);  
        }
    }  
    datatype1=H5Tcopy(type);
    if (datatype == NX_CHAR)
    {
       H5Tset_size(datatype1, byte_zahl);
    }
    if(compress_type == NX_COMP_LZW)
    {
      cparms = H5Pcreate(H5P_DATASET_CREATE);
      iNew = H5Pset_chunk(cparms,rank,chunkdims);
      if (iNew < 0) {
        NXIReportError (NXpData, "ERROR: Size of chunks could not be set!");
        return NX_ERROR;
      }
      H5Pset_deflate(cparms,6); 
      iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, 
			dataspace, cparms);   
    } else if (compress_type == NX_COMP_NONE) {
      if (dimensions[0] == NX_UNLIMITED) {
         cparms = H5Pcreate(H5P_DATASET_CREATE);
         iNew = H5Pset_chunk(cparms,rank,chunkdims);
         if (iNew < 0) {
            NXIReportError (NXpData, 
			    "ERROR: Size of chunks could not be set!");
            return NX_ERROR;
         }
         iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, 
			   dataspace, cparms);   
      } else {
         iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, 
			   dataspace, H5P_DEFAULT);
      }               
    } else {
      NXIReportError (NXpData, 
  "HDF5 doesn't support selected compression method! Dataset was saved without compression");
      iRet = H5Dcreate (pFile->iCurrentG, (char*)name, datatype1, 
			dataspace, H5P_DEFAULT); 
    }
    if (iRet < 0) {
        NXIReportError (NXpData, "ERROR: Creating chunked dataset failed!");
        return NX_ERROR;
    } else {
        pFile->iCurrentD = iRet;
    }
    if (dimensions[0] == NX_UNLIMITED)
    {      
      size[0]   = 1; 
      size[1]   = 1; 
      iNew = H5Dextend (pFile->iCurrentD, size);
      if (iNew < 0) {
        sprintf (pBuffer, "ERROR: cannot create Dataset %s, check arguments",
               name);
        NXIReportError (NXpData, pBuffer);
        return NX_ERROR;
      }
    }
    iRet = H5Sclose(dataspace);
    iRet = H5Tclose(datatype1);
    iRet = H5Dclose(pFile->iCurrentD);
    pFile->iCurrentD = 0;
    if (iRet < 0) {
        NXIReportError (NXpData, "ERROR: HDF cannot close Dataset");
        return NX_ERROR;
     }
     return NX_OK;
  }


  /* --------------------------------------------------------------------- */

  NXstatus  NX5makedata (NXhandle fid, CONSTCHAR *name, int datatype, 
                                  int rank, int dimensions[])
  {
  pNexusFile5 pFile;
  int chunk_size[H5S_MAX_RANK];
  int i;
  
  pFile = NXI5assert (fid);
  memset(chunk_size,0,H5S_MAX_RANK*sizeof(int));
  if (dimensions[0] == NX_UNLIMITED)
     {
     for (i = 0; i < H5S_MAX_RANK; i++)
       {
         chunk_size[i]= 1;
       } 
     }    
  return NX5compmakedata (fid, name, datatype, rank, dimensions, NX_COMP_NONE, chunk_size);
    
  return NX_OK;
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
      sprintf (pBuffer, "ERROR: Dataset %s not found at this level", name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
     /* find the ID number of datatype */
    pFile->iCurrentT = H5Dget_type(pFile->iCurrentD);
    if (pFile->iCurrentT < 0) {
      NXIReportError (NXpData, "ERROR:HDF error opening Dataset");
      pFile->iCurrentT=0;
      return NX_ERROR;
    }
    /* find the ID number of dataspace */
    pFile->iCurrentS = H5Dget_space(pFile->iCurrentD);
    if (pFile->iCurrentS < 0) {
      NXIReportError (NXpData, "ERROR:HDF error opening Dataset");
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
    int iRet;
  
    pFile = NXI5assert (fid);
    iRet = H5Sclose(pFile->iCurrentS);
    iRet = H5Tclose(pFile->iCurrentT);
    iRet = H5Dclose(pFile->iCurrentD);
    if (iRet < 0) {
        NXIReportError (NXpData, "ERROR: HDF cannot end access to Dataset");
        return NX_ERROR;
     }
    pFile->iCurrentD=0;
    return NX_OK;
  }
  
  /* ------------------------------------------------------------------- */
  


  NXstatus  NX5putdata (NXhandle fid, void *data)
  {
    pNexusFile5 pFile;
    hid_t iRet;
    
    char pError[512] = "";
      
    pFile = NXI5assert (fid);
    
    /* actually write */
    iRet = H5Dwrite (pFile->iCurrentD, pFile->iCurrentT, H5S_ALL, H5S_ALL, 
                     H5P_DEFAULT, data);
    if (iRet < 0) {
      snprintf (pError,511, "ERROR: failure to write data");
      NXIReportError (NXpData, pError);
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  /* ------------------------------------------------------------------- */

  NXstatus  NX5putattr (NXhandle fid, CONSTCHAR *name, void *data, 
                                  int datalen, int iType)
  {
    pNexusFile5 pFile;
    hid_t  attr1, aid1, aid2;
    hid_t type;
    int iRet;
  
    pFile = NXI5assert (fid);
    if (iType == NX_CHAR)
    {
        type=H5T_C_S1;
    }
    else if (iType == NX_INT8)
    {
        type=H5T_NATIVE_CHAR;
    }
    else if (iType == NX_UINT8)
    {
        type=H5T_NATIVE_UCHAR;
    }
    else if (iType == NX_INT16)
    {
        type=H5T_NATIVE_SHORT;
    }
    else if (iType == NX_UINT16)
    {
        type=H5T_NATIVE_USHORT;
    }
    else if (iType == NX_INT32)
    {
        type=H5T_NATIVE_INT;
    }
    else if (iType == NX_UINT32)
    {
        type=H5T_NATIVE_UINT;
    }
    else if (iType == NX_FLOAT32)
    {
        type=H5T_NATIVE_FLOAT;
    }
    else if (iType == NX_FLOAT64)
    {
        type=H5T_NATIVE_DOUBLE;
    }
    if (pFile->iCurrentD != 0) {
       /* Dataset attribute */
       aid2=H5Screate(H5S_SCALAR);
       aid1=H5Tcopy(type);
       if (iType == NX_CHAR)
         {
           H5Tset_size(aid1,datalen); 
         }         
       iRet = H5Aopen_name(pFile->iCurrentD, name);
       if (iRet>0) {
          H5Aclose(iRet);
          iRet=H5Adelete(pFile->iCurrentD,name);
          if (iRet<0) {
              NXIReportError (NXpData, "ERROR: Old attribute cannot removed! ");
              return NX_ERROR;
          }
       }
       attr1 = H5Acreate(pFile->iCurrentD, name, aid1, aid2, H5P_DEFAULT);
       if (attr1 < 0) {
          NXIReportError (NXpData, "ERROR: Attribute cannot created! ");
          return NX_ERROR;
       }   
       if (H5Awrite(attr1,aid1,data) < 0) 
       {
          NXIReportError (NXpData, "ERROR: HDF failed to store attribute ");
          return NX_ERROR;
       }
       /* Close attribute dataspace */
       iRet=H5Tclose(aid1);
       iRet=H5Sclose(aid2); 
       /* Close attribute  */
       iRet=H5Aclose(attr1); 
    } else {
       /* global attribute */
       pFile->iVID=H5Gopen(pFile->iFID,"/");
       aid2=H5Screate(H5S_SCALAR);
       aid1=H5Tcopy(type);
       if (iType == NX_CHAR)
         {
           H5Tset_size(aid1,datalen); 
         }         
       iRet = H5Aopen_name(pFile->iVID, name);
       if (iRet>0) {
          H5Aclose(iRet);
          iRet=H5Adelete(pFile->iVID,name);
          if (iRet<0) {
              NXIReportError (NXpData, "ERROR: Old attribute cannot removed! ");
              return NX_ERROR;
          }
       } 
       attr1 = H5Acreate(pFile->iVID, name, aid1, aid2, H5P_DEFAULT);
       if (attr1 < 0) {
          NXIReportError (NXpData, "ERROR: Attribute cannot created! ");
          return NX_ERROR;
       } 
       if (H5Awrite(attr1,aid1,data) < 0) 
       {
          NXIReportError (NXpData, "ERROR: HDf failed to store attribute ");
          return NX_ERROR;
       } 
       /* Close attribute dataspace */
       iRet=H5Tclose(aid1);
       iRet=H5Sclose(aid2); 
        /* Close attribute */
       iRet=H5Aclose(attr1); 
       H5Gclose(pFile->iVID);
    }
    return NX_OK;
  }
  
  /* ------------------------------------------------------------------- */
 
  NXstatus  NX5putslab (NXhandle fid, void *data, int iStart[], int iSize[])
  {
    pNexusFile5 pFile;
    int iRet, i;
    int rank;
    hssize_t myStart[H5S_MAX_RANK];
    hsize_t mySize[H5S_MAX_RANK];
    hsize_t size[1],maxdims[H5S_MAX_RANK];
    hid_t   filespace,dataspace; 
  
    pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
    if (pFile->iCurrentD == 0) {
      NXIReportError (NXpData, "ERROR: no dataset open");
      return NX_ERROR;
    }
    rank = H5Sget_simple_extent_ndims(pFile->iCurrentS);    
    for(i = 0; i < rank; i++)
    {
       myStart[i] = iStart[i];
       mySize[i]  = iSize[i];
    }
    iRet = H5Sget_simple_extent_dims(pFile->iCurrentS, NULL, maxdims);
    dataspace = H5Screate_simple (rank, mySize, NULL);
    if (maxdims[0] == NX_UNLIMITED)
    {
       size[0]=iStart[0] + iSize[0];
       iRet = H5Dextend(pFile->iCurrentD, size);
       filespace = H5Dget_space(pFile->iCurrentD);
       
       /* define slab */
       iRet = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, myStart,
                               NULL, mySize, NULL);
        /* deal with HDF errors */
       if (iRet < 0) 
       {
       NXIReportError (NXpData, "ERROR: selecting slab failed");
       return NX_ERROR;
       }
       /* write slab */ 
       iRet = H5Dwrite(pFile->iCurrentD, pFile->iCurrentT, dataspace, 
                    filespace, H5P_DEFAULT,data);
       iRet = H5Sclose(filespace);             
   } else {
       /* define slab */
       iRet = H5Sselect_hyperslab(pFile->iCurrentS, H5S_SELECT_SET, myStart,
                               NULL, mySize, NULL);
       /* deal with HDF errors */
       if (iRet < 0) 
       {
       NXIReportError (NXpData, "ERROR: selecting slab failed");
       return NX_ERROR;
       }
       /* write slab */ 
       iRet = H5Dwrite(pFile->iCurrentD, pFile->iCurrentT, dataspace, 
                    pFile->iCurrentS, H5P_DEFAULT,data);
   }
   /* deal with HDF errors */
   iRet = H5Sclose(dataspace);
   if (iRet < 0) 
   {
      NXIReportError (NXpData, "ERROR: writing slab failed");
      return NX_ERROR;
   }
    return NX_OK;
  }
 
  /* ------------------------------------------------------------------- */

  NXstatus  NX5getdataID (NXhandle fid, NXlink* sRes)
  {
    pNexusFile5 pFile;
  
    pFile = NXI5assert (fid);

    /*
      we cannot return ID's when no datset is open
    */
    if(pFile->iCurrentD <= 0){
      return NX_ERROR;
    }
    strcpy(sRes->iTag5,"");
    strcpy(sRes->iRef5,"/");
    strcat(sRes->iRef5,pFile->name_ref);
    strcpy(sRes->iRefd,pFile->iCurrentLD);
    return NX_OK;
  }

 /* ------------------------------------------------------------------- */
 
  NXstatus  NX5printlink (NXhandle fid, NXlink* sLink)
  {
    pNexusFile5 pFile;
    pFile = NXI5assert (fid);
     printf("HDF5 link: iTag5 = \"%s\", iRef5 = \"%s\", iRefd = \"%s\"\n", sLink->iTag5, sLink->iRef5, sLink->iRefd);
    return NX_OK;
  }

 /* ------------------------------------------------------------------- */
  
  NXstatus  NX5makelink (NXhandle fid, NXlink* sLink)
  {
    pNexusFile5 pFile;
/*    int iRet; */
    herr_t status, dataID, aid1, aid2, attID;
    int size_type;
    char buffer[1024];
    char name[] = "target";

    pFile = NXI5assert (fid);
    if (pFile->iCurrentG == 0) { /* root level, can not link here */
      return NX_ERROR;
    }
    size_type = strlen(sLink->iRefd); 
    if (size_type > 0)
    {
       /* dataset link */
       strcpy(sLink->iTag5,pFile->name_ref);
    } else {
       /* group link */
       strcpy(buffer,pFile->iCurrentLGG);
       strcat(buffer, sLink->iTag5);
       strcpy(sLink->iTag5,"/");
       strcat(sLink->iTag5,buffer);
    }
    if (size_type>0)
    {
      strcat(sLink->iRef5,"/");
      strcat(sLink->iRef5,sLink->iRefd);
    }
    if (size_type>0)
    {
      strcat(sLink->iTag5,"/");
      strcat(sLink->iTag5,sLink->iRefd);
    }
    status = H5Glink(pFile->iFID, H5G_LINK_HARD, sLink->iRef5, sLink->iTag5);
    if(size_type > 0)
    {
      dataID = H5Dopen(pFile->iFID,sLink->iRef5);
      /*
	remove old attribute if existing
      */
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
      H5Tset_size(aid1,strlen(sLink->iRef5));
      attID = H5Acreate(dataID,name,aid1,aid2,H5P_DEFAULT);
      if(attID < 0)
      {
	return NX_OK;
      }
      H5Awrite(attID,aid1,sLink->iRef5);
      H5Tclose(aid1);
      H5Sclose(aid2); 
      H5Aclose(attID); 
      H5Dclose(dataID);
    }
    return NX_OK;
   }
 
  /*----------------------------------------------------------------------*/

  NXstatus  NX5flush(NXhandle *pHandle)
  {
    pNexusFile5 pFile = NULL;
    int iRet;
   
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
      NXIReportError (NXpData, "ERROR: The object cannot be flushed");
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
    NexusFile5 self;
    H5G_stat_t statbuf;
    self.iNX = *((int*)opdata);
    H5Gget_objinfo(loc_id, name, 0, &statbuf);
    
    switch (statbuf.type) 
    {
      case H5G_GROUP: 
        self.iNX++;
        *((int*)opdata)=self.iNX;
        break;
      case H5G_DATASET:
        self.iNX++;
        *((int*)opdata)=self.iNX;
        break;
    }
    return 0; 
  }
  
  /*-------------------------------------------------------------------------*/

  NXstatus  NX5getgroupinfo (NXhandle fid, int *iN, NXname pName, NXname pClass)
  {
    pNexusFile5 pFile;
    hid_t atype,attr_id;
    char data[64];
    int iRet;
        
    pFile = NXI5assert (fid);
    /* check if there is a group open */
    if (pFile->iCurrentG == 0) {
       strcpy (pName, "root");
       strcpy (pClass, "NXroot");
       pFile->iNX=0;
       iRet=H5Giterate(pFile->iFID,"/",0,group_info1,&pFile->iNX);
       *iN=pFile->iNX;
    }
    else {
      strcpy (pName,pFile->name_ref);
      attr_id = H5Aopen_name(pFile->iCurrentG,"NX_class");
      if (attr_id<0) {
         strcpy(pClass,"non");
      } else {
        atype=H5Tcopy(H5T_C_S1);
        H5Tset_size(atype,64);  
        H5Aread(attr_id, atype, data);
        strcpy(pClass,data);
        pFile->iNX=0;
        iRet=H5Giterate(pFile->iFID,pFile->name_ref,0,group_info1, &pFile->iNX);
        *iN=pFile->iNX;
        H5Aclose(attr_id);
      }
    }
    return NX_OK;
  }
 
  /*-------------------------------------------------------------------------*/

  NXstatus  NX5getnextentry (NXhandle fid,NXname name, NXname nxclass, int *datatype)
  {
    pNexusFile5 pFile;
    hid_t grp, attr1,type,atype;
    int iRet,iPtype, i;
    int idx,data_id,size_id, sign_id;
    char data[128];
    char ph_name[1024];
    info_type op_data;
    int iRet_iNX=-1;
    char pBuffer[256];
     
    pFile = NXI5assert (fid);
    op_data.iname = NULL;
    idx=pFile->iStack5[pFile->iStackPtr].iCurrentIDX;
    if (strlen(pFile->name_ref) == 0) {
       /* root group */
       strcpy(pFile->name_ref,"/");
    }  
    iRet=H5Giterate(pFile->iFID,pFile->name_ref,&idx,nxgroup_info,&op_data);
    strcpy(nxclass,"");
   
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
           strcpy(ph_name,"");
           for(i = 1; i < (pFile->iStackPtr + 1); i++)
           {
              strcat(ph_name,pFile->iStack5[i].irefn);
              strcat(ph_name,"/");
           }
           strcat(ph_name,name);
           grp=H5Gopen(pFile->iFID,ph_name);
           if (grp < 0) {
              sprintf (pBuffer, "ERROR: Group %s does not exist!", ph_name);
              NXIReportError (NXpData, pBuffer);
              return NX_ERROR;  
           }
           attr1 = H5Aopen_name(grp, "NX_class");
           if (attr1 < 0) {
              H5Gclose(grp);
              NXIReportError (NXpData, "Error opening group class");
              return NX_ERROR;  
           }
           type=H5T_C_S1;
           atype=H5Tcopy(type);
           H5Tset_size(atype,128);  
           iRet = H5Aread(attr1, atype, data);
           strcpy(nxclass,data);
           H5Tclose(atype);
           H5Gclose(grp);
	   H5Aclose(attr1);
        } else if (op_data.type==H5G_DATASET)
        {
           grp=H5Dopen(pFile->iCurrentG,name);
           type=H5Dget_type(grp);
           atype=H5Tcopy(type);
           data_id = H5Tget_class(atype);
           if (data_id==H5T_STRING)
           {
             iPtype=NX_CHAR;
           }
           if (data_id==H5T_INTEGER)
          {
             size_id=H5Tget_size(atype);
             sign_id=H5Tget_sign(atype);
             if (size_id==1)
             {
                if (sign_id==H5T_SGN_2)
                {
                   iPtype=NX_INT8;
                } else {
                   iPtype=NX_UINT8;
                }
             } 
             else if (size_id==2) 
             {
                if (sign_id==H5T_SGN_2)
                {
                   iPtype=NX_INT16;
                } else {
                   iPtype=NX_UINT16;
                }
             }
             else if (size_id==4) 
             {
                 if (sign_id==H5T_SGN_2)
                 {
                    iPtype=NX_INT32;
                 } else {
                    iPtype=NX_UINT32;
                 }
             }
        } else if (data_id==H5T_FLOAT)     
            {
            size_id=H5Tget_size(atype);
            if (size_id==4)
            {
               iPtype=NX_FLOAT32;
            } 
            else if (size_id==8) 
            {
               iPtype=NX_FLOAT64;
            }
        }
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
	 NXIReportError (NXpData, 
			    "ERROR: Iteration (directory) was not successful");
	 return NX_ERROR;              
       }
   }

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getdata (NXhandle fid, void *data)
   {
     pNexusFile5 pFile;
     int iStart[H5S_MAX_RANK], status;
     hid_t data_id, memtype_id, size_id, sign_id;    
     int dims;    

     pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
     if (pFile->iCurrentD == 0) 
     {
	NXIReportError (NXpData, "ERROR: no Dataset open");
	return NX_ERROR;
     }
     memset (iStart, 0, H5S_MAX_RANK * sizeof(int));
     /* map datatypes of other plateforms */
     data_id = H5Tget_class(pFile->iCurrentT);
     if (data_id==H5T_STRING)
     {
	dims = H5Tget_size(pFile->iCurrentT);
	memtype_id = H5Tcopy(H5T_C_S1);
	H5Tset_size(memtype_id, dims);
     }
     if (data_id==H5T_INTEGER)
     {
	size_id=H5Tget_size(pFile->iCurrentT);
	sign_id=H5Tget_sign(pFile->iCurrentT);
	if (size_id==1)
	{
	    if (sign_id==H5T_SGN_2)
	    {
	       memtype_id = H5T_NATIVE_INT8;
	    } else {
	       memtype_id = H5T_NATIVE_UINT8;
	    }
	} 
	else if (size_id==2) 
	{
	   if (sign_id==H5T_SGN_2)
	   {
	      memtype_id = H5T_NATIVE_INT16;
	   } else {
	      memtype_id = H5T_NATIVE_UINT16; 
	   }
	}
	else if (size_id==4) 
	{
	  if (sign_id==H5T_SGN_2)
	  {
	     memtype_id = H5T_NATIVE_INT32;
	  } else {
	     memtype_id = H5T_NATIVE_UINT32; 
	  }
	}
      } else if (data_id==H5T_FLOAT)     
	{
	  size_id=H5Tget_size(pFile->iCurrentT);
	  if (size_id==4)
	  {
	     memtype_id = H5T_NATIVE_FLOAT; 
	  } else if (size_id==8) {
	     memtype_id = H5T_NATIVE_DOUBLE;
	  }
      }           

     /* actually read */    
     status = H5Dread (pFile->iCurrentD, memtype_id, 
		       H5S_ALL, H5S_ALL,H5P_DEFAULT, data);
     if(data_id == H5T_STRING)
     {
       H5Tclose(memtype_id);
     }
     if(status < 0)
     {
	NXIReportError (NXpData, "ERROR: failed to transfer dataset");
	return NX_ERROR;

     }
     return NX_OK;
   }

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getinfo (NXhandle fid, int *rank, int dimension[], int *iType)
   {
     pNexusFile5 pFile;
     int i, iRank, mType, iRet;
     hsize_t myDim[H5S_MAX_RANK]; 
     hid_t data_id,size_id,sign_id;

     pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
     if (pFile->iCurrentD == 0) {
       NXIReportError (NXpData, "ERROR: no Dataset open");
       return NX_ERROR;
     }

     /* read information */
     data_id = H5Tget_class(pFile->iCurrentT);
     if (data_id==H5T_STRING)
     {
       mType=NX_CHAR;
     }
     if (data_id==H5T_INTEGER)
     {
       size_id=H5Tget_size(pFile->iCurrentT);
       sign_id=H5Tget_sign(pFile->iCurrentT);
       if (size_id==1)
       {
	  if (sign_id==H5T_SGN_2)
	  {
	   mType=NX_INT8;
	  } else {
	   mType=NX_UINT8;
	  }
       } 
       else if (size_id==2) 
       {
	  if (sign_id==H5T_SGN_2)
	  {
	   mType=NX_INT16;
	  } else {
	   mType=NX_UINT16;
	  }
       }
       else if (size_id==4) 
       {
       if (sign_id==H5T_SGN_2)
	  {
	   mType=NX_INT32;
	  } else {
	   mType=NX_UINT32;
	  }
       }
     } else if (data_id==H5T_FLOAT)     
       {
       size_id=H5Tget_size(pFile->iCurrentT);
       if (size_id==4)
       {
       mType=NX_FLOAT32;
       } 
       else if (size_id==8) 
       {
       mType=NX_FLOAT64;
       }
     } 
     iRank = H5Sget_simple_extent_ndims(pFile->iCurrentS);
     iRet = H5Sget_simple_extent_dims(pFile->iCurrentS, myDim, NULL);   
     /* conversion to proper ints for the platform */ 
     *iType = (int)mType;
     if (data_id==H5T_STRING) {
	for (i = 0; i < iRank; i++)
	{
	  myDim[i] = H5Tget_size(pFile->iCurrentT);    
	}
     } 
     *rank = (int)iRank;
     for (i = 0; i < iRank; i++)
     {
	  dimension[i] = (int)myDim[i];
     }
     return NX_OK;
   }

   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getslab (NXhandle fid, void *data, int iStart[], int iSize[])
   {
     pNexusFile5 pFile;
     hssize_t myStart[H5S_MAX_RANK];
     hsize_t mySize[H5S_MAX_RANK];
     hssize_t mStart[H5S_MAX_RANK];
     hid_t   memspace, iRet, data_id;
     hid_t   memtype_id, size_id, sign_id;
     char *tmp_data;
     char *data1;
     int i, dims, iRank, mtype = 0;

     pFile = NXI5assert (fid);
     /* check if there is an Dataset open */
     if (pFile->iCurrentD == 0) 
       {
	 NXIReportError (NXpData, "ERROR: no Dataset open");
	 return NX_ERROR;
       }
     iRank = H5Sget_simple_extent_ndims(pFile->iCurrentS); 
     for (i = 0; i < iRank; i++)
	{
	  myStart[i] = (hssize_t)iStart[i];
	  mySize[i]  = (hsize_t)iSize[i];
	  mStart[i] = (hsize_t)0;
	}
      data_id = H5Tget_class(pFile->iCurrentT);
      if (data_id == H5T_STRING) {
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
	  NXIReportError (NXpData, "ERROR: selecting slab failed");
	  return NX_ERROR;
	}

      memspace=H5Screate_simple(iRank, mySize, NULL);
      iRet = H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mStart,
				NULL, mySize, NULL);
      if (iRet < 0) 
	{
	  NXIReportError (NXpData, "ERROR: Select memspace failed");
	  return NX_ERROR;
	}
       /* map datatypes of other plateforms */
       if (data_id==H5T_STRING)
       {
	 dims = H5Tget_size(pFile->iCurrentT);
	 memtype_id = H5Tcopy(H5T_C_S1);
	 H5Tset_size(memtype_id, dims);  
       }
       if (data_id==H5T_INTEGER)
       {
	 size_id=H5Tget_size(pFile->iCurrentT);
	 sign_id=H5Tget_sign(pFile->iCurrentT);
	 if (size_id==1)
	 {
	    if (sign_id==H5T_SGN_2)
	    {
	       memtype_id = H5T_NATIVE_INT8;
	    } else {
	       memtype_id = H5T_NATIVE_UINT8;
	    }
	 } 
	 else if (size_id==2) 
	 {
	    if (sign_id==H5T_SGN_2)
	    {
	       memtype_id = H5T_NATIVE_INT16;
	    } else {
	       memtype_id = H5T_NATIVE_UINT16; 
	    }
	 }
	 else if (size_id==4) 
	 {
	   if (sign_id==H5T_SGN_2)
	   {
	      memtype_id = H5T_NATIVE_INT32;
	   } else {
	      memtype_id = H5T_NATIVE_UINT32; 
	   }
	 }
       } else if (data_id==H5T_FLOAT)     
	 {
	   size_id=H5Tget_size(pFile->iCurrentT);
	   if (size_id==4)
	   {
	      memtype_id = H5T_NATIVE_FLOAT; 
	   } else if (size_id==8) {
	      memtype_id = H5T_NATIVE_DOUBLE;
	   }
       }       

      /* read slab */ 
      if (mtype == NX_CHAR) {
	 iRet = H5Dread(pFile->iCurrentD, memtype_id, H5S_ALL, 
		     H5S_ALL, H5P_DEFAULT,tmp_data);
	  data1 = tmp_data + myStart[0];
	  strncpy(data,data1,(hsize_t)iSize[0]);
	  free(tmp_data);           
      } else {    
	 iRet = H5Dread(pFile->iCurrentD, memtype_id, memspace, 
		     pFile->iCurrentS, H5P_DEFAULT,data);
      }    

      if (iRet < 0) 
	{
	  NXIReportError (NXpData, "ERROR: Reading slab failed");
	  return NX_ERROR;
	}
      return NX_OK;
   }

   /*-------------------------------------------------------------------------*/

   /* Operator function. */

   herr_t attr_info(hid_t loc_id, const char *name, void *opdata)
   {
     *((char**)opdata)=strdup(name);
     return 1;
   }

   NXstatus  NX5getnextattr (NXhandle fileid, NXname pName,
				       int *iLength, int *iType)
   {
     pNexusFile5 pFile;
     hid_t attr_id,size_id,sign_id;
     hid_t iRet, atype, aspace;
     int iPType,rank;
     char *iname = NULL; 
     unsigned int idx;
     int intern_idx=-1;

     pFile = NXI5assert (fileid);
     idx=pFile->iAtt5.iCurrentIDX;
     iRet=0;
     if ((pFile->iCurrentD == 0) && (pFile->iCurrentG==0)) 
     {
     /* global attribute */
	pFile->iVID=H5Gopen(pFile->iFID,"/");
	intern_idx=H5Aget_num_attrs(pFile->iVID);
	if (intern_idx > idx) {
	   iRet=H5Aiterate(pFile->iVID,&idx,attr_info,&iname);
	} 
	else
	{
	  iRet=0;
	} 
	intern_idx=-1;
	if (iRet < 0) {
	   NXIReportError (NXpData, "ERROR iterating through ROOT Attr. list!");
	   return NX_ERROR;  
	} 
     } else if (pFile->iCurrentD > 0) { 
	intern_idx=H5Aget_num_attrs(pFile->iCurrentD);
	if (intern_idx > idx) {
	   iRet=H5Aiterate(pFile->iCurrentD,&idx,attr_info,&iname);
	} 
	else
	{
	  iRet=0;
	} 
	intern_idx=-1;
	if (iRet < 0) {
	   NXIReportError (NXpData, "ERROR iterating through data Attr. list!");
	   return NX_ERROR;  
	}
     }
     else  
     {
	pFile->iAtt5.iCurrentIDX = 0;
	return NX_EOD;  /* no group attributes */
     }
     if (iRet>0)
       {
	 pFile->iAtt5.iCurrentIDX++;
	 strcpy(pName, iname);
	 if (iname != NULL) {
	    free(iname);
	 } 
	 if ((pFile->iCurrentD == 0) && (pFile->iCurrentG==0)) {
	    /* global attribute */
	    pFile->iCurrentA = H5Aopen_name(pFile->iVID, pName);
	 } else { 
	    pFile->iCurrentA = H5Aopen_name(pFile->iCurrentD, pName);
	 }
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
	 if (attr_id==H5T_INTEGER)
	    {
	    size_id=H5Tget_size(atype);
	    sign_id=H5Tget_sign(atype);
	    if (size_id==1)
	    {
	       if (sign_id==H5T_SGN_2)
	       {
	       iPType=NX_INT8;
	       } else {
	       iPType=NX_UINT8;
	       }
	    } 
	    else if (size_id==2) 
	    {
	       if (sign_id==H5T_SGN_2)
	       {
	       iPType=NX_INT16;
	       } else {
	       iPType=NX_UINT16;
	       }
	    }
	    else if (size_id==4) 
	    {
	       if (sign_id==H5T_SGN_2)
	       {
	       iPType=NX_INT32;
	       } else {
	       iPType=NX_UINT32;
	       }
	    }
	 } else if (attr_id==H5T_FLOAT)     
	   {
	   size_id=H5Tget_size(atype);
	   if (size_id==4)
	   {
	   iPType=NX_FLOAT32;
	   } 
	   else if (size_id==8) 
	   {
	   iPType=NX_FLOAT64;
	   }
	 } 
	 *iType=iPType;
	 *iLength=rank;
	 H5Tclose(atype);
	 H5Sclose(aspace);
	 H5Aclose(pFile->iCurrentA);
	 return NX_OK;
       }      
       else
       { 
	 if ((pFile->iCurrentD == 0) && (pFile->iCurrentG==0)) 
	 {
	    /* global attribute */
	    intern_idx=H5Aget_num_attrs(pFile->iVID);
	 } else {
	    if (pFile->iCurrentD>0){
	       intern_idx=H5Aget_num_attrs(pFile->iCurrentD);
	    } else {
	       intern_idx=H5Aget_num_attrs(pFile->iCurrentG);
	    }
	 }
	 if ((intern_idx == 0)||(intern_idx == idx)) {
	     pFile->iAtt5.iCurrentIDX = 0;
	     if (iname != NULL) {
		free(iname);
	     } 
	     return NX_EOD;
	 }
	 NXIReportError (NXpData, 
			    "ERROR: Iteration was not successful");
	 return NX_ERROR;              
       }
   }



   /*-------------------------------------------------------------------------*/

   NXstatus  NX5getattr (NXhandle fid, char *name, void *data, int* datalen, int* iType)
   {
     pNexusFile5 pFile;
     int iNew, iRet;
     hid_t type, atype, glob;
     char pBuffer[256];

     pFile = NXI5assert (fid);
     type = *iType;
     glob = 0;  
     if (type == NX_CHAR)
     {
	 type=H5T_C_S1;
     }
     else if (type == NX_INT8)
     {
	 type=H5T_NATIVE_CHAR;
     }
     else if (type == NX_UINT8)
     {
	 type=H5T_NATIVE_UCHAR;
     }
     else if (type == NX_INT16)
     {
	 type=H5T_NATIVE_SHORT;
     }
     else if (type == NX_UINT16)
     {
	 type=H5T_NATIVE_USHORT;
     }
     else if (type == NX_INT32)
     {
	 type=H5T_NATIVE_INT;
     }
     else if (type == NX_UINT32)
     {
	 type=H5T_NATIVE_UINT;
     }
     else if (type == NX_FLOAT32)
     {
	 type=H5T_NATIVE_FLOAT;
     }
     else if (type == NX_FLOAT64)
     {
	 type=H5T_NATIVE_DOUBLE;
     }
     /* find attribute */
     if (pFile->iCurrentD != 0) 
       {
       /* Dataset attribute */
       iNew = H5Aopen_name(pFile->iCurrentD, name);
       } 
       else 
       {
	 /* globale and group attributes */
	 if (pFile->iCurrentG != 0) {
	  /* group attribute */
	  iNew = H5Aopen_name(pFile->iCurrentG, name);
       } else {
	  /* global attributes */
	  glob=H5Gopen(pFile->iFID,"/");
	  iNew = H5Aopen_name(glob, name);
       }
     }
     if (iNew < 0) {
       sprintf (pBuffer, "ERROR: attribute %s not found", name);
       NXIReportError (NXpData, pBuffer);
       return NX_ERROR;
     }
     pFile->iCurrentA = iNew;
     /* finally read the data */
     if (type==H5T_C_S1)
     {
	atype=H5Tcopy(type);
	H5Tset_size(atype,*datalen);  
	iRet = H5Aread(pFile->iCurrentA, atype, data);
	*datalen=strlen(data);
     } else {
       iRet = H5Aread(pFile->iCurrentA, type, data);
       *datalen=1;
     }

     if (iRet < 0) {
       sprintf (pBuffer, "ERROR: HDF could not read attribute data");
       NXIReportError (NXpData, pBuffer);
       return NX_ERROR;
     }

     iRet = H5Aclose(pFile->iCurrentA);
     if (glob > 0)
     {
     H5Gclose(glob);
     }
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
     herr_t iRet;

     pFile = NXI5assert (fid);
     idx=0;
     *iN = idx;
     if (pFile->iCurrentD == 0 && pFile->iCurrentG == 0) {
       /*
	 global attribute
       */
	pFile->iVID=H5Gopen(pFile->iFID,"/");
	iRet = H5Aiterate(pFile->iVID,&idx,attr_info,&iname);
	if (iRet < 0) {
	   NXIReportError (NXpData, "iterating ERROR!");
	   return NX_ERROR;  
	}      
	idx=H5Aget_num_attrs(pFile->iVID);

	if (idx > 0) {
	  *iN = idx;
	} else {
	  *iN = 1;   
	}
	/*
	if (iname != NULL) {
	    free(iname);
	} 
	*/ 
	return NX_OK;
     } 
     else
     {
       if (pFile->iCurrentD>0) {
	 iRet=H5Aiterate(pFile->iCurrentD,&idx,attr_info,&iname);
       } else {
	 iRet=H5Aiterate(pFile->iCurrentG,&idx,attr_info,&iname);
       }
     }
     if (iRet<0) {
       NXIReportError (NXpData, "Attribute number cannot be fixed!");
       return NX_ERROR;
     }
     if ((idx==0) && (iRet==0)) {
	*iN=idx;
	return NX_OK;
     }
       idx=H5Aget_num_attrs(pFile->iCurrentD);
       if (idx > 0) {
	  *iN = idx;
       } else {
	  *iN = 1;   
       }
     return NX_OK;

   }


   /*-------------------------------------------------------------------------*/
   NXstatus  NX5getgroupID (NXhandle fileid, NXlink* sRes)
  {
    pNexusFile5 pFile;
    int u;
    char group_name[64], class_name[64];
  
    pFile = NXI5assert (fileid);
    if (pFile->iCurrentG == 0) {
      return NX_ERROR;
    } 
    else {
      strcpy(sRes->iRef5,"/");
      NX5getgroupinfo(fileid, &u, group_name,class_name);
      strcat(sRes->iRef5,group_name);
      strcpy(sRes->iTag5,"/");
      strcat(sRes->iTag5, pFile->iCurrentLGG); 
      strcpy(sRes->iRefd,"");
      return NX_OK;
    }
    /* not reached */
    return NX_ERROR;
  }  
 
  /* ------------------------------------------------------------------- */

  NXstatus  NX5sameID (NXhandle fileid, NXlink* pFirstID, NXlink* pSecondID)
  {
    pNexusFile5 pFile;

    pFile = NXI5assert (fileid);
    if ((strcmp(pFirstID->iTag5,pSecondID->iTag5) == 0) &&
        (strcmp(pFirstID->iRef5,pSecondID->iRef5) == 0) && 
        (strcmp(pFirstID->iRefd,pSecondID->iRefd) == 0)) {
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
    NXI5KillAttDir (fid);
    return NX_OK;
  }

  /*-------------------------------------------------------------------------*/
 
  NXstatus  NX5initgroupdir (NXhandle fid)
  {
    pNexusFile5 pFile;
        
    pFile = NXI5assert (fid);
    NXI5KillDir (fid);
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
      fHandle->nxmakedata=NX5makedata;
      fHandle->nxcompmakedata=NX5compmakedata;
      fHandle->nxcompress=NX5compress;
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
      fHandle->nxsameID=NX5sameID;
      fHandle->nxinitgroupdir=NX5initgroupdir;
      fHandle->nxinitattrdir=NX5initattrdir;
      fHandle->nxprintlink=NX5printlink;
}
