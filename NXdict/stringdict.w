\subsubsection{Dictionary}
This is just a simple implementation of an dictionary of name value strings
on top of the LLD linked list package. It is needed in the histogram memory
module. This module provides the following functions:

@d Protos @{
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
@}



As usual, all function return 1 on success, 0 if there is a problem.

CreateStringDict creates and initialises a new string dictionanry.

DeleteStringDict deletes the whole dictionary and all ist values from
memory. self will point to rubbish afterwards.

StringDictAddPair adds a new name value pair to the dictionary.

StringDictExists test for the existence of name in the Dictionary.

StringDictUpdate replaces the value for name with the new one specified.

StringDictGet copies the value for name into the string pResult, but maximum
iLen characters. If pResult is NULL, this function returns the length of the
value string.

StringDictDelete deletes the entry for name from the dictionary.

StringDictGetNext implements a scan through the whole dictionary. It returns
a pointer to the current key or NULL if the dictionary is exhausted. 
Maximum iValLen characters of value information will be copied into pValue.
NEVER, ever delete the pointer passed from StringDictGetNext. A core dump
will be your reward. Please note, that each call to the usual search
functions will mess up a dictionary traversal. 



@o stringdict.h -d @{
/*--------------------------------------------------------------------------
			S T R I N G D I C T

 A module which implements a general purpose string dictionary.

 copyright: see implementation file

 Mark Koennecke, April 1997
---------------------------------------------------------------------------*/
#ifndef SICSSTRINGDICT
#define SICSSTRINGDICT

  typedef struct __StringDict *pStringDict;

@<Protos@>

#endif

@}
