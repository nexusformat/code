/*-----------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
   
  Utility to convert a NeXus file into HDF4/HDF5/XML/...
 
  Author: Freddie Akeroyd
 
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
 
 $Id: nxconvert.c 991 2008-03-19 19:30:03Z Freddie Akeroyd $
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "napi.h"
#include "nxconvert_common.h"

static void print_usage()
{
    printf("Usage: nxvalidate [-w] [ -d definition ] [ infile ]\n\n");
    printf("-w    Send file to NeXus web site (using wget) for validation\n");
    printf("      (default: try to run \"xmllint\" program locally\n");
    printf("-d    Use specified definiton (default: BASE)\n");
    printf("-k    keep temporary files\n");
    printf("-q    quiet (only report errors)\n");
}

static int url_encode(char c, FILE* f)
{
	switch(c)
	{
	    case '+':
	    case '&':
	    case '?':
	    case '$':
	    case ',':
	    case '/':
	    case ':':
	    case '=':
	    case ';':
	    case '@':
	    case '%':
	    case '#':
	        fprintf(f, "%%%02X", c);
	  	break;

	    default:
		fputc(c, f);
		break;
	}
	return 0;
}

#define NXVALIDATE_ERROR_EXIT	exit(1)

static const char* definition_name = "BASE";

static int quiet = 0;

int main(int argc, char *argv[])
{
   char inFile[256], outFile[256], command[512], outFile2[256], *strPtr;
   int ret, opt, c, i, fd, use_web = 0;
   FILE *fIn, *fOut2;
   int keep_temps = 0;

   while( (opt = getopt(argc, argv, "wqkhd:")) != -1 )
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
	  case 'd':
	    if (optarg != NULL && *optarg != '\0')
	    {
		printf("Warning: -d option is not implemented yet\n");
		definition_name = optarg;
	    }
	    break;

	  case 'h':
	    print_usage();
	    return 0;

	  case 'q':
            quiet = 1;
            break;
	
	  case 'w':
	    use_web = 1;
	    break;

	  case 'k':
	    keep_temps = 1;
	    break;

	  default:
/*	    printf("Invalid option -%c\n", opt); */
	    print_usage();
	    NXVALIDATE_ERROR_EXIT;
	    break;
	}
   }
   if ((argc - optind) <  1)
   {
/* use command line arguments for input and output filenames if given */
      printf ("Give name of input NeXus file : ");
      fgets (inFile, sizeof(inFile), stdin);
      if ((strPtr = strchr(inFile, '\n')) != NULL) { *strPtr = '\0'; }
   }
   else 
   {
     strcpy (inFile, argv[optind]);
   }
   if (!quiet) {
      printf("* Validating %s using definition %s.xsd\n", inFile, definition_name);
   }

   sprintf(outFile, "%s/%s.XXXXXX", P_tmpdir, inFile);
   if ( (fd = mkstemp(outFile)) == -1 )
   {
	printf("error\n");
	NXVALIDATE_ERROR_EXIT;
   }
   close(fd);
   if (convert_file(NX_DEFINITION, inFile, NXACC_READ, outFile, NXACC_CREATEXML) != NX_OK)
   {
	printf("* Error converting file %s to definiton XML format", inFile);
	NXVALIDATE_ERROR_EXIT;
   }
   if (use_web == 0)
   {
       if (!quiet) {
           printf("* Validating using locally installed \"xmllint\" program\n");
       }
       sprintf(command, "xmllint --noout --schema http://definition.nexusformat.org/schema/3.0/%s.xsd \"%s\" %s", definition_name, outFile, (quiet ? "> /dev/null 2>&1" : ""));
       ret = system(command);
       if (ret != -1 && WIFEXITED(ret))
       {
	    if (!keep_temps)
	    {
		remove(outFile);
	    }
	    if (WEXITSTATUS(ret) != 0)
	    {
	        printf("* Validation with \"xmllint\" of %s failed\n", inFile);
		printf("* If the program was unable to find all the required schema from the web, check your \"http_proxy\" environment variable\n");
	        NXVALIDATE_ERROR_EXIT;
	    }
	    else
	    {
	        if (!quiet) {
                    printf("* Validation with \"xmllint\" of %s OK\n", inFile);
                }
	        return 0;
	    }
       }
       printf("* Unable to find \"xmllint\" to validate file - will try web validation\n");
       printf("* \"xmllint\" is installed as part of libxml2 from xmlsoft.org\n");
   }
   sprintf(outFile2, "%s/%s.XXXXXX", P_tmpdir, inFile);
   if ( (fd = mkstemp(outFile2)) == -1 )
   {
	printf("error\n");
	NXVALIDATE_ERROR_EXIT;
   }
   close(fd);
   fOut2 = fopen(outFile2, "wt");
   fIn = fopen(outFile, "rt");
   fprintf(fOut2, "file_name=");
   for(strPtr = inFile; *strPtr != '\0'; ++strPtr)
   {
	url_encode(*strPtr, fOut2);
   }
   fprintf(fOut2, "&file_data=");
   while( (c = fgetc(fIn)) != EOF )
   {
	url_encode(c, fOut2);
   }
   fclose(fIn);
   fclose(fOut2);
   if (!quiet) {
       printf("* Validating via http://definition.nexusformat.org using \"wget\"\n");
   }
   sprintf(command, "wget --quiet -O %s --post-file=\"%s\" http://definition.nexusformat.org/validate/run", (quiet ? "/dev/null" : "-"), outFile2);
   ret = system(command);
   if (!keep_temps)
   {
	remove(outFile);
	remove(outFile2);
   }
   if (ret == -1)
   {
	printf("* Unable to find \"wget\" to validate the file %s over the web\n", inFile);
	NXVALIDATE_ERROR_EXIT;
   }

   if (WIFEXITED(ret) && (WEXITSTATUS(ret) != 0))
   {
	printf("* Validation via http://definition.nexusformat.org/ of %s failed\n", inFile);
	printf("* If \"wget\" was unable to load the schema files from the web, check your \"http_proxy\" environment variable\n");
	NXVALIDATE_ERROR_EXIT;
   }
   else if (!quiet)
   {
	printf("* Validation via http://definition.nexusformat.org/ of %s OK\n", inFile);
   }
   return 0;
}

