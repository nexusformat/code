/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  Application Program Interface Routines
  
  Copyright (C) 1997 Mark Koennecke, Przemek Klosowski
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

  No warranties of any kind, whether explicit or implied, taken.
  Distributed under the GNU copyleft license as documented elsewhere.

  Mark Koennecke                     Przemek Klosowski
  Labor fuer Neutronenstreuung       U. of Maryland & NIST 
  Paul Scherrer Institut             Przemek.Klosowski@nist.gov
  CH-5232 Villigen-PSI
  Switzerland                        Nick Maliszewskyj
  Mark.Koennecke@psi.ch              NIST Center for Neutron Research 
                                     NickM@rrdjazz.nist.gov

  Version   Date       Name    Comments
  -------   ----       ----    --------
    0.8    Mar 1997    MK/PK   First beta version of NeXus API
    0.9    Jun 1997     NM     Some changes made to allow for directory 
                               searches in file reading.
    0.95   Aug 1997    FAA     Merge in new interface definitions and modifications
                               for C++ and FORTRAN calling
----------------------------------------------------------------------------*/

static const char* rscid = "$Id$";	/* Revision interted by CVS */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <mfhdf.h>
#include "napi.h"

/* FORTRAN helpers */
#define NXfopen 			MANGLE(nxifopen)
#define NXfclose			MANGLE(nxifclose)

#define NXMAXSTACK 50
#define NXSIGNATURE 959697

  typedef struct __NexusFile {
    int iNXID;
    char iAccess[2];
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
  } NexusFile, *pNexusFile;
  
  
  /*---------------------------------------------------------------------*/
     static void NXNXNXReportError(void *pData, char *string)
     {
         printf("%s \n",string);
     }
  /*---------------------------------------------------------------------*/
     static void *NXpData = NULL;
     static void (*NXIReportError)(void *pData, char *string) =
                  NXNXNXReportError;
  /*---------------------------------------------------------------------*/
     void NXMSetError(void *pData, void (*NewError)(void *pD, char *text))
     {
        NXpData = pData;
        NXIReportError = NewError;
     }
  
  /*--------------------------------------------------------------------*/
     static pNexusFile NXIassert(NXhandle fid)
     {
        pNexusFile pRes;
  
        assert(fid != NULL);
        pRes = (pNexusFile)fid;
        assert(pRes->iNXID == NXSIGNATURE);
        return pRes;
     }
  
  /*----------------------------------------------------------------------*/
  static int32
  NXIFindVgroup (pNexusFile pFile, char *name, char *nxclass)
  {
    int32 iNew, iRef, iTag;
    int iN, i;
    int32 *pArray = NULL;
    NXname pText;
  
    assert (pFile != NULL);
  
    if (pFile->iCurrentVG == 0) { /* root level */
      /* get the number and ID's of all lone Vgroups in the file */
      iN = Vlone (pFile->iVID, NULL, 0);
      if(iN == 0) {
         return NX_EOD;
      }
      pArray = (int32 *) malloc (iN * sizeof (int32));
      if (!pArray) {
        NXIReportError (NXpData, "ERROR: out of memory in NXIFindVgroup");
        return NX_EOD;
      }
      Vlone (pFile->iVID, pArray, iN);
  
      /* loop and check */
      for (i = 0; i < iN; i++) {
        iNew = Vattach (pFile->iVID, pArray[i], "r");
        Vgetname (iNew, pText);
        if (strcmp (pText, name) == 0) {
          Vgetclass (iNew, pText);
          if (strcmp (pText, nxclass) == 0) {
            /* found ! */
            Vdetach (iNew);
            iNew = pArray[i];
            free (pArray);
            return iNew;
          }
        }
        Vdetach (iNew);
      }
      /* nothing found */
      free (pArray);
      return NX_EOD;
    } else {                      /* case in Vgroup */
      iN = Vntagrefs (pFile->iCurrentVG);
      for (i = 0; i < iN; i++) {
        Vgettagref (pFile->iCurrentVG, i, &iTag, &iRef);
        if (iTag == DFTAG_VG) {
          iNew = Vattach (pFile->iVID, iRef, "r");
          Vgetname (iNew, pText);
          if (strcmp (pText, name) == 0) {
            Vgetclass (iNew, pText);
            if (strcmp (pText, nxclass) == 0) {
              /* found ! */
              Vdetach (iNew);
              return iRef;
            }
          }
          Vdetach (iNew);
        }
      }                           /* end for */
    }                             /* end else */
    /* not found */
    return NX_EOD;
  }
  
  static int32
  NXIFindSDS (NXhandle fid, char *name)
  {
    pNexusFile self;
    int32 iNew, iRet, iTag, iRef;
    int32 i, iN, iA, iD1, iD2;
    NXname pNam;
    int32 iDim[MAX_VAR_DIMS];
  
    self = NXIassert (fid);
  
    /* root level search */
    if (self->iCurrentVG == 0) {
      i = SDfileinfo (self->iSID, &iN, &iA);
      if (i < 0) {
        NXIReportError (NXpData, "ERROR: failure to read file information");
        return NX_EOD;
      }
      for (i = 0; i < iN; i++) {
        iNew = SDselect (self->iSID, i);
        SDgetinfo (iNew, pNam, &iA, iDim, &iD2, &iD2);
        if (strcmp (pNam, name) == 0) {
          iRet = SDidtoref (iNew);
          SDendaccess (iNew);
          return iRet;
        } else {
          SDendaccess (iNew);
        }
      }
      /* not found */
      return NX_EOD;
    }
    /* end root level */
    else {                        /* search in a Vgroup */
      iN = Vntagrefs (self->iCurrentVG);
      for (i = 0; i < iN; i++) {
        Vgettagref (self->iCurrentVG, i, &iTag, &iRef);
        if ((iTag == DFTAG_SDG) || (iTag == DFTAG_NDG)) {
          iNew = SDreftoindex (self->iSID, iRef);
          iNew = SDselect (self->iSID, iNew);
          SDgetinfo (iNew, pNam, &iA, iDim, &iD2, &iD2);
          if (strcmp (pNam, name) == 0) {
            SDendaccess (iNew);
            return iRef;
          }
          SDendaccess (iNew);
        }
      }                           /* end for */
    }                             /* end Vgroup */
    /* we get here, only if nothing found */
    return NX_EOD;
  }
  
  static int
  NXIInitDir (pNexusFile self)
  {
    int i;
    int32 iTag, iRef;
    int iStackPtr, iCurDir;
  
    iStackPtr = self->iStackPtr;
    if (self->iCurrentVG == 0 &&
        self->iStack[iStackPtr].iRefDir == NULL) {        /* root level */
      /* get the number and ID's of all lone Vgroups in the file */
      self->iStack[iStackPtr].iNDir = Vlone (self->iVID, NULL, 0);
      self->iStack[iStackPtr].iRefDir = 
          (int32 *) malloc (self->iStack[iStackPtr].iNDir * sizeof (int32));
      if (!self->iStack[iStackPtr].iRefDir) {
        NXIReportError (NXpData, "ERROR: out of memory in NXIInitDir");
        return NX_EOD;
      }
      Vlone (self->iVID,
             self->iStack[self->iStackPtr].iRefDir,
             self->iStack[self->iStackPtr].iNDir);
    } else {
      /* Vgroup level */
      self->iStack[iStackPtr].iNDir = Vntagrefs (self->iCurrentVG);
      self->iStack[iStackPtr].iRefDir =
        (int32 *) malloc (self->iStack[iStackPtr].iNDir * sizeof (int32));
      self->iStack[iStackPtr].iTagDir =
        (int32 *) malloc (self->iStack[iStackPtr].iNDir * sizeof (int32));
      if ((!self->iStack[iStackPtr].iRefDir) ||
          (!self->iStack[iStackPtr].iTagDir)) {
        NXIReportError (NXpData, "ERROR: out of memory in NXIInitDir");
        return NX_EOD;
      }
      for (i = 0; i < self->iStack[self->iStackPtr].iNDir; i++) {
        Vgettagref (self->iCurrentVG, i, &iTag, &iRef);
        self->iStack[iStackPtr].iRefDir[i] = iRef;
        self->iStack[iStackPtr].iTagDir[i] = iTag;
      }
    }
    self->iStack[iStackPtr].iCurDir = 0;
    return 1;
  }
  
  static void
  NXIKillDir (pNexusFile self)
  {
    if (self->iStack[self->iStackPtr].iRefDir) {
      free (self->iStack[self->iStackPtr].iRefDir);
      self->iStack[self->iStackPtr].iRefDir = NULL;
    }
    if (self->iStack[self->iStackPtr].iTagDir) {
      free (self->iStack[self->iStackPtr].iTagDir);
      self->iStack[self->iStackPtr].iTagDir = NULL;
    }
    self->iStack[self->iStackPtr].iCurDir = 0;
    self->iStack[self->iStackPtr].iNDir = 0;
  }
  
  /*-------------------------------------------------------------------------*/
  static int
  NXIInitAttDir (pNexusFile pFile)
  {
    int iRet;
    int32 iData, iAtt, iRank, iType;
    int32 iDim[MAX_VAR_DIMS];
    NXname pNam;
  
    pFile->iAtt.iCurDir = 0;
    if (pFile->iCurrentSDS != 0) {        /* SDS level */
      iRet = SDgetinfo (pFile->iCurrentSDS, pNam, &iRank, iDim, &iType,
                        &iAtt);
    } else {                      /* global level */
      iRet = SDfileinfo (pFile->iSID, &iData, &iAtt);
    }
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDF cannot read attribute numbers");
      pFile->iAtt.iNDir = 0;
      return NX_ERROR;
    }
    pFile->iAtt.iNDir = iAtt;
    return NX_OK;
  }
  
  static void
  NXIKillAttDir (pNexusFile self)
  {
    if (self->iAtt.iRefDir) {
      free (self->iAtt.iRefDir);
      self->iAtt.iRefDir = NULL;
    }
    if (self->iAtt.iTagDir) {
      free (self->iAtt.iTagDir);
      self->iAtt.iTagDir = NULL;
    }
    self->iAtt.iCurDir = 0;
    self->iAtt.iNDir = 0;
  }
  
  
  NXstatus  NXfopen(char * filename, NXaccess* am, char* owner, char* owner_address, char* owner_telephone_number,
                char* owner_fax_number, char* owner_email, char* owner_affiliation, NexusFile* pHandle)
  {
	int ret;
 	NXhandle fileid;
	ret = NXopen(filename, *am, owner, owner_address, owner_telephone_number,
			owner_fax_number, owner_email, owner_affiliation, &fileid);
	memcpy(pHandle, fileid, sizeof(NexusFile));
	return ret;
  }

  NXstatus  NXopen(char * filename, NXaccess am, char* owner, char* owner_address, char* owner_telephone_number,
                char* owner_fax_number, char* owner_email, char* owner_affiliation, NXhandle* pHandle)
  {

    pNexusFile pNew = NULL;
    char pBuffer[512];
    int iRet;
  
    *pHandle = NULL;
    /* get memory */
    pNew = (pNexusFile) malloc (sizeof (NexusFile));
    if (!pNew) {
      NXIReportError (NXpData, "ERROR: no memory to create File datastructure");
      return NX_ERROR;
    }
    memset (pNew, 0, sizeof (NexusFile));
  
    /* start SDS interface */
    pNew->iSID = SDstart (filename, am);
    if (pNew->iSID <= 0) {
      sprintf (pBuffer, "ERROR: cannot open file: %s", filename);
      NXIReportError (NXpData, pBuffer);
      free (pNew);
      return NX_ERROR;
    }
    /* 
     * Otherwise we try to create the file two times which makes HDF
     * Throw up on us.
     */
    if (am == NXACC_CREATE) {
      am = NXACC_RDWR;
    }

/*
 * need to create global attributes         file_name file_time NeXus_version owner owner_address
 *                                          owner_telephone_number owner_fax_number owber_email owner_affiliation
 * at some point for new files
*/
  
    /* Set Vgroup access mode */
    if (am == NXACC_READ) {
      strcpy(pNew->iAccess,"r");
    } else {
      strcpy(pNew->iAccess,"w");
    }
  
    /* start Vgroup API */
    pNew->iVID = Hopen (filename, am, 100);
    if (pNew->iVID <= 0) {
      sprintf (pBuffer, "ERROR: cannot open file: %s", filename);
      NXIReportError (NXpData, pBuffer);
      free (pNew);
      return NX_ERROR;
    }
    Vstart (pNew->iVID);
    pNew->iNXID = NXSIGNATURE;
    pNew->iStack[0].iVref = 0;    /* root! */
  
    *pHandle = (NXhandle)pNew;
    return NX_OK;
  }
  
  
  NXstatus
  NXfclose (NexusFile* pHandle)
  {
    NXhandle h;
    int ret;
    h = pHandle;
    ret = NXclose(&h);
    memset(pHandle, 0, sizeof(NexusFile));
    return ret;
  }

  NXstatus
  NXclose (NXhandle* fid)
  {
    pNexusFile pFile = NULL;
    int iRet;
  
    pFile = NXIassert(*fid);
    iRet = 0;
    /* close links into vGroups or SDS */
    if (pFile->iCurrentVG != 0) {
      Vdetach (pFile->iCurrentVG);
    }
    if (pFile->iCurrentSDS != 0) {
      iRet = SDendaccess (pFile->iCurrentSDS);
    }
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: ending access to SDS");
    }
    /* close the SDS and Vgroup API's */
    Vend (pFile->iVID);
    iRet = SDend (pFile->iSID);
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDF cannot close SDS interface");
    }
    iRet = Hclose (pFile->iVID);
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDF cannot close HDF file");
    }
    /* release memory */
    NXIKillDir (pFile);
    free (pFile);
    *fid = NULL;
    return NX_OK;
  }
  
   
  NXstatus NXmakegroup (NXhandle fid, char *name, char *nxclass) {
    pNexusFile pFile;
    int32 iNew, iRet;
    char pBuffer[256];
  
    pFile = NXIassert (fid);
    /* 
     * Make sure that a group with the same name and nxclass does not
     * already exist.
     */
    if ((iRet = NXIFindVgroup (pFile, name, nxclass)) >= 0) {
      sprintf (pBuffer, "ERROR: Vgroup %s, class %s already exists", 
                        name, nxclass);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
  
    /* create and configure the group */
    iNew = Vattach (pFile->iVID, -1, "w");
    if (iNew < 0) {
      NXIReportError (NXpData, "ERROR: HDF could not create Vgroup");
      return NX_ERROR;
    }
    Vsetname (iNew, name);
    Vsetclass (iNew, nxclass);
  
    /* Insert it into the hierarchy, when appropriate */
    iRet = 0;     
    if (pFile->iCurrentVG != 0) {
      iRet = Vinsert (pFile->iCurrentVG, iNew);
    }
    Vdetach (iNew);
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDF failed to insert Vgroup");
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  
  /*------------------------------------------------------------------------*/
  NXstatus
  NXopengroup (NXhandle fid, char *name, char *nxclass)
  {
    pNexusFile pFile;
    int32 iNew, iRef;
    char pBuffer[256];
  
    pFile = NXIassert (fid);
  
    iRef = NXIFindVgroup (pFile, name, nxclass);
    if (iRef < 0) {
      sprintf (pBuffer, "ERROR: Vgroup %s, class %s NOT found", name, nxclass);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    /* are we at root level ? */
    if (pFile->iCurrentVG == 0) {
      pFile->iCurrentVG = Vattach (pFile->iVID, iRef,pFile->iAccess);
      pFile->iStackPtr++;
      pFile->iStack[pFile->iStackPtr].iVref = iRef;
    } else {
      Vdetach (pFile->iCurrentVG);
      pFile->iStackPtr++;
      pFile->iStack[pFile->iStackPtr].iVref = iRef;
      pFile->iCurrentVG = Vattach (pFile->iVID,
                                   pFile->iStack[pFile->iStackPtr].iVref,
                                   pFile->iAccess);
    }
    NXIKillDir (pFile);
    return NX_OK;
  }
  
  
  NXstatus
  NXclosegroup (NXhandle fid)
  {
    pNexusFile pFile;
  
    pFile = NXIassert (fid);
  
    /* first catch the trivial case: we are at root and cannot get 
       deeper into a negative directory hierarchy (anti-directory)
     */
    if (pFile->iCurrentVG == 0) {
      NXIKillDir (pFile);
      return NX_OK;
    } else {                      /* Sighhh. Some work to do */
      /* close the current VG and decrement stack */
      Vdetach (pFile->iCurrentVG);
      NXIKillDir (pFile);
      pFile->iStackPtr--;
      if (pFile->iStackPtr <= 0) {        /* we hit root */
        pFile->iStackPtr = 0;
        pFile->iCurrentVG = 0;
      } else {
        /* attach to the lower Vgroup */
        pFile->iCurrentVG = Vattach (pFile->iVID,
                                     pFile->iStack[pFile->iStackPtr].iVref,
                                     pFile->iAccess);
      }
    }
    return NX_OK;
  }
  
  
  NXstatus
  NXmakedata (NXhandle fid, char *name, int datatype, int rank,
              int dimensions[])
  {
    pNexusFile pFile;
    int32 iNew;
    char pBuffer[256];
    int i, iRet;
  
    pFile = NXIassert (fid);
  
  
    if ((iNew = NXIFindSDS (fid, name))>=0) {
      sprintf (pBuffer, "ERROR: SDS %s already exists at this level", name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
  
  
    /* Do some argument checking */
    switch (datatype) {
    case DFNT_FLOAT32: break;
    case DFNT_FLOAT64: break;
    case DFNT_INT8:    break;
    case DFNT_UINT8:   break;
    case DFNT_INT16:   break;
    case DFNT_UINT16:  break;
    case DFNT_INT32:   break;
    case DFNT_UINT32:  break;
    default:
      sprintf (pBuffer, "ERROR: unknown datatype specified for SDS %s",
               name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
  
    if (rank <= 0) {
      sprintf (pBuffer, "ERROR: invalid rank specified for SDS %s",
               name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    for (i = 0; i < rank; i++) {
      if (dimensions[i] <= 0) {
        sprintf (pBuffer,
                 "ERROR: invalid dimension %d, value %d given for SDS %s",
                 i, dimensions[i], name);
        NXIReportError (NXpData, pBuffer);
        return NX_ERROR;
      }
    }
  
    /* behave nicely, if there is still an SDS open */
    if (pFile->iCurrentSDS != 0) {
      SDendaccess (pFile->iCurrentSDS);
      pFile->iCurrentSDS = 0;
    }
  
    /* Do not allow creation of SDS's at the root level */
    if (pFile->iCurrentVG == 0) {
      sprintf(pBuffer, "ERROR: SDS creation at root level is not permitted");
      NXIReportError(NXpData, pBuffer);
      return NX_ERROR;
    }
          
    /* dataset creation */
    iNew = SDcreate (pFile->iSID, name, datatype, rank, dimensions);
    if (iNew < 0) {
      sprintf (pBuffer, "ERROR: cannot create SDS %s, check arguments",
               name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    /* link into Vgroup, if in one */
    if (pFile->iCurrentVG != 0) {
      iRet = Vaddtagref (pFile->iCurrentVG, DFTAG_SDG, SDidtoref (iNew));
    }
    iRet = SDendaccess (iNew);
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDF cannot end access to SDS");
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  
  NXstatus
  NXopendata (NXhandle fid, char *name)
  {
    pNexusFile pFile;
    int32 iNew;
    char pBuffer[256];
    int iRet;
  
    pFile = NXIassert (fid);
  
    /* First find the reference number of the SDS */
    iNew = NXIFindSDS (fid, name);
    if (iNew < 0) {
      sprintf (pBuffer, "ERROR: SDS %s not found at this level", name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    /* Be nice: properly close the old open SDS silently if there is
     * still an SDS open.
     */
    if (pFile->iCurrentSDS) {
      iRet = SDendaccess (pFile->iCurrentSDS);
      if (iRet < 0) {
        NXIReportError (NXpData, "ERROR: HDF cannot end access to SDS");
      }
    }
    /* clear pending attribute directories first */
    NXIKillAttDir (pFile);
  
    /* open the SDS */
    iNew = SDreftoindex (pFile->iSID, iNew);
    pFile->iCurrentSDS = SDselect (pFile->iSID, iNew);
    if (pFile->iCurrentSDS < 0) {
      NXIReportError (NXpData, "ERROR: HDF error opening SDS");
      pFile->iCurrentSDS = 0;
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  
  NXstatus
  NXclosedata (NXhandle fid)
  {
    pNexusFile pFile;
    int iRet;
  
    pFile = NXIassert (fid);
  
    if (pFile->iCurrentSDS != 0) {
      iRet = SDendaccess (pFile->iCurrentSDS);
      pFile->iCurrentSDS = 0;
      if (iRet < 0) {
        NXIReportError (NXpData, "ERROR: HDF cannot end access to SDS");
        return NX_ERROR;
      }
    } else {
      NXIReportError (NXpData, "ERROR: no SDS open --> nothing to do");
      return NX_ERROR;
    }
    NXIKillAttDir (pFile);                /* for attribute data */
    return NX_OK;
  }
  
  
  NXstatus
  NXgetdata (NXhandle fid, void *data)
  {
    pNexusFile pFile;
    int32 iStart[MAX_VAR_DIMS], iEnd[MAX_VAR_DIMS];
    NXname pBuffer;
    int32 iRank, iAtt, iType;
  
    pFile = NXIassert (fid);
  
    /* check if there is an SDS open */
    if (pFile->iCurrentSDS == 0) {
      NXIReportError (NXpData, "ERROR: no SDS open");
      return NX_ERROR;
    }
    /* first read dimension information */
    memset (iStart, 0, MAX_VAR_DIMS * sizeof (int32));
    SDgetinfo (pFile->iCurrentSDS, pBuffer, &iRank, iEnd, &iType, &iAtt);
    /* actually read */
    SDreaddata (pFile->iCurrentSDS, iStart, NULL, iEnd, data);
    return NX_OK;
  }
  
  
  NXstatus
  NXgetslab (NXhandle fid, void *data, int iStart[], int iEnd[])
  {
    pNexusFile pFile;
  
    pFile = NXIassert (fid);
  
    /* check if there is an SDS open */
    if (pFile->iCurrentSDS == 0) {
      NXIReportError (NXpData, "ERROR: no SDS open");
      return NX_ERROR;
    }
    /* actually read */
    SDreaddata (pFile->iCurrentSDS, iStart, NULL, iEnd, data);
    return NX_OK;
  }
  
  
  NXstatus
  NXgetattr (NXhandle fid, char *name, char *data, int* datalen, int* iType)
  {
    pNexusFile pFile;
    int32 iNew;
    void *pData = NULL;
    int32 iLen, iRet;
    char pBuffer[256];
    NXname pNam;
  
    *datalen = (*datalen) * DFKNTsize(*iType);
    pFile = NXIassert (fid);
  
    /* find attribute */
    if (pFile->iCurrentSDS != 0) {
      /* SDS attribute */
      iNew = SDfindattr (pFile->iCurrentSDS, name);
    } else {
      /* global attribute */
      iNew = SDfindattr (pFile->iSID, name);
    }
    if (iNew < 0) {
      sprintf (pBuffer, "ERROR: attribute %s not found", name);
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    /* get more info, allocate temporary data space */
    if (pFile->iCurrentSDS != 0) {
      iRet = SDattrinfo (pFile->iCurrentSDS, iNew, pNam, iType, &iLen);
    } else {
      iRet = SDattrinfo (pFile->iSID, iNew, pNam, iType, &iLen);
    }
    if (iRet < 0) {
      sprintf (pBuffer, "ERROR: HDF could not read attribute info");
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    iLen = iLen * DFKNTsize (*iType);
    iLen = iLen * DFKNTsize (*iType);
    pData = (void *) malloc (iLen);
    if (!pData) {
      NXIReportError (NXpData, "ERROR: allocating memory in NXgetattr");
      return NX_ERROR;
    }
    memset (pData, 0, iLen);
  
    /* finally read the data */
    if (pFile->iCurrentSDS != 0) {
      iRet = SDreadattr (pFile->iCurrentSDS, iNew, pData);
    } else {
      iRet = SDreadattr (pFile->iSID, iNew, pData);
    }
    if (iRet < 0) {
      sprintf (pBuffer, "ERROR: HDF could not read attribute data");
      NXIReportError (NXpData, pBuffer);
      return NX_ERROR;
    }
    /* copy data to caller */
    memset (data, 0, *datalen);
    if ((*datalen < iLen) && (*iType == DFNT_UINT8)) {
      iLen = *datalen - 1;
    }
    memcpy (data, pData, iLen);
    *datalen = iLen / DFKNTsize(*iType);
    free (pData);
    return NX_OK;
  }
  
  
  NXstatus
  NXputdata (NXhandle fid, void *data)
  {
    pNexusFile pFile;
    int32 iStart[MAX_VAR_DIMS], iEnd[MAX_VAR_DIMS], iStride[MAX_VAR_DIMS];
    NXname pBuffer;
    int32 iRank, iAtt, iType, iRet, i;
    char pError[512];
  
    pFile = NXIassert (fid);
  
    /* check if there is an SDS open */
    if (pFile->iCurrentSDS == 0) {
      NXIReportError (NXpData, "ERROR: no SDS open");
      return NX_ERROR;
    }
    /* first read dimension information */
    memset (iStart, 0, MAX_VAR_DIMS * sizeof (int32));
    SDgetinfo (pFile->iCurrentSDS, pBuffer, &iRank, iEnd, &iType, &iAtt);
  
    /* initialise stride to 1 */
    for (i = 0; i < iRank; i++) {
      iStride[i] = 1;
    }
  
    /* actually write */
    iRet = SDwritedata (pFile->iCurrentSDS, iStart, iStride, iEnd, data);
    if (iRet < 0) {
      sprintf (pError, "ERROR: failure to write data to %s", pBuffer);
      NXIReportError (NXpData, pError);
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  
  NXstatus
  NXputslab (NXhandle fid, void *data, int iStart[], int iEnd[])
  {
    pNexusFile pFile;
    int iRet;
    int iStride[MAX_VAR_DIMS], i;
  
  
    pFile = NXIassert (fid);
  
    /* check if there is an SDS open */
    if (pFile->iCurrentSDS == 0) {
      NXIReportError (NXpData, "ERROR: no SDS open");
      return NX_ERROR;
    }
    /* initialise stride to 1 */
    for (i = 0; i < MAX_VAR_DIMS; i++) {
      iStride[i] = 1;
    }
  
    /* actually write */
    iRet = SDwritedata (pFile->iCurrentSDS, iStart, iStride, iEnd, data);
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: writing slab failed");
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  
  NXstatus
  NXputattr (NXhandle fid, char *name, void *data, int datalen, int iType)
  {
    pNexusFile pFile;
    int iRet;
  
    pFile = NXIassert (fid);
  
    if (pFile->iCurrentSDS != 0) {
      /* SDS attribute */
      iRet = SDsetattr (pFile->iCurrentSDS, name, iType,
                        datalen, data);
    } else {
      /* global attribute */
      iRet = SDsetattr (pFile->iSID, name, iType,
                        datalen, data);
  
    }
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDf failed to store attribute ");
      return NX_ERROR;
    }
    return NX_OK;
  }
  
  
  NXstatus
  NXgetinfo (NXhandle fid, int *rank, int dimension[], int *iType)
  {
    pNexusFile pFile;
    NXname pBuffer;
    int32 iAtt;
  
    pFile = NXIassert (fid);
  
    /* check if there is an SDS open */
    if (pFile->iCurrentSDS == 0) {
      NXIReportError (NXpData, "ERROR: no SDS open");
      return NX_ERROR;
    }
    /* read information */
    SDgetinfo (pFile->iCurrentSDS, pBuffer, rank, dimension, iType, &iAtt);
    return NX_OK;
  }
  
  
  int
  NXgetnextentry (NXhandle fid, NXname name, NXname nxclass, int *datatype)
  {
    pNexusFile pFile;
    int iRet, iStackPtr, iCurDir;
    int32 iTemp, iD1, iD2, iA;
    int32 iDim[MAX_VAR_DIMS];
  
    pFile = NXIassert (fid);
  
    iStackPtr = pFile->iStackPtr;
    iCurDir   = pFile->iStack[pFile->iStackPtr].iCurDir;
  
    /* first case to check for: no directory entry */
    if (pFile->iStack[pFile->iStackPtr].iRefDir == NULL) {
      iRet = NXIInitDir (pFile);
      if (iRet < 0) {
        NXIReportError (NXpData,
                        "ERROR: no memory to store directory info");
        return NX_EOD;
      }
    }
    /* Next case: end of directory */
    if (iCurDir >= pFile->iStack[pFile->iStackPtr].iNDir) {
      NXIKillDir (pFile);
      return NX_EOD;
    }
    /* Next case: we have data! supply it and increment counter */
    if (pFile->iCurrentVG == 0) { /* root level */
      iTemp = Vattach (pFile->iVID,
                       pFile->iStack[iStackPtr].iRefDir[iCurDir], "r");
      if (iTemp < 0) {
        NXIReportError (NXpData, "ERROR: HDF cannot attach to Vgroup");
        return NX_ERROR;
      }
      Vgetname (iTemp, name);
      Vgetclass (iTemp, nxclass);
      *datatype = DFTAG_VG;
      pFile->iStack[pFile->iStackPtr].iCurDir++;
      Vdetach (iTemp);
      return NX_OK;
    } else {                      /* in Vgroup */
      if (pFile->iStack[iStackPtr].iTagDir[iCurDir] == DFTAG_VG) {/* Vgroup */
        iTemp = Vattach (pFile->iVID,
                         pFile->iStack[iStackPtr].iRefDir[iCurDir], "r");
        if (iTemp < 0) {
          NXIReportError (NXpData, "ERROR: HDF cannot attach to Vgroup");
          return NX_ERROR;
        }
        Vgetname (iTemp, name);
        Vgetclass (iTemp, nxclass);
        *datatype = DFTAG_VG;
        pFile->iStack[pFile->iStackPtr].iCurDir++;
        Vdetach (iTemp);
        return NX_OK;
      } else if ((pFile->iStack[iStackPtr].iTagDir[iCurDir] == DFTAG_SDG) ||
                 (pFile->iStack[iStackPtr].iTagDir[iCurDir] == DFTAG_NDG)) {
        iTemp = SDreftoindex (pFile->iSID,
                              pFile->iStack[iStackPtr].iRefDir[iCurDir]);
        iTemp = SDselect (pFile->iSID, iTemp);
        SDgetinfo (iTemp, name, &iA, iDim, &iD1, &iD2);
        strcpy (nxclass, "SDS");
        *datatype = iD1;
        SDendaccess (iTemp);
        pFile->iStack[pFile->iStackPtr].iCurDir++;
        return NX_OK;
      } else {                    /* unidentified */
        strcpy (name, "UNKNOWN");
        strcpy (nxclass, "UNKNOWN");
        pFile->iStack[pFile->iStackPtr].iCurDir++;
        return NX_OK;
      }
    }
    return NX_ERROR;              /* not reached */
  }
  
  
  /*-------------------------------------------------------------------------*/
  NXstatus
  NXgetnextattr (NXhandle fileid, NXname pName,
                 int *iLength, int *iType)
  {
    pNexusFile pFile;
    int iRet;
    int32 iPType, iCount;
  
    pFile = NXIassert (fileid);
  
    /* first check if we have to start a new attribute search */
    if (pFile->iAtt.iNDir == 0) {
      iRet = NXIInitAttDir (pFile);
      if (iRet == NX_ERROR) {
        return NX_ERROR;
      }
    }
    /* are we done ? */
    if (pFile->iAtt.iCurDir >= pFile->iAtt.iNDir) {
      NXIKillAttDir (pFile);
      return NX_EOD;
    }
    /* well, there must be data to copy */
    if (pFile->iCurrentSDS == 0) {        /* global attribute */
      iRet = SDattrinfo (pFile->iSID, pFile->iAtt.iCurDir,
                         pName, &iPType, &iCount);
    } else {
      iRet = SDattrinfo (pFile->iCurrentSDS, pFile->iAtt.iCurDir,
                         pName, &iPType, &iCount);
    }
    if (iRet < 0) {
      NXIReportError (NXpData, "ERROR: HDF cannot read attribute info");
      return NX_ERROR;
    }
    *iLength = iCount * DFKNTsize (iPType);
    *iType = iPType;
    pFile->iAtt.iCurDir++;
    return NX_OK;
  }
  
  
  NXstatus
  NXgetgroupID (NXhandle fileid, NXlink* sRes)
  {
    pNexusFile pFile;
  
    pFile = NXIassert (fileid);
  
    if (pFile->iCurrentVG == 0) {
      sRes->iTag = NX_ERROR;
      return NX_ERROR;
    } else {
      sRes->iTag = DFTAG_VG;
      sRes->iRef = pFile->iCurrentVG;
      return NX_OK;
    }
    /* not reached */
    sRes->iTag = NX_ERROR;
    return NX_ERROR;
  }
  
  
  NXstatus
  NXgetdataID (NXhandle fid, NXlink* sRes)
  {
    pNexusFile pFile;
  
    pFile = NXIassert (fid);
  
    if (pFile->iCurrentSDS == 0) {
      sRes->iTag = NX_ERROR;
      return NX_ERROR;
    } else {
      sRes->iTag = DFTAG_SDS;
      sRes->iRef = SDidtoref (pFile->iCurrentSDS);
      return NX_OK;
    }
    sRes->iTag = NX_ERROR;
    return NX_ERROR;                  /* not reached */
  }
  
  
  NXstatus
  NXmakelink (NXhandle fid, NXlink* sLink)
  {
    pNexusFile pFile;
  
    pFile = NXIassert (fid);
  
    if (pFile->iCurrentVG == 0) { /* root level, can not link here */
      return NX_ERROR;
    }
    if (sLink->iTag == DFTAG_VG) {
      Vinsert (pFile->iCurrentVG, sLink->iRef);
    } else {
      Vaddtagref (pFile->iCurrentVG, sLink->iTag, sLink->iRef);
    }
    return NX_OK;
  }
  
/* allocate space for an array of given dimensions and type */
  NXstatus
  NXmalloc (void** data, int rank, int dimensions[], int datatype)
  {
	int i;
	size_t size = 1;
	*data = NULL;
	for(i=0; i<rank; i++)
	    size *= dimensions[i];
	switch(datatype)
	{
	    case DFNT_INT8:
	    case DFNT_UINT8:
		break;		/* size is correct already */

	    case DFNT_INT16:
	    case DFNT_UINT16:
		size *= 2;
		break;

	    case DFNT_INT32:
	    case DFNT_UINT32:
	    case DFNT_FLOAT32:
		size *= 4;
		break;

	    case DFNT_FLOAT64:
		size *= 8;
		break;

	    default:
      		NXIReportError (NXpData, "ERROR: NXmalloc - unknown data type in array");
		return NX_ERROR;
		break;
	}
	*data = (void*)malloc(size);
	return NX_OK;
  }

/* free space allocated by NXmalloc */
  NXstatus
  NXfree (void** data)
  {
	if (data == NULL)
	{
      	    NXIReportError (NXpData, "ERROR: passing NULL to NXfree");
	    return NX_ERROR;
	}
	if (*data == NULL)
	{
      	    NXIReportError (NXpData, "ERROR: passing already freed pointer to NXfree");
	    return NX_ERROR;
	}
        free(*data);
        *data = NULL;
	return NX_OK;
  }
