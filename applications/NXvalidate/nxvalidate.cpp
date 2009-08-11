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

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include "napi.h"
#include "napiconfig.h"
#include "nxconvert_common.h"
#include <vector>
#include <string>
#include <sstream>
#include "tclap/CmdLine.h"
using namespace TCLAP;

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

static const char* definition_name = NEXUS_SCHEMA_BASE;
static const char* definition_version = NEXUS_SCHEMA_VERSION;

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

static int convertNXS(const std::string& nxsfile_in, std::string& nxsfile_out) 
{
   const char *inFile = nxsfile_in.c_str();
   char outFile[256];
   int fd;

   sprintf(outFile, "%s%s%s.XXXXXX", TMP_DIR, DIR_SEPARATOR, inFile);
   if ( (fd = mkstemp(outFile)) == -1 )
   {
     std::cerr << "error making temporary directory\n" << std::endl;
        return 1;
   }
   close(fd);
   if (!quiet) {
     std::cout << "* Writing tempfile " << outFile << std::endl;
   }
   nxsfile_out = outFile;
   if (convert_file(NX_DEFINITION, inFile, NXACC_READ, outFile,
                    NXACC_CREATEXML) != NX_OK)
   {
     std::cerr << "* Error converting file " << nxsfile_in
          << " to definiton XML format";
     return 1;
   }
   return 0;
}

static int validate(const std::string& nxsfile, const char* definition_name, const char* definition_version, const int keep_temps, int use_web) 
{
   char command[512], outFile2[512], *strPtr;
   const char* cStrPtr;
   int ret, opt, c, i, fd;
   FILE *fIn, *fOut2;

   if (!quiet) {
     std::cout << "* Validating " << nxsfile << " using definition "
          << definition_name << ".xsd" << std::endl;
   }
   std::string nxsfile_xml;
   if (convertNXS(nxsfile, nxsfile_xml) != 0) {
     return 1;
   }

   if (use_web == 0)
   {
       sprintf(command, "xmllint --noout --schema http://definition.nexusformat.org/schema/%s/%s.xsd \"%s\" %s", definition_version, definition_name, nxsfile_xml.c_str(), (quiet ? "> " NULL_DEVICE " 2>&1" : ""));
       if (!quiet) {
         std::cout << "* Validating using locally installed \"xmllint\" program"
              << std::endl;
         std::cout << command << std::endl;
       }
       ret = system(command);
       if (ret != -1 && WIFEXITED(ret))
       {
	    if (!keep_temps)
	    {
		remove(nxsfile_xml.c_str());
	    }
	    if (WEXITSTATUS(ret) != 0)
	    {
              std::cout << "* Validation with \"xmllint\" of " << nxsfile
                   << " failed" << std::endl;
              std::cout << "* If the program was unable to find all the required "
                   << "schema from the web, check your \"http_proxy\" "
                   << "environment variable" << std::endl;
              NXVALIDATE_ERROR_EXIT;
	    }
	    else
	    {
	        if (!quiet) {
                  std::cout << "* Validation with \"xmllint\" of " << nxsfile 
                       << " OK" << std::endl;
                }
	        return 0;
	    }
       }
       std::cout << "* Unable to find \"xmllint\" to validate file - will try "
            << "web validation" << std::endl;
       std::cout << "* \"xmllint\" is installed as part of libxml2 from "
            << "xmlsoft.org" << std::endl;
       NXVALIDATE_ERROR_EXIT;
   }
   sprintf(outFile2, "%s%s%s.XXXXXX", TMP_DIR, DIR_SEPARATOR, nxsfile.c_str());
   if ( (fd = mkstemp(outFile2)) == -1 )
   {
     std::cerr << "Failed to make the temporary file " << outFile2 << std::endl;
     NXVALIDATE_ERROR_EXIT;
   }
   close(fd);
   fOut2 = fopen(outFile2, "wt");
   fIn = fopen(nxsfile_xml.c_str(), "rt");
   fprintf(fOut2, "file_name=");
   for(cStrPtr = nxsfile_xml.c_str(); *cStrPtr != '\0'; ++cStrPtr)
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
     std::cout << "* Validating via http://definition.nexusformat.org using "
          << "\"wget\"" << std::endl;
   }
   sprintf(command, "wget --quiet -O %s --post-file=\"%s\" http://definition.nexusformat.org/dovalidate/run", (quiet ? NULL_DEVICE : "-"), outFile2);
   ret = system(command);
   if (!keep_temps)
   {
	remove(nxsfile_xml.c_str());
	remove(outFile2);
   }
   if (ret == -1)
   {
        std::cout << "* Unable to find \"wget\" to validate the file " << nxsfile
          << " over the web" << std::endl;
	NXVALIDATE_ERROR_EXIT;
   }

   if (WIFEXITED(ret) && (WEXITSTATUS(ret) != 0))
   {
        std::cerr << "* Validation via http://definition.nexusformat.org/ of "
          << nxsfile << " failed" << std::endl;
        std::cerr << "* If \"wget\" was unable to load the schema files from the "
          << "web, check your \"http_proxy\" environment variable" << std::endl;
	NXVALIDATE_ERROR_EXIT;
   }
   else if (!quiet)
   {
        std::cout << "* Validation via http://definition.nexusformat.org/ of "
          << nxsfile << " OK" << std::endl;
   }
   return 0;
}

int main(int argc, char *argv[])
{
   char inFile[256];
   char *strPtr;
   int use_web = 0;
   int keep_temps = 0;

   // set up the command line arguments
   CmdLine cmd("Validate a NeXus file", ' ', "1.1.0");
   std::ostringstream temp;
   temp << "Use specified definiton (default: " << definition_name << ")";
   ValueArg<std::string> definition_name_arg("d", "def", temp.str(), false, 
                                        definition_name, "definition", cmd);
   temp.str("");
   temp << "specify schema version (default: " << definition_version << ")";
   ValueArg<std::string> definition_version_arg("", "defver", temp.str(), false, 
                                           definition_version, "version", cmd);
   SwitchArg quiet_arg("q", "quiet", "Turn on quiet mode(only report errors)",
                       false, cmd);
   SwitchArg keep_arg("k", "keep", "Keep temporary files",
                       false, cmd);
   temp.str("");
   temp << "Send file to NeXus web site (using wget) for validation "
        << "(default: try to run \"xmllint\" program locally";
   SwitchArg send_arg("w", "web", temp.str(), false, cmd);
   std::string sinfile;
   UnlabeledMultiArg<std::string> infiles_arg("filename",
                                         "Name of a file to be viewed",
                                         "filename",cmd);

   // parse the command line and turn it into variables
   cmd.parse(argc, argv);
   definition_name = definition_name_arg.getValue().c_str();
   if (quiet_arg.getValue()) {
     quiet = 1;
   }
   if (send_arg.getValue()) {
     use_web = 1;
   }
   if (keep_arg.getValue()) {
     keep_temps = 1;
   }
   definition_version = definition_version_arg.getValue().c_str();
   std::vector<std::string> infiles = infiles_arg.getValue();
   if (infiles.empty()) {
      printf ("Give name of input NeXus file : ");
      fgets (inFile, sizeof(inFile), stdin);
      if ((strPtr = strchr(inFile, '\n')) != NULL) { *strPtr = '\0'; }
      infiles.push_back(std::string(inFile));
   }

   // do the work
   int result = 0;
   for (size_t i = 0; i < infiles.size(); i++) {
     result += validate(infiles[i], definition_name, definition_version,
                        keep_temps, use_web);
   }
   return result;
}
