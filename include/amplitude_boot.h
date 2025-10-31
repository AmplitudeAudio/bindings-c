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

#ifndef _AM_C_BOOT_H
#define _AM_C_BOOT_H

#include "amplitude_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Amplitude C API system.
 *
 * @warning This function must be called before using any other C API functions.
 *
 * @note This function is thread-safe and can be called multiple times.
 * Subsequent calls after the first will be ignored.
 */
__api void
am_boot();

/**
 * @brief Shutdown the Amplitude C API system.
 *
 * This function should be called when done using the C API.
 * It cleans up internal managers and releases all stored resources.
 *
 * @note This function is thread-safe. After calling this function,
 * am_boot() must be called again before using other C API functions.
 */
__api void
am_shutdown();

/**
 * @brief Check if the C API system is initialized.
 *
 * @return AM_TRUE if the system is initialized, AM_FALSE otherwise.
 */
__api am_bool
am_is_initialized();

#ifdef __cplusplus
}
#endif

#endif // _AM_C_BOOT_H
