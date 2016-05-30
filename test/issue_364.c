#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <napi.h>

#define MAX_CHAR 255

void write_data()
{
    NXhandle fid;
    int start[2] = {0, 0};
    int shape[2] = {-1, MAX_CHAR};
    int n = 5;
    int i;
    char *text;

    text = calloc(MAX_CHAR, 1);
    NXopen("issue_364.nxs", NXACC_CREATE5, &fid);

    NXmakegroup(fid, "test", "NXnote");
    NXopengroup(fid, "test", "NXnote");

    NXmakedata(fid, "stringarray", NX_CHAR, 2, shape);
    NXopendata(fid, "stringarray");

    shape[0] = 1;
    for (i = 0; i < n; i++) {
        snprintf(text, MAX_CHAR-1, "name%d", i);
        start[0] = i;
        NXputslab(fid, text, start, shape);
    }
    NXclose(fid);
    free(text);
}

void read_data()
{
    NXhandle fid;
    void *buffer;
    int start[2] = {0, 0};
    int shape[2] = {2, 1};

    NXmalloc(&buffer, 2, shape, NX_CHAR);
    NXopen("file.nxs", NXACC_READ, &fid);

    NXopenpath(fid, "/test/stringarray");

    NXgetslab(fid, buffer, start, shape);
    NXclose(fid);
    NXfree(buffer);
}

int main(int argc, char **argv) 
{
    write_data();
    read_data();

    return 0;

}
