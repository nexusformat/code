/*---------------------------------------------------------------------------
                           D Y N S T R I N G
                           
    A dynamic String Implementation. You can append and insert into this
    string at will. It automatically takes care of allocating more
    memory when needed.

    COPYRIGHT:
              CopyLeft, 1998, Mark Koennecke
              There are two things you MAY NOT DO with this code:
              - Sue me or my employer because it does not work.
              - Use it in a critical environment, i.e health, 
                radioactive device control, military control
                systems and the like. 
              You may do everything else with this code. It has been
              written with swiss taxpayers money. 
          
    NOTE:
         All functions return 1 or a pointer on success,
         0, or NULL on failure           
    
    Mark Koennecke, March 1998 
----------------------------------------------------------------------------*/
#ifndef DYNAMICSTRING
#define DYNAMICSTRING
       typedef struct __DynString *pDynString;
       
/*----------------------- live and death ----------------------------------*/
   pDynString CreateDynString(int iInitialLength, int iResizeLength);
   /*
     Create a new DynString Object. Its initial length will be iInitialLength.
     It will be resized in iResizeLength steps. This allows for efficient
     storage management. It woul be seriously inefficient to allocate
     per added character.
   */  
     
   void       DeleteDynString(pDynString self);
/*----------------------- interface to it --------------------------------- */          

   int DynStringCopy(pDynString self, char *pText);
   
   /* 
    Copies the text in Text into the DynString starting at 0 and over
    writing anything there beforehand.
   */
   
   int DynStringConcat(pDynString self, char *pText);
   /*
     Concatenates the string in DynString with the one supplied
     in string.
   */

   int DynStringConcatChar(pDynString self, char c);
   /*
     adds one character at the end of the string
   */  
   
   int DynStringInsert(pDynString self, char *pText, int iPos);
   
   /*
     inserts the text in pText at Position iPos in the DynString.
     Everything behind iPos will be pushed outwards in order to create
     space for pText.
   */

   int DynStringReplace(pDynString self, char *pText, int iPos);
   
   /*
     Starting at iPos, replace everything after it with ptext. In
     contrats to insert this won't push data backwards.
   */
   
   char *GetCharArray(pDynString self);
   
   /* 
     retrieves a pointer to the character array keeping the current 
     text. NEVER, ever free this pointer, otherwise you are rewarded
     with a core dump. The pointer belongs to DynString and will be
     deleted when deleting the DynString.
   */
   
   int GetDynStringLength(pDynString self);
   
   /*
     returns the current length of the dynamic string.
   */  
          
#endif    
                       