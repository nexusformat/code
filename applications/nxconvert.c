/*-----------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
   
  Utility to convert a NeXus file into HDF4/HDF5/XML/...
 
  Author: Freddie Akeroyd, Ray Osborn
 
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
#include <stdlib.h>
#include <unistd.h>
#include "napi.h"

static int WriteGroup (int write_data);
static int WriteAttributes (int write_data);

static NXhandle inId, outId;

#define NX_XML	0
#define NX_HDF4	1
#define NX_HDF5	2
#define NX_DTD	3

static void print_usage()
{
    printf("Usage: nxconvert [ -x | -h4 | -h5 | -o keepws ] [ infile ] [ outfile ]\n");
}

#define NXCONVERT_EXIT_ERROR	exit(1)

static const char* nx_formats[] = { "XML", "HDF4", "HDF5", "DTD", NULL };

int main(int argc, char *argv[])
{
   char inFile[256], outFile[256], *stringPtr;
   int opt, nx_format = -1, nx_write_access = 0;
   int nx_write_data = 1;
   int nx_read_access = NXACC_READ;

   while( (opt = getopt(argc, argv, "h:xdo:")) != -1 )
   {
/* use with "-:" in getopt	
	if (opt == '-')
	{
	    opt = optarg[0];
	    optarg++;
	} 
*/
	switch(opt)
	{
	  case 'x':
	    nx_format = NX_XML;
	    nx_write_access |= NXACC_CREATEXML;
	    break;

/*
	  case 'd':
	    nx_format = NX_DTD;
	    nx_write_access |= NXACC_CREATEXML;
	    nx_write_data = 0; 
	    break;
*/

	  case 'h':
	    if (!strcmp(optarg, "4") || !strcmp(optarg, "df4"))
	    {
		nx_format = NX_HDF4;
	        nx_write_access |= NXACC_CREATE4;
	    } 
	    else if (!strcmp(optarg, "5") || !strcmp(optarg, "df5"))
	    {
		nx_format = NX_HDF5;
	        nx_write_access |= NXACC_CREATE5;
	    }
	    else
	    {
	        printf("Invalid option -h%s\n", optarg);
	        print_usage();
		NXCONVERT_EXIT_ERROR;
	    }
	    break;

	  case 'o':
	    if (!strcmp(optarg, "keepws"))
	    {
	        nx_write_access |= NXACC_NOSTRIP;
	        nx_read_access |= NXACC_NOSTRIP;
	    }
	    else
	    {
	        printf("Invalid option -o%s\n", optarg);
	        print_usage();
		NXCONVERT_EXIT_ERROR;
	    }
	    break;

	  default:
/*	    printf("Invalid option -%c\n", opt); */
	    print_usage();
	    NXCONVERT_EXIT_ERROR;
	    break;
	}
   }
   if (nx_format == -1)
   {
	nx_format = NX_HDF4;
	nx_write_access |= NXACC_CREATE4;
   }
   if ((argc - optind) <  1)
   {
/* use command line arguments for input and output filenames if given */
      printf ("Give name of input NeXus file : ");
      fgets (inFile, sizeof(inFile), stdin);
      if ((stringPtr = strchr(inFile, '\n')) != NULL) { *stringPtr = '\0'; }
   }
   else 
   {
     strcpy (inFile, argv[optind]);
   }
   if ((argc - optind) <  2)
   {
      printf ("Give name of output %s NeXus file : ", nx_formats[nx_format]);
      fgets (outFile, sizeof(outFile), stdin);
      if ((stringPtr = strchr(outFile, '\n')) != NULL) { *stringPtr = '\0'; }
   }
   else 
   {
     strcpy (outFile, argv[optind+1]);
   }
   printf("Converting %s to %s NeXus file %s\n", inFile, nx_formats[nx_format], outFile);

/* Open NeXus input file and NeXus output file */
   if (NXopen (inFile, nx_read_access, &inId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", inFile);
      NXCONVERT_EXIT_ERROR;
   }

   if (NXopen (outFile, nx_write_access, &outId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", outFile);
      NXCONVERT_EXIT_ERROR;
   }
   
/* Output global attributes */
   if (WriteAttributes (nx_write_data) != NX_OK)
   {
	NXCONVERT_EXIT_ERROR;
   }
/* Recursively cycle through the groups printing the contents */
   if (WriteGroup (nx_write_data) != NX_OK)
   {
	NXCONVERT_EXIT_ERROR;
   }
/* Close the input and output files */
   if (NXclose (&outId) != NX_OK)
   {
	NXCONVERT_EXIT_ERROR;
   }
   if (NXclose (&inId) != NX_OK)
   {
	NXCONVERT_EXIT_ERROR;
   }
   printf("Convertion successful.\n");
   return 0;
}

/* Prints the contents of each group as XML tags and values */
int WriteGroup (int write_data)
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
            if (WriteGroup (write_data) != NX_OK) return NX_ERROR;
         }
         else if (!strncmp(class,"SDS",3)) {
            if (NXopendata (inId, name) != NX_OK) return NX_ERROR;
            if (NXgetinfo (inId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
            if (NXmakedata (outId, name, dataType, dataRank, dataDimensions) != NX_OK) return NX_ERROR;
            if (NXopendata (outId, name) != NX_OK) return NX_ERROR;
            if (WriteAttributes (write_data) != NX_OK) return NX_ERROR;
	    if (write_data == 1)
	    {
                if (NXmalloc (&dataBuffer, dataRank, dataDimensions, dataType) != NX_OK) return NX_ERROR;
                if (NXgetdata (inId, dataBuffer)  != NX_OK) return NX_ERROR;
                if (NXputdata (outId, dataBuffer) != NX_OK) return NX_ERROR;
                if (NXfree((void**)&dataBuffer) != NX_OK) return NX_ERROR;
	    }
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

int WriteAttributes (int write_data)
{
   int status, i, attrLen, attrType;
   NXname attrName;
   void *attrBuffer;

   i = 0;
   do {
      status = NXgetnextattr (inId, attrName, &attrLen, &attrType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (strcmp(attrName, "NeXus_version") && strcmp(attrName, "XML_version") &&
             strcmp(attrName, "HDF_version") && strcmp(attrName, "HDF5_Version") && 
             strcmp(attrName, "file_name") && strcmp(attrName, "file_time")) {
            attrLen++; /* Add space for string termination */
            if (NXmalloc((void**)&attrBuffer, 1, &attrLen, attrType) != NX_OK) return NX_ERROR;
            if (NXgetattr (inId, attrName, attrBuffer, &attrLen , &attrType) != NX_OK) return NX_ERROR;
	    if (write_data == 1)
	    {
                if (NXputattr (outId, attrName, attrBuffer, attrLen , attrType) != NX_OK) return NX_ERROR;
	    }
	    else
   	    {
                if (NXputattr (outId, attrName, attrBuffer, attrLen , attrType) != NX_OK) return NX_ERROR;
	    }
            if (NXfree((void**)&attrBuffer) != NX_OK) return NX_ERROR;
         }
         i++;
      }
   } while (status != NX_EOD);
   return NX_OK;
}

