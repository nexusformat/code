
/*--------------------------------------------------------------------------
                               D I C T 

 This file exercises some of the NXDICT functionality for test purposes.
 It can also serve as an example for the usage of the API.

 Mark Koennecke, August 1997
  
 Upgraded to support file idetification and text replacement

 Mark Koennecke, April 1998
----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mfhdf.h>
#include "dynstring.h"
#include "napi.h"
#include "nxdict.h"

   int main(int argc, char *argv[])
   {
      NXdict pDict = NULL;
      NXhandle hfil;
      void *pData = NULL;
      float fTina[3] = { 0.123, 0.234, 0.456};
      float fTest[3], fDelta;
      float fTust[20*20];
      char pBuffer[132];
      int i;

      /* test nxdict */
      NXDinitfromfile("test.dict",&pDict);
      NXopen("test.hdf",NXACC_CREATE,&hfil);
      NXDadd(pDict,"Gundula",
            "/entry1,NXentry/SphereOmeter,NXinstrument/SDS");
      NXDupdate(pDict,"Bea","/entry1,NXentry/SDS");
      NXDget(pDict,"Bea",pBuffer,131);
      printf("Bea = %s\n",pBuffer);
      NXDget(pDict,"Linda",pBuffer,131);
      NXDopendef(hfil,pDict,pBuffer);
      NXDputalias(hfil,pDict,"Tina",fTina);
      NXDputalias(hfil,pDict,"Gina",fTina);
      NXDgetalias(hfil,pDict,"Tina",fTest);
      NXDgetalias(hfil,pDict,"Gina",fTest);
      NXDputalias(hfil,pDict,"Linda",fTust);
      NXDaliaslink(hfil,pDict,"Eva","Linda");
      NXDclose(pDict,"close.dict");
      NXclose(&hfil);
      printf("NXDICT seemed to have worked \n");

      /* test Utility functions */
      printf(" Proceeding to test of utility functions \n");
      NXopen("test2.hdf",NXACC_CREATE,&hfil);
      NXUwriteglobals(hfil,
                      "test2.hdf",
                      "Willibald Wuergehals",
                      "Rue des Martyrs, 26505 Timbuktu, Legoland ",
                      "+41-56-3102512",
                      "Nobody@nowhere.edu",
                      " 755-898767",
                      "Dingsbums");
      NXUentergroup(hfil, "TestGroup", "NXtest");
      NXclosegroup(hfil);
      NXUentergroup(hfil, "TestGroup", "NXtest");

      i = 120;
      NXUenterdata(hfil,"TestData",DFNT_INT8, 1,&i,"Testis");
      NXclosedata(hfil);
      NXUenterdata(hfil,"TestData",DFNT_INT8, 1,&i,"Testis");

      NXUallocSDS(hfil,&pData);
      NXUfreeSDS(&pData);
      NXclose(&hfil);
      printf("All tests seem to have worked OK,  %s %s\n",
             "but the test is pathetic\n", 
             "Do not rely, in any circumstances, on this test alone");

 
   }
