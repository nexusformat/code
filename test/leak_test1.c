#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <napi.h>

int main (int argc, char* argv[])
{
	NXaccess access_mode = NXACC_CREATE5;
        const int nReOpen = 1000;
	printf("Running for %d iterations\n", nReOpen);
        int iReOpen;
        const char* szFile = "leak_test1.nxs";

        NXhandle fileid;
	unlink(szFile);
        if (NXopen(szFile, access_mode, &fileid) != NX_OK) return 1;
        if( NXclose(&fileid) != NX_OK) return 1;
        for( iReOpen = 0; iReOpen < nReOpen; iReOpen++ )
        {
                if( 0 == iReOpen % 100 )
                        printf("loop count %d\n", iReOpen);
                if( NXopen(szFile, NXACC_RDWR, &fileid ) != NX_OK) return 1;
                if( NXclose(&fileid) != NX_OK ) return 1;
        }      
	unlink(szFile);
	fileid = NULL;
	_exit(EXIT_FAILURE);
        return 0;
}
