%
%
%  This software is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%  GNU General Public License for more details.
%
%  You may already have a copy of the GNU General Public License; if
%  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
%  Cambridge, MA 02139, USA.
%

\documentclass[12pt]{article}

\setlength{\oddsidemargin}{-.1in}
\setlength{\evensidemargin}{0in}
\setlength{\topmargin}{0in}
\addtolength{\topmargin}{-\headheight}
\addtolength{\topmargin}{-\headsep}
\setlength{\textheight}{8.9in}
\setlength{\textwidth}{6.2in}
\setlength{\marginparwidth}{0.5in}

\begin{document}
\title{The NEXUS Dictionary API}

\author{Mark K\"onnecke\\
  Labor f\"ur Neutronenstreuung\\
  Paul Scherrer Institut\\
  CH-5232 Villigen PSI\\
  Switzerland\\       
  Mark.Koennecke@@psi.ch \\
}



\maketitle

\vskip.3in
\centerline{\large\bf Abstract}
\vskip.2in
\begin{center}
\parbox{.8\textwidth}{
  There is a proposed portable data exchange format for neutron and
  X-ray scattering communities, NEXUS (described in a separate
  publication).   Another document describes an application programmers
 interface to NEXUS. This is a base level API which hides many of the
 hideous details of the HDF interface from the NeXus programmer. The
 present document   introduces a higher level application programmers
 interface sitting on top of the NeXus API. This API (the NEXDICT-API),
 reads all file structure data  from a dictionary data file and creates
 the structure automatically from that information. The NEXDICT user only
 needs to specify the data to write or read.
}
\end{center}

\clearpage

\section{Introduction}
 There exists a proposal for a portable data exchange format for neutron and
 X--ray scattering communities, NeXus. NeXus is fully described
 elsewhere$^{1}$. NeXus sits on top of the hierarchical data format (HDF) as
 defined and specified by the National Center for Supercomputer Applications, 
 NCSA, USA. HDF comes with a library of access functions. On top of the
 HDF-library an application programmers interface (API) for NeXus was
 defined which hides many of the low level details and ideosyncracies of
 the HDF interface form the NeXus programmer. However, writing NeXus files stays
 hideous even with this interface due to the amount of repetitive code
 required to  implement the NeXus structure. Now, repetitive tasks is one
 area a computer is good at. So, why not have the computer take care of all
 the structure associated with the NeXus format? Such an approach has the 
additional benefit that changes in the file structure just require to edit
the dictionary data file with no changes to the source code writing or
reading the data. In order to do all this two
 components are needed:
\begin{itemize}
\item A language which describes the NeXus file structure to the computer.
  This language will be called the NeXus Data Definition Language (NXDDL).
  NXDLL might also be used as a tool for discussing and defining NeXus
  data structures.
\item A application programmers interface which works with the NeXus Data 
 Definition Language.
\end{itemize}
Both of the above will be detailed in this document.

\section{The NeXus Data Definition Language}
The NeXus Data Definition Language's(NXDDL) purpose is to define the structure
and data items in a NeXus file in a form which can be understood by a human
programmer and which can be parsed by the computer in order to create the 
structure. 
For this a dictionary based approach will be used. This dictionary
will contain pairs of short aliases for data items and definition strings 
which hold the structure information. This dictionary will
be initialized from a data file, the NXDDL-file. Such a dictionary can be
used in the following way: Given an appropriate API function, a NXDICT
programmer  needs to specify only the alias and the data to write and 
everything else is taken care of by the API: vGroup creation, opening,
SDS definition etc. Another use may involve the creation of definition
strings
completely or partly at run time which can then be used by an API function
in order to create the structures defined by the definition string. The same
holds for writing as well.


A NXDDL dictionary is preferably initialized from a file.
Such a NXDDL file has to follow these general structure guidelines:
\begin{itemize}
\item All input is in US--ASCII.
\item The first line must read: ##NXDICT-1.0. This is in order to
automatically identify the file.
\item A \verb+#+ in the first column denotes a comment and will be ignored.
\item A \verb+\+ at the end of the line means that the current text will be 
 continued with the next non-blank character for the next line.
\item All other entries follow the form: alias = definition string.
 This defines \verb+alias+ as a short form for the definition string  after the
 equality sign.
\item There is a text replacement facility similar to shell variables under
Unix: The sequence $(alias) anywhere in a definition string will be replaced
 by the value of the alias specified between the braces. This scheme allows
to cater for multiple entries in a file or changing data descriptions as a
function of instrument modes.
\end{itemize}         
It might be considered to add a special global vGroup of class NXdict to the
NexUs API which holds the dictionary information within a NeXus file.

 The next thing to define is the content of the definition string. A
 definition string will have the general form: \\
\centerline{\bf PATH/TerminalSymbol}
 This means a definition string will consist of a path specifier which
 describes the position of a data item in the vGroup hierarchy and a
 terminal symbol which describes the nature of the data item. 

 The path through the vGroup hierarchy to a data item will be described in a
 manner analog to a Unix directory hierarchy. However, NeXus requires two
 pieces of data in order to fully qualify a vGroup. This is it's name and
 class. Consequently, both name and classname will be given for each vGroup,
 separated by a komma. A valid path string then looks like: \\
\begin{verbatim} 
     /scan1,NXentry/DMC,NXinstrument/big_detector,NXdetector/TerminalSymbol
\end{verbatim}
 This translates into: TerminalSymbol in vGroup big\_detector, class
 NXdetector, which resides in vGroup DMC of class NXinstrument, which in
 turn is situated in the vGroup scan1 of class NXentry.

 The terminal symbol in a definition string is used to define the data item
 at the end of the definition. NeXus currently supports only three types of
 data items at the end of the chain: these are scientific data sets (SDS),
 vGroups and links to other data items or vGroups. The terminal symbol for a link
 is specified by the keyword \verb+NXLINK+  
 followed
 by a valid alias of another data item or vGroup. For example the terminal 
 symbol: \\
 \centerline{\bf SDS counts}
 would define a SDS with name counts.

 A vGroup would be denoted by the keyword VGROUP. By then, the vGroup has
 already been defined by the path string. This form of alias is only useful
 for the definition of links to vGroups.

 A SDS is more involved. The definition of an SDS starts with the keyword
 \verb+SDS+. This keyword must then be followed by the name of the SDS.
 Following the name there are option value pairs which define the 
  details of the SDS. The following option exist:
 \begin{itemize}
  \item {\bf -rank} defines the rank of the SDS.
  \item {\bf -dim \{dim0,dim1,dim2,...., dimn\} } defines the dimensions of the
       SDS. Exactly the number of rank numbers defining the dimensions
 length is required inside the curly braces. 
  \item {\bf -type} defines the data type of the SDS as a string corresponding
  to the HDF data types.
  \item {\bf -attr \{name,value\} } defines an attribute. In the curly braces
   there must be the name and value of the attribute separated by a comma.
  \end{itemize}
  If no options are given a default is used. This will be a single floating
  point number, as this is the most frequently written data item. As an 
  example see the definition of a 3d array of 32 bit integers:
  \begin{verbatim}
   PATHSTRING/SDS counts -rank 3 -dim {64,64,712} -type DFNT_INT32 \
                  -attr {Units,Counts}      

  \end{verbatim}

  \section{The NXDICT--API}
  In order to interface with the NeXus dictionary API a set of
  API--functions is needed. All functions and data types belonging to
  this API start with the letters: NXD. The functions belonging to this API
  fall into three groups:
  \begin{itemize}
   \item Dictionary maintainance functions.
   \item Data writing and reading functions.
   \item Utility functions.
  \end{itemize}
  
  One additional data type is needed for this API:
@d tata @{
   typedef struct __NXdict *NXdict;
@}
 NXdict will be used as a handle for the dictionary currently in use.

\subsubsection{Dictionary Maintainance Function}
@d dicman @{
   NXstatus NXDinitfromfile(char *filename, NXdict *pDict);
   NXstatus NXDclose(NXdict handle, char *filename);

   NXstatus NXDadd(NXdict handle, char *alias, char *DefString);
   NXstatus NXDget(NXdict handle, char *alias, char *pBuffer, int iBufLen);
   NXstatus NXDupdate(NXdict handle, char *alias, char *pNewVal);
   NXstatus NXDtextreplace(NXdict handle, char *pDefString, char *pBuffer,
                           int iBuflen);
@}
  
  {\bf NXDinitfromfile} creates a new NeXus dictionary. If filename is NULL, this
  is all that happens. If filename is not NULL, it will be opened and the
  dictionary will be initialized from the file specified.  The return value
  is either 0 for failure or non zero for success. 

  {\bf NXDclose} deletes and writes a NeXus dictionary. If filename is not NULL,
  the dictionary specified by handle is written to the file specified by
  filename. In any case the dictionary specified by handle will be deleted.

  {\bf NXDadd} adds a new alias -- Definition String pair to the dictionary
  specified by handle.

  {\bf NXDget} retrieves the definition string for the alias specified as
  the second parameter from the dictionary handle. The definition string
  is copied to pBuffer. Maximum iBufLen characters will be copied.

  {\bf NXDupdate} replaces the definition for the alias specified as second
 parameter with the new value supplied as last parameter.
  
  If a special dictionary vGroup as extension to NeXus would be accepted,
  two more functions need to be defined which read and write the dictionary 
  from the NeXus file.

 {\bf NXDtextreplace} resolves any text replacements in the definition
 string pDefString. Maximum iBuflen characters of replacement results will
 be copied into the buffer variable. 

\subsubsection{Data Handling functions}
@d dicdata @{
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

@}
 The NXDICT data handling functions go in pairs. The version ending in
 alias expects an NXdict and an alias as input. These routines work
 out the pass from that. These routines also resolve all text replacement 
 operations automatically. The other version ending on def acts upon 
 a definition string specified as second parameter. Using this scheme
 both full dictionary operation is possible, as well as operation with
 program generated definition strings. All routines return the
 usual NeXus status returns. All these routines start at the current vGroup
 level and return back to it.  

 NXDputalias, NXDputdef write the data element specified by the alias or
 the definition string to the NeXus file specified as first parameter. 
 pData is a pointer to the data to be written. These routines will check for
 the existence of all vGroups required in the path part of the definition
 string. If a vGroup  is missing it will be created. These routines step
 back to the same vGroup level from which they were called.

 NXDgetalias, NXDgetdef read a data item from file. pData MUST point to a
 data area large enough to hold the data read. If a vGroup is missing in
 the path for one of these routines an error is generated because it is 
 assumed that the data is present if a program wants to read it. These 
 routines step
 back to the same vGroup level from which they were called.
 
 NXDaliaslink, NXDdeflink links the alias or definition given as fourth
 parameter to the vGroup specified by the  third parameter. pAlias1 or
 pDef1 MUST refer to a vGroup (we cannot link to a SDS, can't we?). The
 item being linked against MUST exist, otherwise the software will complain.
 The vGroup into which the link is installed will be created on the fly,
 if not present.
 Please note, that bot aliases or definition strings specified need to
 start from the same vGroup position.  These routines step
 back to the same vGroup level from which they were called. 

 NXDopenalias, NXDopendef open the specified data items specified by the
 alias or the definition string. Then the usual NeXus functions can be 
 used to interact with the data. These routines use the same scheme for
 creating vGroups on the fly as the put routines above. The status in the
 vGroup hierarchy after this call is dependent on the nature of the terminal
 symbol. If it is a SDS, the vGroup hierarchy will be stepped back to the
 level from which the call occurred. The SDS will be left open. If the
 terminal symbol is a vGroup, then the this vGroup will be made the current
 vGroup. No back stepping occurs. 


  
  \subsection{NeXus Utility Functions}
  This section list a couple of functions which either perform common 
   tasks on NeXus files or relate
  to aspects of error handling and debugging.

@d dicutil @{
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

@}

 {\bf NXUwriteglobals} writes the global attributes to a newly opened 
 NeXus file. The parameters should be self explaining. In addition 
 the file creation date is automatically written.


 {\bf NXUentergroup} tries to open the group specified by name and class.
 If it not present, it will be created and opened.

 {\bf NXUenterdata} tries to open the SDS specified by label.
 If it not present, it will be created and opened.

  {\bf NXUallocSDS} allocates enough space for the currently open SDS. The
  pointer created is returned in pData. 

  {\bf NXUfreeSDS} returns memory allocated by NXUallocSDS to the system.


\section{Implementation}
\subsection{The NXDICT Data Structure}
The NXDICT--API is based on a dictionary which maps an alias to a definition
string in NXDDL. Clearly, the first thing needed is a dictionary which maps
key string to value strings. It was chosen to use an existing string
dictionary (developed for SICS) as the dictionary for the NXDICT--API. This
is realised and documented in files stringdict.*. This string dictionary is
based on a linked list implementation available in the public domain (Files
lld.*). This is not the most efficient approach as any search requires
searching at maximum the whole linked list via strcmp. More efficient
dictionaries would use hash tables or binary trees. However, implementation
of a more efficient dictionary will be    delayed until it is proven that
the current approach poses a serious performance problem.  

Thus, the NXdict data structure looks like this:
@d dicdat @{
    typedef struct __NXdict
                           {
                               int iID;
                               pStringDict pDictionary;
                           } sNXdict;
/*------------------ verbosity level -------------------------------------*/
   static int iVerbosity = 0 ;
@}
The first data member is a magic ID number which will be used for testing
pointers passed in as NXdict pointers. C permits any pointer to be passed
here. But only those with the correct magic number will be accepted. The
next data member is a pointer to the string dictionary used for implementing
the dictionary.

Another feature is the verbosity level. This one is declared as a file
static in order to be available generally.

\subsection{The Dictionary Maintainance Functions}
\subsubsection{NXDinitfromfile}
This routine starts off by creating and initializing a new NXdict structure:
@d iniini @{
     /* allocate a new NXdict structure */
     if(iVerbosity == NXalot)
     {
         NXIReportError(NXpData, "Allocating new NXdict structure ");
     }
     pNew = (NXdict)malloc(sizeof(sNXdict));
     if(!pNew)
     {
         NXIReportError(NXpData, "Insufficient memory for creation of NXdict");
         return NX_ERROR;
     }
     
     /* initialise it */
     pNew->iID = NXDMAGIC;
     pNew->pDictionary = CreateStringDict();
     if(!pNew->pDictionary)
     {
         NXIReportError(NXpData, "Insufficient memory for creation of NXdict");
         free(pNew);
         return NX_ERROR;
     }

@} 

The next step is to check for the existence of a filename. If filename is
NULL we are done:
@d inicheck @{
     /* is there a file name argument */
     if(filename == NULL)
     {
        if(iVerbosity == NXalot)
        {
           NXIReportError(NXpData, "NXDinitfrom file finished without data");
        }
        *pData = pNew;
        return NX_OK;
     }
@}

The next step is to open the file:
@d inifil @{
      fd = fopen(filename,"rb");
      if(!fd)
      {
         sprintf(pError,"ERROR: file %s NOT found ",filename);
         NXIReportError(NXpData, pError);
         NXIReportError(NXpData, "NXDinitfrom file finished without data");
         *pData = pNew;
         return NX_ERROR;
      }


@}

Now this file needs to be parsed and the alias -- definition string pairs to
be extracted. This is done in two steps: First the file contents is copied
in a buffer. Then this buffer is parsed. This aproach has the advantage that
the parsing code can be reused if another source for the dictionary
definition shows up. For instance if it is decided to include it with the
NeXus file in a special vGroup. 
@d iniparse @{
       /* read the file contents */
       if(iVerbosity == NXalot)
       {
          NXIReportError(NXpData, "NXDinitfrom: reading file");
       }
       pBuffer = NXDIReadFile(fd);
       fclose(fd); /* we are done with it then */
       if(!pBuffer)
       {
         sprintf(pError,"ERROR: reading file %s or no memory",filename);
         NXIReportError(NXpData, pError);
         NXIReportError(NXpData, "NXDinitfrom file finished without data");
         *pData = pNew;
         return NX_ERROR;
       }

       /* parse it */
       if(iVerbosity == NXalot)
       {
          NXIReportError(NXpData, "NXDinitfrom: parsing dictionary definitions");
       }
       NXDIParse(pBuffer, pNew->pDictionary);
@}
Once this has been done, the task left is to clean up and exit.  

\paragraph{NXDIReadFile}
This is an internal function which determines the length of the file,
allocates a suitable buffer for it and then reads the file in the buffer.
Please note, that the code for determining the length of the file works
nicely on a Unix or DOS. On a VMS however, there might be a few rubbish
characters at the end of the buffer. This is due to the record structure of
files under VMS. 

\paragraph{NXDIParse}
NXDIParse is an internal function which parses a buffer into aliases and
values. It is one of two parsers in the nxdict system. Later on there will
be a definition string parser.  This file reading parser uses a 
Tokenizer.

The Tokenizer does not need to recognize a lot of
tokens, so it is rather simple. NXDIfNextToken returns a pointer pointing to
the character after the last token read. pPtr is the current position in
the buffer. The value of the last token read will be returned in the buffer
pToken. iToken will be set to the type of token recognized.
  
@d ftoken @{
#define FWORD 1
#define FHASH 2
#define FEOL  3
#define FEOB  4
#define FEQUAL 5
#define FSLASH 6

   static char *NXDIfNextToken(char *pPtr, char *pToken, int *iToken)
   {
       pToken[0] = '\0';
       /* skip whitespace */
       while( (*pPtr == ' ') || (*pPtr == '\t') )
       {
         pPtr++;
       } 

       /* check for special characters */
       if(*pPtr == '#')
       {
         *iToken = FHASH;
         pToken[0] = *pPtr;
         pPtr++;
         return pPtr;
       }
       else if(*pPtr == '\n')
       {
         *iToken = FEOL;
         pToken[0] = *pPtr;
         pPtr++;
         return pPtr;
       }
       else if(*pPtr == '\0')
       {
         *iToken = FEOB;
         pToken[0] = *pPtr;
         pPtr++;
         return pPtr;
       }
       else if(*pPtr == '=')
       {
         *iToken = FEQUAL;
         pToken[0] = *pPtr;
         pPtr++;
         return pPtr;
       }
       else if(*pPtr == '\\')
       {
         *iToken = FSLASH;
         pToken[0] = *pPtr;
         pPtr++;
         return pPtr;
       }
       else 
       {
         *iToken = FWORD;
         /* copy word to pToken */
         while( (*pPtr != ' ') && (*pPtr != '\t') && 
                (*pPtr != '\n') && (*pPtr != '\0') && (*pPtr != '=') )
         {
           *pToken = *pPtr;
           pPtr++;
           pToken++;
         } 
         *pToken = '\0';
         return pPtr;
       }
      /* not reached */
      return pPtr;
   }
    
@}


NXDIParse has two modes: parsing an alias  or parsing a definition string.
 
@d fparse @{
#define AMODE 0
#define DMODE 1

  static void  NXDIParse(char *pBuffer, pStringDict pDict)
  {
     char *pPtr;
     int iToken;
     int iMode;
     char pAlias[132];
     char pDefinition[1024]; /* this is > 10 lines of definition */
     char pWord[132];

     assert(pBuffer);
     assert(pDict);

     iMode = AMODE;
     pPtr = pBuffer;
     iToken = -1;
     pDefinition[0] = '\0';
     pAlias[0] = '\0';
     pWord[0] = '\0';

     while(iToken != FEOB)
     {
         pPtr = NXDIfNextToken(pPtr,pWord,&iToken);
         switch(iToken)
         {
           case FHASH:
           case FSLASH: /* skip over \n to next non blank */
                       while(*pPtr != '\n')
                       {
                          pPtr++;
                          /* check for end of file */
                          if(*pPtr == '\0')
                          {
                            return;
                          }
                       }
                       pPtr++;
                       break;
           case FEQUAL: /* do a mode change */
                       iMode = DMODE;
                       pDefinition[0] = '\0';
                       break;

           case FWORD:
                       if(iMode == AMODE)
                       {
                         strcpy(pAlias,pWord);
                       }  
                       else
                       {
                         strcat(pDefinition,pWord);
                         strcat(pDefinition," ");
                       }
                       break;
           case FEOL:
                       if(iMode == DMODE)
                       {
                          /* enter in dictionary */
                          StringDictAddPair(pDict,pAlias,pDefinition);
                          iMode = AMODE;
                          pAlias[0] = '\0';
                       }
                       break;
           case FEOB:
                       if(iMode == AMODE) 
                       {
                         /* empty line or a problem */
                       }          
                       else
                       {
                          /* enter in dictionary */
                          StringDictAddPair(pDict,pAlias,pDefinition);
                          iMode = AMODE;
                          pAlias[0] = '\0';
                       }
                       return;
           default:
                   assert(0); /* unrecognized token is a programming
                                 error
                              */
                   break;
         }
     }
  }
@}   

\subsubsection{NXDClose}
This routine will just write the dictionary to file if requested and clean
up afterwards. Prior to defining NXDClose anohter internal function is
needed which checks the validity of the handle passed into the routine.

@d dassert @{
   NXdict NXDIAssert(NXdict handle)
   {
      NXdict self = NULL;
      assert(handle);
      self = (NXdict)handle;
      assert(self->iID == NXDMAGIC);
      return self;
   }
@}

@d dclose @{
   NXstatus NXDclose(NXdict handle, char *filename)
   {
      NXdict self;
      const char *pKey = NULL;
      char pValue[1024];
      FILE *fd = NULL;

      self = NXDIAssert(handle);

      if(filename) /* we must write a file */
      {
        if(iVerbosity == NXalot)
        {
           sprintf(pValue,"Writing file %s",filename);
           NXIReportError(NXpData, pValue);
        }
        fd = fopen(filename,"w");
        if(!fd)
        {
           sprintf(pValue,"ERROR: opening file %s for write",filename);
           NXIReportError(NXpData, pValue);
           return NX_ERROR;
        }   
        
        /* write our magic recognition header */
        fprintf(fd,"##NXDICT-1.0\n");

        /* write all our keys */
        pKey = StringDictGetNext(self->pDictionary, pValue,1023);
        while(pKey != NULL)
        {
          fprintf(fd,"%s = %s\n",pKey,pValue);
          pKey = StringDictGetNext(self->pDictionary,pValue,1023);
        }
        fclose(fd);
        if(iVerbosity == NXalot)
        {
           sprintf(pValue,"File %s written",filename);
           NXIReportError(NXpData, pValue);
        }
      }

      /* now we send the cleaners in */
      DeleteStringDict(self->pDictionary);
      free(self);
      return NX_OK;
   }
@}

\subsubsection{NXDadd, NXDget, NXDupdate}
These are very much only wrapper function around the corresponding functions
for maintaining StringDicts. Accordingly, they are fairly simple.
@d dmaintain @{
   NXstatus NXDadd(NXdict handle, char *alias, char *pDef)
   {
     NXdict self;
     int iRet;

     self = NXDIAssert(handle);
     iRet = StringDictAddPair(self->pDictionary,alias,pDef);
     if(!iRet)
     {
       return NX_ERROR;
     }
     return NX_OK;
   }
/*---------------------------------------------------------------------------*/
   NXstatus NXDget(NXdict handle, char *pKey, char *pBuffer, int iBufLen)
   {
     NXdict self;
     int iRet;

     self = NXDIAssert(handle);
     iRet = StringDictGet(self->pDictionary,pKey,pBuffer,iBufLen);
     if(!iRet)
     {
       return NX_ERROR;
     }
#ifdef DEFDEBUG
     printf("Resolved: %s to %s\n",pKey,pBuffer);
#endif
     return NX_OK;
   }
/*-------------------------------------------------------------------------*/
   NXstatus NXDupdate(NXdict handle, char *pKey, char *pNewVal)
   {
     NXdict self;
     int iRet;

     self = NXDIAssert(handle);
     iRet = StringDictUpdate(self->pDictionary,pKey,pNewVal);
     if(!iRet)
     {
       return NX_ERROR;
     }
     return NX_OK;
   }

@}
\subsubsection{Text replacement}
A definition string as retrieved from the string dictionary may still
contain text replacement items. The next function, NXDtextreplace resolves
these text replacements.

@d textrep @{
#define NORMAL 1
#define ALIAS  2
   pDynString NXDItextreplace(NXdict handle, char *pDefString)
   {
     NXdict self;
     int iRet, iPos, i;
     pDynString pReplaced = NULL;
     char pBueffel[1024];
     char pBuffer2[1024];
     char *pPtr;
     int iState;

     self = NXDIAssert(handle);
     
     /* create a dynamic string */
     pReplaced = CreateDynString(strlen(pDefString),512);
     if(!pReplaced)
     {
       NXIReportError(NXpData,"ERROR: out of memory in NXDtextreplace");
       return NULL;
     }

     /* the loop */
     iState = NORMAL;
     for(i = 0, pPtr = pDefString; i < strlen(pDefString); i++,pPtr++)
     {
        if(iState == NORMAL)
        {
           if(*pPtr == '$')
           {
             iState = ALIAS;
             memset(pBueffel,0,1024);
             iPos = 0;
           }
           else
           {
               DynStringConcatChar(pReplaced,*pPtr);
           }
        }
        else if(iState == ALIAS)
        {
           switch(*pPtr)
           {
             case '(': /* ignore */
                       break;
             case ')': 
                       /* do the replacement */
                       memset(pBuffer2,0,1023);
                       iRet = NXDget(handle, pBueffel,pBuffer2,1023);
                       if(iRet != NX_OK)
                       {
                         DeleteDynString(pReplaced);
                         return NULL;
                       }
                       DynStringConcat(pReplaced,pBuffer2);
                       iState = NORMAL;
                       break;
             default:
                       pBueffel[iPos] = *pPtr;
                       iPos++;
                       if(iPos >= 1024)
                       {
                          NXIReportError(NXpData,
                                     "ERROR: buffer overrun in NXDItextreplace");
                          DeleteDynString(pReplaced);
                          return NULL;
                       }
                       break;
           }
        }
     }
#ifdef DEFDEBUG
    printf("Replacement result: %s\n",GetCharArray(pReplaced));
#endif
     return pReplaced;
   }
/*--------------------------------------------------------------------------*/
   NXstatus NXDtextreplace(NXdict handle, char *pDefString, 
                           char *pBuffer, int iBufLen)
   {
     pDynString pResult = NULL;
     char *pPtr = NULL;

     pResult = NXDItextreplace(handle,pDefString);
     if(!pResult)
     {
         return NX_ERROR;
     }

     /* copy results home */
     pPtr = GetCharArray(pResult);
     strncpy(pBuffer,pPtr,iBufLen);   
     DeleteDynString(pResult);
     return NX_OK;
   }
@}

\subsection{Dictionary Added Data Transfer}
The heart of these routines is the NXDopendef function which opens the data
item specified. Most of the other routines can be defined as wrappers to
this one. That is why it is discussed as the first function. Again a parser
is needed for parsing and interpreting the definition string. 

\subsubsection{The Definition String Parser}
The definition string  parser is implemented as a classic recursive descent 
parser. And once
more again a Tokenizer is needed. The Tokenizer has an own datastructure for
holding token information in a static array:

@d tokdat @{
  typedef struct {
                  char pText[20];
                  int iCode;
                 }  TokDat;
@}

In order to do the parsing a data structure for holding parsing information
is necessary:

@d padef @{
#define TERMSDS 100
#define TERMVG  200
#define TERMLINK 300

   typedef struct {
                    char *pPtr;
                    char pToken[256];
                    int iToken;
                    int iDepth;
                    int iMayCreate;
                    int iTerminal;
                  } ParDat;
@}
In this structure pPtr is the current position in the buffer, iToken the ID
of the current token, pToken the text of the current token, iDepth gets
incremented whenever a vGroup is opened. This is needed in order to roll the
vGroups back in the hierarchy after finishing operations. iMayCreate will be
set if the path parsing function may create new vGroups if the one requested
can not be found.

This is the tokenizer:
@d deftok @{
/*---------------- Token name defines ---------------------------*/
#define DSLASH 0
#define DKOMMA 1
#define DSDS   2
#define DLINK  3
#define DGROUP 4
#define DRANK  5
#define DDIM   6
#define DTYPE  7
#define DWORD  9
#define DOPEN  10
#define DCLOSE 11
#define DATTR  12
#define DEND   13

/*----------------- Keywords ----------------------------------------*/

  static TokDat TokenList[9] = { 
                                {"SDS",DSDS},
                                {"NXLINK",DLINK},
                                {"NXVGROUP",DGROUP},
                                {"-dim",DDIM},
                                {"-type",DTYPE},
                                {"-rank",DRANK},
                                {"-attr",DATTR},
                                {NULL,0} };

/*-----------------------------------------------------------------------*/
   static void NXDIDefToken(ParDat *sStat)
   {
       int i;
       

       sStat->pToken[0] = '\0';

       /* skip whitespace */
       while( (*(sStat->pPtr) == ' ') || (*(sStat->pPtr) == '\t') )
       {
         sStat->pPtr++;
       } 

       /* check for special characters */
       if(*(sStat->pPtr) == '/')
       {
         sStat->iToken = DSLASH;
         sStat->pToken[0] = *(sStat->pPtr);
         sStat->pPtr++;
         return;
       }
       else if(*(sStat->pPtr) == ',')
       {
         sStat->iToken = DKOMMA;
         sStat->pToken[0] = *(sStat->pPtr);
         sStat->pPtr++;
         return;
       }
       else if(*(sStat->pPtr) == '\0')
       {
         sStat->iToken = DEND;
         sStat->pToken[0] = *(sStat->pPtr);
         sStat->pPtr++;
         return;
       }
       else if(*(sStat->pPtr) == '{')
       {
         sStat->iToken = DOPEN;
         sStat->pToken[0] = *(sStat->pPtr);
         sStat->pPtr++;
         return;
       }
       else if(*(sStat->pPtr) == '}')
       {
         sStat->iToken = DCLOSE;
         sStat->pToken[0] = *(sStat->pPtr);
         sStat->pPtr++;
         return;
       }
       else 
       {
         sStat->iToken = DWORD;
         /* copy word to pToken */
         i = 0;
         while( (*(sStat->pPtr) != ' ') && (*(sStat->pPtr) != '\t') && 
                (*(sStat->pPtr) != '/') && (*(sStat->pPtr) != '\0') &&
                (*(sStat->pPtr) != ',') && (*(sStat->pPtr) != '}') )
         {
           sStat->pToken[i] = *(sStat->pPtr);
           sStat->pPtr++;
           i++;
         } 
         sStat->pToken[i] = '\0';

         /*--------- try to find word in Tokenlist */
         for(i = 0; i < 7; i++)
         {
           if(strcmp(sStat->pToken,TokenList[i].pText) == 0)
           {
             sStat->iToken = TokenList[i].iCode;
             break;
           }
         }
         return;
       }
      /* not reached */
      return;
   }

@}

Now, finally we can define the parser! This parser is universally used by
all the data transfer functions.  Input is the file handle of the NeXus file
and a pointer to an initialised ParDat structure. It is expected, that the
pPtr field points to the start of the definition string, that iMayCreate is
properly defined and that iDepth is 0. 

@d defpar @{
    static int NXDIDefParse(NXhandle hFil, NXdict pDict, ParDat *pParse)
    {
       int iRet;
       char pError[256];
 
       pParse->iToken = -1;
       while(pParse->iToken != DEND)
       {
         NXDIDefToken(pParse); /* next token */
         switch(pParse->iToken)
         {
            case DEND:
                      break;
            case DSLASH:
                      iRet = NXDIParsePath(hFil, pParse);
                      if(iRet == NX_ERROR)
                      {
                         return NX_ERROR;
                      }
                      break;
            case DSDS:
                      iRet = NXDIParseSDS(hFil, pParse);
                      if(iRet == NX_ERROR)
                      {
                         return NX_ERROR;
                      }
                      pParse->iTerminal = TERMSDS;
                      break;
            case DLINK:
                      iRet = NXDIParseLink(hFil,pDict, pParse);                      
                      if(iRet == NX_ERROR)
                      {
                         return NX_ERROR;
                      }
                      pParse->iTerminal = TERMLINK;
                      break;
            case DGROUP:
                      pParse->iTerminal = TERMVG;
                      return NX_OK;
            default:
                     sprintf(pError,
            "ERROR: Definition String parse error: %s not permitted here",
             pParse->pToken);
                     NXIReportError(NXpData,pError);
                     return NX_ERROR;
                     break;
         }
       }
       return NX_OK;
    }
@}

The next thing to do is to implement ParsePath. This will try to interpret a
path string and initiate the apropriate actions, i.e. opening vGroups or
creating them. However, there is a small problem here. The code needs to
know if the vGroup exists. This can be checked by trying to open the group
with NXopengroup. This will return an error if this group does not exist.
NXopengroup will also print an error message saying so. This is not what is
wanted here, as we might choose to create the missing group silently.
 In order to suppress
that one, it is needed to replace the current error handler by a dummy which
just prints nothing anywhere and to step back to the original handler once
we are done. The other option would be to use internals of the NeXus API
implementation. However, the aim is to keep the original NeXus API and this
as independent as possible.  Consequently, here is the definition of the
dummy error handler:
@d dummyerr @{
   static void DummyError(void *pData, char *pError)
   {
     return;
   }
@}


When NXDIParsePath has been called the / has already been read.
NXDIParsePath has to read the name and class of the vGroup separated by a
komma. Then it has either to open the vGroup, and if this fails create it if
the create flag in pParse is set.

@d defpath @{
   int NXDIParsePath(NXhandle hfil, ParDat *pParse)
   {
       int iRet, iToken;
       void (*ErrFunc)(void *pData, char *pErr); 
       char pName[132], pClass[132];
       char pError[256];

       /* get the name */
       NXDIDefToken(pParse); /* next token */
       if( (pParse->iToken == DSDS) || (pParse->iToken == DGROUP)
           || (pParse->iToken == DLINK) )
       {
         /* put back & OK */
         pParse->pPtr -= strlen(pParse->pToken);
         return NX_OK;
       }
       if(pParse->iToken != DWORD)
       {
          sprintf(pError,"ERROR: parse error at %s, expected vGroup name",
                  pParse->pToken);
          NXIReportError(NXpData, pError);
          return NX_ERROR;
       }       
       strcpy(pName,pParse->pToken);
     
       /* now we expect a komma */
       NXDIDefToken(pParse); /* next token */
       if(pParse->iToken != DKOMMA)
       {
          sprintf(pError,"ERROR: parse error at %s, expected komma",
                  pParse->pToken);
          NXIReportError(NXpData, pError);
          return NX_ERROR;
       }       

       /* next must be the class */
       NXDIDefToken(pParse); /* next token */
       if(pParse->iToken != DWORD)
       {
          sprintf(pError,"ERROR: parse error at %s, expected vGroup class",
                  pParse->pToken);
          NXIReportError(NXpData, pError);
          return NX_ERROR;
       }       
       strcpy(pClass,pParse->pToken);

       /* done reading, ACTION, first install dummy error handler */
       ErrFunc = NXIReportError;
       NXMSetError(NXpData, DummyError);
 
       /* try opening vGroup */
       iRet = NXopengroup(hfil, pName, pClass);
       NXMSetError(NXpData,ErrFunc);
       if(iRet == NX_OK)
       {
         pParse->iDepth++;
         return NX_OK;
       } 
       else
       {
          /* we need to create it, if we may */
          if(pParse->iMayCreate)
          {
            iRet = NXmakegroup(hfil,pName,pClass);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
            iRet = NXopengroup(hfil,pName,pClass);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
            pParse->iDepth++;
            return NX_OK; 
          }
          else
          {
             /* this is an error */
             sprintf(pError,"ERROR: vGroup %s, %s NOT found",pName, pClass);
             NXIReportError(NXpData,pError);
             return NX_ERROR;
          }
       }
       /* not reached */
       return NX_ERROR;
   }
@} 


NXDIParseSDS is more involved, as we have to deal with all the extras an SDS
can have: dimensions, types etc. Each of these options can be present or
not, and these options can go in any order. Particularly troublesome are
attributes which can only be written after opening or creating an SDS. this
implies that attributes have to be stored in a list during parsing in order
to have them available after creation or opening of the SDS. This requires
another private data structure for holding attribute information during
parsing:
@d attitem @{
  typedef struct {
                   char name[256];
                   char value[256];
                  }AttItem;
@}


@d nxpasds @{
   static int NXDIParseSDS(NXhandle hfil, ParDat *pParse)
   {
      int iType = DFNT_FLOAT32;
      int iRank = 1;
      int32 iDim[MAX_VAR_DIMS];
      int iList;
      int iRet, iStat, i;
      char pError[256];
      char pName[MAX_NC_NAME];
      void (*ErrFunc)(void *pData, char *pErr);
      AttItem sAtt; 


       iDim[0] = 1;
       /* first find the name */
       NXDIDefToken(pParse);
       if(pParse->iToken != DWORD)
       {
          sprintf(pError,"ERROR: parsing, expected name, got %s",
                  pParse->pToken);
          NXIReportError(NXpData,pError);
          return NX_ERROR;
       }
        strcpy(pName,pParse->pToken);

       /* create the attribute list */
       iList = LLDcreate(sizeof(AttItem));
       if(iList < 0)
       {
          NXIReportError(NXpData, "ERROR: cannot create list in NXDIParseSDS");
          return NX_ERROR;
       }

       NXDIDefToken(pParse);
       while(pParse->iToken != DEND)
       {
          switch(pParse->iToken)
          {
            case DRANK: /* rank */
                       NXDIDefToken(pParse); /* advance */
                       if(pParse->iToken != DWORD)
                       {
                         sprintf(pError,
                         "ERROR: expected int, got %s", pParse->pToken);
                          NXIReportError(NXpData,pError);
                          LLDdelete(iList);
                          return NX_ERROR; 
                       }
                       iRank = atoi(pParse->pToken);
                       break;
            case DDIM:
                     iRet = NXDIParseDim(pParse, iDim);
                     if(iRet == NX_ERROR)
                     {
                        LLDdelete(iList);
                        return iRet;
                     } 
                     break;
            case DTYPE:
                     iRet = NXDIParseType(pParse, &iType);
                     if(iRet == NX_ERROR)
                     {
                        LLDdelete(iList);
                        return iRet;
                     } 
                     break;
            case DATTR:
                     iRet = NXDIParseAttr(pParse, iList);
                     if(iRet == NX_ERROR)
                     {
                        LLDdelete(iList);
                        return iRet;
                     } 
                     break;
            case DEND:
                     break;
            default:
                     sprintf(pError,"ERROR: cannot identify token %s",
                                     pParse->pToken);
                     NXIReportError(NXpData, pError);
                     LLDdelete(iList);
                     return NX_ERROR;
                    
          }
          NXDIDefToken(pParse);
       }
       
       /* whew! got all information for doing the SDS */
       /* first install dummy error handler, try open it, then
          deinstall again and create if allowed 
       */
       ErrFunc = NXIReportError;
       NXMSetError(NXpData, DummyError);
 
       /* try opening SDS */
       iRet = NXopendata(hfil, pName);
       NXMSetError(NXpData,ErrFunc);
       if(iRet == NX_OK)
       {
         LLDdelete(iList);
         return NX_OK;
       } 
       else
       {
          /* we need to create it, if we may */
          if(pParse->iMayCreate)
          {
            iRet = NXmakedata(hfil,pName,iType, iRank,iDim);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 LLDdelete(iList);
                 return iRet;
            }
            iRet = NXopendata(hfil,pName);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 LLDdelete(iList);
                 return iRet;
            }
            /* put attributes in */
            iRet = LLDnodePtr2First(iList);
            while(iRet != 0)
            {
              LLDnodeDataTo(iList,&sAtt);
              iStat = NXputattr(hfil,sAtt.name,
                                sAtt.value,strlen(sAtt.value),NX_CHAR);
              if(iStat != NX_OK)
              {
                 /* NeXus already complained bitterly */
                 LLDdelete(iList);
                 return iStat;
              }
              iRet = LLDnodePtr2Next(iList);
            }
            LLDdelete(iList);
            return NX_OK; 
          }
          else
          {
             /* this is an error */
             sprintf(pError,"ERROR: SDS %s NOT found",pName);
             NXIReportError(NXpData,pError);
             LLDdelete(iList);
             return NX_ERROR;
          }
       }
       return NX_OK;
   }
@}

NXDIParseType is fairly straightforward: read a word and try to interpret it 
as one of the standard NeXus data types.

@d parsetype @{
   static TokDat tDatType[] = {
                      {"DFNT_FLOAT32",DFNT_FLOAT32},   
                      {"DFNT_FLOAT64",DFNT_FLOAT64},    
                      {"DFNT_INT8",DFNT_INT8},  
                      {"DFNT_UINT8",DFNT_UINT8},
                      {"DFNT_INT16",DFNT_INT16},      
                      {"DFNT_UINT16",DFNT_UINT16},
                      {"DFNT_INT32",DFNT_INT32},
                      {"DFNT_UINT32",DFNT_UINT32},
                      {NULL,-122} };



   static int NXDIParseType(ParDat *pParse, int *iType)
   {
      char pError[256];
      int i = 0;

      NXDIDefToken(pParse);
      if(pParse->iToken != DWORD)
      { 
        sprintf(pError,"ERROR: expected data type, got %s", pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
      }
       
      /* try to interpret data type */
      while(tDatType[i].iCode > 0) {
        if(strcmp(tDatType[i].pText,pParse->pToken) == 0)
        {
           *iType = tDatType[i].iCode;
            return NX_OK;
        }
        i++;
      }
      /* if we are here, the data type has not been recognized. Reason for
         some boring error reporting code
      */
      sprintf(pError,"ERROR: %s not recognized as valid data type",
                      pParse->pToken);
      NXIReportError(NXpData,pError);
      return NX_ERROR;      
   }
@}

NXDIParseDim tries to read dimension information. This starts with a {
followed by numbers and kommas until there is a closing curly brace.

@d parsedim @{
   static int NXDIParseDim(ParDat *pParse, int *iDim)
   {
       char pError[256];
       int iRet, i;

        /* initialise dimensions to 0 */
        for(i = 0; i < MAX_VAR_DIMS; i++)
        {
          iDim[i] = 0;
        }

        NXDIDefToken(pParse);
        if(pParse->iToken != DOPEN)
        {
          sprintf(pError,"ERROR: expected {, got %s",pParse->pToken);
          NXIReportError(NXpData,pError);
          return NX_ERROR;
        }

        i = 0;      
        while(pParse->iToken != DCLOSE)
        {
           /* get a number */
           NXDIDefToken(pParse);
           if(pParse->iToken != DWORD)
           {
              sprintf(pError,"ERROR: expected number, got %s",pParse->pToken);
              NXIReportError(NXpData,pError);
              return NX_ERROR;
           }
           iDim[i] = atoi(pParse->pToken);
           i++;
           /* next must be close of komma */
           NXDIDefToken(pParse);
           if( (pParse->iToken != DKOMMA) && (pParse->iToken != DCLOSE) )
           {
              sprintf(pError,"ERROR: expected , or }, got %s",pParse->pToken);
              NXIReportError(NXpData,pError);
              return NX_ERROR;
           }
           if(pParse->iToken == DCLOSE)
           {
              break;
           }
        }
        return NX_OK;
   }
@}

NXDIParseAttr tries to parse an attribute and enters it into the attribute
list.

@d parseatt @{
   static int NXDIParseAttr(ParDat *pParse, int iList)
   {
     char pError[256];
     int iRet;
     AttItem sAtt;

     /* a { is expected */
     NXDIDefToken(pParse);
     if(pParse->iToken != DOPEN)
     {
        sprintf(pError,"ERROR: expected {, got %s",pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
     }

     /* a word is expected */
     NXDIDefToken(pParse);
     if(pParse->iToken != DWORD)
     {
        sprintf(pError,"ERROR: expected attribute name, got %s",pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
     }
     strcpy(sAtt.name,pParse->pToken);

     /* a , is expected */
     NXDIDefToken(pParse);
     if(pParse->iToken != DKOMMA)
     {
        sprintf(pError,"ERROR: expected , , got %s",pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
     }

     /* a word is expected */
     NXDIDefToken(pParse);
     if(pParse->iToken != DWORD)
     {
        sprintf(pError,"ERROR: expected attribute value, got %s",pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
     }
     strcpy(sAtt.value,pParse->pToken);

     /* a } is expected */
     NXDIDefToken(pParse);
     if(pParse->iToken != DCLOSE)
     {
        sprintf(pError,"ERROR: expected }, got %s",pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
     }

     /* enter into list */
     LLDnodeAppendFrom(iList,&sAtt);
     return NX_OK;
   }
@} 

The last part of the definition string parser is NXDIParseLink. This
function parses and handles a link to another data item in the file.

@d parselink @{
   static int NXDIParseLink(NXhandle hfil, NXdict pDict,ParDat *pParse)
   {
     char pError[256];
     int i, iRet;

     /* need one word of alias */ 
     NXDIDefToken(pParse);
     if(pParse->iToken != DCLOSE)
     {
        sprintf(pError,"ERROR: expected alias , got %s",pParse->pToken);
        NXIReportError(NXpData,pError);
        return NX_ERROR;
     }
    
     /* move back in hierarchy */
     for(i = 0; i < pParse->iDepth; i++)
     {
       iRet = NXclosegroup(hfil);
       if(iRet == NX_ERROR)
       {
         return NX_ERROR;
       }
     }
     
     /* open the link instead */ 
     return NXDopenalias(hfil, pDict, pParse->pToken);

   }
@}

Another helper function unwinds the vGroup hierarchy.
@d unwind @{
  static NXstatus NXDIUnwind(NXhandle hFil, int iDepth)
  {
     int i, iRet;
      
     for(i = 0; i < iDepth; i++)
     {
        iRet = NXclosegroup(hFil);
        if(iRet != NX_OK)
        {
          return NX_ERROR;
        }
     }
     return NX_OK;
  }
@}

\subsubsection{NXDopendef}
NXDopendef calls the definiton string parser and then just steps back in the
vGroup hierarchy and stays there.

@d nxddefopen @{
   NXstatus NXDopendef(NXhandle hfil, NXdict dict, char *pDef)
   {
     NXdict pDict;
     ParDat pParse;
     int iRet, i, iStat;

     pDict = NXDIAssert(dict);
     
     /* parse and act on definition string */
     pParse.iMayCreate = 1;
     pParse.pPtr = pDef;
     pParse.iDepth = 0;
     iRet = NXDIDefParse(hfil,pDict,&pParse);
     if(iRet == NX_ERROR)
     {
         /* unwind and throw up */
         iRet = NXDIUnwind(hfil,pParse.iDepth);
         return NX_ERROR;
     }

     
     /* try rewinding the hierarchy */
     if(pParse.iTerminal == TERMSDS)
     {
         iStat = NXDIUnwind(hfil,pParse.iDepth);
         if(iStat != NX_OK)
         {
            return NX_ERROR;
         }
     }
     /* do not rewind on links */
     return iRet;
   }
@}  

\subsubsection{NXDopenalias}
NXDOpenalias is just a wrapper around  NXDopendef which retrieves a
definition string from the directory first.

@d nxdaliasopen @{
   NXstatus NXDopenalias(NXhandle hfil, NXdict dict, char *pAlias)
   {
     NXdict pDict;
     int iRet;
     char pDefinition[1024];
     pDynString pReplaced = NULL;

     pDict = NXDIAssert(dict);
     
     /* get Definition String  */
     iRet = NXDget(pDict,pAlias,pDefinition,1023);
     if(iRet != NX_OK)
     {
        sprintf(pDefinition,"ERROR: alias %s not recognized",pAlias);
        NXIReportError(NXpData,pDefinition);
        return NX_ERROR;
     }
     
     /* do the text replacement */
     pReplaced = NXDItextreplace(dict,pDefinition);
     if(!pReplaced)
     {
         return NX_ERROR;
     }

     /* call NXDopendef */
     iRet = NXDopendef(hfil,dict,GetCharArray(pReplaced));
     DeleteDynString(pReplaced);
     return iRet;
   }
@}  

\subsubsection{NXDputdef}
This routine puts a data item into a NeXus file using a definition string.
This naturally can work only, if there is an SDS at the end.

@d nxput @{
   NXstatus NXDputdef(NXhandle hFil, NXdict dict, char *pDef, void *pData)
   {
     NXdict pDict;
     ParDat pParse;
     int iRet, i, iStat;

     pDict = NXDIAssert(dict);
     
     /* parse and act on definition string */
     pParse.iMayCreate = 1;
     pParse.pPtr = pDef;
     pParse.iDepth = 0;
#ifdef DEFDEBUG
     printf("Putting: %s\n",pDef);
#endif
     iRet = NXDIDefParse(hFil,pDict,&pParse);
     if(iRet == NX_ERROR)
     {
          NXDIUnwind(hFil,pParse.iDepth);
          return NX_ERROR;
     }

     
     /* only SDS can be written */
     if(pParse.iTerminal != TERMSDS)
     {
        NXIReportError(NXpData,
                     "ERROR: can only write to an SDS!");
        iStat = NX_ERROR;
     } 
     else 
     {
       /* the SDS should be open by now, write it */
       iStat = NXputdata(hFil, pData);
       iRet = NXclosedata(hFil);
     }   


     /* rewind the hierarchy */
     iRet = NXDIUnwind(hFil,pParse.iDepth);
     if(iRet != NX_OK)
     {
       return NX_ERROR;
     }
     return iStat;
   }
@}

\subsubsection{NXDputalias}
Just finds the definition string and calls NXputdef then.
@d nxdputalias @{
  NXstatus NXDputalias(NXhandle hFil, NXdict dict, char *pAlias, void *pData)
  {
     NXdict pDict;
     int iRet;
     char pDefinition[1024];
     pDynString pReplaced = NULL;

     pDict = NXDIAssert(dict);
     
     /* get Definition String  */
     iRet = NXDget(pDict,pAlias,pDefinition,1023);
     if(iRet != NX_OK)
     {
        sprintf(pDefinition,"ERROR: alias %s not recognized",pAlias);
        NXIReportError(NXpData,pDefinition);
        return NX_ERROR;
     }
     
     /* do text replacement */
     pReplaced = NXDItextreplace(dict,pDefinition);
     if(!pReplaced)
     {
       return NX_ERROR;
     }

     /* call NXDputdef */
     iRet = NXDputdef(hFil,dict,GetCharArray(pReplaced),pData);
     DeleteDynString(pReplaced);
     return iRet;
  }
@}

\subsubsection{NXDgetdef}

@d nxget @{
   NXstatus NXDgetdef(NXhandle hFil, NXdict dict, char *pDef, void *pData)
   {
     NXdict pDict;
     ParDat pParse;
     int iRet, i, iStat;

     pDict = NXDIAssert(dict);
     
     /* parse and act on definition string */
     pParse.iMayCreate = 0;
     pParse.pPtr = pDef;
     pParse.iDepth = 0;
#ifdef DEFDEBUG
     printf("Getting: %s\n",pDef);
#endif
     iRet = NXDIDefParse(hFil,pDict,&pParse);
     if(iRet == NX_ERROR)
     {
         /* unwind and throw up */
          NXDIUnwind(hFil,pParse.iDepth);
          return NX_ERROR;
     }


     /* only SDS can be written */
     if(pParse.iTerminal != TERMSDS)
     {
        NXIReportError(NXpData,
                     "ERROR: can only write to an SDS!");
        iStat = NX_ERROR;
     } 
     else 
     {
       /* the SDS should be open by now, read it */
       iStat = NXgetdata(hFil, pData);
       iRet = NXclosedata(hFil);
     }   


     /* rewind the hierarchy */
     iRet = NXDIUnwind(hFil,pParse.iDepth);
     if(iRet != NX_OK)
     {
        return NX_ERROR;
     }
     return iStat;
   }
@}

\subsubsection{NXgetalias}
Just finds the definition string and calls NXgetdef then.
@d nxdgetalias @{
  NXstatus NXDgetalias(NXhandle hFil, NXdict dict, char *pAlias, void *pData)
  {
     NXdict pDict;
     int iRet;
     char pDefinition[1024];
     pDynString pReplaced = NULL;

     pDict = NXDIAssert(dict);
     
     /* get Definition String  */
     iRet = NXDget(pDict,pAlias,pDefinition,1023);
     if(iRet != NX_OK)
     {
        sprintf(pDefinition,"ERROR: alias %s not recognized",pAlias);
        NXIReportError(NXpData,pDefinition);
        return NX_ERROR;
     }
 
     /* do text replacement */
     pReplaced = NXDItextreplace(dict,pDefinition);
     if(!pReplaced)
     {
       return NX_ERROR;
     }
     
     /* call NXDgetdef */
     iRet = NXDgetdef(hFil,dict,GetCharArray(pReplaced),pData);
     DeleteDynString(pReplaced);
     return iRet;
  }
@}

\subsubsection{NXDdeflink, NXDaliaslink}
The first alias/definition string must be a vGroup, the other can be a
vGroup or a SDS.

@d nxlink @{
  NXstatus NXDdeflink(NXhandle hFil, NXdict dict, 
                      char *pTarget, char *pVictim)
  {
     NXdict pDict;
     ParDat pParseT, pParseV;
     int iRet, i, iStat;
     NXlink sLink;

     pDict = NXDIAssert(dict);


#ifdef DEFDEBUG
     printf("Linking: %s\n",pVictim);
     printf("To: %s\n", pTarget);
#endif
     

     /* parse Victim  */
     pParseV.iMayCreate = 0;
     pParseV.pPtr = pVictim;
     pParseV.iDepth = 0;
     iRet = NXDIDefParse(hFil,pDict,&pParseV);
     if(iRet == NX_ERROR)
     {
         /* unwind and throw up */
          NXDIUnwind(hFil,pParseV.iDepth);
          return NX_ERROR;
     }
     /* get link data */
     if(pParseV.iTerminal == TERMSDS)
     {
       NXgetdataID(hFil,&sLink);
       iRet =  NXclosedata(hFil);
       if(iRet != NX_OK)
       {
         /* unwind and throw up */
          NXDIUnwind(hFil,pParseV.iDepth);
          return NX_ERROR;
       }
     }
     else if(pParseV.iTerminal == TERMVG)
     {
       NXgetgroupID(hFil,&sLink);
     }
     else
     {
        assert(0); /* serious programming error */
     }
     /* Unwind */
     iRet = NXDIUnwind(hFil,pParseV.iDepth);
     if(iRet != NX_OK)
     {
       return NX_ERROR;
     }

     /* parse Target */
     pParseT.iMayCreate = 1;
     pParseT.pPtr = pTarget;
     pParseT.iDepth = 0;
     iRet = NXDIDefParse(hFil,pDict,&pParseT);
     if(iRet == NX_ERROR)
     {
         /* unwind and throw up */
          NXDIUnwind(hFil,pParseT.iDepth);
          return NX_ERROR;
     }
     /* check it being a vGroup! */
     if(pParseT.iTerminal != TERMVG)
     {
       NXIReportError(NXpData,"ERROR: can link only into a vGroup");
       NXDIUnwind(hFil,pParseT.iDepth);
       return NX_ERROR;
     }
    
     /* link, finally */
     iRet = NXmakelink(hFil,&sLink);
     /* Unwind anyway */
     iStat = NXDIUnwind(hFil,pParseT.iDepth);
     if(iStat != NX_OK)
     {
       return NX_ERROR;
     }
     return iStat;
  }
/*--------------------------------------------------------------------------*/
  NXstatus NXDaliaslink(NXhandle hFil, NXdict dict, 
                      char *pTarget, char *pVictim)
  {
    char pTargetDef[1024], pVictimDef[1024];
    int iRet;
    NXdict pDict;
    pDynString pRep1 = NULL, pRep2 = NULL;

     pDict = NXDIAssert(dict);
     
     /* get Target Definition String  */
     iRet = NXDget(pDict,pTarget,pTargetDef,1023);
     if(iRet != NX_OK)
     {
        sprintf(pTargetDef,"ERROR: alias %s not recognized",pTarget);
        NXIReportError(NXpData,pTargetDef);
        return NX_ERROR;
     }       

     /* get Victim definition string */
     iRet = NXDget(pDict,pVictim,pVictimDef,1023);
     if(iRet != NX_OK)
     {
        sprintf(pTargetDef,"ERROR: alias %s not recognized",pTarget);
        NXIReportError(NXpData,pTargetDef);
        return NX_ERROR;
     }

     /* do replacements */
     pRep1 = NXDItextreplace(dict,pTargetDef);
     pRep2 = NXDItextreplace(dict,pVictimDef);
     if( (!pRep1) || (!pRep2) )
     {
        if(pRep1)
           DeleteDynString(pRep1);
        if(pRep2)
           DeleteDynString(pRep2);
        return NX_ERROR;
     }

     /* call NXdeflin */
     iRet = NXDdeflink(hFil,pDict,GetCharArray(pRep1),GetCharArray(pRep2));
     DeleteDynString(pRep1);
     DeleteDynString(pRep2);
     return iRet;
  }

@}

\subsection{Utility functions}
\subsubsection{NXUwriteglobals}
@d global @{
/*-------------------------------------------------------------------------*/
  static void SNXFormatTime(char *pBuffer, int iBufLen)
  {
     time_t iDate;
     struct tm *psTime;

     /* make time string */
     iDate = time(NULL);
     psTime = localtime(&iDate);
     memset(pBuffer,0,iBufLen);         
     strftime(pBuffer,iBufLen,"%Y-%d-%m %H:%M:%S",psTime);
  }  
/*--------------------------------------------------------------------------*/
   NXstatus NXUwriteglobals(NXhandle pFile, 
                            char *filename,
                            char *owner,
                            char *adress,
                            char *phone,
                            char *email,
                            char *fax,
                            char *instrument)
   {
     char pBueffel[512];
     int iStat;
     
     /* store global attributes */
     iStat = NXputattr(pFile,"file_name",filename, 
                       strlen(filename)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
       return NX_ERROR;
     }
     
     /* write creation time */
     SNXFormatTime(pBueffel,512);
     iStat = NXputattr(pFile,"file_time",pBueffel,
                       strlen(pBueffel)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
       return NX_ERROR;
     }

     /* instrument name */
     iStat = NXputattr(pFile,"instrument",instrument,
                       strlen(instrument)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
       return iStat;
     }

     /* owner */
     iStat = NXputattr(pFile,"owner",owner,
                       strlen(owner)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
       return iStat;
     }

     /* Adress */
     iStat = NXputattr(pFile,"owner_adress",adress,
                       strlen(adress)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
        return iStat;
     }

     /* phone */
     iStat = NXputattr(pFile,"owner_telephone_number",phone,
                       strlen(phone)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
        return iStat;
     }

     /* fax */
     iStat = NXputattr(pFile,"owner_fax_number",fax,
                       strlen(fax)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
       return iStat;
     }

     /* email */
     iStat = NXputattr(pFile,"owner_email",email,
                       strlen(email)+1,DFNT_INT8);
     if(iStat == NX_ERROR)
     {
       return iStat;
     }
     return NX_OK;
   }
@}

\subsubsection{NXUentergroup}

@d enterg @{
   NXstatus NXUentergroup(NXhandle hFil, char *name, char *class)
   {
       void (*ErrFunc)(void *pData, char *pErr); 
       int iRet;

       /* ACTION, first install dummy error handler */
       ErrFunc = NXIReportError;
       NXMSetError(NXpData, DummyError);
 
       /* try opening vGroup */
       iRet = NXopengroup(hFil, name, class);
       NXMSetError(NXpData,ErrFunc);
       if(iRet == NX_OK)
       {
         return NX_OK;
       } 
       else
       {
          /* we need to create it */
            iRet = NXmakegroup(hFil,name,class);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
            iRet = NXopengroup(hFil,name,class);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
       }
       return NX_OK;
   }
@} 
\subsubsection{NXUenterdata}
@d enterd @{
   NXstatus NXUenterdata(NXhandle hFil, char *label, int datatype,
                         int rank, int dim[], char *pUnits)
   {
       void (*ErrFunc)(void *pData, char *pErr); 
       int iRet;

       /* ACTION, first install dummy error handler */
       ErrFunc = NXIReportError;
       NXMSetError(NXpData, DummyError);
 
       /* try opening SDS */
       iRet = NXopendata(hFil, label);
       NXMSetError(NXpData,ErrFunc);
       if(iRet == NX_OK)
       {
         return NX_OK;
       } 
       else
       {
          /* we need to create it */
            iRet = NXmakedata(hFil,label, datatype, rank,dim);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
            iRet = NXopendata(hFil,label);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
            iRet = NXputattr(hFil, "Units",pUnits,
                             strlen(pUnits) + 1,DFNT_INT8);
            if(iRet != NX_OK)
            { 
                 /* a comment on this one has already been written! */
                 return iRet;
            }
       }
       return NX_OK;
   }
@}

\subsubsection{NXUallocSDS}
@d allocs @{
   NXstatus NXUallocSDS(NXhandle hFil, void **pData)
   {
      int iDIM[MAX_VAR_DIMS];
      int iRank,iType;
      int iRet, i;
      long lLength;

      /* get info */
      iRet = NXgetinfo(hFil,&iRank, iDIM, &iType);
      if(iRet != NX_OK)
      {
        return iRet;
      }      

      /* calculate Size */
      lLength = iDIM[0];
      for(i = 1; i < iRank; i++)
      {
        lLength *= iDIM[i];
      }
      switch(iType)
      {
        case DFNT_FLOAT32:
             lLength *= sizeof(float32);
             break;
        case DFNT_FLOAT64:
             lLength *= sizeof(float64);
             break;
        case DFNT_INT8:
             lLength *= sizeof(int8);
             break;
        case DFNT_UINT8:
             lLength *= sizeof(uint8);
             break;
        case DFNT_INT16:
             lLength *= sizeof(int16);
             break;
        case DFNT_UINT16:
             lLength *= sizeof(uint16);
             break;
        case DFNT_INT32:
             lLength *= sizeof(int32);
             break;
        case DFNT_UINT32:
             lLength *= sizeof(uint32);
             break;
        default:
             NXIReportError(NXpData,"ERROR: Internal: number type not recoginized");
             return NX_ERROR;
      }

      /* time to malloc */
      *pData = NULL;
      *pData = malloc(lLength);
      if(*pData == NULL)
      {
        NXIReportError(NXpData,"ERROR: memory exhausted in NXUallocSDS");
        return NX_ERROR;
      }
      return NX_OK;
   }
@}

@d free @{
   NXstatus NXUfreeSDS(void **pData)
   {
      free(*pData);
      *pData = NULL;
      return NX_OK;
   }
@}

\section{Files}
@o nxdict.h -d @{
/*---------------------------------------------------------------------------
                            NXDICT API header file

   copyleft: Mark Koennecke, March 1997 at LNS,PSI, Switzerland

   No warranties of any kind taken.
----------------------------------------------------------------------------*/
#ifndef NXDICTAPI
#define NXDICTAPI
#include "napi.h" /* make sure, napi is included */

/*-------------------- NXDict data types & defines ----------------------*/
@<tata@>
#define NXquiet 0
#define NXalot  1
/*-------------------- Dictionary Maintainance ----------------------------*/
@<dicman@>
/*----------------- Dictionary added data transfer -----------------------*/ 
@<dicdata@>
/*-------------------- Utility Functions --------------------------------*/
@<dicutil@>
#endif
@} 

@o nxdict.c -d @{
/*---------------------------------------------------------------------------
                 Nexus Dictionary API implementation file.

  For documentation see the nxdict.tex file which comes with this 
  distribution.

  copyleft: Mark Koennecke
            Labor fuer Neutronenstreuung
            Paul Scherrer Institut
            CH-5232 Villigen-PSI
            Switzerland
            Mark.Koennecke@@psi.ch

  No warranties of any kind, whether explicit or implied, taken.
  Distributed under the GNU copyleft license as documented elsewhere.

  August, 1997

  Version: 1.0
-----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <mfhdf.h>
#include "lld.h"
#include "napi.h"
#include "stringdict.h"
#include "dynstring.h"
#include "nxdict.h"
/*------------------ The magic number used for pointer checking */
#define NXDMAGIC 260558
/*--------------------------------------------------------------------------
      Things defined in napi.c for error reporting 
---------------------------------------------------------------------------*/
       extern void *NXpData;
       extern void (*NXIReportError)(void *pData, char *pBuffer);  
/*--------------------------------------------------------------------------*/
#define DEFDEBUG 1
/* define DEFDEBUG when you wish to print your definition strings before
   action. This can help a lot to resolve mysteries when working with
   dictionaries.
*/
/*-------------------------------------------------------------------------*/
@<dicdat@>
/*-------------------------------------------------------------------------*/
  static char *NXDIReadFile(FILE *fd)
  {
     char *pNew = NULL;
     long lLength = 0;
 
     assert(fd); 

     /* determine length of file */
     fseek(fd,0L,SEEK_END);
     lLength = ftell(fd);
     if(lLength <= 0)
     {
        return NULL;
     }
     fseek(fd,0L,SEEK_SET);

     /* allocate buffer */
     lLength += 3;
     pNew = (char *)malloc(lLength*sizeof(char));
     if(!pNew)
     {
       return NULL;
     }
     memset(pNew,0,lLength); /* this ensures a 0 at the end */

     /* read file */
     fread(pNew,sizeof(char),lLength-3,fd);
     
     /* check for existence of the NXDICT string in the file */
     if(strncmp(pNew,"##NXDICT-1.0",12) != 0 )
     {
        NXIReportError(NXpData,"ERROR: This is NO NXdict file");
        free(pNew);
        return NULL;
     }
     return pNew;
  }
/*--------------------------------------------------------------------------*/
@<ftoken@>
/*------------------------------------------------------------------------*/
@<fparse@>
/*--------------------------------------------------------------------------*/
  NXstatus NXDinitfromfile(char *filename, NXdict *pData)
  {
     NXdict pNew = NULL;
     FILE *fd = NULL;
     char *pBuffer = NULL;
     char pError[512];

@<iniini@>
@<inicheck@>
@<inifil@> 
@<iniparse@>

       if(iVerbosity == NXalot)
       {
          NXIReportError(NXpData, "NXDinitfrom: performed successfully");
       }
       free(pBuffer);
       *pData = pNew;
       return NX_OK;
  }   
/*--------------------------------------------------------------------------*/
@<dassert@>
/*-------------------------------------------------------------------------*/
@<dclose@>
/*------------------------------------------------------------------------*/
@<dmaintain@>
/*-----------------------------------------------------------------------*/
@<textrep@>
/*------------------- The Defintion String Parser -----------------------*/
/*------- Data structures */
@<tokdat@>
@<padef@>
@<dummyerr@>
@<attitem@> 
/*------------------------------------------------------------------------*/
@<deftok@>
/*------------------------------------------------------------------------*/
@<defpath@>
/*------------------------------------------------------------------------*/
@<parseatt@>
/*------------------------------------------------------------------------*/
@<parsedim@>
/*------------------------------------------------------------------------*/
@<parsetype@>
/*-------------------------------------------------------------------------*/
@<nxpasds@>
/*------------------------------------------------------------------------*/
@<parselink@>
/*------------------------------------------------------------------------*/
@<defpar@>
/*----------------------------------------------------------------------*/
@<unwind@>
/*-------------------- The Data Transfer Functions ----------------------*/
@<nxddefopen@>
/*------------------------------------------------------------------------*/
@<nxdaliasopen@>
/*------------------------------------------------------------------------*/
@<nxput@>
/*------------------------------------------------------------------------*/
@<nxdputalias@>
/*------------------------------------------------------------------------*/
@<nxget@>
/*------------------------------------------------------------------------*/
@<nxdgetalias@>
/*------------------------------------------------------------------------*/
@<nxlink@>
/*-----------------------------------------------------------------------*/
@<global@>
/*-----------------------------------------------------------------------*/
@<enterg@>
/*-----------------------------------------------------------------------*/
@<enterd@>
/*-----------------------------------------------------------------------*/
@<allocs@>
/*----------------------------------------------------------------------*/
@<free@>
@}

@o dict.c @{
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
                      "Nobody@@nowhere.edu",
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
@}
@o test.dict @{
##NXDICT-1.0
#----------------------------------------------------------------------------
# A dictionary file for test purposes
# Mark Koennecke, August 1997
#----------------------------------------------------------------------------

Linda = /entry1,NXentry/Wuerfelometer,NXinstrument/SDS Counts \
 -type DFNT_INT32 -rank 2 -dim {20,20} -attr {Units,Wuerfel} \
 -attr {axis,1}
Eva   = \
 /entry1,NXentry/NXVGROUP
Chloe = /entry1,NXentry/NXLINK Linda
Bea =
Tina = /entry1,NXentry/InvertedTOPSI,NXinstrument/SDS Tina \
       -type DFNT_FLOAT32 -rank 1 -dim {3} -attr {Units,Fahrenheit}   
Reptil = Alligator,NXanimal/
Gina = /entry1,NXentry/$(Reptil)SDS Tina \
       -type DFNT_FLOAT32 -rank 1 -dim {3} -attr {Units,Fahrenheit}   

@}

\end{document}
