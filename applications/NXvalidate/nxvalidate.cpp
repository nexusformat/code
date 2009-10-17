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
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "napi.h"
#include "napiconfig.h"
#include "nxconvert_common.h"
#include "tclap/CmdLine.h"

using namespace TCLAP;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

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

struct Definition {
  string name;
  string version;
  string nxdl_path;
};

struct NeXus {
  string raw;
  string reduced;
};

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

#define NXVALIDATE_ERROR_EXIT	exit(EXIT_FAILURE)

static const string DEFAULT_NXDL_PATH(".");

static int quiet = 0;

static int convertNXS(NeXus &nexus, Definition &definition) 
{
   const char *inFile = nexus.raw.c_str();
   char outFile[256];
   int fd;

   sprintf(outFile, "%s%s%s.XXXXXX", TMP_DIR, DIR_SEPARATOR, inFile);
   if ( (fd = mkstemp(outFile)) == -1 )
   {
     cerr << "error making temporary directory\n" << endl;
        return 1;
   }
   close(fd);
   if (!quiet) {
     cout << "* Writing tempfile " << outFile << endl;
   }
   nexus.reduced = outFile;
   if (convert_file(NX_DEFINITION, inFile, NXACC_READ, outFile,
                    NXACC_CREATEXML, definition.name.c_str()) != NX_OK)
   {
     cerr << "* Error converting file " << nexus.raw
          << " to definiton XML format";
     return 1;
   }
   return 0;
}

static int validate(const string& nxsfile, Definition &definition, 
		    const int keep_temps, int use_web) 
{
  NeXus nexus;
  nexus.raw = nxsfile;

   if (!quiet) {
     cout << "* Validating " << nexus.raw << " using definition "
          << (!definition.name.empty() ? definition.name : "<default>")
	  << " for all NXentry" << endl;
   }
   if (convertNXS(nexus, definition) != 0) {
     return 1;
   }

   char command[512], outFile2[512], *strPtr;
   const char* cStrPtr;
   int ret, opt, c, i, fd;
   FILE *fIn, *fOut2;

   string extra_xmllint_args;
   extra_xmllint_args.append(" --nowarning");
 
   // check without the web
   if (use_web == 0)
   {
       sprintf(command, "xmllint %s --noout --path \"%s\" --schema \"%s.xsd\" \"%s\" %s", 
	       extra_xmllint_args.c_str(), definition.nxdl_path, NEXUS_SCHEMA_BASE, nexus.reduced.c_str(), (quiet ? "> " NULL_DEVICE " 2>&1" : ""));
       if (!quiet) {
         cout << "* Validating using locally installed \"xmllint\" program"
              << endl;
         cout << command << endl;
       }
       ret = system(command);
       if (ret != -1 && WIFEXITED(ret))
       {
	    if (!keep_temps)
	    {
		remove(nexus.reduced.c_str());
	    }
	    if (WEXITSTATUS(ret) != 0)
	    {
              cout << "* Validation with \"xmllint\" of " << nexus.raw
                   << " failed" << endl;
              cout << "* If the program was unable to find all the required "
                   << "schema from the web, check your \"http_proxy\" "
                   << "environment variable" << endl;
              NXVALIDATE_ERROR_EXIT;
	    }
	    else
	    {
	        if (!quiet) {
                  cout << "* Validation with \"xmllint\" of " << nexus.raw
                       << " OK" << endl;
                }
	        return 0;
	    }
       }
       cout << "* Unable to find \"xmllint\" to validate file - will try "
            << "web validation" << endl;
       cout << "* \"xmllint\" is installed as part of libxml2 from "
            << "xmlsoft.org" << endl;
       NXVALIDATE_ERROR_EXIT;
   } // this section returns whether or not it works

   // set up post request for post request
   sprintf(outFile2, "%s%s%s.XXXXXX", TMP_DIR, DIR_SEPARATOR, nexus.raw.c_str());
   if ( (fd = mkstemp(outFile2)) == -1 )
   {
     cerr << "Failed to make the temporary file " << outFile2 << endl;
     NXVALIDATE_ERROR_EXIT;
   }
   close(fd);
   fOut2 = fopen(outFile2, "wt");
   fIn = fopen(nexus.reduced.c_str(), "rt");
   fprintf(fOut2, "file_name=");
   for(cStrPtr = nexus.reduced.c_str(); *cStrPtr != '\0'; ++cStrPtr)
   {
	url_encode(*cStrPtr, fOut2);
   }
   fprintf(fOut2, "&definition_name=");
   for(cStrPtr = definition.name.c_str(); cStrPtr != NULL && *cStrPtr != '\0'; ++cStrPtr)
   {
	url_encode(*cStrPtr, fOut2);
   }
   fprintf(fOut2, "&definition_version=");
   for(cStrPtr = definition.version.c_str(); *cStrPtr != '\0'; ++cStrPtr)
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

   // post the validation information to the web
   sprintf(command, "wget --quiet -O %s --post-file=\"%s\" http://definition.nexusformat.org/dovalidate/run", (quiet ? NULL_DEVICE : "-"), outFile2);
   if (!quiet) {
     cout << "* Validating via http://definition.nexusformat.org using "
          << "\"wget\"" << endl;
     cout << command << endl;
   }
   ret = system(command);
   if (!keep_temps)
   {
	remove(nexus.reduced.c_str());
	remove(outFile2);
   }
   if (ret == -1)
   {
        cout << "* Unable to find \"wget\" to validate the file " << nexus.raw
          << " over the web" << endl;
	NXVALIDATE_ERROR_EXIT;
   }

   // cleanup and return code
   if (WIFEXITED(ret) && (WEXITSTATUS(ret) != 0))
   {
        cerr << "* Validation via http://definition.nexusformat.org/ of "
          << nexus.raw << " failed" << endl;
        cerr << "* If \"wget\" was unable to load the schema files from the "
          << "web, check your \"http_proxy\" environment variable" << endl;
	NXVALIDATE_ERROR_EXIT;
   }
   else if (!quiet)
   {
        cout << "* Validation via http://definition.nexusformat.org/ of "
          << nexus.raw << " OK" << endl;
   }
   return 0;
}

int main(int argc, char *argv[])
{
   char *strPtr;
   int use_web = 0;
   int keep_temps = 0;
   int ignore_definition = 0;

   // set up the command line arguments
   CmdLine cmd("Validate a NeXus file", ' ', "1.1.0");

   std::ostringstream temp;
   temp << "Use specified definiton for NXentry (default: read from <definition> field in NXentry )";
   ValueArg<string> definition_name_arg("d", "def", temp.str(), false, 
                                        "", "definition", cmd);
   temp.str("");
   temp << "specify schema version (default: " << NEXUS_SCHEMA_VERSION << ")";
   ValueArg<string> definition_version_arg("", "defver", temp.str(), false, 
                                          NEXUS_SCHEMA_VERSION, "version", cmd);

   SwitchArg quiet_arg("q", "quiet", "Turn on quiet mode (only report errors)",
                       false, cmd);
   SwitchArg keep_arg("k", "keep", "Keep temporary files",
                       false, cmd);
   SwitchArg nodef_arg("n", "nodef", "Ignore <definition> field in NXentry",
                       false, cmd);

   temp.str("");
   temp << "Specify path to additional schema files (default: "
	<< DEFAULT_NXDL_PATH << ")";
   ValueArg<string> schema_path_arg("p", "path", temp.str(), false, 
				    DEFAULT_NXDL_PATH, "path", cmd);

   temp.str("");
   temp << "Send file to NeXus web site (using wget) for validation "
        << "(default: try to run \"xmllint\" program locally";
   SwitchArg send_arg("w", "web", temp.str(), false, cmd);
   
   string sinfile;
   UnlabeledMultiArg<string> infiles_arg("filename",
                                         "Name of a file to be viewed",
                                         "filename",cmd);

   // parse the command line and turn it into variables
   cmd.parse(argc, argv);
   if (quiet_arg.getValue()) {
     quiet = 1;
   }
   if (send_arg.getValue()) {
     use_web = 1;
   }
   if (keep_arg.getValue()) {
     keep_temps = 1;
   }
   if (nodef_arg.getValue()) {
     ignore_definition = 1;
   }
   Definition definition;
   if (ignore_definition) {
     definition.name = "NXentry";
   }
   definition.version = definition_version_arg.getValue();
   definition.nxdl_path = schema_path_arg.getValue();
   vector<string> infiles = infiles_arg.getValue();
   if (infiles.empty()) {
     cerr << "Must supply at least one data file" << endl;
     NXVALIDATE_ERROR_EXIT;
   }

   // do the work
   int result = 0;
   for (size_t i = 0; i < infiles.size(); i++) {
     result += validate(infiles[i], definition, keep_temps, use_web);
   }
   return result;
}
