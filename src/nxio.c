/**
 * This file contains functions necessary to perform XML-I/O for
 * NeXus with the mxml-library.
 * 
 * Most notably it contains the callback function for reading and
 * writing  data
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
#include <mxml.h>
#include <assert.h>
#include "nxio.h"
#include "nxdataset.h"

/* #define TESTMAIN 1 */
/*=================== type code handling ================================= */
typedef struct {
  char name[30];
  char format[30];
  int  nx_type;
}type_code;

static type_code typecode[9];
/*-----------------------------------------------------------------------*/
void initializeNumberFormats(){
  type_code myCode;

  strcpy(myCode.name,"NX_FLOAT32");
  strcpy(myCode.format,"%12.4f");
  myCode.nx_type = NX_FLOAT32;
  typecode[0] = myCode;

  strcpy(myCode.name,"NX_FLOAT64");
  strcpy(myCode.format,"%16.5f");
  myCode.nx_type = NX_FLOAT64;
  typecode[1] = myCode;

  strcpy(myCode.name,"NX_INT8");
  strcpy(myCode.format,"%5d");
  myCode.nx_type = NX_INT8;
  typecode[2] = myCode;

  strcpy(myCode.name,"NX_UINT8");
  strcpy(myCode.format,"%5d");
  myCode.nx_type = NX_UINT8;
  typecode[3] = myCode;

  strcpy(myCode.name,"NX_INT16");
  strcpy(myCode.format,"%8d");
  myCode.nx_type = NX_INT16;
  typecode[4] = myCode;

  strcpy(myCode.name,"NX_UINT16");
  strcpy(myCode.format,"%8d");
  myCode.nx_type = NX_UINT16;
  typecode[5] = myCode;

  strcpy(myCode.name,"NX_INT32");
  strcpy(myCode.format,"%12d");
  myCode.nx_type = NX_INT32;
  typecode[6] = myCode;

  strcpy(myCode.name,"NX_UNIT32");
  strcpy(myCode.format,"%12d");
  myCode.nx_type = NX_UINT32;
  typecode[7] = myCode;

  strcpy(myCode.name,"NX_CHAR");
  strcpy(myCode.format,"%c");
  myCode.nx_type = NX_CHAR;
  typecode[8] = myCode;
}
/*----------------------------------------------------------------------*/
void setNumberFormat(int nx_type, char *format){
  int i;

  for(i = 0; i < 9; i++){
    if(typecode[i].nx_type == nx_type){
      strncpy(typecode[i].format,format,29);
    }
  }
}
/*------------------------------------------------------------------*/
static void getNumberFormat(int nx_type, char format[30]){
  int i;

  for(i = 0; i < 9; i++){
    if(typecode[i].nx_type == nx_type){
      strncpy(format,typecode[i].format,29);
    }
  }
}
/*----------------------------------------------------------------*/
void getNumberText(int nx_type, char *typestring, int typeLen){
  int i;

  for(i = 0; i < 9; i++){
    if(typecode[i].nx_type == nx_type){
      strncpy(typestring,typecode[i].name,typeLen);
    }
  }
}
/*
 * 'mxml_add_char()' - Add a character to a buffer, expanding as needed.
 * copied here from mxml-file.c to achieve compatibility with mxml-2.1
 * standard
 */

static int				/* O  - 0 on success, -1 on error */
myxml_add_char(int  ch,			/* I  - Character to add */
              char **bufptr,		/* IO - Current position in buffer */
	      char **buffer,		/* IO - Current buffer */
	      int  *bufsize)		/* IO - Current buffer size */
{
  char	*newbuffer;			/* New buffer value */


  if (*bufptr >= (*buffer + *bufsize - 4))
  {
   /*
    * Increase the size of the buffer...
    */

    if (*bufsize < 1024)
      (*bufsize) *= 2;
    else
      (*bufsize) += 1024;

    newbuffer = (char *)malloc(*bufsize*sizeof(char));
    if(!newbuffer){
      free(*buffer);

      mxml_error("Unable to expand string buffer to %d bytes!", *bufsize);

      return (-1);
    }
    memset(newbuffer,0,*bufsize*sizeof(char));
    memcpy(newbuffer,*buffer,*bufptr - *buffer);
    free(*buffer);

    *bufptr = newbuffer + (*bufptr - *buffer);
    *buffer = newbuffer;
  }

  if (ch < 128)
  {
   /*
    * Single byte ASCII...
    */

    *(*bufptr)++ = ch;
  }
  else if (ch < 2048)
  {
   /*
    * Two-byte UTF-8...
    */

    *(*bufptr)++ = 0xc0 | (ch >> 6);
    *(*bufptr)++ = 0x80 | (ch & 0x3f);
  }
  else if (ch < 65536)
  {
   /*
    * Three-byte UTF-8...
    */

    *(*bufptr)++ = 0xe0 | (ch >> 12);
    *(*bufptr)++ = 0x80 | ((ch >> 6) & 0x3f);
    *(*bufptr)++ = 0x80 | (ch & 0x3f);
  }
  else
  {
   /*
    * Four-byte UTF-8...
    */

    *(*bufptr)++ = 0xf0 | (ch >> 18);
    *(*bufptr)++ = 0x80 | ((ch >> 12) & 0x3f);
    *(*bufptr)++ = 0x80 | ((ch >> 6) & 0x3f);
    *(*bufptr)++ = 0x80 | (ch & 0x3f);
  }

  return (0);
}
/*------------------------------------------------------------------*/
extern char *stptok(char *s, char *tok, size_t toklen, char *brk);
/*=====================================================================
 actual stuff for implementing the callback functions
 =====================================================================*/
static void analyzeDim(const char *typeString, int *rank, 
			    int *iDim, int *type){
  char dimString[132];
  char dim[20];
  char *dimStart, *dimEnd;
  int myRank;

  if(strchr(typeString,(int)'[') == NULL){
    switch(*type){
    case NX_INT8:
    case NX_UINT8:
    case NX_INT16:
    case NX_UINT16:
    case NX_INT32:
    case NX_UINT32:
    case NX_FLOAT32:
    case NX_FLOAT64:
      iDim[0] = 1;
      break;
    case NX_CHAR:
      iDim[0] = -1;
      break;
    }
  } else {
    /*
      we have to determine rank and the dims.
      Start by extracting the dimension string.
    */
    dimStart = strchr(typeString,(int)'[') + 1;
    dimEnd =  strchr(typeString,(int)']');
    if(!dimStart || !dimEnd) {
      mxml_error("ERROR: malformed dimension string in %s",typeString);
      return;
    }
    if((dimEnd - dimStart) > 131){
      mxml_error("ERROR: run away dimension definition in %s",typeString);
      return;
    }
    memset(dimString,0,132);
    memcpy(dimString,dimStart,(dimEnd-dimStart)*sizeof(char));
    dimStart = stptok(dimString,dim,19,",");
    myRank = 0;
    while(dimStart != NULL){
      iDim[myRank] = atoi(dim);
      dimStart = stptok(dimStart,dim,19,",");
      myRank++;
    }
    *rank = myRank;
  }
}
/*--------------------------------------------------------------------*/
int translateTypeCode(char *code){
  int i, result = -1;
  
  for(i = 0; i < 9; i++){
    if(strstr(code,typecode[i].name) != NULL){
      result = typecode[i].nx_type;
      break;
    }
  }
  return result;
}
/*---------------------------------------------------------------------*/
static void analyzeDataType(mxml_node_t *parent, int *rank, int *type,
			    int *iDim){
  const char *typeString;
  mxml_type_t myType;
  int i, nx_type = -1;

  *rank = 1;
  *type = NX_CHAR;
  iDim[0] = -1;

  /*
    get the type attribute. No attribute means: plain text
  */ 
  typeString = mxmlElementGetAttr(parent,"type");
  if(typeString == NULL){
    return;
  }

  nx_type = translateTypeCode((char *)typeString);

  /*
    assign type
  */
  if(nx_type == -1){
    mxml_error(
     "ERROR: %s is an invalid NeXus type, I try to continue but may fail",
     typeString);
    *type =NX_CHAR;
    return;
  }

  *type = nx_type;
  
  analyzeDim(typeString, rank, iDim, type);
}
/*-------------------------------------------------------------------*/
void destroyDataset(void *data){
  if(data != NULL){
    dropNXDataset((pNXDS)data);
  }
}
/*-------------------------------------------------------------------*/
static char *getNextNumber(char *pStart, char pNumber[80]){
  int charCount = 0;
  pNumber[0] = '\0';

  /* advance to first digit */
  while(isspace(*pStart) && *pStart != '\0'){
    pStart++;
  }
  if(*pStart == '\0'){
    return NULL;
  }

  /* copy */
  while(!isspace(*pStart) && *pStart != '\0' && charCount < 78){
    pNumber[charCount] = *pStart;
    pStart++;
    charCount++;
  }
  pNumber[charCount] = '\0';
  return pStart;
}
/*--------------------------------------------------------------------*/
mxml_type_t nexusTypeCallback(mxml_node_t *parent){
  const char *typeString;

  if(strstr(parent->value.element.name,"?xml") != NULL ||
     strstr(parent->value.element.name,"NX") != NULL){
    return MXML_ELEMENT;
  } else {
    typeString = mxmlElementGetAttr(parent,"type");
    if(typeString == NULL){
      /*
	MXML_TEXT seems more appropriate here. But mxml hacks text into
	single words which is not what NeXus want.
      */
      return MXML_OPAQUE;
    } else{
      return MXML_CUSTOM;
    }
  }
}
/*----------------------------------------------------------------------*/
int nexusLoadCallback(mxml_node_t *node, const char *buffer){
  mxml_node_t *parent = NULL;
  int rank, type, iDim[NX_MAXRANK];
  char pNumber[80], *pStart;
  long address, maxAddress;
  pNXDS dataset = NULL;
  int i;

  parent = node->parent;
  analyzeDataType(parent,&rank,&type,iDim);
  if(iDim[0] == -1){
    iDim[0] = strlen(buffer);
    node->value.custom.data = strdup(buffer);
    node->value.custom.destroy = free;
    return 0;
  } else {
    node->value.custom.data = createNXDataset(rank,type,iDim);
    dataset = (pNXDS)node->value.custom.data;
    if(dataset == NULL){
      mxml_error("Failed to allocate custom dataset");
      return 1;
    }
    node->value.custom.destroy = destroyDataset; 
  }

  /*
    load data
  */
  pStart = (char *)buffer;
  maxAddress = getNXDatasetLength(dataset);
  address = 0;
  while( (pStart = getNextNumber(pStart,pNumber)) != NULL && 
	 address < maxAddress){
    putNXDatasetValueAt(dataset,address,atof(pNumber));
    address++;
  }

  return 0;
}
/*---------------------------------------------------------------------*/
static void stringIntoBuffer(char **buffer, char **bufPtr, int *bufSize, 
		      char *string){
  int i;

  for(i = 0; i < strlen(string); i++){
    myxml_add_char(string[i],bufPtr,buffer,bufSize);
  }
}
/*--------------------------------------------------------------------*/
static void formatNumber(double value, char *txt, int txtLen,
			 char *format, int type){
  switch(type){
  case NX_INT8:
  case NX_UINT8:
  case NX_INT16:
  case NX_UINT16:
  case NX_INT32:
  case NX_UINT32:
    snprintf(txt,txtLen,format,(int)value);
    break;
  case NX_FLOAT32:
  case NX_FLOAT64:
    snprintf(txt,txtLen,format,value);
    break;
  default:
    assert(0); /* something is very wrong here */
    break;
  }
}
/*--------------------------------------------------------------------*/
static int countDepth(mxml_node_t *node){
  int count = 0;
  mxml_node_t *cur;

  cur = node;
  while(cur != NULL){
    count++;
    cur = cur->parent;
  }
  count--;
  return count;
}
/*---------------------------------------------------------------------*/
char *nexusWriteCallback(mxml_node_t *node){
  int type, col;
  char pNumber[80], indent[80], format[30];
  char *buffer, *bufPtr;
  pNXDS dataset;
  int bufsize, i, length, currentLen; 

  /*
    allocate output buffer
  */
  buffer = (char *)malloc(1024*sizeof(char));
  if(buffer == NULL){
    mxml_error("Unable to allocate buffer");
    return NULL;
  }
  memset(buffer,0,1024);
  bufPtr = buffer;
  bufsize = 1024;

  dataset = (pNXDS)node->value.custom.data;

  /*
    prepare indentation level
  */
  col = countDepth(node)*2;
  memset(indent,0,80);
  for(i = 0; i < col; i++){
    indent[i] = ' ';
  }

  /*
    get dataset info
  */
  type = getNXDatasetType(dataset);
  length = getNXDatasetLength(dataset);
  if(dataset->format != NULL){
    strcpy(format,dataset->format);
  } else {
    getNumberFormat(type,format);
  }

  /*
    actually get the data out
  */
  currentLen = col;
  myxml_add_char('\n',&bufPtr,&buffer,&bufsize);
  stringIntoBuffer(&buffer,&bufPtr,&bufsize,indent);
  for(i = 0; i < length; i++){
    formatNumber(getNXDatasetValueAt(dataset,i),pNumber,79,format,type);
    if(currentLen + strlen(pNumber) > MXML_WRAP){
      /*
	wrap line
      */
      myxml_add_char('\n',&bufPtr,&buffer,&bufsize);
      stringIntoBuffer(&buffer,&bufPtr,&bufsize,indent);
      currentLen = col;
    }
    stringIntoBuffer(&buffer,&bufPtr,&bufsize,pNumber);
    myxml_add_char(' ',&bufPtr,&buffer,&bufsize);
    currentLen += strlen(pNumber) + 1;
  }
  myxml_add_char('\0',&bufPtr,&buffer,&bufsize);
  return (char *)buffer;
}
/*---------------------------------------------------------------------*/
const char *NXwhitespaceCallback(mxml_node_t *node, int where){
  char *indent;
  int len;  

  if(strcmp(node->value.element.name,"?xml") == 0){
    return NULL;
  }

  if(where == MXML_WS_BEFORE_OPEN || where == MXML_WS_BEFORE_CLOSE){
    len = countDepth(node)*2 + 2;
    indent = (char *)malloc(len*sizeof(char));
    if(indent != NULL){
      memset(indent,' ',len);
      indent[0]= '\n';
      indent[len-1] = '\0';
      return  (const char*)indent;
    }
  }
  return NULL;
}
/*-----------------------------------------------------------------------*/
#ifdef TESTMAIN
#include <stdio.h>

int main(int argc, char *argv[]){
   mxml_node_t *root = NULL;
   FILE *f;

   mxmlSetCustomHandlers(nexusLoadCallback, nexusWriteCallback);
   initializeNumberFormats();

   /*
     read test
   */
   f = fopen("dmc.xml","r");
   root = mxmlLoadFile(NULL,f,nexusTypeCallback);
   fclose(f);

   /*
     write test
   */
   setNumberFormat(NX_INT32,"%8d");
   setNumberFormat(NX_FLOAT32,"%8.2f");
   f = fopen("dmc2.xml","w");
   mxmlSaveFile(root,f,NXwhitespaceCallback);
   fclose(f);

}
#endif
