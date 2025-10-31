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

#ifndef _AM_C_CODEC_H
#define _AM_C_CODEC_H

#include "amplitude_common.h"
#include "amplitude_file.h"

/**
 * @file amplitude_codec.h
 * @brief C API for audio codec management and operations.
 *
 * This header provides a C interface for the Amplitude Audio SDK's codec
 * system, allowing registration of custom audio codecs and performing audio
 * encoding/decoding operations.
 */

/**
 * @brief Audio sample format enumeration for C API.
 */
typedef enum
{
    am_audio_sample_format_float32, /**< 32-bit floating-point sample */
    am_audio_sample_format_int16, /**< 16-bit signed integer sample */
    am_audio_sample_format_unknown /**< Unknown or unsupported format */
} am_audio_sample_format;

/**
 * @brief Audio format structure containing sample format information.
 */
typedef struct
{
    am_uint32 sample_rate; /**< Sample rate in Hz (e.g., 44100) */
    am_uint16 num_channels; /**< Number of audio channels (e.g., 1 for mono, 2 for
                               stereo) */
    am_uint32 bits_per_sample; /**< Number of bits per sample (e.g., 16, 32) */
    am_uint64 frames_count; /**< Total number of audio frames in the file */
    am_uint32 frame_size; /**< Size of each audio frame in bytes */
    am_audio_sample_format sample_type; /**< Type of audio sample format */
} am_sound_format;

/**
 * @brief Opaque handle to a codec instance.
 */
struct am_codec;
typedef struct am_codec am_codec;
typedef am_codec* am_codec_handle;

/**
 * @brief Opaque handle to a codec decoder instance.
 */
struct am_codec_decoder;
typedef struct am_codec_decoder am_codec_decoder;
typedef am_codec_decoder* am_codec_decoder_handle;

/**
 * @brief Opaque handle to a codec encoder instance.
 */
struct am_codec_encoder;
typedef struct am_codec_encoder am_codec_encoder;
typedef am_codec_encoder* am_codec_encoder_handle;

/**
 * @brief Virtual function table for codec decoder operations.
 *
 * This structure contains function pointers that implement the decoder
 * functionality. All functions receive user_data as their first parameter for
 * context.
 */
typedef struct
{
    /**
     * @brief Initialize the decoder instance (optional).
     * @param user_data User-provided context data.
     */
    void (*create)(am_voidptr user_data);

    /**
     * @brief Cleanup the decoder instance (optional).
     * @param user_data User-provided context data.
     */
    void (*destroy)(am_voidptr user_data);

    /**
     * @brief Open an audio file for decoding.
     * @param user_data User-provided context data.
     * @param file File handle to open.
     * @return AM_TRUE if successful, AM_FALSE otherwise.
     */
    am_bool (*open)(am_voidptr user_data, am_file_handle file);

    /**
     * @brief Close the currently opened file.
     * @param user_data User-provided context data.
     * @return AM_TRUE if successful, AM_FALSE otherwise.
     */
    am_bool (*close)(am_voidptr user_data);

    /**
     * @brief Get the audio format of the opened file.
     * @param user_data User-provided context data.
     * @param format Pointer to store the format information.
     */
    void (*get_format)(am_voidptr user_data, am_sound_format* format);

    /**
     * @brief Load the entire audio file into the output buffer.
     * @param user_data User-provided context data.
     * @param out Pointer to the output buffer.
     * @return Number of audio frames loaded.
     */
    am_uint64 (*load)(am_voidptr user_data, am_voidptr out);

    /**
     * @brief Stream a portion of the audio file into the output buffer.
     * @param user_data User-provided context data.
     * @param out Pointer to the output buffer.
     * @param buffer_offset Offset in frames within the output buffer.
     * @param seek_offset Offset in frames within the source file.
     * @param length Number of frames to read.
     * @return Number of frames actually read.
     */
    am_uint64 (*stream)(am_voidptr user_data, am_voidptr out, am_uint64 buffer_offset, am_uint64 seek_offset, am_uint64 length);

    /**
     * @brief Seek to a specific position in the audio file.
     * @param user_data User-provided context data.
     * @param offset Frame offset to seek to.
     * @return AM_TRUE if successful, AM_FALSE otherwise.
     */
    am_bool (*seek)(am_voidptr user_data, am_uint64 offset);
} am_codec_decoder_vtable;

/**
 * @brief Virtual function table for codec encoder operations.
 *
 * This structure contains function pointers that implement the encoder
 * functionality. All functions receive user_data as their first parameter for
 * context.
 */
typedef struct
{
    /**
     * @brief Initialize the encoder instance (optional).
     * @param user_data User-provided context data.
     */
    void (*create)(am_voidptr user_data);

    /**
     * @brief Cleanup the encoder instance (optional).
     * @param user_data User-provided context data.
     */
    void (*destroy)(am_voidptr user_data);

    /**
     * @brief Open or create an audio file for encoding.
     * @param user_data User-provided context data.
     * @param file File handle to open/create.
     * @return AM_TRUE if successful, AM_FALSE otherwise.
     */
    am_bool (*open)(am_voidptr user_data, am_file_handle file);

    /**
     * @brief Close the currently opened file.
     * @param user_data User-provided context data.
     * @return AM_TRUE if successful, AM_FALSE otherwise.
     */
    am_bool (*close)(am_voidptr user_data);

    /**
     * @brief Set the audio format for encoding.
     * @param user_data User-provided context data.
     * @param format Pointer to the format information.
     */
    void (*set_format)(am_voidptr user_data, const am_sound_format* format);

    /**
     * @brief Write audio data to the file.
     * @param user_data User-provided context data.
     * @param in Pointer to the input buffer.
     * @param offset Offset in frames within the input buffer.
     * @param length Number of frames to write.
     * @return Number of frames actually written.
     */
    am_uint64 (*write)(am_voidptr user_data, am_voidptr in, am_uint64 offset, am_uint64 length);
} am_codec_encoder_vtable;

/**
 * @brief Virtual function table for codec operations.
 *
 * This structure contains function pointers for codec-level operations.
 */
typedef struct
{
    /**
     * @brief Called when the codec is registered (optional).
     * @param user_data User-provided context data.
     */
    void (*on_register)(am_voidptr user_data);

    /**
     * @brief Called when the codec is unregistered (optional).
     * @param user_data User-provided context data.
     */
    void (*on_unregister)(am_voidptr user_data);

    /**
     * @brief Check if this codec can handle the given file.
     * @param user_data User-provided context data.
     * @param file File handle to check.
     * @return AM_TRUE if the codec can handle the file, AM_FALSE otherwise.
     */
    am_bool (*on_can_handle_file)(am_voidptr user_data, am_file_handle file);
} am_codec_vtable;

/**
 * @brief Configuration structure for codec registration.
 *
 * This structure contains all the information needed to register a custom codec
 * with the Amplitude Audio engine.
 */
typedef struct
{
    const char* name; /**< Name of the codec (e.g., "WAV", "OGG") */
    am_voidptr user_data; /**< User-provided context data */
    am_codec_vtable* v_table; /**< Virtual function table for codec operations */

    /**
     * @brief Decoder configuration.
     */
    struct
    {
        am_codec_decoder_vtable* v_table; /**< Virtual function table for decoder operations */
        am_voidptr user_data; /**< User-provided context data for decoder */
    } decoder;

    /**
     * @brief Encoder configuration.
     */
    struct
    {
        am_codec_encoder_vtable* v_table; /**< Virtual function table for encoder operations */
        am_voidptr user_data; /**< User-provided context data for encoder */
    } encoder;
} am_codec_config;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize a codec configuration structure with default values.
 *
 * This function creates a codec configuration with all pointers set to NULL
 * and only the name field initialized.
 *
 * @param name The name of the codec.
 * @return Initialized codec configuration structure.
 */
__api am_codec_config
am_codec_config_init(const char* name);

/**
 * @brief Register a codec with the Amplitude Audio engine.
 *
 * This function registers a custom codec implementation with the engine.
 * The codec will be available for encoding/decoding operations after
 * registration.
 *
 * @param config Pointer to the codec configuration structure.
 *
 * @note This function does nothing if the codec registry is locked.
 * @see am_codec_unregister
 */
__api void
am_codec_register(const am_codec_config* config);

/**
 * @brief Unregister a codec from the Amplitude Audio engine.
 *
 * This function removes a previously registered codec from the engine.
 *
 * @param name The name of the codec to unregister.
 *
 * @note This function does nothing if the codec registry is locked.
 * @see am_codec_register
 */
__api void
am_codec_unregister(const char* name);

/**
 * @brief Find a registered codec by name.
 *
 * @param name The name of the codec to find.
 * @return Handle to the codec if found, NULL otherwise.
 */
__api am_codec_handle
am_codec_find(const char* name);

/**
 * @brief Find a codec that can handle the specified file.
 *
 * This function searches through all registered codecs to find one
 * that can handle the given file.
 *
 * @param file File handle to check.
 * @return Handle to a suitable codec if found, NULL otherwise.
 */
__api am_codec_handle
am_codec_find_for_file(am_file_handle file);

/**
 * @brief Check if a codec can handle the specified file.
 *
 * @param codec Handle to the codec to check.
 * @param file File handle to check.
 * @return AM_TRUE if the codec can handle the file, AM_FALSE otherwise.
 */
__api am_bool
am_codec_can_handle_file(am_codec_handle codec, am_file_handle file);

/**
 * @brief Get the name of a codec.
 *
 * @param codec Handle to the codec.
 * @return The name of the codec, or NULL if the handle is invalid.
 *
 * @note The returned string should be freed using am_free_string().
 */
__api const char*
am_codec_get_name(am_codec_handle codec);

// Decoder functions

/**
 * @brief Create a decoder instance from a codec.
 *
 * @param codec_name The name of the codec to create a decoder for.
 * @return Handle to the decoder if successful, NULL otherwise.
 */
__api am_codec_decoder_handle
am_codec_decoder_create(const char* codec_name);

/**
 * @brief Create a decoder instance from a codec handle.
 *
 * @param codec Handle to the codec to create a decoder for.
 * @return Handle to the decoder if successful, NULL otherwise.
 */
__api am_codec_decoder_handle
am_codec_decoder_create_from_codec(am_codec_handle codec);

/**
 * @brief Destroy a decoder instance.
 *
 * @param handle Handle to the decoder to destroy.
 */
__api void
am_codec_decoder_destroy(am_codec_decoder_handle handle);

/**
 * @brief Open an audio file for decoding.
 *
 * @param handle Handle to the decoder.
 * @param file File handle to open.
 * @return AM_TRUE if successful, AM_FALSE otherwise.
 */
__api am_bool
am_codec_decoder_open(am_codec_decoder_handle handle, am_file_handle file);

/**
 * @brief Close the currently opened file.
 *
 * @param handle Handle to the decoder.
 * @return AM_TRUE if successful, AM_FALSE otherwise.
 */
__api am_bool
am_codec_decoder_close(am_codec_decoder_handle handle);

/**
 * @brief Get the audio format of the opened file.
 *
 * @param handle Handle to the decoder.
 * @param format Pointer to store the format information.
 * @return AM_TRUE if successful, AM_FALSE otherwise.
 */
__api am_bool
am_codec_decoder_get_format(am_codec_decoder_handle handle, am_sound_format* format);

/**
 * @brief Load the entire audio file into the output buffer.
 *
 * The output buffer must be allocated with sufficient size to hold
 * all the audio data from the file.
 *
 * @param handle Handle to the decoder.
 * @param out Pointer to the output buffer.
 * @return Number of audio frames loaded into the buffer.
 */
__api am_uint64
am_codec_decoder_load(am_codec_decoder_handle handle, am_voidptr out);

/**
 * @brief Stream a portion of the audio file into the output buffer.
 *
 * This function reads a specific portion of the audio file and writes it
 * to the output buffer at the specified offset.
 *
 * @param handle Handle to the decoder.
 * @param out Pointer to the output buffer.
 * @param buffer_offset Offset in frames within the output buffer to start
 * writing.
 * @param seek_offset Offset in frames within the source file to start reading.
 * @param length Number of frames to read from the file.
 * @return Number of frames actually read and written to the buffer.
 */
__api am_uint64
am_codec_decoder_stream(am_codec_decoder_handle handle, am_voidptr out, am_uint64 buffer_offset, am_uint64 seek_offset, am_uint64 length);

/**
 * @brief Seek to a specific position in the audio file.
 *
 * @param handle Handle to the decoder.
 * @param offset Frame offset to seek to.
 * @return AM_TRUE if successful, AM_FALSE otherwise.
 */
__api am_bool
am_codec_decoder_seek(am_codec_decoder_handle handle, am_uint64 offset);

// Encoder functions

/**
 * @brief Create an encoder instance from a codec.
 *
 * @param codec_name The name of the codec to create an encoder for.
 * @return Handle to the encoder if successful, NULL otherwise.
 */
__api am_codec_encoder_handle
am_codec_encoder_create(const char* codec_name);

/**
 * @brief Create an encoder instance from a codec handle.
 *
 * @param codec Handle to the codec to create an encoder for.
 * @return Handle to the encoder if successful, NULL otherwise.
 */
__api am_codec_encoder_handle
am_codec_encoder_create_from_codec(am_codec_handle codec);

/**
 * @brief Destroy an encoder instance.
 *
 * @param handle Handle to the encoder to destroy.
 */
__api void
am_codec_encoder_destroy(am_codec_encoder_handle handle);

/**
 * @brief Open or create an audio file for encoding.
 *
 * @param handle Handle to the encoder.
 * @param file File handle to open or create.
 * @return AM_TRUE if successful, AM_FALSE otherwise.
 */
__api am_bool
am_codec_encoder_open(am_codec_encoder_handle handle, am_file_handle file);

/**
 * @brief Close the currently opened file.
 *
 * @param handle Handle to the encoder.
 * @return AM_TRUE if successful, AM_FALSE otherwise.
 */
__api am_bool
am_codec_encoder_close(am_codec_encoder_handle handle);

/**
 * @brief Set the audio format for encoding.
 *
 * This function must be called before opening a file for encoding
 * to specify the format of the audio data to be written.
 *
 * @param handle Handle to the encoder.
 * @param format Pointer to the format information.
 */
__api void
am_codec_encoder_set_format(am_codec_encoder_handle handle, const am_sound_format* format);

/**
 * @brief Write audio data to the file.
 *
 * @param handle Handle to the encoder.
 * @param in Pointer to the input buffer containing audio data.
 * @param offset Offset in frames within the input buffer to start reading.
 * @param length Number of frames to write from the input buffer.
 * @return Number of frames actually written to the file.
 */
__api am_uint64
am_codec_encoder_write(am_codec_encoder_handle handle, am_voidptr in, am_uint64 offset, am_uint64 length);

// Utility functions

/**
 * @brief Initialize a sound format structure with default values.
 *
 * @return Initialized sound format structure with default values.
 */
__api am_sound_format
am_sound_format_init(void);

/**
 * @brief Set all properties of a sound format structure.
 *
 * @param format Pointer to the format structure to initialize.
 * @param sample_rate Sample rate in Hz.
 * @param num_channels Number of audio channels.
 * @param bits_per_sample Number of bits per sample.
 * @param frames_count Total number of frames.
 * @param frame_size Size of each frame in bytes.
 * @param sample_type Audio sample format type.
 */
__api void
am_sound_format_set_all(
    am_sound_format* format,
    am_uint32 sample_rate,
    am_uint16 num_channels,
    am_uint32 bits_per_sample,
    am_uint64 frames_count,
    am_uint32 frame_size,
    am_audio_sample_format sample_type);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_CODEC_H
