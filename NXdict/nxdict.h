
#line 2239 "nxdict.w"

/*---------------------------------------------------------------------------
                            NXDICT API header file

   copyleft: Mark Koennecke, March 1997 at LNS,PSI, Switzerland

   No warranties of any kind taken.
----------------------------------------------------------------------------*/
#ifndef NXDICTAPI
#define NXDICTAPI
#include "napi.h" /* make sure, napi is included */

/*-------------------- NXDict data types & defines ----------------------*/

#line 195 "nxdict.w"

   typedef struct __NXdict *NXdict;

#line 2252 "nxdict.w"

#define NXquiet 0
#define NXalot  1
/*-------------------- Dictionary Maintainance ----------------------------*/

#line 201 "nxdict.w"

   NXstatus NXDinitfromfile(char *filename, NXdict *pDict);
   NXstatus NXDclose(NXdict handle, char *filename);

   NXstatus NXDadd(NXdict handle, char *alias, char *DefString);
   NXstatus NXDget(NXdict handle, char *alias, char *pBuffer, int iBufLen);
   NXstatus NXDupdate(NXdict handle, char *alias, char *pNewVal);
   NXstatus NXDtextreplace(NXdict handle, char *pDefString, char *pBuffer,
                           int iBuflen);

#line 2256 "nxdict.w"

/*----------------- Dictionary added data transfer -----------------------*/ 

#line 240 "nxdict.w"

   NXstatus NXDputalias(NXhandle file, NXdict dict, 
                        char *alias, void *pData);
   NXstatus NXDputdef(NXhandle file, NXdict dict, char *pDefString, void *pData);

   NXstatus NXDgetalias(NXhandle file, NXdict dict, 
                        char *alias, void *pData);
   NXstatus NXDgetdef(NXhandle file, NXdict dict, char *pDefString, void *pData);

   NXstatus NXDaliaslink(NXhandle file, NXdict dict, 
                         char *pAlias1, char *pAlias2);
   NXstatus NXDdeflink(NXhandle file, NXdict dict, 
                         char *pDef1, char *pDef2);

   NXstatus NXDopenalias(NXhandle file, NXdict dict, 
                        char *alias);
   NXstatus NXDopendef(NXhandle file, NXdict dict, char *pDefString);


#line 2258 "nxdict.w"

/*-------------------- Utility Functions --------------------------------*/

#line 310 "nxdict.w"

        NXstatus NXUwriteglobals(NXhandle file, 
                            char *filename,
                            char *owner,
                            char *adress,
                            char *phone,
                            char *email,
                            char *fax,
                            char *thing);


   NXstatus NXUentergroup(NXhandle hFil, char *name, char *class);
   NXstatus NXUenterdata (NXhandle fileid, char* label, int datatype, 
                          int rank, int dim[], char *pUnits);
   
   NXstatus NXUallocSDS(NXhandle hFil, void **pData);
   NXstatus NXUfreeSDS(void **pData);


#line 2260 "nxdict.w"

#endif
