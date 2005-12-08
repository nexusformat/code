/*
 * $Id$
 * 
 * Open GENIE bindings to NXtranslate DynamicRetriever class
 * implements functions required by dynamic_retriever.h
 *
 * Freddie Akeroyd, CCLRC ISIS Facility <F.A.Akeroyd@rl.ac.uk>
 *
 */
#include <string.h>
#include <stdlib.h>
#include "napi.h"
#include "genie_data_access.h"

// this is called when a DynamicRetriever instance is created
void* nxtinit(const char* source)
{
    char command[256];
    GX_activate_session("GENIE",1,1);
    if (getenv("NXGENIE_INIT") != NULL)
    {
	sprintf(command, "INCLUDE \"%s\"", getenv("NXGENIE_INIT"));
        GX_assign_handle(command, "");
    }
    return strdup(source); // this will be passed back as "ref" argument to 
                           // other functions. It can be anything e.g. a 
                           // file pointer or handle
}

// this is called when a DynamicRetriever instance is destroyed
int nxtcleanup(void* ref)
{
    free(ref);
    return 0;
}

typedef enum { gnone=0, gstring, ginteger, greal, gstringarray, grealarray, gintegerarray } g_export_type;
static const char* g_export_name[] = {
 "NONE", "STRING", "INT32", "FLOAT64", "STRING[]", "FLOAT64[]", "INT32[]" };
static const int g_export_size[] = {
    0, 1, 4, 8, 1, 8, 4 };
static const int g_nx_type[] = {
 NX_CHAR, NX_CHAR, NX_INT32, NX_FLOAT64, NX_UINT8, NX_FLOAT64, NX_INT32 };

// retrieves data from location specified in arg
// arg is a complete genie expression that evaluates to a data object
void* nxtgetdata(void* ref, const char* arg, int* data_type, int* dims_array, 
              int* ndims, int* free_data)
{
    char* command;
    void* result;
    char genie_type[20];
    int i, n, type_size;
    g_export_type gtype;
    *free_data = 1; // we want NXtranslate to call freedata() on "result" for us
// set input source
    GX_select_source((char*)ref);
// assign results to temporary variable so we can 
// gwt its type before transfer
    command = malloc(strlen(arg) + 20);
    sprintf(command, "__nxtmp <~ %s", arg);
    GX_assign_handle(command, "");
    free(command);
    type_size = sizeof(genie_type);
    GX_get_var_type("__nxtmp", genie_type, &type_size);
    gtype = gnone;
    switch(genie_type[0])
    {
	case 'S':  // "String"
	    gtype = gstring;
	    break;
	case 'I':  // "Integer"
	    gtype = ginteger;
	    break;
	case 'F':  // "Float"
	    gtype = greal;
	    break;
	default:
	    switch (genie_type[2])
	    {
		case 'S':  // "GXStringarray"
		    gtype = gstringarray;
		    break;
		case 'I':  // "GXIntegerarray"
		    gtype = gintegerarray;
		    break;
		case 'R':  // "GXRealarray"
		    gtype = grealarray;
		    break;
		default:
		    gtype = gnone;
		    break;
	    }
    }
    if (gtype == gnone)
    {
	GX_assign_handle("__nxtmp <~ \"<undefined>\"", "");
	gtype = gstring;
    }
// get size
    GX_transfer("__nxtmp", "-->", g_export_name[gtype], NULL, ndims, dims_array);
// allocate storage and get real values
    *data_type = g_nx_type[gtype];
    n = 1;
    for(i=0; i < *ndims; i++)
    {
	n *= dims_array[i];
    }
    result = malloc(n * g_export_size[gtype] + (gtype == gstring ? 1 : 0) );
    GX_transfer("__nxtmp", "-->", g_export_name[gtype], result, ndims, dims_array);
    if (gtype == gstring)
    {
        *((char*)result + n * g_export_size[gtype]) = '\0';
    }
    return result;
}

int nxtfreedata(void* ref, void* arg)
{
    free(arg);
    return 0;
}

