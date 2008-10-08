#ifndef NAPICONFIG_H
#define NAPICONFIG_H

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

#include <nxconfig.h>

/*
 * Integer type definitions
 * 
 * int32_t etc will be defined by configure in nxconfig.h 
 * if they exist; otherwise include an appropriate header
 */
#if HAVE_STDINT_H
#include <stdint.h>
#endif /* HAVE_STDINT_H */


#endif /* NAPICONFIG_H */
