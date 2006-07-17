#ifndef NAPICONFIG_H
#define NAPICONFIG_H

#include <nxconfig.h>

/*
 * Type definitions
 */
#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;

#endif /* HAVE_STDINT_H */
#endif /* NAPICONFIG_H */
