#include "napi.h"

int main()
{
/* create file */
    int i, array_dims[2] = { 2, 2 }; 
    float array[4] = { 1.0, 2.0, 3.0, 4.0 }, *data_array;
    int rank, datatype, dim_array[2];
    char *attr = "something", buffer[128];
    
    NexusFile fileid;
    if (NXopen("nxtest.dat", NXACC_CREATE, &fileid) != NX_OK)
	return 1;
    if (NXmakegroup(&fileid, "mygroup", "myclass") != NX_OK)
	return 1;
    if (NXopengroup(&fileid, "mygroup", "myclass") != NX_OK)
	return 1;
    if (NXmakedata (&fileid, "mylabel", DFNT_FLOAT32, 2, array_dims) != NX_OK)
	return 1;
    if (NXopendata (&fileid, "mylabel") != NX_OK)
	return 1;
    if (NXputdata(&fileid, array) != NX_OK)
	return 1;
    if (NXputattr(&fileid, "attribute", attr, strlen(attr)) != NX_OK)
	return 1;
    if (NXclosedata(&fileid) != NX_OK)
	return 1;
    if (NXclosegroup(&fileid) != NX_OK)
	return 1;
    if (NXclose(&fileid) != NX_OK)
	return 1;
/* read data */
    if (NXopen("nxtest.dat", NXACC_READ, &fileid) != NX_OK)
	return 1;
    if (NXopengroup(&fileid, "mygroup", "myclass") != NX_OK)
	return 1;
    if (NXopendata (&fileid, "mylabel") != NX_OK)
	return 1;
    if (NXgetinfo(&fileid, &rank, dim_array, &datatype) != NX_OK)
	return 1;
    if (NXmalloc((void**)&data_array, rank, dim_array, datatype) != NX_OK)
	return 1;
    if (NXgetdata(&fileid, data_array) != NX_OK)
	return 1;
    if (NXgetattr(&fileid, "attribute", buffer, sizeof(buffer)) != NX_OK)
	return 1;
    for(i=0; i<4; i++)
	printf("%f\n", data_array[i]);
    printf("Attribute: %s\n", buffer);
    if (NXclosedata(&fileid) != NX_OK)
	return 1;
    if (NXclosegroup(&fileid) != NX_OK)
	return 1;
    if (NXclose(&fileid) != NX_OK)
	return 1;
    if (NXfree((void**)&data_array) != NX_OK)
	return 1;
    return 0;
}
