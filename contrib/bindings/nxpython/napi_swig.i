%module nexus

%{

/*extern NexusFunction; 
extern pNexusFunction;*/

#include "napi.h"
%}

%include typemaps.i
%include cpointer.i
%include cstring.i

%include napi_typemaps.i

%pointer_functions(NXhandle, pNXhandle)
%pointer_functions(NXlink, pNXlink)

%cstring_bounded_output(NXname name, 128);
%cstring_bounded_output(NXname nxclass, 128);


/* SPECIFIC CUSTOMIZED TYPEMAPS */

/* NXopen */
%apply NXhandle* pHandle {NXhandle* pHandle};

/* NXclose, NXflush */
%apply NXhandle* ppHandle {NXhandle* ppHandle};

/* NXopendata, NXcompmakedata */
%apply int dim[]  {int dim[]};
%apply int bufsize[]  {int bufsize[]};


/* NXgetdata */ 
%apply void* data_out {void* data_out};

/* NXgetslab */ 
%apply void *data_out_slab {void* data_out_slab}

/* NXgetattr */
%apply void* data_out_att {void* data_out_att};
%apply int* iDataLen {int* iDataLen};
%apply int * iType { int * iType};

/* NXputdata */
%apply void* data_in_put {void* data_in_put};

/* NXputattr */
%apply void* data_in_att {void* data_in_att};
%apply int iDataLen {int iDataLen};

/* NXgetslab, NXputslab */
%apply int SlabIn[]  {int slab_start[], int slab_size[]};


/* NXgetinfo */
%apply int dimension[] {int dimension[]};


/* NXgetdataID, NXgetgroupID */
%apply NXlink* outLink {NXlink* outLink};

/* NXmakelink */
%apply NXlink* pLink {NXlink* pLink};

/* all expect NXopen */ 
%apply NXhandle handle {NXhandle handle};



#define NEXUS_VERSION   "2.0.0."               
typedef void* NXhandle;         
typedef int NXstatus;
typedef char NXname[128];

typedef enum {NXACC_READ=1, NXACC_RDWR=2, NXACC_CREATE=3, NXACC_CREATE4=4, NXACC_CREATE5=5} NXaccess;

typedef struct {
	char *iname;
	int   type;
}info_type, *pinfo;  
 
#define NX_OK 1
#define NX_ERROR 0
#define NX_EOD -1

#define NX_UNLIMITED -1

#define NX_MAXRANK 32
#define NX_MAXNAMELEN 64


#define NX_FLOAT32   5
#define NX_FLOAT64   6
#define NX_INT8     20  
#define NX_UINT8    21
#define NX_INT16    22  
#define NX_UINT16   23
#define NX_INT32    24
#define NX_UINT32   25
#define NX_CHAR      4

#define NX_COMP_NONE 100
#define NX_COMP_LZW 200
#define NX_COMP_RLE 300
#define NX_COMP_HUF 400  


#ifdef HDF4
#include <mfhdf.h>
#endif 

#ifdef HDF5
#include <hdf5.h>
#endif                

  
#ifdef HDF4
typedef struct {
                int32 iTag;          
                int32 iRef;          
               } NXlink;
#endif

#ifdef HDF5
typedef struct {
                char iTag5[1024];      
                char iRef5[1024];     
                char iRefd[1024];     
               } NXlink;
#endif                


#define NXMAXSTACK 50


NXstatus NXopen(char* filename, NXaccess access_method, NXhandle* pHandle);
NXstatus NXclose(NXhandle* ppHandle);

NXstatus NXmakegroup (NXhandle handle, char* name, char* NXclass);
NXstatus NXopengroup (NXhandle handle, char* name, char* NXclass);
NXstatus NXclosegroup(NXhandle handle);

NXstatus NXmakedata (NXhandle handle, char* label, int datatype, int rank, int dim[]);
NXstatus NXcompmakedata (NXhandle handle, char* label, int datatype, int rank, int dim[], int comp_typ, int bufsize[]);
NXstatus NXopendata (NXhandle handle, char* label);
NXstatus NXclosedata(NXhandle handle);



NXstatus NXgetdata(NXhandle handle, void* data_out);
NXstatus NXgetslab(NXhandle handle, void* data_out_slab, int slab_start[], int slab_size[]);
NXstatus NXgetattr(NXhandle handle, char* name, void* data_out_att, int* iDataLen, int* iType);

NXstatus NXputdata(NXhandle handle, void* data_in_put);
NXstatus NXputslab(NXhandle handle, void* data_in_put, int slab_start[], int slab_size[]);    
NXstatus NXputattr(NXhandle handle, char* name, void* data_in_att, int iDataLen, int iType);

NXstatus NXflush(NXhandle* ppHandle);



NXstatus NXgetinfo(NXhandle handle, int* OUTPUT, int dimension[], int* OUTPUT);
NXstatus NXgetgroupinfo(NXhandle handle, int* OUTPUT, NXname name, NXname nxclass);
NXstatus NXinitgroupdir(NXhandle handle);
NXstatus NXgetnextentry(NXhandle handle, NXname name, NXname nxclass, int* OUTPUT);
NXstatus NXgetattrinfo(NXhandle handle, int* OUTPUT);
NXstatus NXinitattrdir(NXhandle handle);
NXstatus NXgetnextattr(NXhandle handle, NXname name, int *OUTPUT, int *OUTPUT);




NXstatus NXgetgroupID(NXhandle handle, NXlink* outLink);
NXstatus NXgetdataID(NXhandle handle, NXlink* outLink);
NXstatus NXmakelink(NXhandle handle, NXlink* pLink);



NXstatus NXcompress (NXhandle handle, int compr_type);
NXstatus NXsameID(NXhandle handle, NXlink* pFirstID, NXlink* pSecondID);
NXstatus NXmalloc(void** data, int rank, int dimensions[], int datatype);
NXstatus NXfree(void** data);


