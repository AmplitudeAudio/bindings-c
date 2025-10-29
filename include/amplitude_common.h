// Copyright (c) 2025-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _AM_C_COMMON_H
#define _AM_C_COMMON_H

#if AM_COMPILER_MSVC
#define __export         __declspec(dllexport)
#define __import         __declspec(dllimport)
#elif AM_COMPILER_GCC && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export         __attribute__((visibility("default")))
#define __import         __attribute__((visibility("default")))
#else
#define __export
#define __import
#endif

#ifdef AM_BUILDSYSTEM_BUILDING_AMPLITUDE
#define __api __export
#else
#define __api __import
#endif

#if AM_PLATFORM_WIN
typedef wchar_t am_oschar;
#elif AM_PLATFORM_ANDROID
typedef char am_oschar;
#elif AM_PLATFORM_LINUX
typedef char am_oschar;
#elif AM_PLATFORM_APPLE
typedef char am_oschar;
#else
typedef char am_oschar;
#endif

typedef unsigned long am_size;
typedef signed char am_int8;
typedef unsigned char am_uint8;
typedef signed short am_int16;
typedef unsigned short am_uint16;
typedef signed int am_int32;
typedef unsigned int am_uint32;
typedef float am_float32;
typedef double am_float64;
typedef am_float64 am_time;
#if defined(_MSC_VER) && !defined(__clang__)
typedef signed __int64 am_int64;
typedef unsigned __int64 am_uint64;
#else
#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wc++11-long-long"
#endif
#endif
typedef signed long long am_int64;
typedef unsigned long long am_uint64;
#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
#pragma GCC diagnostic pop
#endif
#endif

typedef void* am_voidptr;

typedef am_uint32 am_bool;
#define AM_TRUE 1
#define AM_FALSE 0

typedef union { am_float32 data[2]; struct { am_float32 x, y; }; } am_vec2;
typedef union { am_float32 data[3]; struct { am_float32 x, y, z; }; } am_vec3;
typedef union { am_float32 data[4]; struct { am_float32 x, y, z, w; }; } am_vec4;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Free allocated memory for a string.
 *
 * @param str The string to be freed.
 */
__api void am_free_string(const char* str);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_COMMON_H
