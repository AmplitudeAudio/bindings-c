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

#ifndef _AM_C_FILE_SYSTEM_H
#define _AM_C_FILE_SYSTEM_H

#include "amplitude_common.h"
#include "amplitude_file.h"

/**
 * @brief Enumeration of filesystem types.
 */
typedef enum am_filesystem_type : am_uint8
{
    /**
     * @brief Unknown filesystem type. Usually indicates an error.
     */
    am_filesystem_type_unknown = 0,

    /**
     * @brief Custom filesystem type. Used for custom filesystem implementations.
     */
    am_filesystem_type_custom = 1,

    /**
     * @brief Disk filesystem type.
     */
    am_filesystem_type_disk = 2,

    /**
     * @brief Package filesystem type. Used for Amplitude Package (.ampk) files.
     */
    am_filesystem_type_package = 3,

#if AM_PLATFORM_ANDROID
    /**
     * @brief Android asset filesystem type.
     */
    am_filesystem_type_android = 4,
#elif AM_PLATFORM_IOS
    /**
     * @brief iOS filesystem type.
     */
    am_filesystem_type_ios = 5,
#endif
} am_filesystem_type;

struct am_filesystem; // Opaque type for the FileSystem class.
typedef struct am_filesystem am_filesystem;
typedef struct
{
    am_filesystem_type type;
    am_voidptr handle;
} am_filesystem_handle;

typedef struct
{
    void (*create)(am_voidptr user_data);
    void (*destroy)(am_voidptr user_data);
    void (*set_base_path)(am_voidptr user_data, const am_oschar* base_path);
    const am_oschar* (*get_base_path)(am_voidptr user_data);
    const am_oschar* (*resolve_path)(am_voidptr user_data, const am_oschar* path);
    am_bool (*exists)(am_voidptr user_data, const am_oschar* path);
    am_bool (*is_directory)(am_voidptr user_data, const am_oschar* path);
    const am_oschar* (*join)(am_voidptr user_data, const am_oschar** paths, am_uint32 path_count);
    am_file_handle (*open_file)(am_voidptr user_data, const am_oschar* path, am_file_open_mode mode);
    void (*start_open_filesystem)(am_voidptr user_data);
    am_bool (*try_finalize_open_filesystem)(am_voidptr user_data);
    void (*start_close_filesystem)(am_voidptr user_data);
    am_bool (*try_finalize_close_filesystem)(am_voidptr user_data);
} am_filesystem_vtable;

typedef struct
{
    am_filesystem_type type;

    // Only used if type is am_filesystem_type_custom.
    am_voidptr user_data;
    am_filesystem_vtable* v_table;
} am_filesystem_config;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a custom filesystem configuration.
 */
__api am_filesystem_config
am_filesystem_config_init_custom();

/**
 * @brief Initialize a disk filesystem configuration.
 */
__api am_filesystem_config
am_filesystem_config_init_disk();

/**
 * @brief Initialize a package filesystem configuration.
 */
__api am_filesystem_config
am_filesystem_config_init_package();

#if AM_PLATFORM_ANDROID
/**
 * @brief Initialize an Android filesystem configuration.
 */
__api am_filesystem_config
am_filesystem_config_init_android();
#elif AM_PLATFORM_IOS
/**
 * @brief Initialize an iOS filesystem configuration.
 */
__api am_filesystem_config
am_filesystem_config_init_ios();
#endif

/**
 * @brief Create a filesystem.
 */
__api am_filesystem_handle
am_filesystem_create(const am_filesystem_config* config);

/**
 * @brief Destroy a filesystem.
 */
__api void
am_filesystem_destroy(am_filesystem_handle filesystem);

/**
 * @brief Set the base path for a filesystem.
 */
__api void
am_filesystem_set_base_path(am_filesystem_handle filesystem, const am_oschar* base_path);

/**
 * @brief Get the base path for a filesystem.
 */
__api const am_oschar*
am_filesystem_get_base_path(am_filesystem_handle filesystem);

/**
 * @brief Resolve a path relative to a filesystem.
 */
__api const am_oschar*
am_filesystem_resolve_path(am_filesystem_handle filesystem, const am_oschar* path);

/**
 * @brief Check if a path exists within a filesystem.
 */
__api am_bool
am_filesystem_exists(am_filesystem_handle filesystem, const am_oschar* path);

/**
 * @brief Check if a path is a directory within a filesystem.
 */
__api am_bool
am_filesystem_is_directory(am_filesystem_handle filesystem, const am_oschar* path);

/**
 * @brief Join multiple path components within a filesystem.
 */
__api const am_oschar*
am_filesystem_join(am_filesystem_handle filesystem, const am_oschar** parts, am_size count);

/**
 * @brief Open a file within a filesystem handle.
 */
__api am_file_handle
am_filesystem_open_file(am_filesystem_handle filesystem, const am_oschar* path, am_file_open_mode mode);

/**
 * @brief Start opening a file within a filesystem handle.
 */
__api void
am_filesystem_start_open(am_filesystem_handle filesystem);

/**
 * @brief Try to finalize the opening of a file within a filesystem handle.
 */
__api am_bool
am_filesystem_try_finalize_open(am_filesystem_handle filesystem);

/**
 * @brief Start closing a file within a filesystem handle.
 */
__api void
am_filesystem_start_close(am_filesystem_handle filesystem);

/**
 * @brief Try to finalize the closing of a file within a filesystem handle.
 */
__api am_bool
am_filesystem_try_finalize_close(am_filesystem_handle filesystem);

/**
 * @brief Sets the platform filesystem within a package.
 */
__api void
am_filesystem_package_set_filesystem(am_filesystem_handle filesystem, am_filesystem_config* internal);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _AM_C_FILE_SYSTEM_H
