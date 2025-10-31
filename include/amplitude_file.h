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

#ifndef _AM_C_FILE_H
#define _AM_C_FILE_H

#include "amplitude_common.h"

/**
 * @brief Enumeration of file types.
 */
typedef enum am_file_type : am_uint8
{
    /**
     * @brief Unknown file type. Usually indicates an error.
     */
    am_file_type_unknown = 0,

    /**
     * @brief Custom file type. Used for custom file implementations.
     */
    am_file_type_custom = 1,

    /**
     * @brief Disk file type. Used for files on disk with a disk filesystem.
     */
    am_file_type_disk = 2,

    /**
     * @brief Memory file type. Used for files in memory. Not related to any filesystem.
     */
    am_file_type_memory = 3,

    /**
     * @brief Package item file type. Used for files in a package with a package filesystem.
     */
    am_file_type_package_item = 4,

#if AM_PLATFORM_ANDROID
    /**
     * @brief Android asset file type. Used for files in an Android asset with an Android asset filesystem.
     */
    am_file_type_android = 5,
#endif

#if AM_PLATFORM_IOS
    /**
     * @brief iOS file type. Used for files in an iOS filesystem.
     */
    am_file_type_ios = 6,
#endif
} am_file_type;

/**
 * @brief Describes the mode in which to open a file.
 */
typedef enum am_file_open_mode : am_uint8
{
    /**
     * @brief Opens the file only for reading.
     */
    am_file_open_mode_read = 0,

    /**
     * @brief Opens the file only for writing.
     */
    am_file_open_mode_write = 1,

    /**
     * @brief Opens the file only for writing, without discarding the previous content.
     */
    am_file_open_mode_append = 2,

    /**
     * @brief Opens the file for read and write operations.
     */
    am_file_open_mode_readwrite = 3,

    /**
     * @brief Opens the file for read and write operations, without discarding the previous content.
     */
    am_file_open_mode_readappend = 4
} am_file_open_mode;

/**
 * @brief The type of file being opened.
 */
typedef enum am_file_open_kind : am_uint8
{
    /**
     * @brief Opens the file as a binary file.
     */
    am_file_open_kind_binary = 0,

    /**
     * @brief Opens the file as a text file.
     */
    am_file_open_kind_text = 1
} am_file_open_kind;

/**
 * @brief Defines from where to seek in the file.
 */
typedef enum am_file_seek_origin : am_uint8
{
    /**
     * @brief Seek from the beginning of the file.
     *
     * @see [SEEK_SET](https://en.cppreference.com/w/cpp/io/c)
     */
    am_file_seek_origin_start = 0,

    /**
     * @brief Seek from the cursor position.
     *
     * @see [SEEK_CUR](https://en.cppreference.com/w/cpp/io/c)
     */
    am_file_seek_origin_current = 1,

    /**
     * @brief Seek from the end of the file.
     *
     * @see [SEEK_END](https://en.cppreference.com/w/cpp/io/c)
     */
    am_file_seek_origin_end = 2
} am_file_seek_origin;

struct am_file; // Opaque type for the File class.
typedef struct am_file am_file;
typedef struct
{
    am_file_type type;
    am_voidptr handle;
} am_file_handle;

struct am_file_disk; // Opaque type for the DiskFile class.
typedef struct am_file_disk am_file_disk;
typedef am_file_disk* am_file_disk_handle;

struct am_file_memory; // Opaque type for the MemoryFile class.
typedef struct am_file_memory am_file_memory;
typedef am_file_memory* am_file_memory_handle;

typedef struct
{
    void (*create)(am_voidptr user_data);
    void (*destroy)(am_voidptr user_data);
    const am_oschar* (*get_path)(am_voidptr user_data);
    am_bool (*eof)(am_voidptr user_data);
    am_size (*read)(am_voidptr user_data, am_uint8* buffer, am_size bytes);
    am_size (*write)(am_voidptr user_data, const am_uint8* buffer, am_size bytes);
    am_size (*length)(am_voidptr user_data);
    void (*seek)(am_voidptr user_data, am_uint64 offset, am_file_seek_origin origin);
    am_size (*position)(am_voidptr user_data);
    am_voidptr (*get_ptr)(am_voidptr user_data);
    am_bool (*is_valid)(am_voidptr user_data);
    void (*close)(am_voidptr user_data);
} am_file_vtable;

typedef struct
{
    am_file_type type;

    // Only used if type is am_file_type_custom.
    am_voidptr user_data;
    am_file_vtable* v_table;
} am_file_config;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a custom file system configuration.
 */
__api am_file_config
am_file_config_init_custom();

/**
 * @brief Initialize a disk file system configuration.
 */
__api am_file_config
am_file_config_init_disk();

/**
 * @brief Initialize a memory file system configuration.
 */
__api am_file_config
am_file_config_init_memory();

/**
 * @brief Create a new file handle with the given configuration.
 */
__api am_file_handle
am_file_create(const am_file_config* config);

/**
 * @brief Destroy a file handle.
 */
__api void
am_file_destroy(am_file_handle file);

/**
 * @brief Get the path of a file, from within its filesystem.
 */
__api const am_oschar*
am_file_get_path(am_file_handle file);

/**
 * @brief Read an 8-bit unsigned integer from the file.
 */
__api am_uint8
am_file_read8(am_file_handle file);

/**
 * @brief Read a 16-bit unsigned integer from the file.
 */
__api am_uint16
am_file_read16(am_file_handle file);

/**
 * @brief Read a 32-bit unsigned integer from the file.
 */
__api am_uint32
am_file_read32(am_file_handle file);

/**
 * @brief Read a 64-bit unsigned integer from the file.
 */
__api am_uint64
am_file_read64(am_file_handle file);

/**
 * @brief Read a string from the file.
 */
__api const char*
am_file_read_string(am_file_handle file);

/**
 * @brief Write an 8-bit unsigned integer to the file.
 */
__api am_size
am_file_write8(am_file_handle file, am_uint8 value);

/**
 * @brief Write a 16-bit unsigned integer to the file.
 */
__api am_size
am_file_write16(am_file_handle file, am_uint16 value);

/**
 * @brief Write a 32-bit unsigned integer to the file.
 */
__api am_size
am_file_write32(am_file_handle file, am_uint32 value);

/**
 * @brief Write a 64-bit unsigned integer to the file.
 */
__api am_size
am_file_write64(am_file_handle file, am_uint64 value);

/**
 * @brief Write a string to the file.
 */
__api am_size
am_file_write_string(am_file_handle file, const char* string);

/**
 * @brief Check if a file handle is at the end of the file.
 */
__api am_bool
am_file_eof(am_file_handle file);

/**
 * @brief Read a specified number of bytes from a file handle into a buffer.
 */
__api am_size
am_file_read(am_file_handle file, am_uint8* buffer, am_size bytes);

/**
 * @brief Write a specified number of bytes from a buffer to a file handle.
 */
__api am_size
am_file_write(am_file_handle file, const am_uint8* buffer, am_size bytes);

/**
 * @brief Get the length of a file handle.
 */
__api am_size
am_file_length(am_file_handle file);

/**
 * @brief Seek to a specified position in a file handle.
 */
__api void
am_file_seek(am_file_handle file, am_size offset, am_file_seek_origin origin);

/**
 * @brief Get the current position in a file handle.
 */
__api am_size
am_file_position(am_file_handle file);

/**
 * @brief Get the internal file pointer.
 */
__api am_voidptr
am_file_get_ptr(am_file_handle file);

/**
 * @brief Check if a file handle is valid.
 */
__api am_bool
am_file_is_valid(am_file_handle file);

/**
 * @brief Close a file handle.
 */
__api void
am_file_close(am_file_handle file);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _AM_C_FILE_H
