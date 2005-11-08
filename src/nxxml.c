/*
 * This is the implementation file for the XML file driver
 * for NeXus
 *
 *   Copyright (C) 2004 Mark Koennecke
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  For further information, see <http://www.neutron.anl.gov/NeXus/>
 */
#include <stdio.h>
#include <napi.h>
#include <assert.h>
#include <mxml.h>
#include <nxxml.h>
#include "nxio.h"
#include "nxdataset.h"


extern  void *NXpData;


/*----------------------- our data structures --------------------------
  One might wonder why a node stack is still needed even if this API
  operates on top of a tree API. The reason for this are the links.
  Following a link on any NXopenpath, data means a jump through the
  whole tree. In order to correctly return from such adventures,
  a stack is needed. Moreover we need it in order to keep track of the
  state of search operations.

  The true NXroot node is always at stack[0]. The root in the data
  structure is the ?xml element. The latter one is needed to store
  the tree.
-----------------------------------------------------------------------*/
typedef struct {
    mxml_node_t *current;
    mxml_node_t *currentChild;
    int currentAttribute;
}xmlStack;
/*---------------------------------------------------------------------*/
typedef struct {
  mxml_node_t *root;           /* root node */
  int readOnly;                /* read only flag */
  int stackPointer;            /* stack pointer */
  char filename[1024];         /* file name, for NXflush, NXclose */
  xmlStack stack[NXMAXSTACK];  /* stack */
}XMLNexus, *pXMLNexus;
/*===================== support functions ===============================*/
extern char *stptok(char *s, char *tok, size_t toklen, char *brk);
/*----------------------------------------------------------------------*/
static mxml_node_t *getLinkTarget(pXMLNexus xmlHandle, const char *target){
  mxml_node_t *node = NULL;
  mxml_node_t *testNode = NULL;
  char path[132], *pPtr;

  pPtr = (char *)target + 1;
  node = xmlHandle->stack[0].current;
  while((pPtr = stptok(pPtr,path,131,"/")) != NULL){
    /*
      search for group node
    */
    testNode = mxmlFindElement(node,node,NULL,"name",path,MXML_DESCEND_FIRST);
    if(testNode == NULL){
      /*
	it can still be a data node
      */
      testNode = mxmlFindElement(node,node,path,NULL,NULL,MXML_DESCEND_FIRST);
    }
    if(testNode == NULL){
      NXIReportError(NXpData,"Cannot follow broken link");
      return NULL;
    } else {
      node = testNode;
    }
  }
  return node;
}
/*==================== file functions ===================================*/
static void errorCallbackForMxml(const char *txt){
  NXIReportError(NXpData,(char *)txt);
}
/*-----------------------------------------------------------------------*/
NXstatus  NXXopen(CONSTCHAR *filename, NXaccess am, 
			       NXhandle* pHandle) {
  pXMLNexus xmlHandle = NULL;
  FILE *fp = NULL;
  char *time_buffer = NULL;
  mxml_node_t *current;

  /*
    allocate data
  */
  xmlHandle = (pXMLNexus)malloc(sizeof(XMLNexus));
  if(!xmlHandle){
    NXIReportError(NXpData, "Out of memory allocating XML file handle");
    return NX_ERROR;
  }
  memset(xmlHandle,0,sizeof(XMLNexus));

  /*
    initialize mxml XML parser
  */
  mxmlSetCustomHandlers(nexusLoadCallback, nexusWriteCallback);
  initializeNumberFormats();
  mxmlSetErrorCallback(errorCallbackForMxml);

  /*
    open file
  */
  strncpy(xmlHandle->filename,filename,1023);
  switch(am){
  case NXACC_READ:
    xmlHandle->readOnly = 1;
  case NXACC_RDWR:
    fp = fopen(filename,"r");
    if(fp == NULL){
      NXIReportError(NXpData,"Failed to open file:");
      NXIReportError(NXpData,(char *)filename);
      free(xmlHandle);
      return NX_ERROR;
    }
    xmlHandle->root = mxmlLoadFile(NULL,fp,nexusTypeCallback);
    xmlHandle->stack[0].current = mxmlFindElement(xmlHandle->root,
						  xmlHandle->root,
						  "NXroot",
						  NULL,NULL,
						  MXML_DESCEND);
    xmlHandle->stack[0].currentChild = NULL;
    xmlHandle->stack[0].currentAttribute = 0;
    fclose(fp);
    break;
  case NXACC_CREATEXML:
    xmlHandle->root = mxmlNewElement(NULL,
		   "?xml version=\"1.0\" encoding=\"UTF-8\"?");
    current = mxmlNewElement(xmlHandle->root,"NXroot");
    mxmlElementSetAttr(current,"NeXus_version",NEXUS_VERSION);
    mxmlElementSetAttr(current,"XML_version","mxml");
    mxmlElementSetAttr(current,"file_name",filename);
    time_buffer = NXIformatNeXusTime();
    if(time_buffer != NULL){
      mxmlElementSetAttr(current,"file_time",time_buffer);
      free(time_buffer);
    } 
    xmlHandle->stack[0].current = current;
    xmlHandle->stack[0].currentChild = NULL;
    xmlHandle->stack[0].currentAttribute = 0;
    break;
  default:
    NXIReportError(NXpData,"Bad access parameter specified in NXXopen");
    return NX_ERROR;
  }
  if(xmlHandle->stack[0].current == NULL){
      NXIReportError(NXpData,
		     "No NXroot element in XML-file, no NeXus-XML file");
      return NX_ERROR;
  }

  *pHandle = xmlHandle;
  return NX_OK;
}
/*----------------------------------------------------------------------*/
NXstatus  NXXclose (NXhandle* fid){
  pXMLNexus xmlHandle = NULL;
  FILE *fp = NULL;

  xmlHandle = (pXMLNexus)*fid;
  assert(xmlHandle);
  
  if(xmlHandle->readOnly == 0) {
    fp = fopen(xmlHandle->filename,"w");
    if(fp == NULL){
      NXIReportError(NXpData,"Failed to open NeXus XML file for writing");
      return NX_ERROR;
    }
    mxmlSaveFile(xmlHandle->root,fp,NXwhitespaceCallback);
    fclose(fp);
  }
  mxmlDelete(xmlHandle->root);
  free(xmlHandle);
  *fid = NULL;
  return NX_OK;
}
/*----------------------------------------------------------------------*/
NXstatus  NXXflush(NXhandle *fid){
  pXMLNexus xmlHandle = NULL;
  FILE *fp = NULL;

  xmlHandle = (pXMLNexus)*fid;
  assert(xmlHandle);
  
  if(xmlHandle->readOnly == 0) {
    fp = fopen(xmlHandle->filename,"w");
    if(fp == NULL){
      NXIReportError(NXpData,"Failed to open NeXus XML file for writing");
      return NX_ERROR;
    }
    mxmlSaveFile(xmlHandle->root,fp,NXwhitespaceCallback);
    fclose(fp);
  }
  return NX_OK;
}
/*=======================================================================
                   Group functions
=========================================================================*/
NXstatus  NXXmakegroup (NXhandle fid, CONSTCHAR *name, 
				     CONSTCHAR *nxclass){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *newGroup = NULL;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"Close dataset before trying to create a group");
    return NX_ERROR;
  }

  newGroup = mxmlNewElement(xmlHandle->stack[xmlHandle->stackPointer].current,
			    nxclass);
  if(!newGroup){
    NXIReportError(NXpData,"failed to allocate new group");
    return NX_ERROR;
  }
  mxmlElementSetAttr(newGroup,"name",name);
  return NX_OK;
} 
/*----------------------------------------------------------------------*/
static mxml_node_t *searchGroupLinks(pXMLNexus xmlHandle, CONSTCHAR *name, 
				CONSTCHAR *nxclass){
  mxml_node_t *linkNode = NULL;
  mxml_node_t *current;
  mxml_node_t *test = NULL;
  const char *linkTarget;

  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  linkNode = current;
  while((linkNode = mxmlFindElement(linkNode,current,"NAPIlink",NULL,NULL,
				    MXML_DESCEND_FIRST)) != NULL){
    linkTarget = mxmlElementGetAttr(linkNode,"target");
    test = getLinkTarget(xmlHandle,linkTarget);
    if(test != NULL){
      if(strcmp(test->value.element.name,nxclass) == 0){
	if(strcmp(mxmlElementGetAttr(test,"name"),name) == 0){
	  return test;
	}
      }
    }
  }
  return NULL;
}
/*------------------------------------------------------------------------*/
NXstatus  NXXopengroup (NXhandle fid, CONSTCHAR *name, 
				     CONSTCHAR *nxclass){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *newGroup = NULL;
  char error[1024];

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"Close dataset before trying to open a group");
    return NX_ERROR;
  }
  newGroup = mxmlFindElement(xmlHandle->stack[xmlHandle->stackPointer].current,
			     xmlHandle->stack[xmlHandle->stackPointer].current,
			     nxclass,
			     "name",
			     name,
			     MXML_DESCEND_FIRST);
  if(newGroup == NULL){
    newGroup = searchGroupLinks(xmlHandle,name,nxclass);
  }
  if(!newGroup){
    snprintf(error,1023,"Failed to open %s, %s",name,nxclass);
    NXIReportError(NXpData,error);
    return NX_ERROR;
  }
  xmlHandle->stackPointer++;
  xmlHandle->stack[xmlHandle->stackPointer].current = newGroup;
  xmlHandle->stack[xmlHandle->stackPointer].currentChild = NULL;
  xmlHandle->stack[xmlHandle->stackPointer].currentAttribute = 0;
  return NX_OK;
}
/*----------------------------------------------------------------------*/
NXstatus  NXXclosegroup (NXhandle fid){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *newGroup = NULL;
  char error[1024];

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      silently fix this
    */
    NXXclosedata(fid);
  }
  if(xmlHandle->stackPointer > 0){
    xmlHandle->stackPointer--;
  }
  return NX_OK;
}
/*=========================================================================
         dataset functions
=========================================================================*/
NXstatus  NXXcompmakedata (NXhandle fid, CONSTCHAR *name, 
					int datatype, 
					int rank, 
					int dimensions[],
					int compress_type, int chunk_size[]){
  /*
    compression does not relly make sense with XML
  */
  return NXXmakedata(fid,name,datatype,rank,dimensions);
}
/*-----------------------------------------------------------------------*/
static char *buildTypeString(int datatype, int rank, int dimensions[]){
  char *typestring = NULL;
  char pNumber[20];
  int i;

  /*
    allocate data
  */
  typestring = (char *)malloc(132*sizeof(char));
  if(!typestring){
    NXIReportError(NXpData,"Failed to allocate typestring");
    return NULL;
  }
  memset(typestring,0,132*sizeof(char));

  getNumberText(datatype,typestring,130);
  if(rank > 1 || dimensions[0] > 1) {
    strcat(typestring,"[");
    snprintf(pNumber,19,"%d",dimensions[0]);
    strncat(typestring,pNumber,130-strlen(typestring));
    for(i = 1; i < rank; i++){
      snprintf(pNumber,19,",%d",dimensions[i]);
      strncat(typestring,pNumber,130-strlen(typestring));
    }
    strcat(typestring,"]");
  }
  return typestring;
}
/*------------------------------------------------------------------------*/
NXstatus  NXXmakedata (NXhandle fid, 
				    CONSTCHAR *name, int datatype, 
				    int rank, int dimensions[]){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *dataNode = NULL;
  mxml_node_t *newData = NULL;
  mxml_node_t *current;
  char *typestring;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"Close dataset before trying to create a dataset");
    return NX_ERROR;
  }
  if(dimensions[0] < 0){
    NXIReportError(NXpData,
		   "NeXus XML-API does not support unlimited dimensions");
    return NX_ERROR;
  }

  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  /*
    NX_CHAR maps to MXML_OPAQUE datasets
  */
  dataNode = mxmlNewElement(current,name);
  if(datatype == NX_CHAR){
    newData = mxmlNewOpaque(dataNode,"");
    return NX_OK;
  } else {
    typestring = buildTypeString(datatype,rank,dimensions);
    if(typestring != NULL){
      mxmlElementSetAttr(dataNode,TYPENAME,typestring);
      free(typestring);
    } else {
      NXIReportError(NXpData,"Failed to allocate typestring");
      return NX_ERROR;
    }
    newData = (mxml_node_t *)malloc(sizeof(mxml_node_t));
    if(!newData){
      NXIReportError(NXpData,"Failed to allocate space for dataset");
      return NX_ERROR;
    }
    memset(newData,0,sizeof(mxml_node_t));
    mxmlAdd(dataNode, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, newData);
    newData->type = MXML_CUSTOM;
    newData->value.custom.data = createNXDataset(rank,datatype,dimensions);
    if(!newData->value.custom.data){
      NXIReportError(NXpData,"Failed to allocate space for dataset");
      return NX_ERROR;
    }
    newData->value.custom.destroy = destroyDataset;
  }
  return NX_OK;
}
/*----------------------------------------------------------------------*/
static mxml_node_t *searchSDSLinks(pXMLNexus xmlHandle, CONSTCHAR *name){
  mxml_node_t *linkNode = NULL;
  mxml_node_t *current;
  mxml_node_t *test = NULL;
  const char *linkTarget;

  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  linkNode = current;
  while((linkNode = mxmlFindElement(linkNode,current,"NAPIlink",NULL,NULL,
				    MXML_DESCEND_FIRST)) != NULL){
    linkTarget = mxmlElementGetAttr(linkNode,"target");
    test = getLinkTarget(xmlHandle,linkTarget);
    if(test != NULL){
      if(strcmp(test->value.element.name,name) == 0){
	return test;
      }
    }
  }
  return NULL;
}
/*-------------------------------------------------------------------*/
static int strtrimcr(char *szStr, char *szSet)
{
      int   i, j;                                     /* Locale counters */

      /*-------------------------------------------------*/

      j = i = strlen(szStr) - 1;                /* Find length of string */

      while (strrchr(szSet, szStr[ i ])
                  && (0 <= i))
      {
            /* While string is terminated by one of the specified characters */
            szStr[ i-- ] = '\0';          /*- Replace character with '\0' */
      }

      return(j - i);    /* Return the difference between old and new length */
}
/*-----------------------------------------------------------------------*/
NXstatus  NXXopendata (NXhandle fid, CONSTCHAR *name){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *dataNode = NULL;
  char error[1024];

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      silently fix this
    */
    xmlHandle->stackPointer--;
    if(xmlHandle->stackPointer < 0){
      xmlHandle->stackPointer = 0;
    }
  }
  
  dataNode = mxmlFindElement(xmlHandle->stack[xmlHandle->stackPointer].current,
			     xmlHandle->stack[xmlHandle->stackPointer].current,
			     name,
			     NULL,
			     NULL,
			     MXML_DESCEND_FIRST);
  if(dataNode == NULL){
    dataNode = searchSDSLinks(xmlHandle,name);
  }
  if(!dataNode){
    snprintf(error,1023,"Failed to open dataset %s",name);
    NXIReportError(NXpData,error);
    return NX_ERROR;
  }
  xmlHandle->stackPointer++;
  xmlHandle->stack[xmlHandle->stackPointer].current = dataNode;
  xmlHandle->stack[xmlHandle->stackPointer].currentChild = NULL;
  xmlHandle->stack[xmlHandle->stackPointer].currentAttribute = 0;
  return NX_OK;
}
/*----------------------------------------------------------------------*/
NXstatus  NXXclosedata (NXhandle fid){
  pXMLNexus xmlHandle = NULL;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    if(xmlHandle->stackPointer > 0){
      xmlHandle->stackPointer--;
    }
    return NX_OK;
  }
  return NX_OK;
}
/*----------------------------------------------------------------------*/
static mxml_node_t *findData(mxml_node_t *node){
  mxml_node_t *baby = node;
  
  while( (baby = mxmlWalkNext(baby,node,MXML_DESCEND_FIRST)) != NULL){
    if(baby->type == MXML_OPAQUE || baby->type == MXML_CUSTOM){
      return baby;
    }
  }
  return NULL;
}
/*------------------------------------------------------------------------*/
NXstatus  NXXputdata (NXhandle fid, void *data){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *userData = NULL;
  mxml_node_t *current = NULL;
  pNXDS dataset;
  int length;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No dataset open");
    return NX_ERROR;
  }
  
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  userData = findData(current);
  assert(userData != NULL);
  if(userData->type == MXML_OPAQUE){
    /*
      text data
    */
    mxmlSetOpaque(userData,(const char *)data);
  } else {
    dataset = (pNXDS)userData->value.custom.data;
    assert(dataset);
    length = getNXDatasetByteLength(dataset);
    memcpy(dataset->u.ptr,data,length);
  }
  return NX_OK;
}
/*------------------------------------------------------------------------*/
NXstatus  NXXgetdata (NXhandle fid, void *data){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *userData = NULL;
  mxml_node_t *current = NULL;
  pNXDS dataset;
  int length;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No dataset open");
    return NX_ERROR;
  }
  
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  userData = findData(current);
  assert(userData != NULL);
  if(userData->type == MXML_OPAQUE){
    /*
      text data
    */
    strcpy((char *)data,userData->value.opaque);
  } else {
    dataset = (pNXDS)userData->value.custom.data;
    assert(dataset);
    length = getNXDatasetByteLength(dataset);
    memcpy(data,dataset->u.ptr,length);
  }
  return NX_OK;
}
/*------------------------------------------------------------------------*/
NXstatus  NXXgetinfo (NXhandle fid, int *rank, 
				   int dimension[], int *iType){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *userData = NULL;
  mxml_node_t *current = NULL;
  pNXDS dataset;
  int myRank, i;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No dataset open");
    return NX_ERROR;
  }
  
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  userData = findData(current);
  assert(userData != NULL);
  if(userData->type == MXML_OPAQUE){
    /*
      text data
    */
    *rank = 1;
    *iType = NX_CHAR;
    dimension[0]= strlen(userData->value.opaque);
  } else { 
    dataset = (pNXDS)userData->value.custom.data;
    assert(dataset);
    myRank = getNXDatasetRank(dataset);
    *rank = myRank;
    *iType = getNXDatasetType(dataset);
    for(i = 0; i < myRank; i++){
      dimension[i] = getNXDatasetDim(dataset,i);
    }
  }
  return NX_OK;
}
/*---------------------------------------------------------------------
  clone the dataset and set the data pointer. This in order to use
  the addressing and type conversion implemented in nxdataset
---------------------------------------------------------------------*/ 
static pNXDS makeSlabData(pNXDS dataset, void *data, int size[]){
  pNXDS slabData = NULL;
  int rank, i;
  
  slabData = (pNXDS)malloc(sizeof(NXDS));
  if(slabData == NULL){
    return NULL;
  }

  rank = getNXDatasetRank(dataset);
  slabData->rank = rank;
  slabData->dim = (int *)malloc(rank*sizeof(int));
  for(i = 0; i < rank; i++){
    slabData->dim[i] = size[i];
  }
  slabData->type = getNXDatasetType(dataset);
  slabData->u.ptr = data;
  slabData->magic = dataset->magic;
  return slabData;
} 
/*--------------------------------------------------------------------
  This goes by recursion
----------------------------------------------------------------------*/
static void putSlabData(pNXDS dataset, pNXDS slabData, int dim,
			int start[], 
			int sourcePos[],int targetPos[]){
  int i, rank, length;

  rank = getNXDatasetRank(slabData);
  length = getNXDatasetDim(slabData,dim);
  if(dim != rank-1){
    for(i = 0; i < length; i++){
      targetPos[dim] = start[dim] +i;
      sourcePos[dim] = i;
      putSlabData(dataset,slabData, dim+1,start,
		  sourcePos,targetPos);
    }
  } else {
    for(i = 0; i < length; i++){
      targetPos[dim] = start[dim] +i;
      sourcePos[dim] = i;
      putNXDatasetValue(dataset,targetPos,
			getNXDatasetValue(slabData,sourcePos));
    }
  }
}
/*----------------------------------------------------------------------*/
NXstatus  NXXputslab (NXhandle fid, void *data, 
				   int iStart[], int iSize[]){
  
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *userData = NULL;
  mxml_node_t *current = NULL;
  pNXDS dataset, slabData;
  int sourcePos[NX_MAXRANK], targetPos[NX_MAXRANK];

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No dataset open");
    return NX_ERROR;
  }
  
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  userData = findData(current);
  assert(userData != NULL);
  if(userData->type == MXML_OPAQUE){
    NXIReportError(NXpData,"This API does not support slabs on text data");
    return NX_ERROR;
  }
  dataset = (pNXDS)userData->value.custom.data;
  assert(dataset);
  slabData = makeSlabData(dataset, data, iSize);
  if(slabData == NULL){
    NXIReportError(NXpData,"Failed to allocate slab data");
    return NX_ERROR;
  }
  putSlabData(dataset,slabData,0,iStart,sourcePos,targetPos);
  free(slabData->dim);
  free(slabData);
  
  return NX_OK;
}
/*--------------------------------------------------------------------
  This goes by recursion
----------------------------------------------------------------------*/
static void getSlabData(pNXDS dataset, pNXDS slabData, int dim,
			int start[], 
			int sourcePos[],int targetPos[]){
  int i, rank, length;

  rank = getNXDatasetRank(slabData);
  length = getNXDatasetDim(slabData,dim);
  if(dim != rank-1){
    for(i = 0; i < length; i++){
      sourcePos[dim] = start[dim] +i;
      targetPos[dim] = i;
      getSlabData(dataset,slabData, dim+1,start,
		  sourcePos,targetPos);
    }
  } else {
    for(i = 0; i < length; i++){
      sourcePos[dim] = start[dim] +i;
      targetPos[dim] = i;
      putNXDatasetValue(slabData,targetPos,
			getNXDatasetValue(dataset,sourcePos));
    }
  }
}
/*----------------------------------------------------------------------*/
NXstatus  NXXgetslab (NXhandle fid, void *data, 
				   int iStart[], int iSize[]){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *userData = NULL;
  mxml_node_t *current = NULL;
  pNXDS dataset, slabData;
  int sourcePos[NX_MAXRANK], targetPos[NX_MAXRANK];

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No dataset open");
    return NX_ERROR;
  }
  
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  userData = findData(current);
  assert(userData != NULL);
  if(userData->type == MXML_OPAQUE){
    NXIReportError(NXpData,"This API does not support slabs on text data");
    return NX_ERROR;
  }
  dataset = (pNXDS)userData->value.custom.data;
  assert(dataset);
  slabData = makeSlabData(dataset, data, iSize);
  if(slabData == NULL){
    NXIReportError(NXpData,"Failed to allocate slab data");
    return NX_ERROR;
  }
  getSlabData(dataset,slabData,0,iStart,sourcePos,targetPos);
  free(slabData->dim);
  free(slabData);
  
  return NX_OK;
}
/*----------------------------------------------------------------------*/
static NXstatus  NXXsetnumberformat(NXhandle fid,
						 int type, char *format){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  mxml_node_t *userData = NULL;
  pNXDS dataset;
  
  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    current = xmlHandle->stack[xmlHandle->stackPointer].current;
    userData = findData(current);
    assert(userData != NULL);
    if(userData->type == MXML_OPAQUE){
      return NX_OK;
    }
    dataset = (pNXDS)userData->value.custom.data;
    assert(dataset);
    if(dataset->format != NULL){
      free(dataset->format);
    }
    dataset->format = strdup(format);
  } else {
    setNumberFormat(type, format);
  }
  return NX_OK;
}
/*============================ Attributes ============================*/
static char *formatAttributeData(void *data, int datalen, int iType){
  int intData = 0;
  long iValue;
  double dValue;
  char type[20];
  char *number;


  if(iType == NX_CHAR){
    return strdup((char *)data);
  }

  number = (char *)malloc(132*sizeof(char));
  if(!number){
    NXIReportError(NXpData,"Failed to allocate attribute number buffer");
    return NULL;
  }
  
  if(datalen > 1){
    return NULL;
  }
  type[0] = '\0';
  switch(iType){
  case NX_INT32:
    iValue = ((int *)data)[0];
    intData = 1;
    strcpy(type,"NX_INT32:");
    break;
  case NX_UINT32:
    iValue = ((unsigned int *)data)[0];
    intData = 1;
    strcpy(type,"NX_UINT32:");
    break;
  case NX_INT16:
    iValue = ((short *)data)[0];
    intData = 1;
    strcpy(type,"NX_INT16:");
    break;
  case NX_UINT16:
    iValue = ((unsigned short *)data)[0];
    intData = 1;
    strcpy(type,"NX_UINT16:");
    break;
  case NX_INT8:
    iValue = (int)((char *)data)[0];
    intData = 1;
    strcpy(type,"NX_INT8:");
    break;
  case NX_UINT8:
    intData = 1;
    iValue = (int)((unsigned char *)data)[0];
    strcpy(type,"NX_UINT8:");
    break;
  case NX_FLOAT32:
    dValue = ((float *)data)[0];
    strcpy(type,"NX_FLOAT32:");
    intData = 0;
    break;
  case NX_FLOAT64:
    dValue = ((double *)data)[0];
    strcpy(type,"NX_FLOAT64:");
    intData = 0;
    break;
  }
  if(intData){
    snprintf(number,79,"%s%d",type,iValue);
  } else {
    snprintf(number,79,"%s%f",type,dValue);
  }
  return number;
}
/*---------------------------------------------------------------------*/
NXstatus  NXXputattr (NXhandle fid, CONSTCHAR *name, void *data, 
				   int datalen, int iType){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  char *numberData = NULL;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      global attribute
    */
    current = xmlHandle->stack[0].current;
  } else {
    /*
      dataset attribute
    */
    current = xmlHandle->stack[xmlHandle->stackPointer].current;
    if(strcmp(name,TYPENAME) == 0){
      NXIReportError(NXpData,"type is a reserved attribute name, rejected");
      return  NX_ERROR;
    }
  }

  numberData = formatAttributeData(data,datalen,iType);
  if(numberData == NULL){
    NXIReportError(NXpData,"This API does not support non number arrays");
    return NX_ERROR;
  } else {
    mxmlElementSetAttr(current,name,numberData);
    free(numberData);
  }
  return NX_OK;
}
/*--------------------------------------------------------------------------*/
NXstatus  NXXgetattr (NXhandle fid, char *name, 
				   void *data, int* datalen, int* iType){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  const char *attribute = NULL;
  char error[1024];
  char *attData = NULL;
  int iValue, nx_type;
  float fValue;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      global attribute
    */
    current = xmlHandle->stack[0].current;
  } else {
    /*
      dataset attribute
    */
    current = xmlHandle->stack[xmlHandle->stackPointer].current;
  }

  attribute = mxmlElementGetAttr(current,name);
  if(!attribute){
    snprintf(error,1023,"Attribute %s not found", name);
    NXIReportError(NXpData,error);
    return NX_ERROR;
  }
  nx_type = translateTypeCode((char *)attribute);
  if(nx_type < 0) {
    /*
      no type code == text attribute
    */
    nx_type = NX_CHAR;
  } else {
    /*
      We need to find the number after the type code. However, there is
      the complication of the datatype type attribute ...
    */
    if(strcmp(name,TYPENAME) == 0){
      nx_type = NX_CHAR;
    } else {
      attData = strchr(attribute,(int)':');
      if(attData == NULL){
	NXIReportError(NXpData,"ERROR: bad attribute string, : missing");
	return NX_ERROR;
      }
      attData++;
    }
  }
  *iType = nx_type;
  switch(nx_type){
  case NX_CHAR:
    strncpy((char *)data, attribute, *datalen);
    *datalen = strlen(attribute);
    *iType = NX_CHAR;
    break;
  case NX_INT32:
    ((int *)data)[0] = atoi(attData);
    *datalen = 1;
    break;
  case NX_UINT32:
    ((unsigned int *)data)[0] = atoi(attData);
    *datalen = 1;
    break;
  case NX_INT16:
    ((short *)data)[0] = atoi(attData);
    *datalen = 1;
    break;
  case NX_UINT16:
    ((unsigned short *)data)[0] = atoi(attData);
    *datalen = 1;
    break;
  case NX_INT8:
    ((char *)data)[0] = atoi(attData);
    *datalen = 1;
    break;
  case NX_UINT8:
    ((unsigned char *)data)[0] = atoi(attData);
    *datalen = 1;
    break;
  case NX_FLOAT32:
    ((float *)data)[0] = atof(attData);
    *datalen = 1;
    break;
  case NX_FLOAT64:
    ((double *)data)[0] = atof(attData);
    *datalen = 1;
    break;
  }

  return NX_OK;
}
/*====================== search functions =================================*/
NXstatus  NXXgetnextentry (NXhandle fid,NXname name, 
					NXname nxclass, int *datatype){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *next = NULL, *userData;
  int stackPtr;
  const char *target = NULL, *attname = NULL;
  pNXDS dataset;
  char pBueffel[256];

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      be nice to users: close the open dataset even as this is
      a usage error
    */
    NXIReportError(NXpData,
		   "WARNING: fixing bad NAPI usage: dataset still open");
    NXXclosedata(fid);
  }

  stackPtr = xmlHandle->stackPointer;
  if(xmlHandle->stack[stackPtr].currentChild == NULL){
    /*
      initialization of search
    */
    xmlHandle->stack[stackPtr].currentChild = 
      xmlHandle->stack[stackPtr].current->child;
  } else {
    /*
      proceed
    */
    xmlHandle->stack[stackPtr].currentChild = 
      xmlHandle->stack[stackPtr].currentChild->next;
  }
  next = xmlHandle->stack[stackPtr].currentChild;
  if(next == NULL){
    return NX_EOD;
  }
  if(strcmp(next->value.element.name,"NAPIlink") == 0){
    target = mxmlElementGetAttr(next,"target");
    if(target == NULL){
      NXIReportError(NXpData,"Corrupted file, NAPIlink without target");
      return NX_ERROR;
    }
    next = getLinkTarget(xmlHandle,target);
    if(next == NULL){
      NXIReportError(NXpData,"Corrupted file, broken link");
      return NX_ERROR;
    }
  }

  if(isDataNode(next)){
    strcpy(name,next->value.element.name);
    strcpy(nxclass,"SDS");
    userData = findData(next);
    if(userData == NULL){
	snprintf(pBueffel,255,"Corrupted file, userData for %s not found",
		 name);
      NXIReportError(NXpData,pBueffel);
      return NX_ERROR;
    }
    if(userData->type == MXML_OPAQUE){
      *datatype = NX_CHAR;
    } else {
      dataset = (pNXDS)userData->value.custom.data;
      assert(dataset);
      *datatype = getNXDatasetType(dataset);
    }
  } else {
    strcpy(nxclass,next->value.element.name);
    attname = mxmlElementGetAttr(next,"name");
    strcpy(name,attname);
  }
  return NX_OK;
}
/*----------------------------------------------------------------------*/
extern  NXstatus NXXinitgroupdir(NXhandle fid){
  pXMLNexus xmlHandle = NULL;
  int stackPtr;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"Cannot search datasets");
    return NX_ERROR;
  }

  stackPtr = xmlHandle->stackPointer;
  xmlHandle->stack[stackPtr].currentChild = NULL;
  return NX_OK;
}
/*-------------------------------------------------------------------------*/
NXstatus  NXXgetnextattr (NXhandle fid, NXname pName,
					  int *iLength, int *iType){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  int stackPtr, currentAtt, nx_type;
  char *attVal; 

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      dataset attribute
    */
    stackPtr = xmlHandle->stackPointer;
  } else {
    /*
      global attribute
    */
    stackPtr = 0;
  }

  current = xmlHandle->stack[stackPtr].current;
  currentAtt = xmlHandle->stack[stackPtr].currentAttribute;
  if(currentAtt >= 
     current->value.element.num_attrs ){
    xmlHandle->stack[stackPtr].currentAttribute = 0;
    return NX_EOD;
  }

  /*
    hide type attribute
  */
  if(strcmp(current->value.element.attrs[currentAtt].name,TYPENAME) == 0){
    xmlHandle->stack[stackPtr].currentAttribute++;
    return NXXgetnextattr(fid,pName,iLength,iType);
  }

  strcpy(pName,current->value.element.attrs[currentAtt].name);
  attVal = current->value.element.attrs[currentAtt].value;
  nx_type = translateTypeCode((char *)attVal);
  if(nx_type < 0 || strcmp(pName,TYPENAME) == 0){
    /*
      no type == NX_CHAR
    */
    *iLength = strlen(attVal);
    *iType = NX_CHAR;
  } else {
    *iLength = 1;
    *iType = nx_type;
  }

  xmlHandle->stack[stackPtr].currentAttribute++;
  return NX_OK;
}
/*-------------------------------------------------------------------------*/
extern  NXstatus  NXXinitattrdir(NXhandle fid){
  pXMLNexus xmlHandle = NULL;
  int stackPtr;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      dataset attribute
    */
    stackPtr = xmlHandle->stackPointer;
  } else {
    /*
      global attribute
    */
    stackPtr = 0;
  }

  xmlHandle->stack[stackPtr].currentAttribute = 0;
  return NX_OK;
}
/*-------------------------------------------------------------------------*/
NXstatus  NXXgetgroupinfo (NXhandle fid, int *iN, 
					NXname pName, NXname pClass){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *child = NULL;
  mxml_node_t *current = NULL;
  const char *nameAtt = NULL;
  int childCount;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No group open");
    return NX_ERROR;
  } 
  current = xmlHandle->stack[xmlHandle->stackPointer].current;

  nameAtt = mxmlElementGetAttr(current,"name");
  if(nameAtt !=  NULL){
    strcpy(pName,nameAtt);
  }
  strcpy(pClass,current->value.element.name);

  childCount = 0;
  child = current->child;
  while(child != NULL){
    childCount++;
    child = child->next;
  }
  *iN = childCount;
  return NX_OK;
}
/*----------------------------------------------------------------------*/
NXstatus  NXXgetattrinfo (NXhandle fid, int *iN){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  int stackPtr, currentAtt;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    /*
      dataset attribute
    */
    stackPtr = xmlHandle->stackPointer;
  } else {
    /*
      global attribute
    */
    stackPtr = 0;
  }

  current = xmlHandle->stack[stackPtr].current;
  /*
    hide type attribute
  */
  if(mxmlElementGetAttr(current,TYPENAME) != NULL){
    *iN = current->value.element.num_attrs -1;
  } else {
    *iN = current->value.element.num_attrs;
  }
  return NX_OK;
}
/*================= Linking functions =================================*/
static int countPathChars(mxml_node_t *path[], int stackPtr){
  int count = 1;
  const char *name = NULL;

  while(stackPtr >= 0) {
    if(isDataNode(path[stackPtr])){
      count += strlen(path[stackPtr]->value.element.name);
    } else {
      name = mxmlElementGetAttr(path[stackPtr],"name");
      if(name != NULL){
	count += strlen(name);
      }
    }
    stackPtr--;
    count += 1;
  }
  return count;
}
/*-------------------------------------------------------------------*/
static char *buildPathString(mxml_node_t *path[], int stackPtr){
  int count = 0;
  const char *name = NULL;
  char *pathString = NULL;

  count = countPathChars(path,stackPtr);
  pathString = (char *)malloc((count+10)*sizeof(char));
  if(pathString == NULL){
    return NULL;
  }
  memset(pathString,0,(count+10)*sizeof(char));

  while(stackPtr >= 0) {
    if(isDataNode(path[stackPtr])){
      strcat(pathString,"/");
      strcat(pathString,path[stackPtr]->value.element.name);
    } else {
      name = mxmlElementGetAttr(path[stackPtr],"name");
      if(name != NULL){
	strcat(pathString,"/");
	strcat(pathString,name);
      }
    }
    stackPtr--;
  }
  return pathString;
}
/*--------------------------------------------------------------------*/
static char *findLinkPath(mxml_node_t *node){
  mxml_node_t **path = NULL;
  int stackPtr;
  mxml_node_t *current = NULL;
  char *pathString = NULL, *result = NULL;
  int count;

  path = (mxml_node_t **)malloc(NXMAXSTACK*sizeof(mxml_node_t *));
  if(path == NULL){
    NXIReportError(NXpData,"ERROR: out of memory follwoing link path");
    return NULL;
  }
  memset(path,0,NXMAXSTACK*sizeof(mxml_node_t *));

  /*
    first path:  walk up the tree untill NXroot is found
  */
  current = node;
  stackPtr = 0;
  while(current != NULL && 
	strcmp(current->value.element.name,"NXroot") != 0){
    path[stackPtr] = current;
    stackPtr++;
    current = current->parent;
  }
  stackPtr--;

  /*
    path now contains the nodes to the root node in reverse order.
    From this build the path string
  */
  result = buildPathString(path,stackPtr);
  free(path);
  return result;
}
/*--------------------------------------------------------------------*/
NXstatus  NXXgetdataID (NXhandle fid, NXlink* sRes){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  char *linkPath = NULL;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(!isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    return NX_ERROR;
  } 
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  
  linkPath = findLinkPath(current);
  if(!linkPath){
    NXIReportError(NXpData,"Failed to allocate link path string");
    return NX_ERROR;
  }
  strncpy(sRes->targetPath,linkPath,1023);
  free(linkPath);
  return NX_OK;
}
/*--------------------------------------------------------------------*/
NXstatus  NXXgetgroupID (NXhandle fid, NXlink* sRes){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL;
  char *linkPath = NULL;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No group open");
    return NX_ERROR;
  } 
  current = xmlHandle->stack[xmlHandle->stackPointer].current;

  if(xmlHandle->stackPointer == 0){
    return NX_ERROR;
  }

  linkPath = findLinkPath(current);
  if(!linkPath){
    NXIReportError(NXpData,"Failed to allocate link path string");
    return NX_ERROR;
  }
  strncpy(sRes->targetPath,linkPath,1023);
  free(linkPath);
  return NX_OK;
}
/*-----------------------------------------------------------------------*/
NXstatus  NXXmakelink (NXhandle fid, NXlink* sLink){
  pXMLNexus xmlHandle = NULL;
  mxml_node_t *current = NULL, *linkNode = NULL;
  mxml_node_t *linkedNode = NULL;

  xmlHandle = (pXMLNexus)fid;
  assert(xmlHandle);

  if(isDataNode(xmlHandle->stack[xmlHandle->stackPointer].current)){
    NXIReportError(NXpData,"No group to link to open");
    return NX_ERROR;
  } 
  current = xmlHandle->stack[xmlHandle->stackPointer].current;
  linkNode = mxmlNewElement(current,"NAPIlink");
  if(!linkNode){
    NXIReportError(NXpData,"Failed to allocate new link element");
    return NX_ERROR;
  }
  mxmlElementSetAttr(linkNode,"target",sLink->targetPath);
  linkedNode = getLinkTarget(xmlHandle,sLink->targetPath);
  if(linkedNode != NULL){
    mxmlElementSetAttr(linkedNode,"target",sLink->targetPath);
  }
  return NX_OK;
}
/*----------------------------------------------------------------------*/
NXstatus  NXXsameID (NXhandle fileid, NXlink* pFirstID, 
				  NXlink* pSecondID){
  if(strcmp(pFirstID->targetPath,pSecondID->targetPath) == 0) {
    return NX_OK;
  } else {
    return NX_ERROR;
  }
}
/*--------------------------------------------------------------------*/
int  NXXcompress(NXhandle fid, int comp){
  NXIReportError(NXpData,"NXcompress is deprecated, IGNORED");
  return NX_OK;
}
/*----------------------------------------------------------------------*/
void NXXassignFunctions(pNexusFunction fHandle){
      fHandle->nxclose=NXXclose;
      fHandle->nxflush=NXXflush;
      fHandle->nxmakegroup=NXXmakegroup;
      fHandle->nxopengroup=NXXopengroup;
      fHandle->nxclosegroup=NXXclosegroup;
      fHandle->nxmakedata=NXXmakedata;
      fHandle->nxcompmakedata=NXXcompmakedata;
      fHandle->nxcompress=NXXcompress;
      fHandle->nxopendata=NXXopendata;
      fHandle->nxclosedata=NXXclosedata;
      fHandle->nxputdata=NXXputdata;
      fHandle->nxputattr=NXXputattr;
      fHandle->nxputslab=NXXputslab;    
      fHandle->nxgetdataID=NXXgetdataID;
      fHandle->nxmakelink=NXXmakelink;
      fHandle->nxgetdata=NXXgetdata;
      fHandle->nxgetinfo=NXXgetinfo;
      fHandle->nxgetnextentry=NXXgetnextentry;
      fHandle->nxgetslab=NXXgetslab;
      fHandle->nxgetnextattr=NXXgetnextattr;
      fHandle->nxgetattr=NXXgetattr;
      fHandle->nxgetattrinfo=NXXgetattrinfo;
      fHandle->nxgetgroupID=NXXgetgroupID;
      fHandle->nxgetgroupinfo=NXXgetgroupinfo;
      fHandle->nxsameID=NXXsameID;
      fHandle->nxinitgroupdir=NXXinitgroupdir;
      fHandle->nxinitattrdir=NXXinitattrdir;
      fHandle->nxsetnumberformat=NXXsetnumberformat;
}

