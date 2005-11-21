#ifndef GENIEDEFS__
#define GENIEDEFS__

#if defined(__unix__) && !defined(__unix)
#define __unix 1
#endif 

/*
 * Define the calling mechanism for calls external to C/C++
 * ie FORTRAN and Windows system calls on Win32
 */
#ifdef _WIN32
#	define GENIEMECHANISM __stdcall

#	ifdef IN_GENIE_DLL					/* Test whether we are a user of */
#		define GENIEIMPORT    __declspec( dllexport )	/* the dll or the producer */
#		define GENIE_IMPLEMENTATION__			/* Only trap streams inside the DLL */
#	else
#		define GENIEIMPORT    __declspec( dllimport )	/* Import and export for Win 32 */
#	endif

#	define GENIEEXPORT    __declspec( dllexport )		/* DLL shareable libraries */

#else
#	define GENIEMECHANISM
#	define GENIEIMPORT
#	define GENIEEXPORT
#endif


 /* General definitions which pervade all GENIE source, not specific to the
   parser or the smalltalk interpreter */
#define LONGCHARBUFF	13200	    /* Should hold at least 100 lines of 132 column text */
#define SHORTCHARBUFF	132	    /* One screen widths worth of 132 column text */


/* PROJECT GLOBAL INCLUDES   
 * =======================
 * This breaks the rule of not nesting includes but is done to ensure that anything
 * including geniedefs.h also gets the genie IO interface.  All the includes here are
 * essential for iofuncs.h and hence will be included for the whole of Open GENIE.
 */

/* Include gnu autoconfig header file to sort out versioning */
#ifdef HAVE_CONFIG_H
#	if defined(READLINE_LIBRARY)
#		include <readlineconfig.h>
#	elif defined(__GST__)
#		include <smalltalkconfig.h>
#	elif defined(IN_TERMCAP)
#		include <termcapconfig.h>
#	endif
#endif

/* This include file must really be near the top of everything if it is used at all */
#ifdef HAVE_LIBPTHREADS
#	include <pthread.h>
#endif

#if defined(__VMS) && HAVE_LIBCMA
#    define HAVE_LIBPTHREADS 	1
#endif

#ifdef _WIN32
#	ifndef STRICT
#	    define  STRICT 1	// Ensure we only use fully prototyped headers in win32
#	endif
#	include <windows.h>	// Include to make avoid namespace clashes later
#	include <process.h>	// do this as we are multithreaded
#	include <stdlib.h>	// Include this before iofuncs.h because stdlib defines perror
#endif

#include <errno.h>
#include <stdio.h>

#ifdef __cplusplus
#   include <iostream>
#	include <gnustring.h>
#	if defined(_WIN32) && !defined(__VMS)
#		include <sstream>
#	else
#		include <sstream>
#	endif
#   include <iomanip>
#   ifdef __GNUG__
#	define OMANIP(T) omanip<T>	// GNU uses templates normally
#   endif
#   include "genie_ostream.h"
#endif


/* Don't I/O Sabotage if this is included by a file outside GENIE */
#ifdef GENIE_IMPLEMENTATION__	/* i.e. we are in the genie code */
#   include "iofuncs.h"
#endif

/* END PROJECT GLOBAL INCLUDES */


#ifdef __cplusplus
#    define EXTERN_PREFIX extern "C"
#else
#    define EXTERN_PREFIX extern
#endif /* __cplusplus */

EXTERN_PREFIX const char* g_version;  /* GENIE wide versioning available */
EXTERN_PREFIX void* genie_malloc(size_t nbytes); 
EXTERN_PREFIX void genie_free(void* addr); 

#define MAIL_GENIE_BUG 	"Please email a bug report to genie@isise.rl.ac.uk"

/* 
 *  This is taken from the CVS manual. If you have used a line of the form:
 *
 *   		static const char* rcsid = "$Id$";
 *
 * Then you should add the line:
 * 
 *		USE_RCSID(rcsid);
 *
 * Immediately below it so (1) it is not optimised away and (2) so you get no "unused variable" warnings
 */ 

/* #define USE_RCSID(rcsid)	static void* use_##rcsid = (&use_##rcsid, (void*)&rcsid) */
#define USE_RCSID(rcsid)	static void* use_##rcsid = (void*)&rcsid

#if (defined(vms) || defined(VMS)) && !defined(__VMS)
#define __VMS
#endif

#if defined(__VMS) && !defined(VMS)
#define VMS
#endif

/* 
 * some file system specific definitions such as the
 * character we need to insert when concatenating a directory
 * and a file together to make a complete path 
 */
#if defined(__VMS)
#define DIRECTORY_SEPARATOR	""
#define NULL_OUTPUT_DEVICE	"NL:"
#define CURRENT_DIRECTORY	"SYS$DISK:[]"
#elif defined(_WIN32)
#define DIRECTORY_SEPARATOR	"\\"
#define NULL_OUTPUT_DEVICE	"NUL"
#define CURRENT_DIRECTORY	"."
#else	/* unix */
#define DIRECTORY_SEPARATOR	"/"
#define NULL_OUTPUT_DEVICE	"/dev/null"
#define CURRENT_DIRECTORY	"."
#endif
#define CURRENT_DIRECTORY_PATH	CURRENT_DIRECTORY DIRECTORY_SEPARATOR


/* 
 * Replace sigsetjmp() and siglongjmp() if we lack them
 */
#ifndef HAVE_SIGJMP_BUF
#define sigjmp_buf jmp_buf
#define siglongjmp longjmp
#define sigsetjmp(__env,__savemask)	setjmp(__env)
#endif /* HAVE_SIGJMP_BUF */

#ifdef __cplusplus
    // Make available a global pointer to the GENIE object (needs casting by the user)
    // This is set in the CGenie class on creation of a CGenie object
    extern void* g_pGenie;
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#define my_strdup(__string) strcpy((char*)malloc(strlen(__string)+1), __string)

#endif /* GENIEDEFS__ */
