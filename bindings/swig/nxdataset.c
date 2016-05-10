/*
  This is a module which implements the notion of a dataset. Its is
  designed for the use with scripting languages.
  
  copyright: GPL

  Mark Koennecke, October 2002

  Added cut and sum array operations.

  Mark Koennecke, February 2006

*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nxdataset.h"

/*-----------------------------------------------------------------------*/
static int getTypeSize(int typecode){
  switch(typecode){
  case NX_FLOAT32:
  case NX_INT32:
  case NX_UINT32:
    return 4;
    break;
  case NX_FLOAT64:
    return 8;
    break;
  case NX_INT16:
  case NX_UINT16:
    return 2;
    break;
  default:
    return 1;
    break;
  }
}
/*-----------------------------------------------------------------------*/
pNXDS createNXDataset(int rank, int typecode, int dim[]){
  pNXDS pNew = NULL;
  int i, length;

  pNew = (pNXDS)malloc(sizeof(NXDS));
  if(pNew == NULL){
    return NULL;
  }

  pNew->dim = (int *)malloc(rank*sizeof(int));
  for(i = 0, length = 1; i < rank; i++){
    length *= dim[i];
  }
  pNew->u.ptr = malloc(length*getTypeSize(typecode));

  if(pNew->dim == NULL || pNew->u.ptr == NULL){
    free(pNew);
    return NULL;
  }
  pNew->rank = rank;
  pNew->type = typecode;
  pNew->format = NULL;
  for(i = 0; i < rank; i++){
    pNew->dim[i] = dim[i];
  }
  pNew->magic = MAGIC;
  memset(pNew->u.ptr,0,length*getTypeSize(typecode));
  return pNew;
}
/*---------------------------------------------------------------------*/
pNXDS createTextNXDataset(char *name){
  pNXDS pNew = NULL;

  pNew = (pNXDS)malloc(sizeof(NXDS));
  if(pNew == NULL){
    return NULL;
  }
  pNew->dim = (int *)malloc(sizeof(int));
  pNew->u.cPtr = strdup(name);
  if(pNew->dim == NULL || pNew->u.ptr == NULL){
    free(pNew);
    return NULL;
  }
  pNew->rank = 1;
  pNew->type = NX_CHAR;
  pNew->magic = MAGIC;
  pNew->dim[0] = strlen(name);
  return pNew;
}
/*-----------------------------------------------------------------------*/
void  dropNXDataset(pNXDS dataset){
  if(dataset == NULL){
    return;
  }
  if(dataset->magic != MAGIC){
    return;
  }
  if(dataset->dim != NULL){
    free(dataset->dim);
  }
  if(dataset->u.ptr != NULL){
    free(dataset->u.ptr);
  }
  if(dataset->format != NULL){
    free(dataset->format);
  }
  free(dataset);
}
/*-----------------------------------------------------------------------*/
int   getNXDatasetRank(pNXDS dataset){
  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }
  return dataset->rank;
}
/*-----------------------------------------------------------------------*/
int   getNXDatasetDim(pNXDS dataset, int which){
  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }
  if(which < 0 || which >= dataset->rank){
    return 0;
  }
  return dataset->dim[which];
}
/*------------------------------------------------------------------------*/
int   getNXDatasetType(pNXDS dataset){
  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }
  return dataset->type;
}
/*--------------------------------------------------------------------*/
int getNXDatasetLength(pNXDS dataset){
  int length, i;

  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }
  length = dataset->dim[0];
  for(i = 1; i < dataset->rank; i++){
    length *= dataset->dim[i];
  }
  return length;
}
/*---------------------------------------------------------------------*/
int getNXDatasetByteLength(pNXDS dataset){
  return getNXDatasetLength(dataset)*getTypeSize(dataset->type);
}
/*----------------------------------------------------------------------
  This calculates an arbitray address in C storage order
  -----------------------------------------------------------------------*/
static int calculateAddress(pNXDS dataset, int pos[]){
  int result, mult;
  int i, j;

  result = pos[dataset->rank - 1];
  for(i = 0; i < dataset->rank -1; i++){
    mult = 1;
    for(j = dataset->rank -1; j > i; j--){
      mult *= dataset->dim[j];
    }
    if(pos[i] < dataset->dim[i] && pos[i] > 0){
      result += mult*pos[i];
    }
  }
  return result;
}
/*-----------------------------------------------------------------------*/
double getNXDatasetValue(pNXDS dataset, int pos[]){
  int address;

  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }

  address = calculateAddress(dataset,pos);
  return getNXDatasetValueAt(dataset, address);
}
/*----------------------------------------------------------------------*/
double getNXDatasetValueAt(pNXDS dataset, int address){
  double value;

  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }

  switch(dataset->type){
  case NX_FLOAT64:
    value = dataset->u.dPtr[address];
    break;
  case NX_FLOAT32:
    value = (double)dataset->u.fPtr[address];
    break;
  case NX_INT32:
  case NX_UINT32:
    value = (double)dataset->u.iPtr[address];
    break;
  case NX_INT16:
  case NX_UINT16:
    value = (double)dataset->u.sPtr[address];
    break;
  default:
    value = (double)dataset->u.cPtr[address];
    break;
  }
  return value;
}
/*-----------------------------------------------------------------------*/
char  *getNXDatasetText(pNXDS dataset){
  char *resultBuffer = NULL;
  int status = 1;

  if(dataset == NULL){
    status = 0;
  }
  if(dataset->magic != MAGIC){
    status = 0;
  }
  if(dataset->rank > 1){
    status = 0;
  } 
  if(dataset->type == NX_FLOAT32 ||
     dataset->type == NX_FLOAT64 ||
     dataset->type == NX_INT32 ||
     dataset->type == NX_UINT32 ||
     dataset->type == NX_INT16 ||
     dataset->type == NX_UINT16 ) {
    status = 0;
  }

  if(status == 0){
    return strdup("NO type problem");
  }else{
    resultBuffer = (char *)malloc((dataset->dim[0]+10)*sizeof(char));
    if(resultBuffer == NULL){
      return strdup("NO Memory");
    }
    memset(resultBuffer,0,(dataset->dim[0]+10)*sizeof(char));
    strncpy(resultBuffer,dataset->u.cPtr,dataset->dim[0]);
  }
  return resultBuffer;
}
/*----------------------------------------------------------------------*/
int   putNXDatasetValue(pNXDS dataset, int pos[], double value){
  int address;

  if(dataset == NULL){
    return 0;
  }
  if(dataset->magic != MAGIC){
    return 0;
  }

  address = calculateAddress(dataset,pos);
  return putNXDatasetValueAt(dataset,address,value);
}
  /*---------------------------------------------------------------------*/
int putNXDatasetValueAt(pNXDS dataset, int address, double value){
  /*
    this code is dangerous, it casts without checking the data range.
    This may cause trouble in some cases
  */
  switch(dataset->type){
  case NX_FLOAT64:
    dataset->u.dPtr[address] = value;
    break;
  case NX_FLOAT32:
    dataset->u.fPtr[address] = (float)value;
    break;
  case NX_INT32:
  case NX_UINT32:
    dataset->u.iPtr[address] = (int)value;
    break;
  case NX_INT16:
  case NX_UINT16:
    dataset->u.sPtr[address] = (short int)value;
    break;
  default:
    dataset->u.cPtr[address] = (char)value;
    break;
  }
  return 1;
}
/*----------------------------------------------------------------------
  This is working recursively through the dimensions. When at the last:
  actual copying takes place.
 -----------------------------------------------------------------------*/
static void copyCutData(pNXDS source, pNXDS target, int sourceDim[], 
			int targetDim[], int start[], int end[], 
			int dim){
  int i, length;
  double val;

  targetDim[dim] = 0;
  length = end[dim] - start[dim];
  if(dim == source->rank -1){
    for(i = 0; i < length; i++){
      sourceDim[dim] = start[dim] + i;
      val = getNXDatasetValue(source,sourceDim);
      targetDim[dim] = i;
      putNXDatasetValue(target, targetDim, val);
    }
  } else {
    for(i = 0; i < length; i++){
      sourceDim[dim] = start[dim] + i;
      targetDim[dim] = i;
      copyCutData(source,target, sourceDim, targetDim, start, end, dim+1);
    }
  }
}
/*-----------------------------------------------------------------------*/
pNXDS cutNXDataset(pNXDS source, int start[], int end[]){
  pNXDS result = NULL;
  int newDim[NX_MAXRANK], i;
  int sourceDim[NX_MAXRANK], targetDim[NX_MAXRANK];

  for(i = 0; i < source->rank; i++){
    if(start[i] < 0 || end[i] > source->dim[i]){
      fprintf(stderr,"ERROR: invalid boundaries specified for cutting");
      return NULL;
    }
    newDim[i] = end[i] - start[i];
    if(newDim[i] <= 0){
      fprintf(stderr,"ERROR: invalid cut limits specified for cutting dataset");
      return NULL;
    }
  }
  result = createNXDataset(source->rank, source->type, newDim);
  if(result == NULL){
    fprintf(stderr,"ERROR: out of memory creating result dataset");
    return NULL;
  }
  
  copyCutData(source, result, sourceDim, targetDim, start, end, 0);

  return result;
}
/*----------------------------------------------------------------------
 This recurses through all dimesnions, thereby skipping the summed one.
 At the end of the rescusion the actual summing is performed.
 ----------------------------------------------------------------------*/
static void sumData(pNXDS source, pNXDS target, int sourceDim[], 
		    int targetDim[], int targetDimCount, int dimNo, 
		    int start, int end, int currentDim){
  int i, length;
  double val, sumVal;

  /*
    when we have recursed through  all dimensions
    we actually do the sums...
  */
  if(currentDim == source->rank){
    length = end - start;
    sumVal = getNXDatasetValue(target, targetDim);
    for(i = 0; i < length; i++){
      sourceDim[dimNo] = start + i;
      val = getNXDatasetValue(source,sourceDim);
      sumVal += val;
    }
    putNXDatasetValue(target, targetDim, sumVal);
  } else {
    /*
      jump over the summed dimension while recursing
      through the dimensions
    */
    if(currentDim == dimNo){
      sumData(source,target,sourceDim,
	      targetDim,targetDimCount, 
	      dimNo,start,end,currentDim+1);
    } else {
      /*
	loop over all values of the non summed dimension
      */
      for(i = 0; i < source->dim[currentDim]; i++){
	/*
	  the problem here is that we have to jump over the summed
	  dimension here. This why we have to maintain a separate 
	  dimension count for the target array. Jumping is done
	  above.
	*/
	targetDim[targetDimCount] = i;
	targetDimCount++;

	sourceDim[currentDim] = i;
	sumData(source,target,sourceDim,targetDim,targetDimCount,
		dimNo,start,end,currentDim+1);
	targetDimCount--;
      }
    }
  }
}
/*-----------------------------------------------------------------------*/
pNXDS sumNXDataset(pNXDS source, int dimNo, int start, int end){
  int newDim[NX_MAXRANK], targetDim[NX_MAXRANK], sourceDim[NX_MAXRANK];
  pNXDS result = NULL;
  int i, count;

  if(dimNo < 0 || dimNo > source->rank-1) {
    fprintf(stderr,"ERROR: invalid dimension for summing requested");
    return NULL;
  }

  /*
    make result dataset with missing summed dimension
  */
  for(i = 0, count = 0; i < source->rank; i++){
    if(i != dimNo){
      newDim[count] = source->dim[i];
      count++;
    }
  }
  result = createNXDataset(source->rank-1, source->type, newDim);
  if(result == NULL){
    fprintf(stderr,"ERROR: out of memory creating result dataset");
    return NULL;
  }
  sumData(source,result,sourceDim,targetDim,0,
	  dimNo, start, end, 0);
  return result;
}

