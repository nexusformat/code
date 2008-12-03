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
#include "napi.h"
#include "napiconfig.h"
#include "nxconvert_common.h"
#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif
#if HAVE_MKSTEMP
#else
static int mkstemp(char* template)
{
    if (mktemp(template) != NULL)
    {
	return open(template, O_RDWR | O_CREAT, 0600);
    }
    else
    {
	return -1;
    }
}
#endif

#ifdef _WIN32
#define NULL_DEVICE "NUL"
#define DIR_SEPARATOR "\\"
#define TMP_DIR getenv("TEMP")
#else
#define NULL_DEVICE "/dev/null"
#define DIR_SEPARATOR "/"
#define TMP_DIR P_tmpdir
#endif /* _WIN32 */

static const char* definition_name = "BASE";
static const char* definition_version = "3.0";

static void print_usage()
{
    printf("Usage: nxvalidate [-w] [ -d definition ] [ infile ]\n\n");
    printf("-w    Send file to NeXus web site (using wget) for validation\n");
    printf("      (default: try to run \"xmllint\" program locally\n");
    printf("-d    Use specified definiton (default: %s)\n", definition_name);
    printf("-k    keep temporary files\n");
    printf("-q    quiet (only report errors)\n");
    printf("-v    specify schema version (default: %s)\n", definition_version);
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


static int quiet = 0;

int main(int argc, char *argv[])
{
   char inFile[256], outFile[256], command[512], outFile2[256], *strPtr;
   const char* cStrPtr;
   int ret, opt, c, i, fd, use_web = 0;
   FILE *fIn, *fOut2;
   int keep_temps = 0;

   while( (opt = getopt(argc, argv, "wqkhd:v:")) != -1 )
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

	  case 'v':
	    if (optarg != NULL && *optarg != '\0')
	    {
	        definition_version = optarg;
	    }
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

   sprintf(outFile, "%s%s%s.XXXXXX", TMP_DIR, DIR_SEPARATOR, inFile);
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
       sprintf(command, "xmllint --noout --schema http://definition.nexusformat.org/schema/%s/%s.xsd \"%s\" %s", definition_version, definition_name, outFile, (quiet ? "> " NULL_DEVICE " 2>&1" : ""));
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
   sprintf(outFile2, "%s%s%s.XXXXXX", TMP_DIR, DIR_SEPARATOR, inFile);
   if ( (fd = mkstemp(outFile2)) == -1 )
   {
	printf("error\n");
	NXVALIDATE_ERROR_EXIT;
   }
   close(fd);
   fOut2 = fopen(outFile2, "wt");
   fIn = fopen(outFile, "rt");
   fprintf(fOut2, "file_name=");
   for(cStrPtr = inFile; *cStrPtr != '\0'; ++cStrPtr)
   {
	url_encode(*cStrPtr, fOut2);
   }
   fprintf(fOut2, "&definition_name=");
   for(cStrPtr = definition_name; *cStrPtr != '\0'; ++cStrPtr)
   {
	url_encode(*cStrPtr, fOut2);
   }
   fprintf(fOut2, "&definition_version=");
   for(cStrPtr = definition_version; *cStrPtr != '\0'; ++cStrPtr)
   {
	url_encode(*cStrPtr, fOut2);
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
   sprintf(command, "wget --quiet -O %s --post-file=\"%s\" http://definition.nexusformat.org/dovalidate/run", (quiet ? NULL_DEVICE : "-"), outFile2);
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

