/*-------------------------------------------------------------------------
                            D Y N S T R I N G
                            
  Implementation file for a dynamic string object.
  Usage and copyright: see dynstring.h
  
  Mark Koennecke, March 1998
-----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "dynstring.h"

#define DYNMAGIC 27031998 

/*--------------------------------------------------------------------------*/
  typedef struct __DynString {
                               int iMAGIC;                            
                               char *pBuffer;
                               int iTextLen;
                               int iBufferSize;
                               int iResize;
                              } DynString;
/*--------------------------------------------------------------------------*/
 pDynString CreateDynString(int iInitial, int iResize)
 {
   pDynString pNew = NULL;
   
   if(iInitial <= 0)
           iInitial = 512;
   if(iResize <= 0)
           iResize = 512;
           
   /* new memory */
   pNew = (pDynString)malloc(sizeof(DynString));
   if(!pNew)
   {
     return NULL;
   }      
   memset(pNew,0,sizeof(DynString));
   pNew->pBuffer = (char *)malloc(iInitial*sizeof(char));
   if(!pNew->pBuffer)
   {
     free(pNew);
     return NULL;
   }          
   memset(pNew->pBuffer,0,iInitial*sizeof(char));
   
   /* initialise the rest */
   pNew->iMAGIC = DYNMAGIC;
   pNew->iBufferSize = iInitial;
   pNew->iResize =   iResize;
   pNew->iTextLen = 0;
   
   return pNew;
 }         
/*--------------------------------------------------------------------------*/
 static int Resize(pDynString self, int iRequested)
 {
     char *pNewBuffer;
     int iNewSize;
     
     if(iRequested < self->iBufferSize)
     {
       return 1;
     } 
     
     /* get new data space */
     iNewSize = iRequested + self->iResize;
     pNewBuffer = (char *)malloc(iNewSize *sizeof(char));
     if(!pNewBuffer)
     {
       return 0;
     }
     memset(pNewBuffer,0,iNewSize);
     
     /* copy data */
     memcpy(pNewBuffer,self->pBuffer,self->iTextLen);
     
     /* swap things around */
     if(self->pBuffer)
     {
        free(self->pBuffer);
     }
     self->pBuffer = pNewBuffer;
     self->iBufferSize = iNewSize;
     return 1;
 }  
/*--------------------------------------------------------------------------*/
 void DeleteDynString(pDynString self)
 {
    assert(self);
    assert(self->iMAGIC == DYNMAGIC);
    
    if(self->pBuffer)
        free(self->pBuffer);
        
    free(self);    
 }  
/*-------------------------------------------------------------------------*/
 int DynStringCopy(pDynString self, char *pText)
 {
    int iRequested, iRet;

    assert(self);
    assert(self->iMAGIC == DYNMAGIC);
    
    iRequested = strlen(pText);
    if(iRequested >= self->iBufferSize) 
    {
       iRet = Resize(self,iRequested);
       if(!iRet)
       {
         return 0;
       }
    }
    else /* safety: remove rubbish */
    {
       memset(self->pBuffer,0,self->iBufferSize*sizeof(char));
    }
    strcpy(self->pBuffer,pText);
    self->iTextLen = iRequested;
    return 1;
 }           
/*--------------------------------------------------------------------------*/
 int DynStringConcat(pDynString self, char *pText)
 {
    int iRequested, iRet;

    assert(self);
    assert(self->iMAGIC == DYNMAGIC);
    
    iRequested = strlen(pText) + self->iTextLen;
    if(iRequested >= self->iBufferSize) 
    {
       iRet = Resize(self,iRequested);
       if(!iRet)
       {
         return 0;
       }
    }

    strcat(self->pBuffer,pText);
    self->iTextLen = iRequested;
    return 1;
 }      
/*--------------------------------------------------------------------------*/
 int DynStringConcatChar(pDynString self, char c)
 {
    int iRequested, iRet;

    assert(self);
    assert(self->iMAGIC == DYNMAGIC);
    
    iRequested = self->iTextLen + 1;
    if(iRequested >= self->iBufferSize) 
    {
       iRet = Resize(self,iRequested);
       if(!iRet)
       {
         return 0;
       }
    }

    self->pBuffer[self->iTextLen] = c;
    self->iTextLen++;
    return 1;
 }      
/*---------------------------------------------------------------------------*/
 int DynStringInsert(pDynString self, char *pText, int iPos)
 {
    int iRequested, iRet, iPush, iRest;
    char *pPtr;

    assert(self);
    assert(self->iMAGIC == DYNMAGIC);
    
    /* provide space */
    iPush = strlen(pText);
    iRequested = self->iTextLen + iPush;
    if(iRequested >= self->iBufferSize) 
    {
       iRet = Resize(self,iRequested);
       if(!iRet)
       {
         return 0;
       }
    }
    
    /* we need a temporary buffer to hold the backend of the string */
    iRest = self->iTextLen - iPos;
    pPtr = (char *)malloc((iRest+10)*sizeof(char));
    if(!pPtr)
    {
      return 0;
    }
    memset(pPtr,0,(iRest+10)*sizeof(char));
    strcpy(pPtr,&self->pBuffer[iPos]);
    
    /* OK build the result string */
    memset(&self->pBuffer[iPos],0,iRest*sizeof(char));
    strcat(self->pBuffer,pText);
    strcat(self->pBuffer,pPtr);
    free(pPtr);
    self->iTextLen = iRequested;
    return 1;
 }    
/*--------------------------------------------------------------------------*/
 int DynStringReplace(pDynString self, char *pText, int iPos)
 {
    int iRequested, iRet;

    assert(self);
    assert(self->iMAGIC == DYNMAGIC);
    
    iRequested = strlen(pText) + iPos;
    if(iRequested >= self->iBufferSize) 
    {
       iRet = Resize(self,iRequested);
       if(!iRet)
       {
         return 0;
       }
    }

    memcpy(&self->pBuffer[iPos],pText,strlen(pText)*sizeof(char));
    self->iTextLen = strlen(self->pBuffer);
    return 1;
 }
/*---------------------------------------------------------------------------*/
 char *GetCharArray(pDynString self)
 {
    assert(self);
    assert(self->iMAGIC == DYNMAGIC);

    return self->pBuffer;     
 } 
/*---------------------------------------------------------------------------*/
 int GetDynStringLength(pDynString self)
 {
    assert(self);
    assert(self->iMAGIC == DYNMAGIC);

    return self->iTextLen;     
 } 
 