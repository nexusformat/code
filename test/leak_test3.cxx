#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <napi.h>

#define PSZ(s) (s).c_str()

using namespace std;

const int nFiles = 10;
const int nEntry = 2;
const int nData = 2;
int array_dims[2] = {512, 512};
const char szFile[] = "leak_test.nxs";
const int iBinarySize = 512*512;
int aiBinaryData[iBinarySize];

int main ()
{
  int i, iFile, iEntry, iData, iNXdata;
 
  for(i=0; i < iBinarySize; i++)
  {
	aiBinaryData[i] = rand();
  }	
  for( iFile = 0; iFile < nFiles; iFile++ )
  {
    printf("file %d\n", iFile);
	
    NXhandle fileid;
    NXlink aLink;
    if( NXopen(szFile, NXACC_CREATE5, &fileid ) != NX_OK) return 1;
    for( iEntry = 0; iEntry < nEntry; iEntry++ )
    {
      ostringstream oss;
      oss << "entry_" << iEntry;
      if (NXmakegroup (fileid, PSZ(oss.str()), "NXentry") != NX_OK) return 1;
      if (NXopengroup (fileid, PSZ(oss.str()), "NXentry") != NX_OK) return 1;
      for( iNXdata = 0; iNXdata < nData; iNXdata++ )
      {
        ostringstream oss;
        oss << "data_" << iNXdata;
        if (NXmakegroup (fileid, PSZ(oss.str()), "NXdata") != NX_OK) return 1;
        if (NXopengroup (fileid, PSZ(oss.str()), "NXdata") != NX_OK) return 1;
        NXgetgroupID(fileid, &aLink);
        for( iData = 0; iData < nData; iData++ )
        {
          ostringstream oss;
          oss << "i2_data_" << iData;
          if (NXcompmakedata (fileid, PSZ(oss.str()), NX_INT16, 2, array_dims, NX_COMP_LZW, array_dims) != NX_OK)
//          if (NXmakedata (fileid, PSZ(oss.str()), NX_INT16, 2, array_dims) != NX_OK)
	  	return 1;
          if (NXopendata (fileid, PSZ(oss.str())) != NX_OK) return 1;
            if (NXputdata (fileid, aiBinaryData) != NX_OK) return 1;
          if (NXclosedata (fileid) != NX_OK) return 1;
        }
        if (NXclosegroup (fileid) != NX_OK) return 1;
      }
      if (NXclosegroup (fileid) != NX_OK) return 1;
    }
    if (NXclose (&fileid) != NX_OK) return 1;

    // Delete file
    remove(szFile);
  }

  printf("done...\n");
  _exit(EXIT_FAILURE);
}


