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
#include "nxconvert_common.h"

static void print_usage()
{
    printf("Usage: nxconvert [ -x | -h4 | -h5 | -d | -o keepws | -o table ] [ infile ] [ outfile ]\n");
}

#define NXCONVERT_EXIT_ERROR	exit(1)

static const char* definition_name = "NXroot";

int main(int argc, char *argv[])
{
   char inFile[256], outFile[256], *stringPtr;
   int opt, nx_format = -1, nx_write_access = 0;
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

	  case 'd':
	    nx_format = NX_DEFINITION;
	    nx_write_access |= NXACC_CREATEXML;
	    if (optarg != NULL && *optarg != '\0')
	    {
		definition_name = optarg;
	    }
	    break;

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
	    else if (!strcmp(optarg, "table"))
	    {
	        nx_write_access |= NXACC_TABLE;
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

   if (convert_file(nx_format, inFile, nx_read_access, outFile, nx_write_access) != NX_OK) {
      NXCONVERT_EXIT_ERROR;
   }
   printf("Convertion successful.\n");
   return 0;
}

