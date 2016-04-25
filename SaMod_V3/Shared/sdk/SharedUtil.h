/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        SharedUtil.h
*  PURPOSE:     Solution wide utility functions
*  DEVELOPERS:  ccw <chris@codewave.co.uk>
*               Alberto Alonso <rydencillo@gmail.com>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/
//#pragma once      Don't do this

/*************************************************************************
	Simplification of some 'unsigned' types
*************************************************************************/
//                                                  VS      GCC
//                      Actual sizes:      32bit   64bit   64bit
typedef	unsigned long	    ulong;      //  32      32      64
typedef unsigned int	    uint;       //  32
typedef unsigned short	    ushort;     //  16  
typedef unsigned char	    uchar;      //  8

typedef unsigned long long  uint64;     //  64
typedef unsigned int        uint32;     //  32
typedef unsigned short      uint16;     //  16
typedef unsigned char       uint8;      //  8

// signed types
typedef signed long long    int64;      //  64
typedef signed int          int32;      //  32
typedef signed short        int16;      //  16
typedef signed char         int8;       //  8

// Windowsesq types
typedef unsigned char       BYTE;       //  8
typedef unsigned short      WORD;       //  16
typedef unsigned long       DWORD;      //  32      32      64
typedef float               FLOAT;      //  32

// Type: considerations:
// a) long (and therefore DWORD) is 64 bits when compiled using 64 bit GCC 
// b) char range can be -127 to 128 or 0 to 255 depending on compiler options/mood


#include <assert.h>
#include "SharedUtil.Defines.h"
#include "SharedUtil.AllocTracking.h"
#include <list>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdarg.h>

// Vendor
#ifndef _
#define _ //Use a dummy localisation define for modules that don't need it
#endif

#include "SString.h"
#include "WString.h"

#define _E(code) SString(" [%s]",code)

#include "SharedUtil.Map.h"
#if defined(SHARED_UTIL_WITH_HASH_MAP) || defined(SHARED_UTIL_WITH_FAST_HASH_MAP)
    #include "SharedUtil.HashMap.h"
#endif
#if defined(SHARED_UTIL_WITH_FAST_HASH_MAP)
    #include "SharedUtil.FastHashMap.h"
    #include "SharedUtil.FastHashSet.h"
#endif
#include "SharedUtil.Misc.h"
#include "SharedUtil.File.h"
#include "SharedUtil.Time.h"
#include "SharedUtil.Buffer.h"
#include "SharedUtil.Game.h"
#include "SharedUtil.Math.h"
#include "SharedUtil.ClassIdent.h"
#include "SharedUtil.Hash.h"
#if defined(SHARED_UTIL_WITH_SYS_INFO)
    #include "SharedUtil.SysInfo.h"
#endif
#include "SharedUtil.Profiling.h"
#include "SharedUtil.Logging.h"
#include "CFastList.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#ifndef stricmp
#ifdef _MSC_VER
#define stricmp _stricmp
#else
#define stricmp strcasecmp
#endif
#endif

#ifndef strnicmp
#ifdef _MSC_VER
#define strnicmp _strnicmp
#else
#define strnicmp strncasecmp
#endif
#endif
