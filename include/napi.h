/*---------------------------------------------------------------------------
  NeXus - Neutron & X-ray Common Data Format
  
  NeXus API header file
  
  Copyright (C) 1997-2000 Mark Koennecke, Przemek Klosowski
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
  Contact : Mark Koennecke <Mark.Koennecke@psi.ch>
            Labor fuer Neutronenstreuung
            Paul Scherrer Institut             
            CH-5232 Villigen-PSI
            Switzerland
            
            Przemek Klosowski <Przemek.Klosowski@nist.gov>
            NIST Center for Neutron Research
            100 Bureau Drive, Stop 8562
            Gaithersburg, MD 20899-8562
            USA
            
  For further information, see <http://www.neutron.anl.gov/NeXus/>

  $Id$

----------------------------------------------------------------------------*/

#ifndef NEXUSAPI
#define NEXUSAPI

#define NEXUS_VERSION	"1.3.2"		/* major.minor.patch */

#ifdef GENIE_IMPLEMENTATION__		/* OpenGENIE is fussy about consts */
#   define CONSTCHAR	const char
#else
#   define CONSTCHAR	char
#endif /* GENIE_IMPLEMENTATION__ */

/*
 * Set up NX_EXTERNAL macro for declaring functions for import/export from
 * a Windows DLL. We need to do this differently for building the library and for a user program,
 * so we check the NEXUS_LIBRARY macro which is defined only when compiling the library
 */
#ifdef NEXUS_LIBRARY	/* we are building the library, so need to export */
#  ifdef _WIN32
#    define NX_EXTERNAL	__declspec( dllexport )
#  else
#    define NX_EXTERNAL
#  endif /* _WIN32 */
#else			/* we are building an application, so need to import */
#  ifdef _WIN32
#    define NX_EXTERNAL	__declspec( dllimport )
#  else
#    define NX_EXTERNAL
#  endif /* _WIN32 */
#endif /* NEXUS_LIBRARY */

/*
 * check for ints.h header on VMS - it defines conflicting types
 */
#if defined(__VMS) && defined(__INTS_LOADED)
#   define int8 hdf_int8
#   define int16 hdf_int16
#   define uint16 hdf_uint16
#   define int32 hdf_int32
#   define uint32 hdf_uint32
#   include <mfhdf.h>
#   undef int8
#   undef int16
#   undef uint16
#   undef int32
#   undef uint32
#else
#   include <mfhdf.h>
#endif /* defined(__VMS) && defined(__INTS_LOADED) */

typedef enum {NXACC_READ = DFACC_READ, 
              NXACC_RDWR = DFACC_RDWR, 
              NXACC_CREATE = DFACC_CREATE } NXaccess;

typedef void* NXhandle;		/* really a pointer to a NexusFile structure */
typedef int NXstatus;
typedef char NXname[VGNAMELENMAX];

    typedef struct {
                    int32 iTag;
                    int32 iRef;
                   } NXlink;

#define NX_OK 1
#define NX_ERROR 0
#define NX_EOD -1

#define NX_MAXRANK 32
#define NX_MAXNAMELEN 64

/*-------------------------------------------------------------------------
                HDF Datatype values for datatype parameters 
                       in the Nexus API

  NX_FLOAT32     32 bit float
  NX_FLOAT64     64 nit float == double
  NX_INT8        8 bit integer == byte
  NX_UINT8       8 bit unsigned integer
  NX_INT16       16 bit integer
  NX_UINT16      16 bit unsigned integer
  NX_INT32       32 bit integer
  NX_UINT32      32 bit unsigned integer
  NX_CHAR        8 bit character

  This list is a edited version of the one found in the HDF header file
  htndefs.h. That source will always be the real reference, this is
  documented here for your convenience.
--------------------------------------------------------------------------*/ 

  
/* Map NeXus to HDF types */
#define NX_FLOAT32	DFNT_FLOAT32
#define NX_FLOAT64	DFNT_FLOAT64
#define NX_INT8		DFNT_INT8
#define NX_UINT8	DFNT_UINT8
#define NX_INT16	DFNT_INT16
#define NX_UINT16	DFNT_UINT16
#define NX_INT32	DFNT_INT32
#define NX_UINT32	DFNT_UINT32
#define NX_CHAR		DFNT_CHAR8

/* Map NeXus compression methods to HDF compression methods */
#define NX_COMP_NONE COMP_CODE_NONE
#define NX_COMP_LZW COMP_CODE_DEFLATE
#define NX_COMP_RLE COMP_CODE_RLE
#define NX_COMP_HUF COMP_CODE_SKPHUFF          

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 
 *  Now, we have the interface visible from FORTRAN and C. On UNIX system
 *  FORTRAN routines usually get an extra training 
 *  Do not mangle using "_" as f2c mangles _ containing and non _
 *  containing names differently
 *  We must also lowercase anything that is called from FORTRAN
 *  else we can't link
 */

#define CONCAT(__a,__b)	__a##__b	/* token concatenation */

/* 
 * Define a macro for FORTRAN name mangling _ pften we have to add an "_"
 */
#if defined(__VMS) || defined(__unix__) || defined(__MWERKS__)
#    if defined(__VMS) || defined(__MWERKS__) || defined(__ABSOFT)
#        define MANGLE(__arg)	__arg
#    else /* unix */
#        define MANGLE(__arg)	CONCAT(__arg,_)
#    endif
#    define CALLING_STYLE	/* blank */

#    define NXopen 		MANGLE(nxiopen)
#    define NXclose 		MANGLE(nxiclose)
#    define NXmakegroup 	MANGLE(nximakegroup)
#    define NXopengroup 	MANGLE(nxiopengroup)
#    define NXclosegroup 	MANGLE(nxiclosegroup)
#    define NXmakedata 		MANGLE(nximakedata)
#    define NXcompress 		MANGLE(nxicompress)
#    define NXopendata 		MANGLE(nxiopendata)
#    define NXclosedata 	MANGLE(nxiclosedata)
#    define NXgetdata 		MANGLE(nxigetdata)
#    define NXgetslab 		MANGLE(nxigetslab)
#    define NXgetattr 		MANGLE(nxigetattr)
#    define NXgetdim 		MANGLE(nxigetdim)
#    define NXputdata 		MANGLE(nxiputdata)
#    define NXputslab 		MANGLE(nxiputslab)
#    define NXputattr 		MANGLE(nxiputattr)
#    define NXputdim 		MANGLE(nxiputdim)
#    define NXgetinfo 		MANGLE(nxigetinfo)
#    define NXgetgroupinfo 	MANGLE(nxigetgroupinfo)
#    define NXinitgroupdir 	MANGLE(nxiinitgroupdir)
#    define NXgetnextentry 	MANGLE(nxigetnextentry)
#    define NXgetattrinfo 	MANGLE(nxigetattrinfo)
#    define NXinitattrdir 	MANGLE(nxiinitattrdir)
#    define NXgetnextattr 	MANGLE(nxigetnextattr)
#    define NXgetgroupID 	MANGLE(nxigetgroupid)
#    define NXgetdataID 	MANGLE(nxigetdataid)
#    define NXmakelink 		MANGLE(nximakelink)
#    define NXmalloc 		MANGLE(nximalloc)
#    define NXfree 		MANGLE(nxifree)
/* FORTRAN helpers - for NeXus internal use only */
#    define NXfopen		MANGLE(nxifopen)
#    define NXfclose		MANGLE(nxifclose)
#    define NXfmakedata		MANGLE(nxifmakedata)
#    define NXfcompress		MANGLE(nxifcompress)
#    define NXfputattr		MANGLE(nxifputattr)
#elif defined(_WIN32)
/* 
 * START OF WINDOWS SPECIFIC CONFIGURATION
 *
 * Various PC calling conventions - you need only uncomment one of the following definitions of MANGLE()
 * anlong with the appropriate CALLING_STYLE
 * The choice arises because under Windows the default way FORTRAN calls FORTRAN is different
 * from the dafault way C calls C, and so when you need to have FORTRAN calling C you must
 * force them to use the same convention. Notice the use of "default way" above ... by choice
 * of compiler options (or compiler vendor) you may actually have FORTRAN calling in the C way 
 * etc., so you might need to experiment with the options below until you get no "unresolved symbols" 
 *
 * Choice 1: Should allow both FORTRAN and C NeXus interfaces to work in a "default" setup 
 * Choice 2: For when choice 1: gives problems and you only require the C interface
 * Choice 3: An alternative to 1: which may allow both FORTRAN and C in a non-default setup
 */
#	define MANGLE(__arg)		__arg				/* Choice 1 */
#	define CALLING_STYLE		__stdcall			/* Choice 1 */
/* #	define MANGLE(__arg)		__arg				/* Choice 2 */
/* #    define CALLING_STYLE						/* Choice 2 */
/* #	define MANGLE(__arg)		CONCAT(__arg,_)			/* Choice 3 */
/* #    define CALLING_STYLE		__stdcall			/* Choice 3 */
/* 
 * END OF WINDOWS SPECIFIC CONFIGURATION 
 */
#       define NXopen 			MANGLE(NXIOPEN)
#       define NXclose 			MANGLE(NXICLOSE)
#       define NXmakegroup 		MANGLE(NXIMAKEGROUP)
#       define NXopengroup 		MANGLE(NXIOPENGROUP)
#       define NXclosegroup 		MANGLE(NXICLOSEGROUP)
#       define NXmakedata 		MANGLE(NXIMAKEDATA)
#       define NXcompress 		MANGLE(NXICOMPRESS)
#       define NXopendata 		MANGLE(NXIOPENDATA)
#       define NXclosedata 		MANGLE(NXICLOSEDATA)
#       define NXgetdata 		MANGLE(NXIGETDATA)
#       define NXgetslab 		MANGLE(NXIGETSLAB)
#       define NXgetattr 		MANGLE(NXIGETATTR)
#       define NXgetdim 		MANGLE(NXIGETDIM)
#       define NXputdata 		MANGLE(NXIPUTDATA)
#       define NXputslab 		MANGLE(NXIPUTSLAB)
#       define NXputattr 		MANGLE(NXIPUTATTR)
#       define NXputdim 		MANGLE(NXIPUTDIM)
#       define NXgetinfo 		MANGLE(NXIGETINFO)
#       define NXgetgroupinfo 		MANGLE(NXIGETGROUPINFO)
#       define NXinitgroupdir 		MANGLE(NXIINITGROUPDIR)
#       define NXgetnextentry 		MANGLE(NXIGETNEXTENTRY)
#       define NXgetattrinfo 		MANGLE(NXIGETATTRINFO)
#       define NXinitattrdir 		MANGLE(NXIINITATTRDIR)
#       define NXgetnextattr 		MANGLE(NXIGETNEXTATTR)
#       define NXgetgroupID 		MANGLE(NXIGETGROUPID)
#       define NXgetdataID 		MANGLE(NXIGETDATAID)
#       define NXmakelink 		MANGLE(NXIMAKELINK)
#       define NXmalloc 		MANGLE(NXIMALLOC)
#       define NXfree 			MANGLE(NXIFREE)
/* FORTRAN helpers - for NeXus internal use only */
#	define NXfopen 			MANGLE(NXIFOPEN)
#	define NXfclose			MANGLE(NXIFCLOSE)
#	define NXfmakedata		MANGLE(NXIFMAKEDATA)
#	define NXfcompress		MANGLE(NXIFCOMPRESS)
#	define NXfputattr		MANGLE(NXIFPUTATTR)
#else
#   error Cannot compile - unknown operating system
#endif

/* 
 * Standard interface 
 */
NX_EXTERNAL  NXstatus CALLING_STYLE NXopen(CONSTCHAR * filename, NXaccess access_method, NXhandle* pHandle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXclose(NXhandle* pHandle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NXmakegroup (NXhandle handle, CONSTCHAR* Vgroup, char* NXclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NXopengroup (NXhandle handle, CONSTCHAR* Vgroup, char* NXclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NXclosegroup(NXhandle handle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NXmakedata (NXhandle handle, CONSTCHAR* label, int datatype, int rank, int dim[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NXopendata (NXhandle handle, CONSTCHAR* label);
NX_EXTERNAL  NXstatus CALLING_STYLE NXcompress (NXhandle handle, int compr_type);
NX_EXTERNAL  NXstatus CALLING_STYLE NXclosedata(NXhandle handle);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetdata(NXhandle handle, void* data);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetslab(NXhandle handle, void* data, int start[], int size[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetattr(NXhandle handle, char* name, void* data, int* iDataLen, int* iType);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NXputdata(NXhandle handle, void* data);
NX_EXTERNAL  NXstatus CALLING_STYLE NXputslab(NXhandle handle, void* data, int start[], int size[]);
NX_EXTERNAL  NXstatus CALLING_STYLE NXputattr(NXhandle handle, CONSTCHAR* name, void* data, int iDataLen, int iType);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetinfo(NXhandle handle, int* rank, int dimension[], int* datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetgroupinfo(NXhandle handle, int* no_items, NXname name, NXname nxclass);
NX_EXTERNAL  NXstatus CALLING_STYLE NXinitgroupdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetnextentry(NXhandle handle, NXname name, NXname nxclass, int* datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetattrinfo(NXhandle handle, int* no_items);
NX_EXTERNAL  NXstatus CALLING_STYLE NXinitattrdir(NXhandle handle);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetnextattr(NXhandle handle, NXname pName, int *iLength, int *iType);
  
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetgroupID(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NXgetdataID(NXhandle handle, NXlink* pLink);
NX_EXTERNAL  NXstatus CALLING_STYLE NXmakelink(NXhandle handle, NXlink* pLink);
  
/* 
 * Helper interface 
 */

NX_EXTERNAL  NXstatus CALLING_STYLE NXmalloc(void** data, int rank, int dimensions[], int datatype);
NX_EXTERNAL  NXstatus CALLING_STYLE NXfree(void** data);

/*-----------------------------------------------------------------------
    A non Nexus standard function to set an error handler 
*/
NX_EXTERNAL  void CALLING_STYLE NXMSetError(void *pData, void (*ErrFunc)(void *pD, char *text));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NEXUSAPI */
