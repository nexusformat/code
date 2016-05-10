/*
  This is a module which implements the notion of a dataset. Its is
  designed for the use with scripting languages.
  
  copyright: GPL

  Mark Koennecke, October 2002
*/
#ifndef NXDATASET
#define NXDATASET


#define MAGIC 7776622 

typedef struct {
                   int32_t magic;
                   int32_t rank;
                   int32_t type;
                   int32_t *dim;
                   char *format;
                   union {
		     void *ptr;
		     float *fPtr;
		     double *dPtr;
		     int32_t    *iPtr;
		     int16_t   *sPtr;
		     char   *cPtr;
		   } u;
}*pNXDS, NXDS;

/*
  include NeXus type codes if not already defined
*/
#ifndef NX_FLOAT32

#define NX_FLOAT32   5
#define NX_FLOAT64   6
#define NX_INT8     20  
#define NX_UINT8    21
#define NX_INT16    22  
#define NX_UINT16   23
#define NX_INT32    24
#define NX_UINT32   25
#define NX_CHAR      4

#define NX_MAXRANK 32

#endif


pNXDS createNXDataset(int32_t rank, int32_t typecode, int32_t dim[]);
pNXDS createTextNXDataset(char *name);

void  dropNXDataset(pNXDS dataset);

int32_t   getNXDatasetRank(pNXDS dataset);
int32_t   getNXDatasetDim(pNXDS dataset, int32_t which);
int32_t   getNXDatasetType(pNXDS dataset);
int32_t   getNXDatasetLength(pNXDS dataset);
int32_t   getNXDatasetByteLength(pNXDS dataset);

double getNXDatasetValue(pNXDS dataset, int32_t pos[]);
double getNXDatasetValueAt(pNXDS dataset, int32_t address);
char  *getNXDatasetText(pNXDS dataset);

int32_t   putNXDatasetValue(pNXDS dataset, int32_t pos[], double value);
int32_t   putNXDatasetValueAt(pNXDS dataset, int32_t address, double value);

pNXDS cutNXDataset(pNXDS source, int32_t start[], int32_t end[]);
pNXDS sumNXDataset(pNXDS source, int32_t dimNo, int32_t start, int32_t end);
#endif
