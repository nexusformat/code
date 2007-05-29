/*-----------------------------------------------------------------------------
 NeXus - Neutron & X-ray Common Data Format
  
 NeXus Browser

 Copyright (C) 2000, Ray Osborn

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

 Contact : R. Osborn <ROsborn@anl.gov>
           Materials Science Division
           Argonne National Laboratory
           Argonne, IL 60439-4845
           USA

 For further information, see <http://www.neutron.anl.gov/NeXus/>

 $Id$
!----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "napi.h"

#define StrEq(s1, s2) (strcmp((s1), (s2)) == 0)

int NXBdir (NXhandle fileId);
int NXBopen (NXhandle fileId, NXname groupName);
int NXBread (NXhandle fileId, NXname dataName, char *dimensions);
int NXBdump (NXhandle fileId, NXname dataName, char *fileName);
void ConvertUpperCase (char *string);
void PrintAttributes (NXhandle fileId);
void PrintDimensions (int rank, int dimensions[]);
void PrintType (int dataType);
void PrintData (void* data, int dataType, int numElements);
void DumpData(FILE *fd, int rank, int dimensions[], int dataType, void *data);
void WriteData (FILE *fd, char *data, int dataType, int numElements);
int FindGroup (NXhandle fileId, char *groupName, char *groupClass);
int FindData (NXhandle fileId, char *dataName);
static int nxTypeSize(int dataType);

/* if iByteAsChar, NX_INT8 and NX_UINT8 are treated as characters */
static int iByteAsChar = 1; /* Assume global attributes are all characters */
static char nxFile[256];

int main(int argc, char *argv[])
{
   NXhandle fileId;
   char fileName[80], inputText[255], path[80], *command, *dimensions, *stringPtr;
   NXname groupName, dataName;
   int status, groupLevel = 0, i;

   printf ("NXBrowse %s Copyright (C) 2000 R. Osborn, M. Koennecke, P. Klosowski\n", NEXUS_VERSION);

/* if there is a filename given on the command line use that,
      else ask for a filename */
   if (argc < 2) {
      printf ("Give name of NeXus file : ");
      fgets (fileName, sizeof(fileName), stdin);
      if ((stringPtr = strchr(fileName, '\n')) != NULL) 
         *stringPtr = '\0';
   }
   else {
     strcpy (fileName, argv[1]);
   }
   strcpy (nxFile, fileName);
 
/* Open input file and output global attributes */
   if (NXopen (fileName, NXACC_READ, &fileId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", fileName);
      return NX_ERROR;
   }
   PrintAttributes (fileId);
   iByteAsChar = 0; /* Display remaining NX_INT8 and NX_UINT8 variables as integers by default */
/* Input commands until the EXIT command is given */
   strcpy (path, "NX");
   do {
      printf ("%s> ", path);
      if (fgets(inputText, sizeof(inputText), stdin) == NULL)
      {
         return NX_OK;
      }

      if ((stringPtr = strchr(inputText, '\n')) != NULL) 
         *stringPtr = '\0';
      command = strtok(inputText," ");
      /* Check if a command has been given */
      if (command == NULL) command = " ";
      /* Convert it to upper case characters */
      ConvertUpperCase (command);
      /* Command is to print a directory of the current group */
      if (StrEq(command, "DIR") || StrEq(command, "LS")) {
         status = NXBdir (fileId);
      }    
      /* Command is to open the specified group */
      if (StrEq(command, "OPEN") || StrEq(command, "CD")) {
         stringPtr = strtok(NULL, " "); 
         if (stringPtr != NULL) {
            strcpy (groupName, stringPtr);
            status = NXBopen (fileId, groupName);
            /* Add the group to the prompt string */
            if (status == NX_OK) {
               strcat (path, "/");
               strcat (path, groupName);
               groupLevel++;
            }
         }
         else {
            printf ("NX_ERROR: Specify a group\n");
         }
      }
      /* Command is to  dump data values to a file */
      if (StrEq(command, "DUMP")) {
         stringPtr = strtok (NULL, " ");
         if (stringPtr != NULL) {
            strcpy (dataName, stringPtr);
            stringPtr = strtok(NULL," ");
            if (stringPtr != NULL) {
               strcpy (fileName, stringPtr);
               status = NXBdump (fileId, dataName, fileName);
            }
            else {
               printf ("NX_ERROR: Specify a dump file name \n");
            }
         }
         else {
            printf ("NX_ERROR: Specify a data item\n");
         }
      }
      /* Command is to print the values of the data */
      if (StrEq(command, "READ") || StrEq(command, "CAT")) {
         stringPtr = strtok (NULL, " [");
         if (stringPtr != NULL) {
            strcpy (dataName, stringPtr);
            dimensions = strtok(NULL, "[]");
            status = NXBread (fileId, dataName, dimensions);
         }
         else {
            printf ("NX_ERROR: Specify a data item\n");
         }
      }
      /* Command is to close the current group */
      if (StrEq(command, "CLOSE")) {
         if (groupLevel > 0) {
            if (NXclosegroup (fileId) == NX_OK) {
               /* Remove the group from the prompt string */
               stringPtr = strrchr (path, '/'); /* position of last group delimiter */
               if (stringPtr != NULL) 
                  *stringPtr = '\0';            /* terminate the string there */
               groupLevel--;
            }
         }
         else {
            printf ("NX_WARNING: Already at root level of file\n");
         }
      }
      /* Command is to print help information */
      if (StrEq(command, "HELP") || StrEq(command, "INFO")) {
         printf ("NXbrowse commands : DIR\n");
         printf ("                    OPEN <groupName>\n");
         printf ("                    READ <dataName>\n");
         printf ("                    READ <dataName>[<dimension indices...>]\n");
         printf ("                    DUMP <dataName> <fileName> \n");
         printf ("                    CLOSE\n");
         printf ("                    BYTEASCHAR\n");
         printf ("                    HELP\n");
         printf ("                    EXIT\n");
      }
      /* Command is to print byte as char information */
      if (StrEq(command, "BYTEASCHAR")) {
         if (iByteAsChar == 1)
            iByteAsChar = 0;
         else
            iByteAsChar = 1;
      }
      /* Command is to exit the program */
      if (StrEq(command, "EXIT") || StrEq(command, "QUIT")) {
         for (i = groupLevel; i > 0; i--) NXclosegroup (fileId);
         NXclose (&fileId);
         return NX_OK;
      }
      status = NX_OK;
   } while (status == NX_OK);
   return NX_OK;
}
/* Checks for attributes and outputs their values */
void PrintGroupAttributes (NXhandle fileId, char *groupname)
{
   int status, attrLen, attrType;
   NXname attrName;
   void *attrBuffer;

   do {
      status = NXgetnextattr (fileId, attrName, &attrLen, &attrType);
      if (status == NX_ERROR) return;
      if (status == NX_OK) {
         attrLen++; /* Add space for string termination */
         if (NXmalloc((void**)&attrBuffer, 1, &attrLen, attrType) != NX_OK) return;
         if (NXgetattr (fileId, attrName, attrBuffer,&attrLen , &attrType) != NX_OK) return;
         printf ("             %s attribute: %s = ", groupname, attrName);
         PrintData (attrBuffer, attrType, attrLen);
         printf ("\n");
         if (NXfree((void**)&attrBuffer) != NX_OK) return;
      }
   } while (status != NX_EOD);
   return;
}

/* Outputs the contents of a NeXus group */
int NXBdir (NXhandle fileId)
{
   int status, dataType, dataRank, dataDimensions[NX_MAXRANK], length;
   NXname name, class, nxurl;

   if (NXinitgroupdir (fileId) != NX_OK) return NX_ERROR;
   do {
      status = NXgetnextentry (fileId, name, class, &dataType);
      if (status == NX_ERROR) break;
      if (status == NX_OK) {
	if (strncmp(class,"CDF",3) == 0){ 
	    ;
	}
	else if (strcmp(class,"SDS") == 0){ 
            printf ("  NX Data  : %s", name);
            if (NXopendata (fileId, name) != NX_OK) return NX_ERROR;
            if (NXgetinfo (fileId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
            if (NXclosedata(fileId) != NX_OK) return NX_ERROR;
            PrintDimensions (dataRank, dataDimensions);
            printf (" ");
            PrintType (dataType);
            printf ("\n");
	} else {
	    length = sizeof(nxurl);
	    if(NXisexternalgroup(fileId, name,class,nxurl,length) == NX_OK){
	      printf ("  NX external Group: %s (%s), linked to: %s \n",name,class,nxurl); 
            } else {
	      printf ("  NX Group : %s (%s)\n", name, class);
	      if((status = NXopengroup(fileId,name,class)) != NX_OK){
		return status;
	      } 
	      PrintGroupAttributes(fileId, name);
	      if((status = NXclosegroup(fileId)) != NX_OK){
		return status;
	      } 
            }
	}
      }
   } while (status == NX_OK);
   return status;
}

/* Opens the requested group */
int NXBopen (NXhandle fileId, NXname groupName)
{
   NXname groupClass;

   if (groupName == NULL) {
      printf ("NX_ERROR: Specify a group name with the OPEN command\n");
      return NX_ERROR;
   }
   if (FindGroup (fileId, groupName, groupClass) != NX_OK) return NX_ERROR;
   if (NXopengroup (fileId, groupName, groupClass) != NX_OK) return NX_ERROR;
   return NX_OK;
}

/* Outputs requested data */
int NXBread (NXhandle fileId, NXname dataName, char *dimensions)
{
   int dataRank, dataDimensions[NX_MAXRANK], dataType, start[NX_MAXRANK], size[NX_MAXRANK], i, j, total_size;
   char dimString[80], *subString;
   void *dataBuffer;
  
   /* Check the specified data item exists */
   if (FindData (fileId, dataName) != NX_OK) return NX_ERROR;
   /* Open the data and obtain its type and rank details */
   if (NXopendata (fileId, dataName) != NX_OK) return NX_ERROR;
   if (NXgetinfo (fileId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
   /* Check if a single element has been specified */
   /* If so, read in the indices */
   if (dimensions != NULL) {
      strcpy (dimString, dimensions);      
      subString = strtok (dimString, ",");
      for (i = 0; subString != NULL && i < NX_MAXRANK; i++) {
         if (i >= dataRank) {
            printf ("NX_ERROR: Data rank = %d\n", dataRank);
            return NX_ERROR;
         }
         sscanf (subString, "%d", &j);
         if (j > dataDimensions[i] || j < 1) {
            printf ("NX_ERROR: Data dimension %d = %d\n", (i+1), dataDimensions[i]);
            return NX_ERROR;
         }
         start[i] = j - 1;
         size[i] = 1;
         subString = strtok (NULL, ",");
      }
      if (i != dataRank) {
         printf ("NX_ERROR: Data rank = %d\n", dataRank);
         return NX_ERROR;
      }
   }
   /* Otherwise, allocate enough space for the first 3 elements of each dimension */
   else {
      for (i = 0; i < dataRank; i++) {
         if (dataDimensions[i] > 3 && dataType != NX_CHAR) {
            start[i] = 0;
            size[i] = 3;
         } /* unless it's a character string */
         else {
            start[i] = 0;
            size[i] = dataDimensions[i];
         }
      }
   }
   total_size = 1;
   for(i = 0; i < dataRank; i++)
   {
       total_size *= dataDimensions[i];
   }
   if (NXmalloc((void**)&dataBuffer, dataRank, size, dataType) != NX_OK) return NX_ERROR;
   /* Read in the data with NXgetslab */
   if (dataType == NX_CHAR) {
      if (NXgetdata(fileId, dataBuffer) != NX_OK) return NX_ERROR;
   }
   else {
      if (NXgetslab (fileId, dataBuffer, start, size) != NX_OK) return NX_ERROR;
   }
   /* Output data name, dimensions and type */
   printf ("  %s", dataName);
   if (dimensions == NULL)
      PrintDimensions (dataRank, dataDimensions);
   else 
      printf ("[%s]", dimensions);
   printf (" ");
   PrintType (dataType);
   printf (" = ");
   /* Output the data according to data type */
   if (dimensions == NULL) {  /* Print the first few values (max 3) */
      if (dataType == NX_CHAR) { /* If the data is a string, output the whole buffer */
/* this prints the first line of an array; could print more */
         for(i=0; i<total_size / dataDimensions[dataRank-1]; i++)
	 {
             PrintData (dataBuffer + i * dataDimensions[dataRank-1], dataType, dataDimensions[dataRank-1]);
	     PrintData ("\n", NX_CHAR, 1);
         }
      }
      else {
         if (dataRank == 1 && dataDimensions[0] == 1) {    /* It's a scalar */
            PrintData (dataBuffer, dataType, 1);
         }
         else {                                            /* It's an array */
            printf ("[ ");
            /* Determine total size of input buffer */
            for (i = 0, j = 0; i < dataRank; i++)
               j += dataDimensions[i];
            /* Output at least 3 values */
            if (j > 3) {
               PrintData (dataBuffer, dataType, 3);
               printf ("...");
            }
            /* unless the total size is smaller */
            else {
               PrintData (dataBuffer, dataType, j);
            }
            printf ("]");
         }
      }
   }
   else {                      /* Print the requested item */    
      PrintData (dataBuffer, dataType, 1);
   }
   printf ("\n");
   if (NXfree((void**)&dataBuffer) != NX_OK) return NX_ERROR;

   /* Check for attributes unless a single element is specified */
   if (dimensions == NULL) 
      PrintAttributes (fileId);

   /* Close data set */
   if (NXclosedata(fileId) != NX_OK) return NX_ERROR;

   return NX_OK;
}

/* Dumps requested data */
int NXBdump (NXhandle fileId, NXname dataName, char *fileName)
{
   int dataRank, dataDimensions[NX_MAXRANK], dataType, i;
   FILE *fd = NULL;
   void *dataBuffer;
  
   /* Check the specified data item exists */
   if (FindData (fileId, dataName) != NX_OK) return NX_ERROR;

   /* Open the data and obtain its type and rank details */
   if (NXopendata (fileId, dataName) != NX_OK) return NX_ERROR;
   if (NXgetinfo (fileId, &dataRank, dataDimensions, &dataType) != NX_OK) 
      return NX_ERROR;
   
   /* Open the file */
   fd = fopen (fileName,"w");
   if(!fd) {
     printf ("ERROR: failed to open--> %s <-- for writing\n",
            fileName);
     return NX_ERROR;
   }

   /* Allocate data space */
   if (NXmalloc(&dataBuffer, dataRank, dataDimensions, dataType) != NX_OK)
      return NX_ERROR;

   /* Read the lot */
   if (NXgetdata(fileId, dataBuffer) != NX_OK)
      return NX_ERROR;

   if (NXclosedata(fileId) != NX_OK) return NX_ERROR;

   /* Print a header */
   fprintf (fd,"File : %s, DataSet: %s \n", nxFile,  dataName);
   for (i=0; i < dataRank; i++) {
      fprintf(fd," %d ", dataDimensions[i]);
   }
   fprintf(fd,"\n");

   /* Dump the data */
   DumpData (fd, dataRank, dataDimensions, dataType, dataBuffer);

   /* Clean up */
   fclose (fd);
   NXfree (&dataBuffer);
   return NX_OK;
}

/* Converts command string to upper case */
void ConvertUpperCase (char *string)
{
   int i;
   
   for (i=0; string[i]!=0; i++) {
      if (string[i] >= 97 && string[i] <= 122) {
         string[i] = string[i] - 32;
      }
   }
}   

/* Checks for attributes and outputs their values */
void PrintAttributes (NXhandle fileId)
{
   int status, attrLen, attrType;
   NXname attrName;
   void *attrBuffer;

   do {
      status = NXgetnextattr (fileId, attrName, &attrLen, &attrType);
      if (status == NX_ERROR) return;
      if (status == NX_OK) {
         attrLen++; /* Add space for string termination */
         if (NXmalloc((void**)&attrBuffer, 1, &attrLen, attrType) != NX_OK) return;
         if (NXgetattr (fileId, attrName, attrBuffer,&attrLen , &attrType) != NX_OK) return;
         printf ("    %s = ", attrName);
         PrintData (attrBuffer, attrType, attrLen);
         printf ("\n");
         if (NXfree((void**)&attrBuffer) != NX_OK) return;
      }
   } while (status != NX_EOD);
   return;
}

/* Outputs the specified dimensions as a formatted string */
void PrintDimensions (int rank, int dimensions[])
{
   int i;

   if (rank > 1 || dimensions[0] != 1) {
      printf ("[");
      for (i=0; i<rank; i++) {
         if (i > 0) 
            printf (",");
         printf ("%d", dimensions[i]);
      }
      printf ("]");
   }
}

/* Converts the NeXus data type into a character string */
void PrintType (int dataType)
{
   switch (dataType) {
      case NX_CHAR:
        printf ("(NX_CHAR)");
        break;
      case NX_FLOAT32:
        printf ("(NX_FLOAT32)");
        break;
      case NX_FLOAT64:
        printf ("(NX_FLOAT64)");
        break;
      case NX_INT8:
        printf ("(NX_INT8)");
        break;
      case NX_UINT8:
        printf ("(NX_UINT8)");
        break;
      case NX_INT16:
        printf ("(NX_INT16)");
        break;
      case NX_UINT16:
        printf ("(NX_UINT16)");
        break;
      case NX_INT32:
        printf ("(NX_INT32)");
        break;
      case NX_UINT32:
        printf ("(NX_UINT32)");
        break;
      default:
        printf ("(UNKNOWN)");
        break;
   }
}

/* Dumps data to a file. Uses recursion because of unknown number of dimensions. */
void DumpData(FILE *fd, int rank, int dimensions[], int dataType, void *data)
{
   char *dataPtr;
   int i, dimSize, dataSize, lineSize;
   int type_size = 0;

   if (rank > 1) { /* Recursively call DumpData until rank = 1 */
      for (i = 1, dimSize = 1; i < rank; i++) {
         dimSize *= dimensions[i];
      }
      for (i = 0; i < dimensions[0]; i++) {
         dataPtr = (char *)data + i * dimSize * nxTypeSize(dataType);        
         DumpData (fd, rank-1, &dimensions[1], dataType, dataPtr);
      }
      return;
   }
   else { /* Actually print the data */
      dataSize = dimensions[0];
      dataPtr = (char *)data;
      while(dataSize > 0) {
        if (dataSize > 10) {
           lineSize = 10;
        }
        else {
           lineSize = dataSize;
        }
        WriteData (fd, dataPtr, dataType, lineSize);
        fprintf(fd,"\n");
        dataSize -= lineSize;
        dataPtr += lineSize * nxTypeSize(dataType);
        printf("%p\n", dataPtr);

      }
   }
}

/* Writes data items with the requested type */
void WriteData (FILE *fd, char *data, int dataType, int numElements)
{
   int i;

   for (i=0; i<numElements; i++) {
      switch(dataType) {
         case NX_CHAR:
            fprintf (fd, "%c", ((char *)data)[i]);
            break;
         case NX_INT8:
            if(iByteAsChar) {
               fprintf (fd, "%c", ((char *)data)[i]);
            }
            else {
               fprintf (fd, "%d ", ((char *)data)[i]);
            }
            break;
         case NX_UINT8:
            if(iByteAsChar) {
                fprintf (fd, "%c", ((unsigned char *)data)[i]);
            }
            else {
                fprintf (fd, "%d ", ((unsigned char *)data)[i]);
            }
            break;
         case NX_INT16:
            fprintf (fd, "%d ", ((short *)data)[i]);
            break;
         case NX_UINT16:
            fprintf (fd, "%d ", ((unsigned short *)data)[i]);
            break;
         case NX_INT32:
            fprintf (fd, "%d ", ((int *)data)[i]);
            break;
         case NX_UINT32:
            fprintf (fd, "%d ", ((unsigned *)data)[i]);
            break;
         case NX_FLOAT32:
            fprintf (fd, "%f ", ((float *)data)[i]);
            break;
         case NX_FLOAT64:
            fprintf (fd, "%f ", ((double *)data)[i]);
            break;
         default:
            printf ("WriteData: invalid type");
            break;
      }
   }
}

/* Outputs data items with the requested type */
void PrintData (void *data, int dataType, int numElements)
{
   WriteData (stdout, data, dataType, numElements);
}
/* Searches group for requested group and return its class */
int FindGroup (NXhandle fileId, char *groupName, char *groupClass)
{
   int status, dataType;
   NXname name, class;

   NXinitgroupdir (fileId);
   do {
      status = NXgetnextentry (fileId, name, class, &dataType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
          if (StrEq (groupName, name)) {
            strcpy (groupClass, class);
            if (!strncmp(groupClass,"NX",2)) {
               return NX_OK;
            }
            else {
/*
 *               printf ("NX_ERROR: %s is not a group\n", groupName);
 *               return NX_ERROR;
 */
		return NX_OK; /* allow non NX (i.e. user defined) groups */
            }
         }
      }
   } while (status != NX_EOD);
   printf ("NX_ERROR: %s does not exist\n", groupName);
   return NX_EOD;
}

/* Searches group for the requested data item */
int FindData (NXhandle fileId, char *dataName)
{
   int status, dataType;
   NXname name, class;

   NXinitgroupdir (fileId);
   do {
      status = NXgetnextentry (fileId, name, class, &dataType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (StrEq(dataName,name)) {
            if (!strncmp(class,"SDS",3)) { /* Data has class "SDS" */
               return NX_OK;
            }
            else {
               printf ("NX_ERROR: %s is not data\n", dataName);
               return NX_ERROR;
            }
         }
      }
   } while (status != NX_EOD);
   printf ("NX_ERROR: %s does not exist\n", dataName);
   return NX_EOD;
}

static int nxTypeSize(int dataType)
{
#ifdef XXXHDF4
    return DFKNTsize(dataType);        
#else
	int type_size = 0;
           if ((dataType == NX_CHAR) || (dataType == NX_INT8) || (dataType == NX_UINT8)) {
            type_size = 1;
         } else if ((dataType == NX_INT16) || (dataType == NX_UINT16)) {
            type_size = 2;
         } else if ((dataType == NX_INT32) || (dataType == NX_UINT32) || (dataType == NX_FLOAT32)) {
            type_size = 4;
         } else if (dataType == NX_FLOAT64) {
            type_size = 8;
         }
	 else
	 {
	    printf("error in type %d\n", dataType);
 	 }
    return type_size;
#endif
}
