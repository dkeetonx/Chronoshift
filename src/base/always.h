/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Basic header files and defines that are always needed.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BASE_ALWAYS_H
#define BASE_ALWAYS_H

#include "bittype.h"
#include "config.h"
#include "macros.h"
#include "targetver.h"
#include "watcomintrin.h"
#include <sys/types.h>
#include <sys/stat.h>

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#define NAME_MAX FILENAME_MAX
#if !defined(PATH_MAX)
#define PATH_MAX MAX_PATH
#endif

#endif

//  Define nullptr when standard is less than C++x0
#if __cplusplus <= 199711L && !defined COMPILER_MSVC
#define nullptr NULL
#define override
#define static_assert(x, ...)
#endif

//  General compiler specific
#if defined(COMPILER_MSVC)
// Allow inline recursive functions within inline recursive functions.
#pragma inline_recursion(on)

// Compilers don't necessarily inline code with inline keyword, especially in debug builds.
// Use FORCE_INLINE to force them to where it is possible.
//#define FORCE_INLINE	__forceinline
#define __noinline __declspec(noinline)
#define __unused __pragma(warning(suppress : 4100 4101))
// When including windows, lets just bump the warning level back to 3...
#pragma warning(push, 3)

#else // !COMPILER_MSVC
#if defined(COMPILER_GNUC) || defined(COMPILER_CLANG)
#if defined(PROCESSOR_X86) // Only applies to 32bit
#if !defined(__fastcall)
#define __fastcall __attribute__((fastcall))
#endif
#if !defined(__cdecl)
#define __cdecl __attribute__((cdecl))
#endif
#if !defined(__stdcall)
#define __stdcall __attribute__((stdcall))
#endif
#else
#if !defined(__fastcall)
#define __fastcall
#endif
#if !defined(__cdecl)
#define __cdecl
#endif
#if !defined(__stdcall)
#define __stdcall
#endif
#endif

#define __noinline __attribute__((noinline))
#define __unused __attribute__((unused))

#if !defined(__forceinline)
#define __forceinline inline __attribute__((__always_inline__))
#endif
#elif defined COMPILER_WATCOM
#define __forceinline inline
#else // !COMPILER_GNUC || !COMPILER_CLANG
// otherwise, nullify fastcall
#if !defined(__fastcall)
#define __fastcall
#endif
#if !defined(__cdecl)
#define __cdecl
#endif
#if !defined(__stdcall)
#define __stdcall
#endif
#if !defined(__forceinline)
#define __forceinline inline
#endif
#define __noinline
#endif // COMPILER_GNUC || COMPILER_CLANG

#endif // COMPILER_MSVC

// Few defines to keep things straight between windows and posix
#if defined(PLATFORM_WINDOWS)
// Not sure what flags need passing to MinGW-w64 GCC to get wcscasecmp without this
#define wcscasecmp(a, b) _wcsicmp(a, b)
#define localtime_r(a, b) localtime_s(b, a)
typedef struct stat stat_t;

#if !defined(PLATFORM_MINGW)
#define strdup(s) _strdup(s)
#define strcasecmp(a, b) _stricmp(a, b)
#define strncasecmp(a, b, c) _strnicmp(a, b, c)
#endif

// These are apparently implemented correctly in MSVC 2015
#if defined(COMPILER_MSVC) && (COMPILER_VERSION < 1900)
#define snprintf(...) _snprintf_s(__VA_ARGS__)
#define vsnprintf(...) _vsnprintf_s(__VA_ARGS__)
#endif

#else
typedef struct stat stat_t;
#endif // PLATFORM_WINDOWS

// This includes the minimum set of compiler defines and pragmas in order to bring the
// various compilers to a common behavior such that the engine will compile without
// error or warning.

// TODO: Evaluate the value of some of these.

////////////////////////////////////////////////////////////////////////////////
//
//  Watcom
//
////////////////////////////////////////////////////////////////////////////////
#if defined COMPILER_WATCOM

// Turns off stack checking.
#pragma off(check_stack)

// Turn all warnings into errors.
//#pragma warning * 0

// No reference to symbol.
#pragma warning 14 9

// Disables warning when "sizeof" is used on an object with virtual functions.
//#pragma warning 549 9

// Disable the "Integral value may be truncated during assignment or initialization".
//#pragma warning 389 9

// Allow constructing a temporary to be used as a parameter.
//#pragma warning 604 9

// Disable the construct resolved as an expression warning.
//#pragma warning 595 9

// Disable the strange "construct resolved as a declaration/type" warning.
//#pragma warning 594 9

// Disable the "pre-compiled header file cannot be used" warning.
//#pragma warning 698 9

// Disable the "temporary object used to initialize a non-constant reference" warning.
//#pragma warning 665 9

// Disable the "pointer or reference truncated by cast. Cast is supposed to REMOVE warnings, not create them.
//#pragma warning 579 9

// Disable the warning that suggests a null destructor be placed in class definition.
//#pragma warning 656 9

// Disable the warning about moving empty constructors/destructors to the class declaration.
//#pragma warning 657 9

// No virtual destructor is not an error.
//#pragma warning 004 9

// Integral constant will be truncated warning is usually ok when dealing with bitfields.
//#pragma warning 388 9

#endif

////////////////////////////////////////////////////////////////////////////////
//
//  Microsoft / Visual Studio
//
////////////////////////////////////////////////////////////////////////////////
#if defined COMPILER_MSVC

// "conversion from 'double' to 'float', possible loss of data".
#pragma warning(disable : 4244)

// warning C4800: 'BOOL' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4800)

//
// Turn off some unneeded warnings.
//

// It is safe to ignore the following warning from MSVC 7.1 or higher:
#if (COMPILER_VERSION >= 1310)
// "new behavior: elements of array will be default initialized"
//#pragma warning(disable : 4351)
#endif // COMPILER_VERSION >= 1310

// "unreferenced inline function has been removed" Yea, so what?
//#pragma warning(disable : 4514)

// Disable warning about exception handling not being enabled. It's used as part of STL - in a part of STL we don't use.
//#pragma warning(disable : 4530)

// "overflow in floating-point constant arithmetic" This warning occurs even if the
// loss of precision is insignificant.
//#pragma warning(disable : 4056)

// "function not inlined" This warning is typically useless. The inline keyword
// only serves as a suggestion to the compiler and it may or may not inline a
// function on a case by case basis. No need to be told of this.
//#pragma warning(disable : 4710)

// "'this' used in base member initializer list" Using "this" in a base member
// initializer is valid -- no need for this warning.
//#pragma warning(disable : 4355)

// "typedef-name used as a synonym for class-name". This is by design and should
// not be a warning.
//#pragma warning(disable : 4097)

// Unreferenced local function removed.
//#pragma warning(disable : 4505)

// 'function selected for automatic inlining'
//#pragma warning(disable : 4711)
    
// 'This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation,
// use _CRT_SECURE_NO_WARNINGS. See online help for details.'
//#pragma warning(disable : 4996)

// 'copy constructor could not be generated'
//#pragma warning(disable : 4511)

// 'assignment operator could not be generated'
//#pragma warning(disable : 4512)

// 'unreferenced formal parameter'
//#pragma warning(disable : 4100)

// HIDE WARNING 4786 "identifier was truncated to '255' characters in the browser information"
// Tempates create LLLOOONNNGGG identifiers!
//#pragma warning(disable : 4786)

// 'function selected for automatic inline expansion'.  Cool, but since we're treating
// warnings as errors, don't warn me about this!
//#pragma warning(disable : 4711)

//for catching unrefernced local variables, we do not need to worry about this when compiling debug builds.
#if !defined(NDEBUG)
//#pragma warning(disable : 4189)
//#pragma warning(disable : 4101)
//#pragma warning(disable : 4700) // uninitialized local variable 'x' used
#endif // !NDEBUG

#endif // COMPILER_MSVC

#endif // _BASE_ALWAYS_H_
