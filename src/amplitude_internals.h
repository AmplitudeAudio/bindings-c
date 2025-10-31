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

#pragma once

#ifndef _AM_IMPLEMENTATION_INTERNALS_H
#define _AM_IMPLEMENTATION_INTERNALS_H

#include <SparkyStudios/Audio/Amplitude/Amplitude.h>

#include <amplitude_common.h>

#include "amplitude_shared_ptr_manager.h"

#define BOOL_TO_AM_BOOL(x) ((x) ? AM_TRUE : AM_FALSE)
#define AM_BOOL_TO_BOOL(x) ((x) == AM_TRUE)

using namespace SparkyStudios::Audio::Amplitude;

inline const char*
am_allocate_string(const AmString& str)
{
    const size_t count = str.size();
    const size_t len = sizeof(char) * (count + 1);
    auto* result = static_cast<char*>(ammalloc(len));
    std::memcpy(result, str.c_str(), len);
    result[count] = '\0';
    return result;
}

inline void
am_free_string(const char* str)
{
    amfree(const_cast<char*>(str));
}

inline const am_oschar*
am_allocate_osstring(const AmOsString& str)
{
    const size_t count = str.size();
    const size_t len = sizeof(am_oschar) * (count + 1);
    auto* result = static_cast<am_oschar*>(ammalloc(len));
    std::memcpy(result, str.c_str(), len);
    result[count] = '\0';
    return result;
}

inline void
am_free_osstring(const am_oschar* str)
{
    amfree(const_cast<am_oschar*>(str));
}

inline am_quaternion
from_cpp(const AmQuaternion& quat)
{
    return {quat.w, quat.x, quat.y, quat.z};
}

inline AmQuaternion
to_cpp(const am_quaternion& quat)
{
    return {quat.w, quat.x, quat.y, quat.z};
}

inline am_vec4
from_cpp(const AmVector4& vec)
{
    return {vec.x, vec.y, vec.z, vec.w};
}

inline AmVector4
to_cpp(const am_vec4& vec)
{
    return {vec.x, vec.y, vec.z, vec.w};
}
inline am_vec3
from_cpp(const AmVector3& vec)
{
    return {vec.x, vec.y, vec.z};
}

inline AmVector3
to_cpp(const am_vec3& vec)
{
    return {vec.x, vec.y, vec.z};
}

inline am_vec2
from_cpp(const AmVector2& vec)
{
    return {vec.x, vec.y};
}

inline AmVector2
to_cpp(const am_vec2& vec)
{
    return {vec.x, vec.y};
}

inline am_mat4
from_cpp(const AmMatrix4& mat)
{
    am_mat4 result;
    std::memcpy(result.data, mat.data, sizeof(am_mat4));
    return result;
}

inline AmMatrix4
to_cpp(const am_mat4& mat)
{
    AmMatrix4 result;
    std::memcpy(result.data, mat.data, sizeof(AmMatrix4));
    return result;
}

// SharedPtrManager helper macros
#define STORE_SHARED_PTR(type, shared_ptr) SharedPtrManager::Instance().Store<type>(shared_ptr)

#define GET_SHARED_PTR(type, handle) SharedPtrManager::Instance().Get<type>(reinterpret_cast<type*>(handle))

#define REMOVE_SHARED_PTR(type, handle) SharedPtrManager::Instance().Remove<type>(reinterpret_cast<type*>(handle))

#define CREATE_SHARED_PTR(pool, type, ...) SharedPtrManager::Instance().Store<type>(ampoolshared(pool, type, __VA_ARGS__))

#define VALIDATE_SHARED_PTR_HANDLE(type, handle) SharedPtrManager::Instance().HasType<type>(reinterpret_cast<type*>(handle))

#endif // _AM_IMPLEMENTATION_INTERNALS_H
