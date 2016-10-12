#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <napi.h>

#define MAX_CHAR 255


static const char filename[] = "issue_364.nxs";

void write_data()
{
    NXhandle fid;
    int start[2] = {0, 0};
    int shape[2] = {-1, MAX_CHAR};
    int n = 5;
    int i, status;
    char *text;
    

    text = calloc(1,sizeof(char)*MAX_CHAR); 
    status = NXopen(filename, NXACC_CREATE5, &fid);

    status = NXmakegroup(fid, "test", "NXnote");
    status = NXopengroup(fid, "test", "NXnote");

    status = NXmakedata(fid, "stringarray", NX_CHAR, 2, shape);
    status = NXopendata(fid, "stringarray");

    shape[0] = 1;
    for (i = 0; i < n; i++) {
        snprintf(text, MAX_CHAR-1, "name%d", i);
        start[0] = i;
        status = NXputslab(fid, text, start, shape);
    }
    status = NXclosedata(fid);

    status = NXclosegroup(fid);
    status = NXclose(&fid);
    free(text); 
}

void read_data()
{
    NXhandle fid;
    void *buffer;
    int status;
    int start[2] = {0, 0};
    int shape[2] = {2, MAX_CHAR};

    NXmalloc((void **)&buffer, 2, shape, NX_CHAR);
    status = NXopen(filename, NXACC_RDWR, &fid);

    status = NXopenpath(fid, "/test/stringarray");

    status = NXgetslab(fid, buffer, start, shape);
    status = NXclose(&fid);
    NXfree(&buffer); 
}

int main(int argc, char **argv) 
{
    write_data();
    read_data();

    return 0;

}

