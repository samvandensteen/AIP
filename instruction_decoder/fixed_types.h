/*
*  This file is part of the program: Architecture Independent Profiler (AIP).
*
*  AIP is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  AIP is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with AIP.  If not, see <http://www.gnu.org/licenses/>.
*
*  Authors: Wim Heirman, Ghent University
*           Trevor Carlson, Ghent University
*           Sam Van den Steen, Ghent University
*  Copyright: 2016, Ghent University
*/

#ifndef __FIXED_TYPES_H
#define __FIXED_TYPES_H

#ifndef __STDC_LIMIT_MACROS
# define __STDC_LIMIT_MACROS
#endif
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif
#ifndef __STDC_FORMAT_MACROS
# define __STDC_FORMAT_MACROS
#endif

#include <stdint.h>
#include <inttypes.h>

// We define __STDC_LIMIT_MACROS and then include stdint.h
// But if someone else already included stdint.h without first defining __STDC_LIMIT_MACROS,
// UINT64_MAX and friends will not be defined. Test for this here.
#ifndef UINT64_MAX
# error "UINT64_MAX is not defined. Make sure fixed_types.h is first in the include order."
#endif
#ifndef PRId64
# error "PRId64 is not defined. Make sure fixed_types.h is first in the include order."
#endif

typedef uint64_t UInt64;
typedef uint32_t UInt32;
typedef uint16_t UInt16;
typedef uint8_t  UInt8;

typedef int64_t SInt64;
typedef int32_t SInt32;
typedef int16_t SInt16;
typedef int8_t  SInt8;

typedef UInt8 Byte;
typedef UInt8 Boolean;

typedef uintptr_t IntPtr;

typedef uintptr_t carbon_reg_t;

// Carbon core types
typedef SInt32 thread_id_t;
typedef SInt32 app_id_t;
typedef SInt32 core_id_t;
typedef SInt32 carbon_thread_t;

#define INVALID_THREAD_ID ((thread_id_t) -1)
#define INVALID_APP_ID ((app_id_t) -1)
#define INVALID_CORE_ID ((core_id_t) -1)
#define INVALID_ADDRESS  ((IntPtr) -1)

#ifdef __cplusplus
// std::string isn't tread-safe when making copies
// See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=21334
#include <ext/vstring.h>
typedef __gnu_cxx::__versa_string<char> String;
#endif /* __cplusplus */

#endif
