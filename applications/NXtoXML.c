/*-----------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
   
  Utility to convert a NeXus file into XML
 
  Copyright (C) 2002, Ray Osborn
 
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
 
 $Id$
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "napi.h"

int PrintGroup ();
int PrintData ();
int PrintAttributes ();
void PrintValues (void* data, int dataType, int numElements);
void PrintArray (void* dataBuffer, int dataType, int dataLen, int stepSize, int typeSize);
void PrintDimensions (int rank, int dimensions[]);
void PrintType (int dataType);
void PrintIndent ();

static NXhandle inId;
static FILE *outId=NULL;
static int indent=0;

int main(int argc, char *argv[])
{
   char inFile[256], outFile[256], *stringPtr;

/* use command line arguments for input and output filenames if given */
   if (argc < 2) {
      printf ("Give name of NeXus file : ");
      fgets (inFile, sizeof(inFile), stdin);
      if ((stringPtr = strchr(inFile, '\n')) != NULL) *stringPtr = '\0';
   }
   else {
     strcpy (inFile, argv[1]);
   }
   if (argc < 3) {
      printf ("Give name of XML file : ");
      fgets (outFile, sizeof(outFile), stdin);
      if ((stringPtr = strchr(outFile, '\n')) != NULL) *stringPtr = '\0';
   }
   else {
     strcpy (outFile, argv[2]);
   }

/* Open NeXus input file and XML output file */
   if (NXopen (inFile, NXACC_READ, &inId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", inFile);
      return NX_ERROR;
   }

   outId = fopen (outFile, "w");
   if (!outId) {
     printf ("NX_ERROR: Can't open %s \n", outFile);
     return NX_ERROR;
   }
   
/* Output XML header and global attributes */
   fprintf (outId, "<?xml version=\"1.0\"?>\n");
   fprintf (outId, "<NXroot");
   PrintAttributes ();
   fprintf (outId, ">\n");
/* Recursively cycle through the groups printing the contents */
   indent = 0;
   PrintGroup ();
/* Close the input and output files */
   fclose (outId);
   NXclose (&inId);
   return NX_OK;
}

/* Prints the contents of each group as XML tags and values */
int PrintGroup ()
{ 
   int status, dataType, dataRank, dataDimensions[NX_MAXRANK], dataLen;     
   NXname name, class;

   indent += 3;
   do {
      status = NXgetnextentry (inId, name, class, &dataType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (!strncmp(class,"NX",2)) {
            if (NXopengroup (inId, name, class) != NX_OK) return NX_ERROR;
            PrintIndent ();
            fprintf (outId, "<%s name=\"%s\">\n", class, name);
            PrintGroup ();
         }
         else if (!strncmp(class,"SDS",3)) {
            PrintIndent ();
            fprintf (outId, "<%s", &name);
            if (NXopendata (inId, name) != NX_OK) return NX_ERROR;
            if (NXgetinfo (inId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
            if (dataType != NX_CHAR) {
               fprintf (outId, " type=\"");
               PrintType (dataType);
               PrintDimensions (dataRank, dataDimensions);
               fprintf (outId, "\"");
            }
            PrintAttributes ();
            fprintf (outId, ">");
            if (PrintData () != NX_OK) return NX_ERROR;
            if (NXclosedata (inId) != NX_OK) return NX_ERROR;
            fprintf (outId, "</%s>\n", name);
         }
      }
      else if (status == NX_EOD) {
         if (NXgetgroupinfo (inId, &dataRank, name, class) != NX_OK) return NX_ERROR;
         if (NXclosegroup (inId) != NX_OK) return NX_ERROR;
         indent -= 3;
         PrintIndent ();
         fprintf (outId, "</%s>\n", class);
         return NX_OK;
      }
   } while (status == NX_OK);
   return NX_OK;
}

/* Prints the values of each data item according to its data type */
int PrintData ()
{
   int status, i, stepSize, typeSize, dataType, dataRank, dataDimensions[NX_MAXRANK], dataLen;     
   void *dataBuffer;

   if (NXgetinfo (inId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
   if (NXmalloc (&dataBuffer, dataRank, dataDimensions, dataType) != NX_OK) return NX_ERROR;
   dataLen = 1;
   for (i = 0, dataLen = 1; i < dataRank; i++) {
      dataLen *= dataDimensions[i];
   }
   if (NXgetdata (inId, dataBuffer) != NX_OK) return NX_ERROR;
   switch(dataType) {
      case NX_CHAR:
         stepSize = dataLen; typeSize = 1;
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_INT8:
         stepSize = 20; typeSize = 1;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_UINT8:
         stepSize = 20; typeSize = 1;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_INT16:
         stepSize = 10; typeSize = 2;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_UINT16:
         stepSize = 10; typeSize = 2;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_INT32:
         stepSize = 10; typeSize = 4;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_UINT32:
         stepSize = 10; typeSize = 4;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_FLOAT32:
         stepSize = 8; typeSize = 4;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
      case NX_FLOAT64:
         stepSize = 10; typeSize = 8;
         if (dataDimensions[dataRank-1] < stepSize) stepSize = dataDimensions[dataRank-1];
         PrintArray (dataBuffer, dataType, dataLen, stepSize, typeSize);
         break;
   }
   if (NXfree((void**)&dataBuffer) != NX_OK) return NX_ERROR;
   return NX_OK;
}

/* Checks for data attributes and prints their values as XML attributes*/
int PrintAttributes ()
{
   int status, i, attrLen, attrType;
   NXname attrName;
   void *attrBuffer;

   i = 0;
   do {
      status = NXgetnextattr (inId, attrName, &attrLen, &attrType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         attrLen++; /* Add space for string termination */
         if (NXmalloc((void**)&attrBuffer, 1, &attrLen, attrType) != NX_OK) return NX_ERROR;
         if (NXgetattr (inId, attrName, attrBuffer, &attrLen , &attrType) != NX_OK) return NX_ERROR;
         if (i >= 2) {
            fprintf (outId, "\n");
            indent++;
            PrintIndent ();
            indent--;
            i = 0;
         }
         fprintf (outId, " %s=\"", &attrName);
         PrintValues (attrBuffer, attrType, attrLen);
         fprintf (outId, "\"");
         if (NXfree((void**)&attrBuffer) != NX_OK) return NX_ERROR;
         i++;
      }
   } while (status != NX_EOD);
   return NX_OK;
}

/* Prints the data values with the appropriate type */
void PrintValues (void *data, int dataType, int numElements)
{
   int i;

   for (i=0; i<numElements; i++) {
      switch(dataType) {
         case NX_CHAR:
            fprintf (outId, "%c", ((char *)data)[i]);
            break;
         case NX_INT8:
            fprintf (outId, "%d", ((char *)data)[i]);
            if (i < numElements-1) fprintf (outId, " ");
            break;
         case NX_UINT8:
            fprintf (outId, "%d", ((unsigned char *)data)[i]);
            if (i < numElements-1) fprintf (outId, " ");
            break;
         case NX_INT16:
            fprintf (outId, "%d", ((short *)data)[i]);
            if (i < numElements-1) fprintf (outId, " ");
            break;
         case NX_UINT16:
            fprintf (outId, "%d", ((unsigned short *)data)[i]);
            if (i < numElements-1) fprintf (outId, " ");
            break;
         case NX_INT32:
            fprintf (outId, "%d", ((int *)data)[i]);
             if (i < numElements-1) fprintf (outId, " ");
           break;
         case NX_UINT32:
            fprintf (outId, "%d", ((uint *)data)[i]);
             if (i < numElements-1) fprintf (outId, " ");
           break;
         case NX_FLOAT32:
            fprintf (outId, "%f", ((float *)data)[i]);
             if (i < numElements-1) fprintf (outId, " ");
           break;
         case NX_FLOAT64:
            fprintf (outId, "%f", ((double *)data)[i]);
            if (i < numElements-1) fprintf (outId, " ");
            break;
         default:
            printf ("NX_ERROR: Invalid type in PrintValues");
            break;
      }
   }
}

/* Prints an array of data values as multiple lines if necessary */
void PrintArray (void *dataBuffer, int dataType, int dataLen, int stepSize, int typeSize) 
{
   int i;
   
   if (dataType == NX_CHAR || dataLen <= stepSize) {
      PrintValues (dataBuffer, dataType, dataLen);
   } else {
      fprintf (outId, "\n");
      indent = indent++;
      i = 0;
      do {
         PrintIndent ();
         if (i+stepSize > dataLen) stepSize = dataLen - i;
         PrintValues (dataBuffer+i*typeSize, dataType, stepSize);
         fprintf (outId, "\n");
         i += stepSize;
      } while (i < dataLen);
      indent--;
      PrintIndent ();
   }   
}

/* Prints the specified dimensions as a formatted string */
void PrintDimensions (int rank, int dimensions[])
{
   int i;

   if (rank > 1 || dimensions[0] != 1) {
      fprintf (outId, "[");
      for (i=0; i<rank; i++) {
         if (i > 0) 
            fprintf (outId, ",");
         fprintf (outId, "%d", dimensions[i]);
      }
      fprintf (outId, "]");
   }
}

/* Converts the NeXus data type into a character string */
void PrintType (int dataType)
{
   switch (dataType) {
      case NX_CHAR:
        fprintf (outId, "NX_CHAR");
        break;
      case NX_FLOAT32:
        fprintf (outId, "NX_FLOAT32");
        break;
      case NX_FLOAT64:
        fprintf (outId, "NX_FLOAT64");
        break;
      case NX_INT8:
        fprintf (outId, "NX_INT8");
        break;
      case NX_UINT8:
        fprintf (outId, "NX_UINT8");
        break;
      case NX_INT16:
        fprintf (outId, "NX_INT16");
        break;
      case NX_UINT16:
        fprintf (outId, "NX_UINT16");
        break;
      case NX_INT32:
        fprintf (outId, "NX_INT32");
        break;
      case NX_UINT32:
        fprintf (outId, "NX_UINT32");
        break;
      default:
        fprintf (outId, "UNKNOWN");
        break;
   }
}

/* Prints line indents according to the current nesting value */
void PrintIndent ()
{
   int i;

   for (i=0; i<indent; i++) {
      fprintf (outId, " ");
   }
}

