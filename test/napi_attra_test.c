/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Data Format
  
  Test program for attribute array C API 
  
  Copyright (C) 2014 NIAC
  
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
             
  For further information, see <http://www.nexusformat.org>

----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "napi.h"
#include "napiconfig.h"

static void print_data(const char *prefix, void *data, int type, int num);

int createAttrs(const NXhandle file)
{
	int array_dims[2] = { 5, 4 };
	int i = 5;

	float r4_array[5][4] =
	    { {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 10., 11., 12.}, {13., 14., 15., 16.},
		{17., 18., 19., 20.}
	};

	double r8_array[5][4] =
	    { {1., 2., 3., 4.}, {5., 6., 7., 8.}, {9., 10., 11., 12.}, {13., 14., 15., 16.},
		{17., 18., 19., 20.}
	};


	if (NXputattra(file, "attribute_1d", r4_array, 1, array_dims, NX_FLOAT32) != NX_OK)
		return 1;
	if (NXputattra (file, "attribute_2d", r8_array, 2, array_dims, NX_FLOAT64) != NX_OK)
		return 1;

	if (NXputattr(file, "old_style_int_attribute", &i, 1, NX_INT32) != NX_OK)
		return 1;
	if (NXputattr (file, "oldstylestrattr", "i:wq!<ESC><ESC>", strlen("i:wq!<ESC><ESC>"), NX_CHAR) != NX_OK)
		return 1;
	return 0;
}

int main(int argc, char *argv[])
{
	int i, NXrank, NXdims[32], NXtype, NXlen, entry_status, attr_status;
	float r;

	int i4_array[4] = { 1000000, 2000000, 3000000, 4000000 };

	char name[64], char_class[64], char_buffer[128];
	char group_name[64], class_name[64];
	NXhandle fileid;
	int nx_creation_code;
	char nxFile[80];
	char path[512];

	if (strstr(argv[0], "hdf4") != NULL) {
		nx_creation_code = NXACC_CREATE;
		strcpy(nxFile, "attra.h4");
	} else if (strstr(argv[0], "xml") != NULL) {
		nx_creation_code = NXACC_CREATEXML;
		strcpy(nxFile, "attra.xml");
	} else {
		nx_creation_code = NXACC_CREATE5;
		strcpy(nxFile, "attra.h5");
	}

/* make sure to test strings (we might not support vlen or only support that) and numbers */

	fprintf(stderr, "\nstarting attra napi test\n");
	fprintf(stderr, "creating file\n");

	if (NXopen(nxFile, nx_creation_code, &fileid) != NX_OK)
		return 1;
	NXsetnumberformat(fileid, NX_FLOAT32, "%9.3f");

/* create global attributes */
	fprintf(stderr, "creating global attributes\n");

	if (createAttrs(fileid) != NX_OK)
		return 1;

/* create group attributes */
	if (NXmakegroup(fileid, "entry", "NXentry") != NX_OK)
		return 1;
	if (NXopengroup(fileid, "entry", "NXentry") != NX_OK)
		return 1;

	fprintf(stderr, "creating group attributes\n");
	if (createAttrs(fileid) != NX_OK)
		return 1;

/* create dataset attributes */
	NXlen = 4;
	if (NXmakedata(fileid, "dataset", NX_INT32, 1, &NXlen) != NX_OK)
		return 1;
	if (NXopendata(fileid, "dataset") != NX_OK)
		return 1;
	if (NXputdata(fileid, i4_array) != NX_OK)
		return 1;

	fprintf(stderr, "creating dataset attributes\n");
	if (createAttrs(fileid) != NX_OK)
		return 1;

	if (NXclosedata(fileid) != NX_OK)
		return 1;

/* interate over attributes */
	fprintf(stderr, "iterating over attributes\n");

	if (NXgetattrinfo(fileid, &i) != NX_OK)
		return 1;
	if (i > 0) {
		fprintf(stderr, "      Number of attributes : %d\n", i);
	}
	do {
		attr_status = NXgetnextattr(fileid, name, NXdims, &NXtype);
		if (attr_status == NX_ERROR)
			return 1;
		if (attr_status == NX_OK) {
			if (NXgetattrainfo
			    (fileid, name, &NXrank, NXdims, &NXtype) != NX_OK)
				return 1;	/* hehe */
			fprintf(stderr, " found attribute named %s of rank %d and first dimension %d - hooray!\n",
			     name, NXrank, NXdims[0]);
			switch (NXtype) {
			case NX_INT32:
				NXlen = 1;
				if (NXgetattr
				    (fileid,
				     name, &i, &NXlen, &NXtype) != NX_OK)
					return 1;
				fprintf(stderr, "         %s : %d\n", name, i);
				break;
			case NX_FLOAT32:
				NXlen = 1;
				if (NXgetattr
				    (fileid,
				     name, &r, &NXlen, &NXtype) != NX_OK)
					return 1;
				fprintf(stderr, "         %s : %f\n", name, r);
				break;
			case NX_CHAR:
				NXlen = sizeof(char_buffer);
				if (NXgetattr
				    (fileid,
				     name,
				     char_buffer, &NXlen, &NXtype) != NX_OK)
					return 1;
				fprintf(stderr, "         %s : %s\n", name, char_buffer);
				break;
			}
		}
	} while (attr_status == NX_OK);

/*
	if (NXfree((void **)&data_buffer) != NX_OK)
		return 1;
*/

/* find attributes by name */
	fprintf(stderr, "accessing attributes by name\n");

/* make sure old api fails correctly */
	fprintf(stderr, "checking for old api interoperability\n");

	if (NXclosegroup(fileid) != NX_OK)
		return 1;

	if (NXclose(&fileid) != NX_OK)
		return 1;

	/*
	   read test
	 */
	if (NXopen(nxFile, NXACC_RDWR, &fileid) != NX_OK)
		return 1;

	NXgetattrinfo(fileid, &i);
	if (i > 0) {
		fprintf(stderr, "Number of global attributes: %d\n", i);
	}
	do {
		attr_status = NXgetnextattr(fileid, name, NXdims, &NXtype);
		if (attr_status == NX_ERROR)
			return 1;
		if (attr_status == NX_OK) {
			switch (NXtype) {
			case NX_CHAR:
				NXlen = sizeof(char_buffer);
				if (NXgetattr
				    (fileid, name, char_buffer, &NXlen, &NXtype)
				    != NX_OK)
					return 1;
				if (strcmp(name, "file_time") &&
				    strcmp(name, "HDF_version") &&
				    strcmp(name, "HDF5_Version") &&
				    strcmp(name, "XML_version")) {
					fprintf(stderr, "   %s = %s\n", name,
					       char_buffer);
				}
				break;
			}
		}
	} while (attr_status == NX_OK);
	if (NXopengroup(fileid, "entry", "NXentry") != NX_OK)
		return 1;
	NXgetattrinfo(fileid, &i);
	fprintf(stderr, "Number of group attributes: %d\n", i);
	if (NXgetpath(fileid, path, 512) != NX_OK)
		return 1;
	fprintf(stderr, "NXentry path %s\n", path);
	do {
		attr_status = NXgetnextattr(fileid, name, NXdims, &NXtype);
		if (attr_status == NX_ERROR)
			return 1;
		if (attr_status == NX_OK) {
			switch (NXtype) {
			case NX_CHAR:
				NXlen = sizeof(char_buffer);
				if (NXgetattr
				    (fileid, name, char_buffer, &NXlen, &NXtype)
				    != NX_OK)
					return 1;
				fprintf(stderr, "   %s = %s\n", name, char_buffer);
			}
		}
	} while (attr_status == NX_OK);

	/*
	   tests for NXopenpath
	 */
	if (NXopenpath(fileid, "/entry/dataset") != NX_OK) {
		fprintf(stderr, "Failure on NXopenpath\n");
		return 1;
	}
	if (NXopenpath(fileid, "/entry") != NX_OK) {
		fprintf(stderr, "Failure on NXopenpath\n");
		return 1;
	}

	if (NXclose(&fileid) != NX_OK)
		return 1;

	fprintf(stderr, "we reached the end - this looks good\n");
	return 0;
}

/*----------------------------------------------------------------------*/
static void print_data(const char *prefix, void *data, int type, int num)
{
	int i;
	fprintf(stderr, "%s", prefix);
	for (i = 0; i < num; i++) {
		switch (type) {
		case NX_CHAR:
			fprintf(stderr, "%c", ((char *)data)[i]);
			break;

		case NX_INT8:
			fprintf(stderr, " %d", ((unsigned char *)data)[i]);
			break;

		case NX_INT16:
			fprintf(stderr, " %d", ((short *)data)[i]);
			break;

		case NX_INT32:
			fprintf(stderr, " %d", ((int *)data)[i]);
			break;

		case NX_INT64:
			fprintf(stderr, " %lld", (long long)((int64_t *) data)[i]);
			break;

		case NX_UINT64:
			fprintf(stderr, " %llu",
			       (unsigned long long)((uint64_t *) data)[i]);
			break;

		case NX_FLOAT32:
			fprintf(stderr, " %f", ((float *)data)[i]);
			break;

		case NX_FLOAT64:
			fprintf(stderr, " %f", ((double *)data)[i]);
			break;

		default:
			fprintf(stderr, "print_data: invalid type");
			break;
		}
	}
	fprintf(stderr, "\n");
}
