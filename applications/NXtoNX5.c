/*-----------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
   
  Utility to convert a NeXus file into HDF5
 
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

int WriteGroup ();
int WriteAttributes ();

static NXhandle inId, outId;

int main(int argc, char *argv[])
{
   char inFile[256], outFile[256], *stringPtr;

/* use command line arguments for input and output filenames if given */
   if (argc < 2) {
      printf ("Give name of input NeXus file : ");
      fgets (inFile, sizeof(inFile), stdin);
      if ((stringPtr = strchr(inFile, '\n')) != NULL) *stringPtr = '\0';
   }
   else {
     strcpy (inFile, argv[1]);
   }
   if (argc < 3) {
      printf ("Give name of output NeXus 5 file : ");
      fgets (outFile, sizeof(outFile), stdin);
      if ((stringPtr = strchr(outFile, '\n')) != NULL) *stringPtr = '\0';
   }
   else {
     strcpy (outFile, argv[2]);
   }

/* Open NeXus input file and NeXus 5 output file */
   if (NXopen (inFile, NXACC_READ, &inId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", inFile);
      return NX_ERROR;
   }

   if (NXopen (outFile, NXACC_CREATE5, &outId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", outFile);
      return NX_ERROR;
   }
   
/* Output global attributes */
   if (WriteAttributes () != NX_OK) return NX_ERROR;
/* Recursively cycle through the groups printing the contents */
   if (WriteGroup () != NX_OK) return NX_ERROR;
/* Close the input and output files */
   NXclose (&outId);
   NXclose (&inId);
   return NX_OK;
}

/* Prints the contents of each group as XML tags and values */
int WriteGroup ()
{ 
   int status, dataType, dataRank, dataDimensions[NX_MAXRANK], dataLen;     
   NXname name, class;
   void *dataBuffer;

   do {
      status = NXgetnextentry (inId, name, class, &dataType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (!strncmp(class,"NX",2)) {
            if (NXopengroup (inId, name, class) != NX_OK) return NX_ERROR;
            if (NXmakegroup (outId, name, class) != NX_OK) return NX_ERROR;
            if (NXopengroup (outId, name, class) != NX_OK) return NX_ERROR;
            if (WriteGroup () != NX_OK) return NX_ERROR;
         }
         else if (!strncmp(class,"SDS",3)) {
            if (NXopendata (inId, name) != NX_OK) return NX_ERROR;
            if (NXgetinfo (inId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
            if (NXmakedata (outId, name, dataType, dataRank, dataDimensions) != NX_OK) return NX_ERROR;
            if (NXopendata (outId, name) != NX_OK) return NX_ERROR;
            if (WriteAttributes () != NX_OK) return NX_ERROR;
            if (NXmalloc (&dataBuffer, dataRank, dataDimensions, dataType) != NX_OK) return NX_ERROR;
            if (NXgetdata (inId, dataBuffer)  != NX_OK) return NX_ERROR;
            if (NXputdata (outId, dataBuffer) != NX_OK) return NX_ERROR;
            if (NXclosedata (inId) != NX_OK) return NX_ERROR;
            if (NXclosedata (outId) != NX_OK) return NX_ERROR;
         }
      }
      else if (status == NX_EOD) {
         if (NXclosegroup (inId) != NX_OK) return NX_ERROR;
         if (NXclosegroup (outId) != NX_OK) return NX_ERROR;
         return NX_OK;
      }
   } while (status == NX_OK);
   return NX_OK;
}


/* Checks for data attributes and prints their values as XML attributes*/
int WriteAttributes ()
{
   int status, i, attrLen, attrType;
   NXname attrName;
   void *attrBuffer;

   i = 0;
   do {
      status = NXgetnextattr (inId, attrName, &attrLen, &attrType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (strcmp(attrName, "NeXus_version") && 
             strcmp(attrName, "HDF_version") && strcmp(attrName, "HDF5_version") && 
             strcmp(attrName, "file_name") && strcmp(attrName, "file_time")) {
            attrLen++; /* Add space for string termination */
            if (NXmalloc((void**)&attrBuffer, 1, &attrLen, attrType) != NX_OK) return NX_ERROR;
            if (NXgetattr (inId, attrName, attrBuffer, &attrLen , &attrType) != NX_OK) return NX_ERROR;
            if (NXputattr (outId, attrName, attrBuffer, attrLen , attrType) != NX_OK) return NX_ERROR;
            if (NXfree((void**)&attrBuffer) != NX_OK) return NX_ERROR;
         }
         i++;
      }
   } while (status != NX_EOD);
   return NX_OK;
}

