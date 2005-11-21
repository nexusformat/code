#ifndef GENIE_DATA_ACCESS
#define	GENIE_DATA_ACCESS

#define HAVE_LIBF2C

/* Include this here for external use of this header */
#include "geniedefs.h"

/* GENIE data access interface in C++
 *
 * Designed for calling directly from FORTRAN but not
 * with strings !! (we expect NULL terminated strings by
 * ref at this point). The header will also allow linking
 * from C (as long as parameters are passed by reference).
 */ 

#ifdef __cplusplus

// Simple utility class to allow easy creation of Dims arrays when calling from C++
// ie  CDims(n,...) =  { n, ... n }
class CDims
{
	int *m_pDims, m_iSize;

public:
	CDims(int dim1)
	{
		m_iSize = 1;
		m_pDims = new int [1];
		m_pDims[0] = dim1;
	}

	CDims(int dim1, int dim2)
	{
		m_iSize = 2;
		m_pDims = new int [2];
		m_pDims[0] = dim1;
		m_pDims[1] = dim2;
	}

	CDims(int dim1, int dim2, int dim3)
	{
		m_iSize = 3;
		m_pDims = new int [3];
		m_pDims[0] = dim1;
		m_pDims[1] = dim2;
		m_pDims[2] = dim3;
	}

	CDims(int dim1, int dim2, int dim3, int dim4)
	{
		m_iSize = 4;
		m_pDims = new int [4];
		m_pDims[0] = dim1;
		m_pDims[1] = dim2;
		m_pDims[2] = dim3;
		m_pDims[3] = dim4;
	}

	~CDims()
	{
		delete m_pDims;
	}

	operator int*()
	{
		return m_pDims;
	}

	int GetDims()
	{
		return  m_iSize;
	}
};


#endif /* C++ */

/*
 * Interface routines are all declared so C/FORTRAN can get at them
 */

/* First get cross platform/language portability sorted out */

#define CONCAT(__a,__b)	__a##__b

#ifndef _WIN32
#	if defined(__VMS)
#		define MANGLE(__arg)	__arg
#	elif defined(HAVE_LIBF2C)
#		define MANGLE(__arg)	CONCAT(__arg,__)
#	else /* __unix__ */
#		define MANGLE(__arg)	CONCAT(__arg,_)
#	endif
#	define	GX_activate_session	MANGLE(gx_activate_session)
#	define	GX_deactivate_session	MANGLE(gx_deactivate_session)
#	define	GX_select_source	MANGLE(gx_select_source)
#	define	GX_select_destination	MANGLE(gx_select_destination)
#	define	GX_directory		MANGLE(gx_directory)
#	define	GX_get			MANGLE(gx_get)
#	define	GX_put			MANGLE(gx_put)
#	define	GX_assign_handle	MANGLE(gx_assign_handle)
#	define	GX_release_handle	MANGLE(gx_release_handle)
#	define	GX_transfer		MANGLE(gx_transfer)
#	define	GX_get_var_type		MANGLE(gx_get_var_type)
#	define	GX_free				MANGLE(gx_free)
#else
#	define	GX_activate_session	GX_ACTIVATE_SESSION
#	define	GX_deactivate_session	GX_DEACTIVATE_SESSION
#	define	GX_select_source	GX_SELECT_SOURCE
#	define	GX_select_destination	GX_SELECT_DESTINATION
#	define	GX_directory		GX_DIRECTORY
#	define	GX_get			GX_GET
#	define	GX_put			GX_PUT
#	define	GX_assign_handle	GX_ASSIGN_HANDLE
#	define	GX_release_handle	GX_RELEASE_HANDLE
#	define	GX_transfer		GX_TRANSFER
#	define	GX_get_var_type		GX_GET_VAR_TYPE
#	define	GX_free				GX_FREE
#endif

typedef const char* GDAI_STRING;	// Only string to be used in the DATA interface

/*
 * Define the external interface for FORTRAN and C/C++, note that for C we
 * are just defining the interface, not the rest of the class stuff.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Get GENIE going */
GENIEIMPORT int GENIEMECHANISM GX_activate_session( GDAI_STRING default_format, int inf, int debug );
/* and the other thing */
GENIEIMPORT int GENIEMECHANISM GX_deactivate_session( void );

/* for file association and contents inspection */
GENIEIMPORT int GENIEMECHANISM GX_select_source( GDAI_STRING filename );
GENIEIMPORT int GENIEMECHANISM GX_select_destination( GDAI_STRING filename, GDAI_STRING file_format );
GENIEIMPORT int GENIEMECHANISM GX_directory( GDAI_STRING filename );

/* Data reading/writing */
GENIEIMPORT int GENIEMECHANISM GX_get( GDAI_STRING handle, GDAI_STRING tag, int object_id );
GENIEIMPORT int GENIEMECHANISM GX_put( GDAI_STRING handle, GDAI_STRING tag, int object_id, GDAI_STRING flag, GDAI_STRING comment );


/* Routines for manipulating handles */
GENIEIMPORT int GENIEMECHANISM GX_assign_handle( GDAI_STRING handle_lval, GDAI_STRING handle_rval );
GENIEIMPORT int GENIEMECHANISM GX_release_handle( GDAI_STRING handle );

/* Bi-directional access to Open GENIE Data objects */
GENIEIMPORT int GENIEMECHANISM GX_transfer( GDAI_STRING handle, GDAI_STRING direction, GDAI_STRING data_type,
                                  void* the_data, int* ndims, int dims[]);

GENIEIMPORT int GENIEMECHANISM GX_get_var_type( GDAI_STRING var, void* the_data, int* len);

GENIEIMPORT int GENIEMECHANISM GX_free(void* data, int is_array);

/*
 * Some other possibilities
 * friend int GX_compress_file( const char* filename );
 */

#ifdef __cplusplus
}

// Currently there is only one session global to Open GENIE but this could change
// When Open GENIE is running interatively this is initialized in main().
#endif /* __cplusplus */


#endif /* GENIE_DATA_ACCESS */
