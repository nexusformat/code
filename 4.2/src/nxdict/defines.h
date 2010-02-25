/*  ======================================================================
    DEFINES.h       Standard definitions etc.
                    For simplification or for debugging substitution.

                    v1.02  94-08-11  Stripped version.

 _____              This version is Public Domain.
 /_|__|             A.Reitsma, Delft, Nederland.
/  | \  --------------------------------------------------------------- */

#include <stdlib.h>         /* for malloc() prototype */
#include <string.h>         /* for memcpy() prototype */

#define MALLOC(size,type)   (type *) malloc( (size) * sizeof( type ))
#define FREE(mem)           free( mem )
#define CALLOC(size,type)   (type *) calloc( (size), sizeof( type))

/*  === DEFINES.h end ================================================= */
