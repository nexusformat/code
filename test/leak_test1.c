#include <stdio.h>
#include <unistd.h>
#include <napi.h>

int main (int argc, char* argv[])
{
	NXaccess access_mode = NXACC_CREATE5;
        const int nReOpen = 2000;
//        const int nReOpen = 1000000000;
        int iReOpen;
        const char* szFile = "leak_test1.nxs";

        NXhandle fileid;
	unlink(szFile);
        if (NXopen(szFile, access_mode, &fileid) != NX_OK) return 1;
        if( NXclose(&fileid) != NX_OK) return 1;
        for( iReOpen = 0; iReOpen < nReOpen; iReOpen++ )
        {
                if( 0 == iReOpen % 1000 )
                        printf("loop count %d\n", iReOpen);
                if( NXopen(szFile, NXACC_RDWR, &fileid ) != NX_OK) return 1;
                if( NXclose(&fileid) != NX_OK ) return 1;
        }      
	unlink(szFile);
	fileid = NULL;
        printf("done ... now sleeping so you can Ctrl-C\n");
	sleep(2000);
        return 0;
}

