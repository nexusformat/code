/*-------------------------------------------------------------------------

			S T R I N G D I C T
			
	Implementation file for a simple list based string dictionary
	of name value pairs.

       Mark Koennecke, April 1997
	
       Copyright:

       Labor fuer Neutronenstreuung
       Paul Scherrer Institut
       CH-5423 Villigen-PSI


      The authors hereby grant permission to use, copy, modify, distribute,
      and license this software and its documentation for any purpose, provided
      that existing copyright notices are retained in all copies and that this
      notice is included verbatim in any distributions. No written agreement,
      license, or royalty fee is required for any of the authorized uses.
      Modifications to this software may be copyrighted by their authors
      and need not follow the licensing terms described here, provided that
      the new terms are clearly indicated on the first page of each file where
      they apply.

      IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
      FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
      ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
      DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
      POSSIBILITY OF SUCH DAMAGE.

      THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
      INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
      FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
      IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
      NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
      MODIFICATIONS.
----------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
/* #include "fortify.h" */
#include "lld.h"
#include "stringdict.h"

/*-------------------------------------------------------------------------*/
  typedef struct __StringDict {
                                 int iList;
                                 int iTraverse;
                              } StringDict;

  typedef struct {
                   char *name;
                   char *value;
                  } SDE, *pSDE;
/*-------------------------------------------------------------------------*/                  
  pStringDict CreateStringDict(void)
  {
     pStringDict pNew = NULL;
     
     pNew = (pStringDict)malloc(sizeof(StringDict));
     if(!pNew)
     {
       return NULL;
     }
     pNew->iList = LLDcreate(sizeof(SDE));
     if(pNew->iList < 0)
     {
       free(pNew);
       return NULL;
     }
     pNew->iTraverse = 0;
     return pNew;
  }
/*------------------------------------------------------------------------*/  
  void        DeleteStringDict(pStringDict self)
  {
     int iRet;
     SDE sVal;
     
     assert(self);
     iRet = LLDnodePtr2First(self->iList);
     while(iRet != 0)
     {
        LLDnodeDataTo(self->iList,&sVal);
        if(sVal.name)
        {
          free(sVal.name);
        }
        if(sVal.value)
        {
          free(sVal.value);
        }
        iRet = LLDnodePtr2Next(self->iList);
     }
     LLDdelete(self->iList);
     free(self);
  }
/*-------------------------------------------------------------------------*/
  int StringDictAddPair(pStringDict self, char *name, char *value)
  {
     SDE sVal;
     
     assert(self);
     sVal.name = NULL;
     sVal.value = NULL;
     
     sVal.name = strdup(name);
     sVal.value = strdup(value);
     
     LLDnodeAppendFrom(self->iList,&sVal);
     return 1;
  }
/*---------------------------------------------------------------------------*/  
  int StringDictExists(pStringDict self, char *name)
  {
     SDE sVal;
     int iRet;
     
     iRet = LLDnodePtr2First(self->iList);
     while(iRet != 0)
     {
        LLDnodeDataTo(self->iList,&sVal);
        if(strcmp(sVal.name,name) == 0)
        {
          return 1;
        }
        iRet = LLDnodePtr2Next(self->iList);
     }
     return 0;
  }  
/*--------------------------------------------------------------------------*/  
  int StringDictUpdate(pStringDict self, char *name, char *value)
  {
     SDE sVal;
     int iRet;
     
     iRet = LLDnodePtr2First(self->iList);
     while(iRet != 0)
     {
        LLDnodeDataTo(self->iList,&sVal);
        if(strcmp(sVal.name,name) == 0)
        {
          if(sVal.value)
          {
            free(sVal.value);
          }
          sVal.value = strdup(value);
          LLDnodeDataFrom(self->iList,&sVal);
          return 1;
        }
        iRet = LLDnodePtr2Next(self->iList);
     }
     return 0;
  }  
/*--------------------------------------------------------------------------*/  
  int StringDictGet(pStringDict self, char *name, char *pResult, int iLen)
  {
     SDE sVal;
     int iRet;
     
     iRet = LLDnodePtr2First(self->iList);
     while(iRet != 0)
     {
        LLDnodeDataTo(self->iList,&sVal);
        if(strcmp(sVal.name,name) == 0)
        {
          if(pResult == NULL)
          {
            return strlen(sVal.value);
          }
          else
          {
             strncpy(pResult,sVal.value,iLen);
             return 1;
          }   
        }
        iRet = LLDnodePtr2Next(self->iList);
     }
     return 0;
  }  
/*------------------------------------------------------------------------*/  
  int StringDictDelete(pStringDict self, char *name)
  {
     SDE sVal;
     int iRet;
     
     iRet = LLDnodePtr2First(self->iList);
     while(iRet != 0)
     {
        LLDnodeDataTo(self->iList,&sVal);
        if(strcmp(sVal.name,name) == 0)
        {
          if(sVal.name)
          {
            free(sVal.name);
          }
          if(sVal.value)
          {
            free(sVal.value);
          }
          LLDnodeDelete(self->iList);
          return 1;
        }
        iRet = LLDnodePtr2Next(self->iList);
     }
     return 0;
  }  
/*------------------------------------------------------------------------*/
 int StringDictGetAsNumber(pStringDict self, char *name, float *fVal)
 {
   char pBueffel[80];
   int iRet;
   SDE sVal;
   
   assert(self);
   
   iRet = StringDictGet(self,name,pBueffel,79);
   if(!iRet)
   {
     return iRet;
   }
   
   iRet = sscanf(pBueffel,"%f",fVal);
   if(iRet != 1)
   {
     return 0;
   }
   return 1;
 }
/*-------------------------------------------------------------------------*/
  const char *StringDictGetNext(pStringDict self, char *pValue, int iValLen)
  {
     int iRet;
     SDE sVal;
     
     assert(self);
     
     if(self->iTraverse)
     {
       iRet = LLDnodePtr2Next(self->iList);
       if(iRet == 0) /* exhausted */
       {
         self->iTraverse = 0;
         return NULL;
       }
       else
       {
        LLDnodeDataTo(self->iList,&sVal);
        strncpy(pValue,sVal.value,iValLen);
        return sVal.name;          
       }
     }
     else /* start a new one */
     {
       iRet = LLDnodePtr2First(self->iList);
       if(iRet == 0)
       {
         return NULL;
       }
       else
       {
         self->iTraverse = 1;
         LLDnodeDataTo(self->iList,&sVal);
         strncpy(pValue,sVal.value,iValLen);
         return sVal.name;          
       }
     }
     return NULL;
  }
