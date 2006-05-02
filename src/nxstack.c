/*
  This is some code to handle a stack of NeXus files. This is used to implement 
  external linking within the NeXus-API

  Copyright (C) 1997-2006 Mark Koennecke
  
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
*/
#include <stdlib.h>
#include <napi.h>
#include "nxstack.h"

/*-----------------------------------------------------------------------
 Data definitions
---------------------------------------------------------------------*/

typedef struct {
  pNexusFunction pDriver;
  NXlink closeID;
  char filename[1024];
}fileStackEntry;


typedef struct __fileStack {
  int fileStackPointer;
  fileStackEntry fileStack[MAXEXTERNALDEPTH];
}fileStack;
/*---------------------------------------------------------------------*/
pFileStack makeFileStack(){
  pFileStack pNew = NULL;
  
  pNew = malloc(sizeof(fileStack));
  if(pNew == NULL){
    return NULL;
  }
  memset(pNew,0,sizeof(fileStack));
  pNew->fileStackPointer = -1;
  return pNew;
}
/*---------------------------------------------------------------------*/
void killFileStack(pFileStack self){
  if(self != NULL){
    free(self);
  }
}
/*----------------------------------------------------------------------*/
void pushFileStack(pFileStack self, pNexusFunction pDriv, char *file){
  int length;

  self->fileStackPointer++;
  self->fileStack[self->fileStackPointer].pDriver = pDriv;
  memset(&self->fileStack[self->fileStackPointer].closeID,0,sizeof(NXlink));
  length = strlen(file);
  if(length >= 1024){
    length = 1023;
  }
  memcpy(&self->fileStack[self->fileStackPointer].filename,file,length);
}
/*----------------------------------------------------------------------*/
void popFileStack(pFileStack self){
  self->fileStackPointer--;
  if(self->fileStackPointer < -1){
    self->fileStackPointer = -1;
  }
}
/*----------------------------------------------------------------------*/
pNexusFunction peekFileOnStack(pFileStack self){
  return self->fileStack[self->fileStackPointer].pDriver;
}
/*---------------------------------------------------------------------*/
char *peekFilenameOnStack(pFileStack self){
  return self->fileStack[self->fileStackPointer].filename;
}
/*----------------------------------------------------------------------*/
void peekIDOnStack(pFileStack self, NXlink *id){
  memcpy(id, &self->fileStack[self->fileStackPointer].closeID, sizeof(NXlink));
}
/*---------------------------------------------------------------------*/
void setCloseID(pFileStack self, NXlink id){
  memcpy(&self->fileStack[self->fileStackPointer].closeID, &id, sizeof(NXlink));
}
/*----------------------------------------------------------------------*/
int fileStackDepth(pFileStack self){
  return self->fileStackPointer;
}
