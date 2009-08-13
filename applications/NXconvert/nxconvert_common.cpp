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
 
 $Id: nxconvert.c 991 2008-03-19 19:30:03Z Freddie Akeroyd $
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "napi.h"
#include "NeXusFile.hpp"
#include "NeXusStream.hpp"
#include "nxconvert_common.h"

static int WriteGroup (int is_definition);
static int WriteAttributes (int is_definition);

#define MAX_LINKS 1024
struct link_to_make
{
    char from[1024];   /* path of directory with link */
    char name[256];    /* name of link */
    char to[1024];     /* path of real item */
};

static struct link_to_make links_to_make[MAX_LINKS];
static int links_count = 0;
static char current_path[1024];

static int add_path(const char* path)
{
    int i;
    i = strlen(current_path);
    sprintf(current_path + i, "/%s", path);
    return 0;
}

static int remove_path(const char* path)
{
    char *tstr; 
    tstr = strrchr(current_path, '/');
    if (tstr != NULL && !strcmp(path, tstr+1))
    {
	*tstr = '\0';
    }
    else
    {
	printf("path error\n");
    }
    return 0;
}

static NXhandle inId, outId;
static const char* definition_name = NULL;

int convert_file(int nx_format, const char* inFile, int nx_read_access, const char* outFile, int nx_write_access, const char* definition_name_)
{
   int i, nx_is_definition = 0;
   definition_name = definition_name_;
   char* tstr;
   links_count = 0;
   current_path[0] = '\0';
   NXlink link;
   if (nx_format == NX_DEFINITION)
   {
	nx_is_definition = 1;
	putenv("NX_IS_DEFINITION=1");
   }
/* Open NeXus input file and NeXus output file */
   if (NXopen (inFile, nx_read_access, &inId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", inFile);
      return NX_ERROR;
   }

   if (NXopen (outFile, nx_write_access, &outId) != NX_OK) {
      printf ("NX_ERROR: Can't open %s\n", outFile);
      return NX_ERROR;
   }

/* Output global attributes */
   if (WriteAttributes (nx_is_definition) != NX_OK)
   {
	return NX_ERROR;
   }
/* Recursively cycle through the groups printing the contents */
   if (WriteGroup (nx_is_definition) != NX_OK)
   {
	return NX_ERROR;
   }
/* close input */
   if (NXclose (&inId) != NX_OK)
   {
	return NX_ERROR;
   }
   if (nx_format != NXACC_CREATE && nx_format != NXACC_CREATE4)
   {
/* now create any required links */
       for(i=0; i<links_count; i++)
       {
	    if (NXopenpath(outId, links_to_make[i].to) != NX_OK) return NX_ERROR;
	    if (NXgetdataID(outId, &link) == NX_OK  || NXgetgroupID(outId, &link) == NX_OK)
	    {	
	        if (NXopenpath(outId, links_to_make[i].from) != NX_OK) return NX_ERROR;
	        tstr = strrchr(links_to_make[i].to, '/');
	        if (!strcmp(links_to_make[i].name, tstr+1))
	        {
	            if (NXmakelink(outId, &link) != NX_OK) return NX_ERROR;
	        }
	        else
	        {
	            if (NXmakenamedlink(outId, links_to_make[i].name, &link) != NX_OK) return NX_ERROR;
	        }
	    }
	    else
	    {
	        return NX_ERROR;
	    }
	}
   }
/* Close the input and output files */
   if (NXclose (&outId) != NX_OK)
   {
	return NX_ERROR;
   }
   return NX_OK;
}

/* Prints the contents of each group as XML tags and values */
static int WriteGroup (int is_definition)
{ 
   int i;
   int status, dataType, dataRank, dataDimensions[NX_MAXRANK], dataLen;     
   NXname name, nxclass;
   void *dataBuffer;
   NXlink link;
   std::string definition;
   using namespace NeXus;
   using namespace NeXus::Stream;
   File nfile_in(inId), nfile_out(outId);

   do {
      status = NXgetnextentry (inId, name, nxclass, &dataType);
      if (status == NX_ERROR) return NX_ERROR;
      if (status == NX_OK) {
         if (!strncmp(nxclass,"NX",2)) {
            if (NXopengroup (inId, name, nxclass) != NX_OK) return NX_ERROR;
	    add_path(name);
	    if (NXgetgroupID(inId, &link) != NX_OK) return NX_ERROR;
	    if (!strcmp(current_path, link.targetPath))
	    {
                if (NXmakegroup (outId, name, nxclass) != NX_OK) return NX_ERROR;
                if (NXopengroup (outId, name, nxclass) != NX_OK) return NX_ERROR;
                if (WriteAttributes (is_definition) != NX_OK) return NX_ERROR;
		if (is_definition && !strcmp(nxclass, "NXentry"))
		{
		    if (definition_name != NULL)
		    {
		        nfile_out.putAttr("xsi:type", definition_name);
		    }
  		    else
		    {
		        try {
			    nfile_in.openData("definition");
		            definition = nfile_in.getStrData();
		            nfile_in.closeData();
		            nfile_out.putAttr("xsi:type", definition);
		        }
		        catch(std::exception& ex)
		        {
			    ; // definition not found
                        }
		    }
		}
                if (WriteGroup (is_definition) != NX_OK) return NX_ERROR;
	        remove_path(name);
	    }
	    else
	    {
	        remove_path(name);
		strcpy(links_to_make[links_count].from, current_path); 
		strcpy(links_to_make[links_count].to, link.targetPath); 
		strcpy(links_to_make[links_count].name, name); 
		links_count++;
         	if (NXclosegroup (inId) != NX_OK) return NX_ERROR;
	    }
         }
         else if (!strncmp(nxclass,"SDS",3)) {
	    add_path(name);
            if (NXopendata (inId, name) != NX_OK) return NX_ERROR;
	    if (NXgetdataID(inId, &link) != NX_OK) return NX_ERROR;
	    if (!strcmp(current_path, link.targetPath))
	    {
                if (NXgetinfo (inId, &dataRank, dataDimensions, &dataType) != NX_OK) return NX_ERROR;
                if (NXmalloc (&dataBuffer, dataRank, dataDimensions, dataType) != NX_OK) return NX_ERROR;
                if (NXgetdata (inId, dataBuffer)  != NX_OK) return NX_ERROR;
                if (NXmakedata (outId, name, dataType, dataRank, dataDimensions) != NX_OK) return NX_ERROR;
                if (NXopendata (outId, name) != NX_OK) return NX_ERROR;
                if (WriteAttributes (is_definition) != NX_OK) return NX_ERROR;
                if (NXputdata (outId, dataBuffer) != NX_OK) return NX_ERROR;
                if (NXfree((void**)&dataBuffer) != NX_OK) return NX_ERROR;
                if (NXclosedata (outId) != NX_OK) return NX_ERROR;
	        remove_path(name);
	    }
	    else
	    {
	        remove_path(name);
		strcpy(links_to_make[links_count].from, current_path); 
		strcpy(links_to_make[links_count].to, link.targetPath); 
		strcpy(links_to_make[links_count].name, name); 
		links_count++;
	    }
            if (NXclosedata (inId) != NX_OK) return NX_ERROR;
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

static int WriteAttributes (int is_definition)
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
            if (NXputattr (outId, attrName, attrBuffer, attrLen , attrType) != NX_OK) return NX_ERROR;
            if (NXfree((void**)&attrBuffer) != NX_OK) return NX_ERROR;
         }
         i++;
      }
   } while (status != NX_EOD);
   return NX_OK;
}

