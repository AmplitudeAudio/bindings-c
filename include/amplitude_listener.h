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

#ifndef _AM_C_LISTENER_H
#define _AM_C_LISTENER_H

#include "amplitude_common.h"

struct am_listener;

/**
 * @brief An object used to render sound sources.
 *
 * Listeners define the position and orientation from which audio is perceived
 * in the 3D audio space.
 */
typedef struct am_listener am_listener;

/**
 * @brief Represents a handle to a listener.
 */
typedef am_listener* am_listener_handle;

/**
 * @brief Represents the unique ID of a listener.
 *
 * @note This is a 64-bit unsigned integer.
 */
typedef am_uint64 am_listener_id;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks whether a listener has been initialized.
 *
 * @param[in] listener The listener to check.
 *
 * @return Whether the listener is valid.
 */
__api am_bool
am_listener_is_valid(am_listener_handle listener);

/**
 * @brief Gets the unique ID of a listener.
 *
 * @param[in] listener The listener to get the ID of.
 *
 * @return The listener's unique ID.
 */
__api am_listener_id
am_listener_get_id(am_listener_handle listener);

/**
 * @brief Gets the velocity of a listener.
 *
 * @param[in] listener The listener to get the velocity of.
 *
 * @return The listener's velocity.
 */
__api am_vec3
am_listener_get_velocity(am_listener_handle listener);

/**
 * @brief Gets the location of a listener.
 *
 * @param[in] listener The listener to get the location of.
 *
 * @return The listener's location.
 */
__api am_vec3
am_listener_get_location(am_listener_handle listener);

/**
 * @brief Sets the location of a listener.
 *
 * @param[in] listener The listener to set the location of.
 * @param[in] location The new location of the listener.
 */
__api void
am_listener_set_location(am_listener_handle listener, am_vec3 location);

/**
 * @brief Gets the direction vector of a listener.
 *
 * @param[in] listener The listener to get the direction of.
 *
 * @return The direction vector.
 */
__api am_vec3
am_listener_get_direction(am_listener_handle listener);

/**
 * @brief Gets the up vector of a listener.
 *
 * @param[in] listener The listener to get the up vector of.
 *
 * @return The up vector.
 */
__api am_vec3
am_listener_get_up(am_listener_handle listener);

/**
 * @brief Sets the orientation of a listener.
 *
 * @param[in] listener The listener to set the orientation of.
 * @param[in] orientation The new orientation of the listener.
 */
__api void
am_listener_set_orientation(am_listener_handle listener, am_quaternion orientation);

/**
 * @brief Gets the orientation of a listener.
 *
 * @param[in] listener The listener to get the orientation of.
 *
 * @return The listener's orientation.
 */
__api am_quaternion
am_listener_get_orientation(am_listener_handle listener);

/**
 * @brief Sets the directivity and sharpness of a listener.
 *
 * This affects how sounds are perceived by the listener.
 *
 * @param[in] listener The listener to set the directivity of.
 * @param[in] directivity The directivity of the listener, in the range [0, 1].
 * @param[in] sharpness The directivity sharpness of the listener, in the range [1, +INF].
 * Increasing this value increases the directivity towards the front of the listener.
 */
__api void
am_listener_set_directivity(am_listener_handle listener, am_float32 directivity, am_float32 sharpness);

/**
 * @brief Gets the directivity of sounds rendered by a listener.
 *
 * @param[in] listener The listener to get the directivity of.
 *
 * @return The directivity of sound sources.
 */
__api am_float32
am_listener_get_directivity(am_listener_handle listener);

/**
 * @brief Gets the directivity sharpness of sounds rendered by a listener.
 *
 * @param[in] listener The listener to get the directivity sharpness of.
 *
 * @return The directivity sharpness of sounds rendered by the listener.
 */
__api am_float32
am_listener_get_directivity_sharpness(am_listener_handle listener);

/**
 * @brief Gets the inverse matrix of a listener.
 *
 * You can use this matrix to convert locations from global space to listener space.
 *
 * @param[in] listener The listener to get the inverse matrix of.
 *
 * @return The inverse matrix of the listener.
 */
__api am_mat4
am_listener_get_inverse_matrix(am_listener_handle listener);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_LISTENER_H
