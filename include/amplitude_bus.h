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

#ifndef _AM_C_BUS_H
#define _AM_C_BUS_H

#include "amplitude_common.h"

struct am_bus;

/**
 * @brief An object representing one node in the tree of buses. Buses are used to adjust a set of channel gains in tandem.
 */
typedef struct am_bus am_bus;

/**
 * @brief Represents a handle to a bus object.
 */
typedef am_bus* am_bus_handle;

/**
 * @brief Represents the unique ID of a bus.
 *
 * @note This is a 64-bit unsigned integer.
 */
typedef am_uint64 am_bus_id;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks if the given bus is valid.
 *
 * @param[in] bus The bus to check.
 *
 * @return `AM_TRUE` if the bus is valid, `AM_FALSE` otherwise.
 */
__api am_bool
am_bus_is_valid(am_bus_handle bus);

/**
 * @brief Gets the unique ID of the given bus.
 *
 * @param[in] bus The bus.
 *
 * @return The Bus unique ID.
 */
__api am_bus_id
am_bus_get_id(am_bus_handle bus);

/**
 * @brief Gets the name of the given bus.
 *
 * @param[in] bus The bus.
 *
 * @return The bus name.
 */
__api const char*
am_bus_get_name(am_bus_handle bus);

/**
 * @brief Sets the gain of the given bus.
 *
 * @param[in] bus The bus.
 * @param[in] gain The new gain value.
 */
__api void
am_bus_set_gain(am_bus_handle bus, am_float32 gain);

/**
 * @brief Returns the user specified gain on the given bus.
 *
 * @param[in] bus The bus.
 *
 * @return The user specified gain.
 */
__api am_float32
am_bus_get_gain(am_bus_handle bus);

/**
 * @brief Fades to `gain` over `duration` seconds.
 *
 * @param[in] bus The bus.
 * @param[in] target_gain The gain value to fade to.
 * @param[in] duration The amount of time in seconds to take to reach the desired gain.
 */
__api void
am_bus_fade_to(am_bus_handle bus, am_float32 target_gain, am_time duration);

/**
 * @brief Returns the final calculated gain of the given bus.
 *
 * @note The final gain of a bus is the product of the gain specified in the bus
 * definition file, the gain specified by the user, and the final gain of the
 * parent Bus.
 *
 * @param[in] bus The bus.
 *
 * @return The final calculated gain.
 */
__api am_float32
am_bus_get_final_gain(am_bus_handle bus);

/**
 * @brief Sets the muted state of the given bus.
 *
 * @param[in] bus The bus to mute/unmute.
 * @param[in] mute The muted state.
 */
__api void
am_bus_set_mute(am_bus_handle bus, am_bool mute);

/**
 * @brief Returns whether the given bus is muted.
 *
 * @param[in] bus The bus to check.
 *
 * @return @c true if the bus is muted, @c false otherwise.
 */
__api am_bool
am_bus_is_muted(am_bus_handle bus);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_BUS_H
