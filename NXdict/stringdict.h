
#line 49 "stringdict.w"

/*--------------------------------------------------------------------------
                        S T R I N G D I C T

 A module which implements a general purpose string dictionary.

 copyright: see implementation file

 Mark Koennecke, April 1997
---------------------------------------------------------------------------*/
#ifndef SICSSTRINGDICT
#define SICSSTRINGDICT

  typedef struct __StringDict *pStringDict;


#line 6 "stringdict.w"

  pStringDict CreateStringDict(void);
  void        DeleteStringDict(pStringDict self);

  int StringDictAddPair(pStringDict self, char *name, char *value);
  int StringDictExists(pStringDict self, char *name);
  int StringDictUpdate(pStringDict self, char *name, char *value);
  int StringDictGet(pStringDict self, char *name, char *pResult, int iLen);
  int StringDictGetAsNumber(pStringDict self, char *name, float *fVal);
  int StringDictDelete(pStringDict self, char *name);

  const char *StringDictGetNext(pStringDict self, 
                                char *pValue, int iValLen);

#line 64 "stringdict.w"


#endif

